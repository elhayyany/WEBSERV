/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:43:26 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/05/23 20:02:28 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "includes.hpp"
#include "../pars/parsing.hpp"

class Socket;
class Request;
class Response;
struct all_infos
{
	std::string host;
	std::string port;
	std::string server_name;
	std::string URI;
	std::string upload_pass;
	std::list<std::string> index_files;
	int allow_methods[3];
	bool autoindex;
	std::string root;
	std::string alias;
	size_t max_client_body_size;
	bool return_;
	std::string return__;
	bool cgi_pass_;
	double max_body_size;
	std::map<std::string, std::string> cgi_pass;
	struct s_location location_div;
	
	
};
class Client
{
    public:
    Client(void);
    explicit Client(SOCKET sockett);
    Client(const Client& client);
    
    ~Client();
	SOCKET&  getSocket();
    void    setSocket(SOCKET sockett);
    SOCKADDRE& getAddress();
    const socklen_t&  getAddrtLen();
    Request&     getRequest();
    std::string&    getResponse();
    Response&   getRes();
    std::map<std::string, std::string>& getHeaderInfos();
    int    writable;
    bool   is_dir;
	bool	requestvalid;
    std::fstream file;
    t_server server;
	bool& isitnew();
	ParsConf	parsingInfos;
	struct all_infos& GetClientinfos();
	std::string dir_body;
	std::string dirname;
	std::string file_path;
	pid_t cgi_pid;
	int&	getStatus();

	bool is_cgi;
	bool cgi_finished;
	std::string cgi_filename;
	void setAddrtLen(socklen_t len)
	{
		address_length = len;
	}
private:
    SOCKADDRE   address;
    SOCKET      sockett;
    socklen_t address_length;
    Request     request;
    Response    response;
	struct all_infos allinfos;
	bool is_new;
	int			status;
};


#endif