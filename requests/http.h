#pragma once
#ifndef http996
#define http996
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <fstream>
#include <Windows.h>
#include "wininet.h"
#include "BinaryData.h"
#include "utils.h"
#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "ws2_32.lib")
using namespace std;
namespace requests{
	class Response
	{
	public:
		Response();
		Response(std::shared_ptr<BinaryData> rep);
		Response(std::string &h, std::shared_ptr<BinaryData> data);
		~Response();
		std::string	GetText();
		map<std::string,std::string>	Header();
		const  byte*GetBinary();
		unsigned int size();
		std::string	operator[](std::string key);
		unsigned int status;
	private:
		map<std::string,std::string>	header;
		std:: string	text;
		std::shared_ptr<BinaryData>	pContent;
		void SplitString(const string& s, vector<string>& v, const string& c);
		unsigned int	status2int(string &s);
	};

	class Request
	{
	public:
		Request(std::string url, std::string method,map<string, string> &header = map<string, string>(), map<string, string> &options = map<string, string>());
		~Request();
		string	HeaderToString();
		void  SetPostHeader(BinaryData &data);
		std::string	url;
		std::string	domain;
		std::string param;
		unsigned int timeout;
		std::string proxy;
		map<string, string>	header;
		int	port;
		std::string method;
	};
	class Session{
	public:
		~Session();
		Response	Get(std::string url, map<string, string> &head = map<string, string>(), map<string, string> &options = map<string, string>());
		Response	Post(std::string url, map<string, string> &data, map<string, string> files = map<string, string>(), map<string, string> &head = map<string, string>(), map<string, string> &options = map<string, string>());
		Response	Post(std::string url, BinaryData &data, map<string, string> &head = map<string, string>(), map<string, string> &options = map<string, string>());
		map<std::string, std::string> cookies;
	};
	std::string GetIpByDomainName(const char *szHost);
	Response	Get(std::string url, map<string, string> &head = map<string, string>(),std::string cookie="", map<string, string> &options = map<string, string>());
	Response	Post(std::string url, map<string, string> &data, map<string, string> files = map<string, string>(), map<string, string> &head = map<string, string>(),std::string cookie="", map<string, string> &options = map<string, string>());
	Response	Post(std::string url, BinaryData &data, map<string, string> &head = map<string, string>(),std::string cookie="", map<string, string> &options = map<string, string>());
	Response    https_get(string url, map<string, string> &head = map<string, string>(),std::string cookie="", map<string, string> &options = map<string, string>());
	Response	https_post(string url, BinaryData &data, map<string, string> &head = map<string, string>(),std::string cookie="",map<string, string> &options = map<string, string>());
	Response	https_post(string url, map<string, string> &data, map<string, string> files, map<string, string> &head = map<string, string>(),std::string cookie="", map<string, string> &options = map<string, string>());
	Response	request(string method, string url, BinaryData &data, map<string, string> &head = map<string, string>(),std::string cookie="", map<string, string> &options = map<string, string>());
	Response	https_send(string method, string url, int port, DWORD flags, BinaryData &data, map<string, string> &head = map<string, string>(),std::string cookie="", map<string, string> &options = map<string, string>());
}
#endif