/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandDeleteRequest.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:37:36 by aaitbelh          #+#    #+#             */
/*   Updated: 2023/05/21 17:49:01 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../HEADERS/includes.hpp"

void 			handlDeleteRequest(Client& client)
{
    std::fstream file;
    std::string filename = find_filename(client);
    int Rvalue = 0;
    file.open(filename);
    struct stat buffer;
    stat(filename.c_str(), &buffer);
    if(!access(filename.c_str(), F_OK) && !access(filename.c_str(), W_OK))
    {
        if(S_ISDIR(buffer.st_mode))
        {
            int len = 0;
            DIR *dir = NULL;
            dir = opendir(filename.c_str());
            if(!dir)
            {
                if(!client.server.error_page.count(404))
                    sendResponse(404, client);
                client.getRes().getHeader() = setInfos_header(client, client.server.error_page[404], &Rvalue);   
                changeTheHeaderby(client, client.getHeaderInfos()["VERSION"] + " 404 Not Found");
                if(Rvalue)
                    sendResponse(404, client);
                return ;
            }
            struct dirent *ent = NULL;
            while((ent = readdir(dir)) != NULL)
            {
                if(ent->d_name[0] != '.')
                    len++;
            }
            if(len)
            {
                if(!client.server.error_page.count(409))
                    sendResponse(409, client);
                client.getRes().getHeader() = setInfos_header(client, client.server.error_page[409], &Rvalue);
                if(Rvalue)
                    sendResponse(409, client);
                changeTheHeaderby(client, client.getHeaderInfos()["VERSION"] + " 409 Conflict");
                return ;
            }
            rmdir(filename.c_str());
        }
        else
            remove(filename.c_str());    
        sendResponse(204, client);
    }
    else
    {
        if(!access(filename.c_str(), F_OK) && access(filename.c_str(), R_OK))
        {
            if(!client.server.error_page.count(403))
                sendResponse(403, client);
            client.getRes().getHeader() = setInfos_header(client, client.server.error_page[403], &Rvalue);
            changeTheHeaderby(client, client.getHeaderInfos()["VERSION"] + " 403 Forbiden");
            if(Rvalue)
                sendResponse(403, client);
            return ;
        }
        if(!client.server.error_page.count(404))
            sendResponse(404, client);
        client.getRes().getHeader() = setInfos_header(client, client.server.error_page[404], &Rvalue);
        changeTheHeaderby(client, client.getHeaderInfos()["VERSION"] + " 403 Forbiden");
        if(Rvalue)
            sendResponse(404, client);
    }
}