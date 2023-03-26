/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hayy <ael-hayy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:43:26 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/03/24 15:05:09 by ael-hayy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "Response.hpp"
#include "includes.hpp"

class Client
{
    public:
    Client(SOCKET sockett = -1);
    Client(const Client& client);
    
    ~Client();
    const SOCKET&  getSocket();
    void    setSocket(SOCKET sockett);
    const SOCKADDRE& getAddress();
    const socklen_t&  getAddrtLen();
    Request&     getRequest();
    std::string&    getResponse();
    bool    jj;

private:
    SOCKADDRE   address;
    SOCKET      sockett;
    socklen_t address_length;
    Request     request;
    Response    response;
};


#endif