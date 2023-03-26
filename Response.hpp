/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hayy <ael-hayy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:39:54 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/03/22 15:54:54 by ael-hayy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <string>
class Response
{
public:
    Response(/* args */);
    ~Response();
    std::string&    getResponse();
private:
    std::string header;
};




#endif