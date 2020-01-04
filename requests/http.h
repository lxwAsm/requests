#include <string>
#include <vector>
#include <map>
#include <Windows.h>
#include "wininet.h"
#include "BinaryData.h"
#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "ws2_32.lib")
using namespace std;
class Response
{
public:
	Response();
	Response(BinaryData rep);
	Response(std::string &h, BinaryData data);
	~Response();
	string	GetText();
	const  byte*GetBinary();
	unsigned int size();
	string	operator[](string key);
private:
	map<string, string>	header;
	string	text;
	BinaryData	pContent;
	void SplitString(const string& s, vector<string>& v, const string& c);
};

class Request
{
public:
	Request(std::string url,std::string method,map<string, string> &header = map<string, string>());
	~Request();
	string	HeaderToString();
	string  GetRequest(const string &data);
	std::string	url;
	std::string	domain;
	std::string param;
	map<string, string>	header;
	int	port;
	std::string method;
};

std::string GetIpByDomainName(const char *szHost);
Response	Get(std::string url, const map<string,string> &head = map<string,string>());
Response	Post(std::string url, const string &data,map<string, string> &head = map<string, string>());
Response    https_get(const string &url);