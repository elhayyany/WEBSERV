#ifndef PARSING
#define PARSING

#include <iostream>	
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <utility>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sstream>

#define ELEM 21

using namespace std;
typedef struct s_location
{
	std::string location_path;
	std::map<std::string, std::list<std::string> > location_map;
	bool isExist(const std::string &element)
	{
		try
		{
			location_map.at(element);
		}
		catch(std::exception)
		{
			return 0;
		}
		return 1;
	}
	std::list<std::string>& getElemetnBylocation(const std::string& element)
	{
		return location_map[element];
	}

}t_location;
typedef struct s_server
{
	std::vector <std::string> each_server;
	std::map<std::string, std::list<std::string> > server_map;
	std::map<int, std::string>  error_page;
	std::vector <t_location> locations;
	std::list<std::string>& getFromServerMap(std::string element){
		return server_map[element];
	}
	std::string getErrorPage(int StatusCode);
	s_server &operator=(s_server &obj)
	{
		this->each_server = obj.each_server;
		this->server_map = obj.server_map;
		this->error_page = obj.error_page;
		this->locations = obj.locations;
		return *this;
	}
} t_server;



class ParsConf
{
	private :
		int count_server;
		int closed;
		std::vector <std::string> vec;
		int closed_brack;
	public :
	int count_location;
	std::vector <t_server> servers;
	std::vector <t_server> servers_;
	ParsConf():count_server(0), count_location(0), closed_brack(0){}
	// ParsConf &operator=(ParsConf &obj);
	void countserver(const char *av);
	void fill_server();
	void fill_server_element();
	void check_value(std::string &value);
	void check_duplcates(std::vector<std::string> &duplicate, int check);
	void check_element(std::vector<std::string> &vec);
	int check_validelem1(std::string str);
	int check_validelem(std::string str);
	std::list <std::string> split_string(std::string str);
	void brackets_errors();
	std::vector<t_server>& getServer();
	std::vector<t_location>::iterator getAll_locations(std::vector <t_server>::iterator it);
	void exec_cgi();
	void split_host();
	void check_host();
	// void limitlisten();
	ParsConf &operator=(const ParsConf &obj);
};
#endif