/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:41:50 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/05/23 17:51:51 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../HEADERS/Server.hpp"

Server& Server::operator=(const Server& srv)
{
    return(*this);
}
void Server::serverRun(t_server &server)
{
    fd_set  readSet;
    fd_set  writeSet;
    setSocketForReadAndWrite(&readSet, &writeSet, socketListen());
    waitingForClients(&readSet, &writeSet, socketListen(), clientList);
    acceptNewConnictions(&readSet, &writeSet, socketListen(), clientList);
    signal(SIGPIPE, SIG_IGN);
    if (clientList.size())
    {
        std::list<Client>::iterator i = clientList.begin(), j;
        clientList.begin()->parsingInfos = this->pars;
        j = i;
        while (i != clientList.end())
        {
            ++j;
            acceptREADsocket(&readSet,&writeSet, *i, clientList, i);
            i = j;
        }
    }
}
Server::Server(std::string host, std::string port):socketListen(host.c_str(), port.c_str()), clientList()
{
    if (socketListen() < 0)
        throw std::exception();
}
void    Server::operator()(std::string host, std::string port)
{
    socketListen(host.c_str(), port.c_str());
    if (socketListen() < 0)
        throw std::exception();
}
Server::Server()
{
}
Server::~Server()
{
}

int main(int ac, char **av)
{
    try
    {
        signal(SIGPIPE, SIG_IGN);
        ParsConf pars;
        if (av[1])
            pars.countserver(av[1]);
        else
            pars.countserver("./pars/config.conf");
        pars.fill_server();
        size_t  sx = 0;
        Server  servers[pars.servers.size()];
        for (size_t i = 0; i < pars.servers.size(); i++)
        {
            servers[i](pars.servers[i].server_map["host"].front(),\
                    pars.servers[i].server_map["listen"].front());
            servers[i].pars = pars;
        }
        
        while(true)
        {
            servers[sx % pars.servers.size()].serverRun(pars.servers[sx % pars.servers.size()]);
            sx++;
        }
        return (0);
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}