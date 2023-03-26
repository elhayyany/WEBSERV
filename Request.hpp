/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:39:49 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/03/22 17:45:11 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "includes.hpp"


class Request
{
public:
    Request(/* args */);
    ~Request();
    size_t                  getLnght();
    void                    addToReqyest(char *req, int r);
    void                    setType(REQUES_TYPE reqType);
    REQUES_TYPE             getType();
    const std::string&     getHttpRequest();
    void parseInfos();
    std::map<std::string, std::string>& getHeaderInfos();
    
private:
    REQUES_TYPE type;
    size_t      resevedBytes;
    int         fd;
    std::string httpRequest;
    std::map<std::string, std::string> HeaderInfos;
    static int        status;
    //...
};





#endif