/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hayy <ael-hayy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:43:30 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/03/24 15:25:02 by ael-hayy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

int main()
{
    Socket  socketListen("0", "8080");
    if (socketListen() < 0)
        return (-1);
    std::list<Client>   clientList;
    // int l = 0;
    while (true)
    {
        fd_set  readSet;
        fd_set  writeSet;
        setSocketForReadAndWrite(&readSet, &writeSet, socketListen());
        if (waitingForClients(&readSet, &writeSet, socketListen(), clientList) < 0)
            return (-1);
        acceptNewConnictions(&readSet, &writeSet, socketListen(), clientList);
        // std::cout<<"Clinet list size: "<<clientList.size()<<std::endl;
        std::list<Client>::iterator i = clientList.begin(), j;
        j = i;
        while (i != clientList.end())
        {
            j++;
            acceptREADsocket(&readSet,&writeSet, *i, clientList, i);
            // acceptWRITEEsocket(, *i, clientList, i);
            i = j;
        }
        
    }
    return (0);
}