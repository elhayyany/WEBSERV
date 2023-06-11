/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamellal <mamellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/05/22 17:36:14 by mamellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "includes.hpp"

class Client;
class Request
{
public:
    Request(/* args */);
    ~Request();
    size_t                  getLnght();
    void                    addToReqyest(const char *req, int r);
    void                    setType(REQUES_TYPE reqType);
    REQUES_TYPE             getType();
    std::string&            getMyFilename();
    const std::string&     getHttpRequest();
    void parseInfos(std::list<Client>::iterator& i, std::list<Client>& clientList);
    std::map<std::string, std::string>  types_rev;
    std::map<std::string, std::string>& getHeaderInfos();
    void setToFile(const std::string& str);
    size_t  getResevedByts();
    void    postRequestHandl(Client& client);
	std::fstream&    getMyfile();
    void    setAllinfos(Client &client);
    char    *removeContentLinght(char *buffer, int *r);
    void    openFile(std::string& extention, Client& client);
    int     checkRequest_validation(Client& client);
    void    exec_cgi(Client &client);
    size_t&  getTotalBytes();
    bool     isAllowedMethod(Client &client);
    void free_all();
private:
    REQUES_TYPE type;
    size_t      resevedBytes;
    size_t      totalBytes;
    int         fd;
    std::string httpRequest;
    std::map<std::string, std::string> HeaderInfos;
    static int        status;
    std::string         chunkOfChuk;
    int              chunkedSize;
    FILE             *tmp;
    std::fstream MyFile;
    std::string MyFilename;
    char **env;
    time_t start, end;
    char *arg[3];
};
std::vector<std::string> PathTovector(std::string element);
#endif
