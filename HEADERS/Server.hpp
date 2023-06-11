/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamellal <mamellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:43:32 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/05/17 17:19:53 by mamellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "includes.hpp"
#include "../pars/parsing.hpp"

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

class Server
{
    private:
        Socket  socketListen;
        std::list<Client>   clientList;
        t_server server;
    public:
        Server();
        Server(std::string host, std::string port);
        void    operator()(std::string host, std::string port);
        Server& operator=(const Server& srv);
        void serverRun(t_server &server);
		ParsConf pars;
        ~Server();
};


#endif