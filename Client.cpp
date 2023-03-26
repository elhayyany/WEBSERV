/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hayy <ael-hayy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:43:01 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/03/24 15:04:29 by ael-hayy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

Client::Client(SOCKET socktt): sockett(socktt), jj(0)
{
    address_length = sizeof(address);
}
Client::Client(const Client& client): sockett(client.sockett)
{
    address_length = client.address_length;
    address = client.address;
}

Client::~Client()
{
}

const SOCKET&  Client::getSocket(){return sockett;}
void    Client::setSocket(SOCKET Sockett)
{
    sockett = Sockett;
}
const SOCKADDRE& Client::getAddress()
{
    return address;
}

const  socklen_t&  Client::getAddrtLen()
{
    return (address_length);
}

Request&    Client::getRequest()
{
    return (request);
}

std::string&    Client::getResponse() {return (response.getResponse());}