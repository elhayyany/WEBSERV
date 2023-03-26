/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hayy <ael-hayy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:42:58 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/03/24 15:24:34 by ael-hayy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

Socket::Socket(std::string host, std::string service)
{
    creatSocket(host, service);
}

Socket::~Socket()
{
}

void    Socket::creatSocket(std::string& host, std::string& service)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *bind_address;
    getaddrinfo(host.c_str(), service.c_str(), &hints, &bind_address);
    socketfd = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    if (socketfd < 0)
    {
        std::cerr<< strerror(socketfd)<<std::endl;
        return ;
    }
    int yes = 1;
    setsockopt(socketfd,SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    int b = bind(socketfd, bind_address->ai_addr, bind_address->ai_addrlen);
    if (b)
    {
        std::cerr<<"error in bind(): "<< strerror(b)<<std::endl;
        socketfd = -1;
        return ;
    }
    freeaddrinfo(bind_address);
    b = listen(socketfd, SOMAXCONN);
    if (b < 0)
    {
        std::cerr<<"error in listn(): "<< strerror(b)<<std::endl;
        socketfd = -1;
        return ;
    }
}

SOCKET  Socket::operator()()
{
    return (socketfd);
}


int  waitingForClients(fd_set *readSet, fd_set *writeSet, SOCKET socketListen, std::list<Client>& clientList)
{
    SOCKET max_socket = socketListen;
    for (std::list<Client>::iterator  i = clientList.begin(); i != clientList.end(); i++)
    {
        FD_SET((*i).getSocket(), readSet);
        FD_SET((*i).getSocket(), writeSet);
        if (max_socket < (*i).getSocket())
            max_socket = (*i).getSocket();
    }
    // struct timeval timeout;
    // timeout.tv_sec = 0;
    // timeout.tv_usec = 10000;
    if (select(max_socket + 1, readSet, writeSet, 0, 0) < 0)
    {
        std::cerr<<"waitingForClients listn"<<strerror(errno)<<std::endl;
        return (-1);
    }
    return (0);
}
void    setSocketForReadAndWrite(fd_set *readSet, fd_set *writeSet, SOCKET socketListen)
{
    FD_ZERO(readSet);
    FD_SET(socketListen, readSet);
    FD_ZERO(writeSet);
    FD_SET(socketListen, writeSet);
}

int		acceptNewConnictions(fd_set *readSet, fd_set *writeSet, SOCKET socketListen, std::list<Client>& clientList)
{
    if (FD_ISSET(socketListen, readSet))
    {
        Client  client;
        SOCKET sock = accept(socketListen, (struct sockaddr *)&(client.getAddress()), &(const_cast<socklen_t&>(client.getAddrtLen())));
        client.setSocket(sock);
        if (client.getSocket() < 0)
        {
            std::cerr<<"socket < 0"<<strerror(errno)<<std::endl;
            return (-1);
        }
        std::cout<<"new READ conniction: "<<get_name(client)<<"its socket: "<<sock<<std::endl;
        clientList.push_front(client);
    }
    // else if (FD_ISSET(socketListen, writeSet))
    // {
    //     Client  client;
    //     SOCKET sock = accept(socketListen, (struct sockaddr *)&(client.getAddress()), &(const_cast<socklen_t&>(client.getAddrtLen())));
    //     client.setSocket(sock);
    //     if (client.getSocket() < 0)
    //     {
    //         std::cerr<<"socket < 0"<<strerror(errno)<<std::endl;
    //         return (-1);
    //     }
    //     std::cout<<"new WRITE conniction: "<<get_name(client)<<"its socket: "<<sock<<std::endl;
    //     clientList.push_front(client);
    // }
    return (0);

}

char* get_name(Client& client)
{
    static char address_buffer[100];
    getnameinfo((struct sockaddr *)&client.getAddress(),
                client.getAddrtLen(),
                address_buffer, sizeof(address_buffer), 0, 0,
                NI_NUMERICHOST);
    return address_buffer;
}
int		acceptREADsocket(fd_set *readSet, fd_set *writeSet, Client& client, std::list<Client>& clientList, std::list<Client>::iterator& i)
{
    if (FD_ISSET(client.getSocket(), readSet))
    {
        char    buffer[BUFFER_SIZE + 1];
        int r = recv(client.getSocket(), buffer, BUFFER_SIZE, 0);
        // buffer[r] = 0;
        if (r < -1)
        {
            std::cerr<<"error in resv(): "<<strerror(errno)<<std::endl;
            return (-1);
        }
        if (r > 0)
            client.jj = 1;
        // r++;
        Request& request = client.getRequest();
        if (!request.getLnght())
        {
            request.addToReqyest(buffer, r);
            request.parseInfos();
            if (request.getHeaderInfos()["METHOD"] == "GET")
            {
                request.setType(GET_REQ);
            }
            else if (request.getHeaderInfos()["METHOD"] == "POST")
            {
                std::cout<<"POST REQUEST " <<std::endl;
                request.setType(POST_REQ);
            }
            else if (request.getHeaderInfos()["METHOD"] == "DELETE")
            {
                std::cout<<"DELET REQUEST " <<std::endl;
                request.setType(DELETE_REQ);
            }
            else
            {
                std::cout<<"DELET REQUEST " <<std::endl;
                // ! UNKNOWN REQUEST ..... . . . . . . . . 
                // sorry our server dont support that type of request
                
                return (-1);
            }
        }
        else
        {
            request.addToReqyest(buffer, r);
        }
        std::cout<<"REQUEST FROM: "<<get_name(client)<<" and its type is: "<<request.getType()<<" and its content is:"<<std::endl<<std::endl<<std::endl;
        std::cout<<request.getHttpRequest()<<std::endl;
        
        // send(client.getSocket(), client.getResponse().c_str(), client.getResponse().size() - 5, 0);
        // close(client.getSocket());
        // clientList.erase(i);
        // return (1);
        //!   PARSE THE PART OF THE REQUEST THAT HAS BEEN RECEIVED.
    }
    else
    {
        std::cout<<"write set :"<<client.getSocket()<<std::endl;
        if (FD_ISSET(client.getSocket(), writeSet) && client.jj)
        {
            std::cout<<"is a set\n";
            int r = send(client.getSocket(), client.getResponse().c_str(), client.getResponse().length(), 0);
            std::cout<<"the number of byts sent is: "<<r<<std::endl;
            close(client.getSocket());
            clientList.erase(i);
        }
    }
        return (0);
}
// int		acceptWRITEEsocket(fd_set *writeSet, Client& client, std::list<Client>& clientList, std::list<Client>::iterator& i)
// {
//     // std::cout<<"["<<client.getResponse().c_str()<<"]"<<"             :     "<<client.getResponse().size()<<std::endl;
    
//     return (0);
// }