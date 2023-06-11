/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:42:56 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/05/11 13:28:20 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP
#include "includes.hpp"

#define __BADREQUEST 400
#define __FORBIDDEN 403
#define __NOTFOUND 404
#define __METHODNOTALLOWED 405
#define __REQUESTTIMEOUT   408
#define __BADGATEWAY       502
#define __SUCCESS          200
#define __NOTIMPLEMENTED   501
#define __CONFLICT         409
#define __NOCONTENT        204
#define __REQUESTTOOLARGE  413
class Socket
{
private:
	SOCKET  socketfd;
public:
	Socket(std::string host, std::string service);
	~Socket();
	Socket();
	void    creatSocket(std::string& host, std::string& service);
	SOCKET  operator()();
	void  operator()(std::string host, std::string service);
};


int				waitingForClients(fd_set *readSet, fd_set *writeSet, SOCKET socketListen, std::list<Client>& clientList);
void			setSocketForReadAndWrite(fd_set *readSet, fd_set *writeSet, SOCKET socketListen);
int				acceptREADsocket(fd_set *readSet, fd_set *writeSet, Client& client, std::list<Client>& clientList, std::list<Client>::iterator& i);
int				acceptWRITEEsocket(fd_set *writeSet, Client& client, std::list<Client>& clientList, std::list<Client>::iterator& i);
int				acceptNewConnictions(fd_set *readSet, fd_set *writeSet, SOCKET socketListen, std::list<Client>& clientList);
char*			get_name(Client& client);
void 			handlGetRequest(Client& client);
void 			handlDeleteRequest(Client& client);
void 			sendResponse(int status, Client& client);
#endif