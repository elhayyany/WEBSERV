/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:39:47 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/03/22 22:33:04 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

Request::Request(/* args */): resevedBytes(0)
{
    
}

Request::~Request()
{
}

std::map<std::string, std::string>& Request::getHeaderInfos()
{
    return HeaderInfos;
}
void Request::parseInfos()
{
    HeaderInfos["METHOD"] =  httpRequest.substr(0, httpRequest.find(" "));
    HeaderInfos["URI"] = httpRequest.substr(httpRequest.find(" ") + 1, httpRequest.find("HTTP") - httpRequest.find(" ", httpRequest.find(" ", httpRequest.find(" ") + 1)));
    HeaderInfos["Host"] = httpRequest.substr(httpRequest.find("Host: ") + 6, httpRequest.find(":", httpRequest.find("Host: ") + 6) - (httpRequest.find("Host: ") + 6));
    size_t pos = 0;
    pos = httpRequest.find("\r\n", httpRequest.find("\r\n") + 2) + 2;
    std::string httpRequestTmp = httpRequest.substr(pos);
    pos = 0;
    for(;httpRequestTmp.size();)
    {
        HeaderInfos[httpRequestTmp.substr(pos, httpRequestTmp.find(":"))] = httpRequestTmp.substr(httpRequestTmp.find(": ") + 2, httpRequestTmp.find("\r\n") - (httpRequestTmp.find(": ") + 2));
        httpRequestTmp = httpRequestTmp.substr(httpRequestTmp.find("\r\n") + 2);
        std::cout << httpRequestTmp << std::endl;
    }
}
size_t  Request::getLnght(){ return (resevedBytes);}
void    Request::addToReqyest(char *req, int r)
{
    httpRequest.append(req, r);
    resevedBytes += r;
}

void    Request::setType(REQUES_TYPE reqType) { type  = reqType;}
REQUES_TYPE    Request::getType() { return(type);}
const std::string&     Request::getHttpRequest() { return (httpRequest);}