/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:39:47 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/05/23 21:25:56 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../HEADERS/includes.hpp"

Request::Request(/* args */): resevedBytes(0), chunkedSize(0), totalBytes(0)
{
    types_rev["text/html"] = "html";
    types_rev["text/html"] = "htm";
    types_rev["text/plain"] = "txt";
    types_rev["image/jpg"] = "jpg";
    types_rev["image/jpeg"] = "jpeg";
    types_rev["image/png"] = "png";
    types_rev["image/gif"] = "gif";
    types_rev["text/css"] = "css";
    types_rev["application/javascript"] = "js";
    types_rev["application/json"] = "json";
    types_rev["application/pdf"] = "pdf";
    types_rev["audio/mpeg"] = "mp3";
    types_rev["video/mp4"] = "mp4";
    types_rev["image/x-icon"] = "ico";
    types_rev["image/svg+xml"] = "svg";
    types_rev["application/octet-stream"] = "default";
    types_rev["application/x-httpd-php"] = "php";
    types_rev["application/x-perl"] = "pl";
}

Request::~Request()
{
}

std::map<std::string, std::string>& Request::getHeaderInfos()
{
    return HeaderInfos;
}

void Request::setToFile(const std::string&  str)
{
    if(HeaderInfos.count("Content-Length"))
    {
        fwrite(str.c_str(), sizeof(char), str.size(), tmp);
    }
}
int Request::checkRequest_validation(Client& client)
{
    int rvalue = 0;
    std::string allowedchars  = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
    if(HeaderInfos["METHOD"] != "GET" && HeaderInfos["METHOD"] != "POST" && HeaderInfos["METHOD"] != "DELETE")
    {
        client.getRes().getHeader() = setInfos_header(client, client.server.error_page[501], &rvalue);
        if(rvalue)
        changeTheHeaderby(client, "HTTP/1.1 501 Not Implemented");
        sendResponse(501, client);
        return 1;
    }
    if(HeaderInfos["VERSION"] != "HTTP/1.1" && HeaderInfos["VERSION"] != "HTTP/1.0")
    {
        client.getRes().getHeader() = setInfos_header(client, client.server.error_page[400], &rvalue);
        HeaderInfos["VERSION"] = "HTTP/1.1";
        if(rvalue)
            sendResponse(400, client);
        changeTheHeaderby(client, client.getHeaderInfos()["VERSION"] + "400 Bad Request");
        return 1;
    }
    for(size_t i = 0; i < HeaderInfos["URI"].size(); ++i)
    {
        if(allowedchars.find(HeaderInfos["URI"][i]) == std::string::npos)
        {
            client.getRes().getHeader() = setInfos_header(client, client.server.error_page[400], &rvalue);
            if(rvalue)
                sendResponse(400, client);
            changeTheHeaderby(client, client.getHeaderInfos()["VERSION"] + " 400 Bad Request");
            return 1;
        }
    }
    if(HeaderInfos["METHOD"] == "POST" && !rvalue)
    {
        if(!HeaderInfos.count("Content-Length") && !HeaderInfos.count("Transfer-Encoding")){
            client.getRes().getHeader() = setInfos_header(client, client.server.error_page[400], &rvalue);
            if(rvalue)
                rvalue = 400;
            changeTheHeaderby(client, client.getHeaderInfos()["VERSION"] + " 400 Bad Request");
            return 1;
        }
        else if(HeaderInfos.count("Transfer-Encoding") && HeaderInfos["Transfer-Encoding"] != "chunked"){
            sendResponse(501, client);
        }
        else if(HeaderInfos["Transfer-Encoding"] != "chunked" && std::atol(HeaderInfos["Content-Length"].c_str()) > client.GetClientinfos().max_client_body_size)
        {
            client.getRes().getHeader() = setInfos_header(client, client.server.error_page[413], &rvalue);
            if(rvalue)
                sendResponse(413, client);
            changeTheHeaderby(client, client.getHeaderInfos()["VERSION"] + " 413 Request Entity Too Large");
            return 1;
        }
        else if(HeaderInfos["URI"].length() > 2048){
            client.getRes().getHeader() = setInfos_header(client, client.server.error_page[414], &rvalue);
            changeTheHeaderby(client, client.getHeaderInfos()["VERSION"] + " 414 Request-URI Too Long");
            if(rvalue)
                sendResponse(414, client);
            return 1;
        }
    }
    return 0;
}
std::string GetquerySting(std::string &URI)
{
    if(URI.find("?") != std::string::npos)
    {
        std::string str;
        str = URI.substr(URI.find("?"));
        URI = URI.substr(0, URI.find("?"));
        return str;
    }
    return "";
}
void Request::parseInfos(std::list<Client>::iterator& i, std::list<Client>& clientList)
{
    size_t pos = httpRequest.find(" ");
    HeaderInfos["METHOD"] =  httpRequest.substr(0, pos);
    httpRequest = httpRequest.substr(pos + 1);
    HeaderInfos["URI"] = httpRequest.substr(0, httpRequest.find(" "));
    httpRequest = httpRequest.substr(httpRequest.find(" ") + 1);
    HeaderInfos["query"] = GetquerySting(HeaderInfos["URI"]);
	HeaderInfos["VERSION"] = httpRequest.substr(0, httpRequest.find("\r\n"));
    pos = httpRequest.find("\r\n") + 2;
    httpRequest = httpRequest.substr(pos);
    pos = 0;
    for(;httpRequest.size();)
    {
        if(httpRequest[0] == '\r' && httpRequest[1] == '\n')
        {
            httpRequest = httpRequest.substr(2);
            break;
        }
        HeaderInfos[httpRequest.substr(pos, httpRequest.find(":"))] = httpRequest.substr(httpRequest.find(": ") + 2, httpRequest.find("\r\n") - (httpRequest.find(": ") + 2));
        httpRequest = httpRequest.substr(httpRequest.find("\r\n") + 2);
    }
}
void    Request::openFile(std::string& extention, Client& client)
{
    std::stringstream ss;
    DIR *dir = opendir(client.GetClientinfos().upload_pass.c_str());
    if (!dir)
    {
        client.getStatus() = 403;
        sendResponse(403, client);
    }
    closedir(dir);
    ss << time(0);
    ss << ".";
    if (!extention.length())
        extention = "no_extention";
    ss << extention;
    std::string path = client.GetClientinfos().upload_pass + "/" + std::string(ss.str());
    MyFilename = path;
    if (access(path.c_str(), F_OK) == 0)
    {
        path = path.insert(0, "11");
    }
    MyFile.open(path, std::ios::app);
}

int stringToHexx(std::string& hexString)
{
    int intValue;

    std::stringstream ss;
    ss << std::hex << hexString; 
    ss >> intValue;
    return (intValue);
}

char    *Request::removeContentLinght(char *buffer, int *r)
{
    int i = 0;
    while (buffer[i] && buffer[i] != '\r' && buffer[i + 1] != '\n')
        i++;
    if (!buffer[i])
    {
        chunkOfChuk.append(buffer);
        return NULL;
    }
    std::string tem(buffer, buffer + i);
    i += 2;
    *r -= i;
    chunkedSize = stringToHexx(tem);
    if (!chunkedSize)
        throw exception();
    return buffer + i;
}
std::fstream&    Request::getMyfile(){return (MyFile);}

size_t& Request::getTotalBytes(){return (totalBytes);}

std::string& Request::getMyFilename(){return (MyFilename);}

void    Request::postRequestHandl(Client& client)
{
    char    *buffer = const_cast<char *>(httpRequest.c_str());
    int r = httpRequest.length();
    if (HeaderInfos["Transfer-Encoding"] != "chunked")
    {
        MyFile.write(buffer, r);
        resevedBytes += r;
        if (resevedBytes >= totalBytes)
            throw std::exception();
    }
    else
    {
            std::string tem(chunkOfChuk + std::string(buffer, r));
            if (chunkOfChuk.size())
            {
                r = tem.length();
                buffer = removeContentLinght(const_cast<char *>(tem.c_str()), &r);
                if (resevedBytes + chunkedSize > client.GetClientinfos().max_body_size)
                {
                    client.getStatus() = 413;
                    throw std::exception();
                }
                if (!buffer)
                    return ;
            }
            if (chunkedSize < r && chunkedSize > 0)
            {
                MyFile.write(buffer, chunkedSize);
                buffer = buffer + chunkedSize + 2;
                r -= chunkedSize + 2;
                chunkedSize = 0;
                chunkOfChuk.clear();
				resevedBytes += chunkedSize;
            }
            while (r > 0)
            {
                if (chunkedSize <= 0)
                    buffer = removeContentLinght(const_cast<char*>(buffer), &r);
                if (!buffer)
                    return ;
                if (r <= chunkedSize){
                    MyFile.write(buffer, r);
                    chunkedSize -= r;
                    chunkOfChuk.clear();
                    resevedBytes += r;
                    r = 0;
                }
                else
                {
                    MyFile.write(buffer, chunkedSize);
                    buffer = buffer + chunkedSize + 2;
                    r -= chunkedSize + 2;
                    chunkedSize = 0;
                    chunkOfChuk.clear();
                    resevedBytes += chunkedSize;
                }
            }
    }
}


size_t  Request::getLnght(){ return (resevedBytes);}
void    Request::addToReqyest(const char *req, int r)
{
    httpRequest.clear();
    httpRequest.append(req, r);
}

void    Request::setType(REQUES_TYPE reqType) { type  = reqType;}
REQUES_TYPE    Request::getType() { return(type);}
const std::string&     Request::getHttpRequest() { return (httpRequest);}

size_t  Request::getResevedByts() {return (resevedBytes);}

void setTherootLocation(Client &client, std::vector<t_server>& servers)
{
	for(size_t i = 0; i < servers.size(); ++i)
	{
		for(std::vector<t_location>::iterator it = servers[i].locations.begin();it != servers[i].locations.end(); ++it)
		{
			if(it->location_path == "/")
			{
				client.GetClientinfos().location_div = *it;
				return ;
			}
		}
	}
}

std::vector<std::string> PathTovector(std::string element)
{
    std::vector<std::string> path;
    std::string tmp;
    for(size_t i = 0; i < element.length();)
    {
        while (element[i] && element[i] == '/')
            i++;
        while (element[i] && element[i] != '/')
            tmp += element[i++];
        if (tmp.length())
        {
            path.push_back(tmp);
            tmp.clear();
        }
        if(tmp == "..")
            throw std::exception();
    }
    return (path);
}
bool compare(t_location& a, t_location& b)
{
    return (a.location_path.length() > b.location_path.length());
}

std::vector<t_location>::iterator getTheLocation(Client& client, t_server &server)
{
    std::string tmp = client.GetClientinfos().URI;
    sort(server.locations.begin(), server.locations.end(), compare);
    std::vector<t_location>::iterator it_tmp = server.locations.end();
     std::vector<std::string> URI_path;
    try
    {
        URI_path = PathTovector(tmp);
    }
    catch(const std::exception& e)
    {
        sendResponse(400, client);
    }
    for(std::vector<t_location>::iterator it = server.locations.begin(); it != server.locations.end(); ++it)
    {
        std::vector<std::string> path = PathTovector(it->location_path);
        if(path.size() > URI_path.size())
            continue;
        if(std::equal(path.begin(), path.end(), URI_path.begin()))
        {
            std::string tmp;
            for(size_t i = 0; i < path.size(); ++i)
                tmp += "/" + URI_path[i];
            std::vector<std::string> root = PathTovector(it->getElemetnBylocation("root").front());
            std::string root_path;
            for(size_t i = 0; i < root.size(); ++i)
                root_path += "/" + root[i];
            client.GetClientinfos().alias = root_path;
            for(size_t i = path.size(); i < URI_path.size(); ++i)
                client.GetClientinfos().alias += "/" + URI_path[i];
            client.GetClientinfos().root = root_path;
            return (it);
        }

    }
    return (server.locations.begin());
}

void	matchTheLocation(Client& client, std::vector<t_server> servers)
{
	bool isfounded = 0;
	struct all_infos rootLocation;
	std::string path;
	for(size_t i = 0; i < servers.size(); ++i)
	{
        std::vector<t_location>::iterator it = getTheLocation(client, servers[i]);
        client.server = servers[i];
		if(it != servers[i].locations.end())
		{
			isfounded = 1;
            struct all_infos &tmpstruct = client.GetClientinfos();
			tmpstruct.server_name = servers[i].server_map["server_name"].front();
			if(it->isExist("allow_methods"))
			{
				std::list<std::string> lst = it->getElemetnBylocation("allow_methods");
				if(lst.size())
				{
					for(std::list<std::string>::iterator lst_it = lst.begin(); lst_it != lst.end(); ++lst_it)
					{
						if(*lst_it == "GET")
							tmpstruct.allow_methods[0] = 1;
						else if(*lst_it == "POST")
							tmpstruct.allow_methods[1] = 1;
						else if(*lst_it == "DELETE")
							tmpstruct.allow_methods[2] = 1;
					}
				}
				lst.clear();
			}
			else
				for(size_t i = 0; i < 3; ++i)
					tmpstruct.allow_methods[i] = 1;
			if(it->isExist("autoindex") && it->getElemetnBylocation("autoindex").front() == "1")
				tmpstruct.autoindex = 1;
			else
				tmpstruct.autoindex = 0;
			if(it->isExist("return"))
			{
				tmpstruct.return_ = 1;
				tmpstruct.return__ = it->getElemetnBylocation("return").front();
			}
			else
				tmpstruct.return_ = 0;
			if(it->isExist("php") || it->isExist("pl"))
			{
				tmpstruct.cgi_pass_ = 1;
                if(it->isExist("php"))
                    tmpstruct.cgi_pass["php"] = *it->getElemetnBylocation("php").begin();
                if(it->isExist("pl"))
                    tmpstruct.cgi_pass["pl"] = *it->getElemetnBylocation("pl").begin();
			}
			else
				tmpstruct.cgi_pass_ = 0;
			tmpstruct.index_files = it->getElemetnBylocation("index");
			if(it->isExist("root"))
            {
				tmpstruct.root = it->getElemetnBylocation("root").front();
            }
            if(it->isExist("upload_pass"))
            {
                tmpstruct.upload_pass = it->getElemetnBylocation("upload_pass").front();
            }
            else
                tmpstruct.upload_pass = "";
			tmpstruct.location_div = *it;
            tmpstruct.max_client_body_size = std::atol(servers[i].getFromServerMap("max_client_body_size").front().c_str());
			return ;
		}	
	}
	if(!isfounded)
    {
        int rvalue = 0;
        client.GetClientinfos().server_name = servers[0].server_map["server_name"].front();
        client.getRes().getHeader() =  setInfos_header(client, client.server.error_page[404], &rvalue);
        changeTheHeaderby(client, client.getHeaderInfos()["VERSION"] + " 404 Not Found");
        if(rvalue)
            sendResponse(404, client);
    }
}

std::vector<t_server> GettheServer(ParsConf &pars, Client &client)
{
	std::vector<t_server> servers;
	std::string::iterator it =  client.GetClientinfos().host.begin() + client.GetClientinfos().host.find(":");
    client.GetClientinfos().port = client.GetClientinfos().host.substr(it - client.GetClientinfos().host.begin() + 1);
	client.GetClientinfos().host.erase(it, client.GetClientinfos().host.end());
	for(size_t i = 0; i < pars.servers.size(); ++i)
	{
		if(client.GetClientinfos().host == pars.servers[i].getFromServerMap("host").front() && client.GetClientinfos().port == pars.servers[i].getFromServerMap("listen").front())
			servers.push_back(pars.servers[i]);
	}
	return servers;
}
void        Request::setAllinfos(Client &client)
{
	struct all_infos &tmpstruct = client.GetClientinfos();
	struct ParsConf &tmp_parsstruct = client.parsingInfos;
	tmpstruct.host = this->getHeaderInfos()["Host"];
	tmpstruct.URI = this->getHeaderInfos()["URI"];
	std::vector<t_server> servers = GettheServer(client.parsingInfos, client);
	matchTheLocation(client, servers);
}
std::string generaterandname()
{
    std::string str;
    std::srand(std::time(0));
    std::stringstream ss;
    ss << std::rand();
    ss >> str;
    return str;
}
void Request::free_all()
{
    int i = 0;
    while(i < 2)
    {
        free(env[i]);
        i++;
    }
    free(env);
}
void Request::exec_cgi(Client &client)
{
    if(client.is_cgi == false)
    {
        time(&start);
	    env = (char **)malloc(sizeof(char **) * 3);
        client.cgi_filename = generaterandname();
	    int fd = open(client.cgi_filename.c_str(), O_TRUNC | O_RDWR | O_CREAT, 0666);
        int fd2 = 0;
        env[0] = strdup(("HTTP_COOKIE="+HeaderInfos["Cookie"]).c_str()); 
        if(HeaderInfos["METHOD"] == "POST")
        {
            if(client.getRes().getFileType(MyFilename) == "text/php")
                arg[0] =  strdup(client.GetClientinfos().cgi_pass["php"].c_str());
            else if(client.getRes().getFileType(MyFilename) == "text/perl")
                arg[0] =  strdup(client.GetClientinfos().cgi_pass["pl"].c_str());
            env[1] = strdup(("PATH_INFO="+ MyFilename).c_str());
	        arg[1] = strdup(MyFilename.c_str());
            fd2 = open(MyFilename.c_str(), O_RDWR | O_CREAT ); 
        }
        else
        {
            if(client.getRes().getFileType(client.file_path) == "text/php")
                arg[0] =  strdup(client.GetClientinfos().cgi_pass["php"].c_str());
            else if(client.getRes().getFileType(client.file_path) == "text/perl")
                arg[0] =  strdup(client.GetClientinfos().cgi_pass["pl"].c_str());
            env[1] = strdup(("PATH_INFO="+ client.file_path).c_str());
	        arg[1] = strdup(client.file_path.c_str());
        }
	    env[2] = NULL;
	    arg[2] = NULL;
        std::cout << arg[0] << std::endl;
	    client.cgi_pid = fork();
        if(client.cgi_pid == 0)
	    {
	    	dup2(fd, 1);
            if(HeaderInfos["METHOD"] == "POST")
                dup2(fd2, 0);
	    	if(execve(arg[0], arg, env) == -1)
                std::cerr << "execve failure" <<std::endl;
            exit(10);
	    }
        client.is_cgi = true;
    }
    int status = -1;
    time(&end);
    if (double(end - start) > 5)
    {
        std::cout << "time out" << std::endl;
        kill(client.cgi_pid, SIGKILL);
        remove(client.cgi_filename.c_str());
        sendResponse(408, client);
    }
    waitpid(client.cgi_pid, &status, WNOHANG);
    if(WIFEXITED(status))
    {
        if(WEXITSTATUS(status) == 10)
            sendResponse(500, client);
        free_all();
        free(arg[0]);
        free(arg[1]);
        if(HeaderInfos["METHOD"] == "GET") {
            std::string head;
            std::string &header = client.getRes().getHeader();
            struct stat b;
            std::stringstream s;
            std::ifstream tmpfile(client.cgi_filename);
            std::string strbuf;
            char charbuf[1024];
            stat(client.cgi_filename.c_str(), &b);
            while(!tmpfile.eof()) {
                tmpfile.read(charbuf, 1024);
                strbuf.append(charbuf, tmpfile.gcount());
                if(strbuf.find("\r\n") != std::string::npos) {
                    s << b.st_size - strbuf.find("\r\n\r\n") - 4;
                    break ;
                }        
            }
            int Rvalue;
            header = setInfos_header(client, client.cgi_filename, &Rvalue);
            if(Rvalue)
                sendResponse(Rvalue, client);
            header.append("Content-Length: " + s.str() + "\r\n");
            client.cgi_finished = true;
        }
    }
}

bool Request::isAllowedMethod(Client &client)
{
    if(HeaderInfos["METHOD"] == "GET" && !client.GetClientinfos().allow_methods[0])
        return false;
    else if(HeaderInfos["METHOD"] == "POST" && !client.GetClientinfos().allow_methods[1])
        return false;
    else if(HeaderInfos["METHOD"] == "DELETE" && !client.GetClientinfos().allow_methods[2])
        return false;
    return true;
}
