/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:39:54 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/05/14 16:22:00 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "includes.hpp"
class Client;
class Response
{
    public:
        Response(/* args */);
        ~Response();
        std::string&    getResponse();
		void fillTheBody(Client &client); 
        void fillTheHeader(Client &client);
        std::map<std::string, std::string> types;
        std::string&    getHeader();
        std::string&    getBody();
        void            setResponse(const std::string & str);
        std::string     getFileType(std::string filename);
		void checkRediraction(Client &client);
    private:
        std::string header;
        std::string body;
        std::string httpResponse;
};
std::string		find_filename(Client &client);
std::string setInfos_header(Client &client, std::string filename, int *Rvalue);
void			sendBody(Client& client);
void			sendHeader(Client& client);
void            changeTheHeaderby(Client &client, const std::string &element);
#endif