/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandlGetRequest.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 18:02:37 by aaitbelh          #+#    #+#             */
/*   Updated: 2023/05/23 20:09:07 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../HEADERS/includes.hpp"

void			sendBody(Client& client)
{
	if(client.is_cgi && !client.cgi_finished)
		return;
	client.getRes().fillTheBody(client);
	std::string req =  client.getRes().getBody();
	int r = send(client.getSocket(), req.c_str(),  req.length(), 0);
	if(r <= 0)
		throw std::exception();
}
void			sendHeader(Client& client)
{
	if(!client.isitnew() || (client.is_cgi && !client.cgi_finished))
		return;
	std::string req = client.getRes().getHeader();
	int r = send(client.getSocket(), req.c_str(), req.length(), 0);
	client.isitnew() = 0;
	if(r <= 0)
		throw std::exception();
	
}
void	handlGetRequest(Client& client)
{
	if(!client.getRes().getHeader().empty())
		return;
	client.getRes().fillTheHeader(client);
}