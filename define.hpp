/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:11:52 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/03/22 13:49:04 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_HPP
#define DEFINE_HPP


//* DEFINING ...


#define     POST_REQ    0
#define     GET_REQ     1
#define     DELETE_REQ  2


//status 1 for finished body and 0 for not finished
#define     FINISHED_BODY 1
#define     NOT_FINISHED_BODY 0

#define     BUFFER_SIZE 1024
//* TYPEDEF ....
typedef struct sockaddr_storage SOCKADDRE;

typedef     int         REQUES_TYPE;

typedef     int         SOCKET;

#endif