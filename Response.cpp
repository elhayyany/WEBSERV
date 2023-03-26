/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hayy <ael-hayy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:39:51 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/03/24 14:38:46 by ael-hayy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <iostream>
Response::Response(/* args */):header("HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Length: 190\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\n<html>\n<body>\n\n<h1>My First Heading</h1>\n\n<p>My first paragraph.</p>\n\n</body>\n</html>")
{
}

Response::~Response()
{
}

std::string&    Response::getResponse() {return (header);};
