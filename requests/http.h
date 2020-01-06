#include <string>
#include <vector>
#include <map>
#include <memory>
#include <Windows.h>
#include "wininet.h"
#include "BinaryData.h"
#include "utils.h"
#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "ws2_32.lib")
using namespace std;
class Response
{
public:
	Response();
	Response(shared_ptr<BinaryData> rep);
	Response(std::string &h,shared_ptr<BinaryData> data);
	~Response();
	string	GetText();
	map<string, string>	Header();
	const  byte*GetBinary();
	unsigned int size();
	string	operator[](string key);
private:
	map<string, string>	header;
	string	text;
	shared_ptr<BinaryData>	pContent;
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
Response    https_get(string url,map<string, string> &head = map<string, string>());
Response	https_post(string url, BinaryData &data,const map<string, string> &head = map<string, string>());
Response	request(string method,string url, BinaryData &data,map<string, string> &head = map<string, string>());
Response	https_send(string method, string url, BinaryData &data,map<string, string> &head = map<string, string>());