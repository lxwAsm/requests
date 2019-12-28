#include "http.h"

Response::Response(string &rep)
{
	int pos = rep.find("\r\n\r\n");
	text = rep.substr(pos+4);
	string head = rep.substr(0, pos);
	vector<string>	line;
	SplitString(head, line, "\r\n");
	for (int i = 0; i < line.size(); i++){
		int p = line[i].find(':');
		if (p == -1){
			header["status"] = line[i];
			continue;
		}
		string key = line[i].substr(0, p);
		string value = line[i].substr(p + 1, line[i].size() - 3 - p);
		header[key] = value;
	}
}

void Response::SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

string	Response::GetText()const{
	return text;
}

string	Response::operator[](string key){
	return header[key];
}
Request::Request(std::string url) :url(url)
{
	//http://www.baidu.com/hello?jack=123
	int pos = url.find("://");
	if (pos == -1) return;
	int end = url.find('/', pos + 3);
	if (end == -1){
		domain = url.substr(pos + 3);
		port = 80;
		param = "/";
	}
	else{
		domain = url.substr(pos + 3, end - pos - 3);
		port = 80;
		param = url.substr(end);
	}
}

Response::~Response(){

}
Request::~Request(){

}




std::string GetIpByDomainName(const char *szHost)
{
	WSADATA     wsaData;
	std::string	ip;
	HOSTENT		*pHostEnt;
	int             nAdapter = 0;
	struct       sockaddr_in   sAddr;
	if (WSAStartup(0x0101, &wsaData))
	{
		printf(" gethostbyname error for host:\n");
		return "";
	}
	pHostEnt = gethostbyname(szHost);
	if (pHostEnt)
	{
		if (pHostEnt->h_addr_list[nAdapter])
		{
			memcpy(&sAddr.sin_addr.s_addr, pHostEnt->h_addr_list[nAdapter], pHostEnt->h_length);
			ip = inet_ntoa(sAddr.sin_addr);
		}
	}
	WSACleanup();
	return ip;
}

Response	Get(std::string url)
{
	std::string ret = ""; //返回Http Response
	Request	req(url);
	try
	{
		// 开始进行socket初始化
		WSADATA wData;
		::WSAStartup(MAKEWORD(2, 2), &wData);
		std::string	m_ip = GetIpByDomainName(req.domain.c_str());
		SOCKET clientSocket = socket(AF_INET, 1, 0);
		struct sockaddr_in ServerAddr = { 0 };
		ServerAddr.sin_addr.s_addr = inet_addr(m_ip.c_str());
		ServerAddr.sin_port = htons(80);
		ServerAddr.sin_family = AF_INET;
		int errNo = connect(clientSocket, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
		if (errNo == 0)
		{
				
			
			std::string strSend = "GET " + req.param + " HTTP/1.1\r\n"
				"Connection:Keep-Alive\r\n"
				"host:" + req.domain + "\r\n"
				"User-Agent:Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.88 Safari/537.36\r\n\r\n";
			printf(strSend.c_str());
			// 发送
			errNo = send(clientSocket, strSend.c_str(), strSend.length(), 0);
			if (errNo > 0)
			{
				// 接收
				char bufRecv[3069] = { 0 };
				while (1){
					errNo = recv(clientSocket, bufRecv, 3069, 0);
					if (errNo > 0)
					{
						//printf("%d\n", errNo);
						ret += bufRecv;// 如果接收成功，则返回接收的数据内容
					}
					else{ break; };
				}

			}	
		}
		::WSACleanup();
	}
	catch (...)
	{
		;
	}
	return Response(ret);
}

