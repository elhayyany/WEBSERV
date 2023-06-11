/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 22:12:50 by mamellal          #+#    #+#             */
/*   Updated: 2023/05/23 18:49:20 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"
void ParsConf::countserver( const char *av)
{
	fstream file;
	std::string str;
	file.open(av);
	if(!file.good())
	{
		std::cout << "file does not exists"<< std::endl;
		exit (0);
	}
	while(getline(file, str))
	{    
		if(str == "server")
			count_server++;
		vec.push_back(str);
	}
	if(!count_server || vec[0] != "server"){
		std::cout << "There is no server"<< std::endl;
		exit (0);
	}
	brackets_errors();
}
void ParsConf::fill_server()
{
	t_server tmp_server;
	for(unsigned int j = 0; j < vec.size() ; j++)
	{
		if(vec[j] == "};")
		{
			servers_.push_back(tmp_server);
			tmp_server.each_server.clear();
		}
		tmp_server.each_server.push_back(vec[j]);
	}
	fill_server_element();
}

void ParsConf::fill_server_element()
{
	std::vector<std::string> duplicate;
	std::vector<std::string> duplocation;
	std::string arr[4] = {"host", "listen", "max_client_body_size", "server_name"};
	std::string loc_arr[7] = {"allow_methods", "autoindex", "root", "upload_pass", "return", "cgi_pass", "index"};
	t_location tmp_location;
	for (unsigned int i = 0; i < servers_.size() ; i++)
	{
		count_location = 0;
		closed = 0;
		for(unsigned int j = 0;j < servers_[i].each_server.size() ;j++)
		{
			std::list <std::string> mylist = split_string(servers_[i].each_server[j]);
			std::list<std::string>::iterator it;
			int index = check_validelem(mylist.front());
			if(index != -1)
			{
				if(index != 3 && mylist.size() != 2)
				{
					std::cout << "value of " << arr[index] << " not found or there is a lot of values"<<std::endl;
					exit(0);
				}
				if(index == 3 && mylist.size() < 2)
				{
					std::cout << "value of " << arr[index] << " not found"<<std::endl;
					exit(0);
				}
				duplicate.push_back(mylist.front());
				mylist.pop_front();
				if(index != 3)
					check_value(mylist.front());
				servers_[i].server_map.insert(std::pair<std::string, std::list<string> >(arr[index], mylist));
			}
			else if(mylist.front() == "error_page")
			{
				if(mylist.size() != 3)
				{
					std::cout << "value of " <<arr[index] << " not found or there is a lot of values"<<std::endl;
					exit(0);
				}
					mylist.pop_front();
					duplicate.push_back(mylist.front());
					std::list<std::string>::iterator it = mylist.begin();
					it++;
					servers_[i].error_page.insert(std::pair<int, std::string>(atoi(mylist.front().c_str()), *it));
			}
			else if(mylist.front() == "location")
			{
				if(mylist.size() != 2)
				{
					std::cout << "value of location not found or there is a lot of values"<<std::endl;
					exit(0);
				}
				count_location += 1;
				duplicate.push_back(mylist.front());
				mylist.pop_front();
				tmp_location.location_path = mylist.front();
				duplicate.push_back(tmp_location.location_path);
			}
			if(mylist.front() == "},")
			{
				closed += 1;
				check_duplcates(duplocation, 1);
				duplocation.clear();
				servers_[i].locations.push_back(tmp_location);
				tmp_location.location_map.clear();
				tmp_location.location_path.clear();
			}
			int _index = check_validelem1(mylist.front());
			if(_index != -1)
			{
				std::string key = mylist.front();
				if(_index != 5)
					duplocation.push_back(mylist.front());
				else if(_index == 5)
				{
					mylist.pop_front();
					key = mylist.front();
				}
				mylist.pop_front();
				if(_index == 1)
				{
					int autoi = atoi((mylist.front()).c_str()) ;
					if(autoi != 1 && autoi != 0)
						throw std::out_of_range("check auto index");
				}
				tmp_location.location_map.insert(std::pair<std::string, std::list<string> >(key, mylist));
			}
		}
		split_host();
		check_duplcates(duplicate, 0);
		// limitlisten();
		duplicate.clear();
		if(closed != count_location || closed_brack != count_server)
		{
			std::cout << "locations or server: syntax error" << std::endl;
			exit (0);
		}
	}
	check_host();
}


void ParsConf::check_host()
{
	servers.push_back(servers_[0]);
	for(unsigned int i = 0; i < servers_.size(); i++)
	{
		int r = 0;
		for(int j = 0; j < servers.size();j++)
		{
			if(servers[j].server_map["listen"].front() == servers_[i].server_map["listen"].front())
			{
				r++;
			}
		}
		int listen = atoi((servers_[i].server_map["listen"].front()).c_str());
		int max_body = atoi((servers_[i].server_map["max_client_body_size"].front()).c_str());
		if(listen > 65535 || listen < 0)
			throw std::out_of_range("check port please");
		if(max_body > 1000000 || listen < 0)
			throw std::out_of_range("check port body size please");
		if(r == 0)
			servers.push_back(servers_[i]);
	}
	servers_.clear();
}
void ParsConf::split_host()
{
	std::string s;
	size_t _find = 0;
	for(int i = 0; i < servers.size();i++)
	{
		std::string str = servers[i].server_map["host"].front();
		int j = 0;
		while((_find = str.find('.')) != std::string::npos)
		{
			s = str.substr(0, _find);
			if(atoi(s.c_str()) > 256 || atoi(s.c_str()) < 0)
				throw std::out_of_range("check host please!!");
		}
	}
}
ParsConf &ParsConf::operator=(const ParsConf &obj)
{
	count_server = obj.count_server;
	closed = obj.closed;
	this->vec = obj.vec;
	servers = obj.servers;
	// servers_ = obj.servers_;
	return *this;
}
void ParsConf::check_value(std::string &value)
{
	for(unsigned int i = 0; i < value.size(); i++)
	{
		if(value[i] == '.')
		{
			if(i == value.size())
				throw std::out_of_range("there is dot at end of host");
			else if(value[i + 1] == '.')
				throw std::out_of_range("there is a series of dots in host");
		}
		if(!isdigit(value[i]) && value[i] != '.')
		{
			std::cout << "value format is incorrect" << std::endl;
			exit (0);
		}
	}
}
void ParsConf::check_duplcates(std::vector<std::string> &duplicate, int check)
{
	for (std::vector<std::string>::size_type i = 0; i < duplicate.size(); ++i) {
	    for (std::vector<std::string>::size_type j = i + 1; j < duplicate.size(); ++j) {
	        if (duplicate[i] == duplicate[j] && duplicate[i] != "location") {
	            std::cout << "there is duplicate" << std::endl;
				exit(0);
	        }
	    }
	}
	if(!check)
		check_element(duplicate);
	else if(check)
	{
		std::vector<std::string>::iterator it;
		std::vector<std::string>::iterator it1;
		it = find(duplicate.begin(), duplicate.end(), "root");
		it1 = find(duplicate.begin(), duplicate.end(), "index");
		if(it == duplicate.end())
			throw std::out_of_range("there is no root in your location");
		if(it1 == duplicate.end())
			throw std::out_of_range("there is no index in your location");
		
	}
}

void ParsConf::check_element(std::vector<std::string> &vec)
{
	std::string arr[5] = {"host", "listen", "max_client_body_size", "server_name", "location"};
	int i = -1;
	while(++i < 5)
	{		
		if (std::find(vec.begin(), vec.end(), arr[i]) == vec.end()){
			std::cout << "your element in your file not enough"<< std::endl;
			exit (0);
		}
	}
}
int ParsConf::check_validelem1(std::string str)
{
	std::string arr[7] = {"allow_methods", "autoindex", "root", "upload_pass", "return", "cgi_pass", "index"};
	int i = -1;
	while(++i < 7)
	{
		if(arr[i] == str)
			return (i);
	}
	return (-1);
}

int ParsConf::check_validelem(std::string str)
{
	std::string arr[4] = {"host", "listen", "max_client_body_size", "server_name"};
	unsigned int i = 0;
	while(i < arr->size())
	{
		if(arr[i] == str)
			return (i);
		i++;
	}
	return (-1);
}

std::list <std::string> ParsConf::split_string(std::string str)
{
	std::string buffer;
	std::list <std::string> mylist;
	for(unsigned int i = 0; i < str.size(); i++)
	{
		while(isspace(str[i]))
			i++;
		while(!isspace(str[i]) && str[i])
			buffer += str[i++];
		if(buffer != "{" && buffer != "\n")
		{
			if(buffer.size())
			{
				mylist.push_back(buffer);
				buffer.clear();
			}
		}
	}
	return (mylist);
}
void ParsConf::brackets_errors()
{
	unsigned int i = 0;
	int open = 0;
	while(i < vec.size())
	{
		if(vec[i] == "{" && !open)
			open = 1;
		else if((vec[i] == "{" && open))
		{
			std::cout << "there is problem in brackets "<< std::endl;
			exit (0);
		}
		if(vec[i] == "};" && open)
		{
			open = 0;
			closed_brack++;
		}
		else if(vec[i] == "};" && !open)
		{
			std::cout <<"there is problem in brackets"<< std::endl;
			exit (0);
		}
		i++;
	}
}
