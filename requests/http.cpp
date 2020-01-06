#include "http.h"

Response::Response(shared_ptr<BinaryData> rep)
{
	int pos = rep->find("\r\n\r\n");
	string head = rep->substr(0, pos);
	rep->erase(0, pos+4);
	pContent = rep;
	vector<string>	line;
	SplitString(head, line, "\r\n");
	for (int i = 0; i < line.size(); i++){
		int p = line[i].find(':');
		if (p == -1){
			header["status"] = line[i];
			continue;
		}
		string key = line[i].substr(0, p);
		string value = line[i].substr(p + 1, line[i].size() - 1 - p);
		header[key] = value;
		
	}
}

Response::Response(std::string &h,shared_ptr<BinaryData> data){
	string head;
	int pos = h.find("\r\n\r\n");
	if (pos != -1){
		head = h.substr(0, pos);
	}
	else{
		head = h;
	}
	pContent = data;
	vector<string>	line;
	SplitString(head, line, "\r\n");
	for (int i = 0; i < line.size(); i++){
		int p = line[i].find(':');
		if (p == -1 && line[i].find("HTTP/")!=-1){
			header["status"] = line[i];
			continue;
		}
		string key = line[i].substr(0, p);
		string value = line[i].substr(p + 1, line[i].size() - 1 - p);
		header[key] = value;
		//printf("https header%s:%s\n", key.c_str(), value.c_str());
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

string	Response::GetText(){
	return pContent->to_string();
}

map<string, string>	Response::Header(){
	return header;
}
const byte* Response::GetBinary(){
	return pContent->raw_buffer();
}

unsigned int Response::size(){
	return pContent->size();
}
string	Response::operator[](string key){
	return header[key];
}

Response::~Response(){
	
}
Request::Request(std::string url, std::string method,map<string, string> &head) :url(url)
{
	//http://www.baidu.com/hello?jack=123
	header["User-Agent"] = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.88 Safari/537.36\r\n";
	header["Connection"] = "Keep-Alive";
	this->method = method;//请求get post delete put
	if (header.size() > 0){
		map<string, string>::const_iterator	begin = head.cbegin();
		for (; begin != head.cend(); begin++){
			header[begin->first] = begin->second;
		}
	}
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
string  Request::GetRequest(const string &data){
	if (method == "POST "){
		header["Content-Length"] = to_string(data.size()+2);
		header["Content-Type"] = "application/x-www-form-urlencoded; charset=UTF-8";
	}
	return method + param + " HTTP/1.1\r\n"
			"host:" + domain + "\r\n" + HeaderToString()+data;
	
}

string Request::HeaderToString(){
	string	head;
	map<string, string>::const_iterator	begin = header.cbegin();
	for (; begin != header.cend(); begin++){
		head+=begin->first + ": " + begin->second + "\r\n";
	}
	head += "\r\n";//next to content;
	return head;
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

Response	DoSend(std::string url, map<string, string> &head,string method="GET ",string data=""){
	shared_ptr<BinaryData> pData(new BinaryData(1000));
	Request	req(url,method,head);
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
			string strSend = req.GetRequest(data);//with post data
			printf(strSend.c_str());
			errNo = send(clientSocket, strSend.c_str(), strSend.length(), 0);
			if (errNo > 0)
			{
				// 接收
				byte bufRecv[3069] = { 0 };
				while (1){
					errNo = recv(clientSocket, (char*)bufRecv, 3069, 0);
					if (errNo > 0)
					{
						pData->append(bufRecv, errNo);
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
	return Response(pData);

}
Response	Get(std::string url, map<string, string> &head)
{
	return DoSend(url, head);
}

Response	Post(std::string url, const string &data, map<string, string> &head){
	return DoSend(url, head, "POST ", data);
}

Response	request(string method,string url, BinaryData &data,map<string, string> &head){
	return https_send(method, url, data, head);
}
Response https_get(string url, map<string, string> &head){
	BinaryData db;
	return request("GET", url,db, head);
}


Response	https_send(string method,string url, BinaryData &data,map<string, string> &head){
	Request req(url,method+" ", head);
	LPCTSTR lpszAgent = L"WinInetGet/0.1";
	HINTERNET hInternet = InternetOpen(lpszAgent,
		INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	auto domain = s2ws(req.domain);
	//printf("domain:%S", domain.c_str());
	LPCTSTR lpszServerName = domain.c_str();//"ssl.google-analytics.com"; //设置server
	INTERNET_PORT nServerPort = INTERNET_DEFAULT_HTTPS_PORT; // HTTPS端口443
	LPCTSTR lpszUserName = NULL; //无登录用户名
	LPCTSTR lpszPassword = NULL; //无登录密码
	DWORD dwConnectFlags = 0;
	DWORD dwConnectContext = 0;
	//连接
	HINTERNET hConnect = InternetConnect(hInternet,
		lpszServerName, nServerPort,
		lpszUserName, lpszPassword,
		INTERNET_SERVICE_HTTP,
		dwConnectFlags, dwConnectContext);
	//使用Get
	LPCTSTR lpszVerb = s2ws(method).c_str();
	auto param = s2ws(req.param);
	//printf("param:%S", param.c_str());
	LPCTSTR lpszObjectName = param.c_str();
	LPCTSTR lpszVersion = L"HTTP/1.1";    // 默认.
	LPCTSTR lpszReferrer = NULL;   // 没有引用页
	LPCTSTR *lplpszAcceptTypes = NULL; // Accpet所有类型.
	DWORD dwOpenRequestFlags = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
		INTERNET_FLAG_KEEP_CONNECTION |
		INTERNET_FLAG_NO_AUTH |
		INTERNET_FLAG_NO_COOKIES |
		INTERNET_FLAG_NO_UI |
		//设置启用HTTPS
		INTERNET_FLAG_SECURE |
		INTERNET_FLAG_RELOAD;
	DWORD dwOpenRequestContext = 0;
	//初始化Request
	HINTERNET hRequest = HttpOpenRequest(hConnect, lpszVerb, lpszObjectName, lpszVersion,
		lpszReferrer, lplpszAcceptTypes,
		dwOpenRequestFlags, dwOpenRequestContext);
	//发送Request
again:
	DWORD dwError = 0;
	auto he = s2ws(req.HeaderToString());
	printf("https:%S", he.c_str());
	if (!HttpSendRequest(hRequest, he.c_str(),method=="GET"? 0:-1,(LPVOID)data.raw_buffer(),data.size()))
	{
		dwError = GetLastError();
	}
	if (dwError == ERROR_INTERNET_INVALID_CA)
	{
		fprintf(stderr, "HttpSendRequest failed, error = %d (0x%x)/n",
			dwError, dwError);

		DWORD dwFlags;
		DWORD dwBuffLen = sizeof(dwFlags);
		InternetQueryOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS,
			(LPVOID)&dwFlags, &dwBuffLen);

		dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
		InternetSetOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS,
			&dwFlags, sizeof(dwFlags));
		goto again;
	}
	//获得HTTP Response Header信息
	DWORD dwInfoLevel = HTTP_QUERY_RAW_HEADERS_CRLF;
	DWORD dwInfoBufferLength = 2048;
	BYTE *pInfoBuffer = (BYTE *)malloc(dwInfoBufferLength + 2);
	while (!HttpQueryInfo(hRequest, dwInfoLevel, pInfoBuffer, &dwInfoBufferLength, NULL)) {
		DWORD dwError = GetLastError();
		if (dwError == ERROR_INSUFFICIENT_BUFFER) {
			free(pInfoBuffer);
			pInfoBuffer = (BYTE *)malloc(dwInfoBufferLength + 2);
		}
		else {
			fprintf(stderr, "HttpQueryInfo failed, error = %d (0x%x)/n",
				GetLastError(), GetLastError());
			break;
		}
	}
	pInfoBuffer[dwInfoBufferLength] = '/0';
	pInfoBuffer[dwInfoBufferLength + 1] = '/0';
	//printf("Header:%S", pInfoBuffer); //很奇怪HttpQueryInfo保存的格式是wchar_t 和下面的InternetReadFile不一样
	wstring header = (WCHAR*)pInfoBuffer;
	free(pInfoBuffer);
	//HTTP Response 的 Body, 需要的内容就在里面
	DWORD dwBytesAvailable;
	BYTE *pMessageBody = NULL;
	shared_ptr<BinaryData> content(new BinaryData(1000));
	while (InternetQueryDataAvailable(hRequest, &dwBytesAvailable, 0, 0)) {
		pMessageBody = (BYTE *)malloc(dwBytesAvailable + 1);
		DWORD dwBytesRead;
		BOOL bResult = InternetReadFile(hRequest, pMessageBody,
			dwBytesAvailable, &dwBytesRead);
		if (!bResult) {
			fprintf(stderr, "InternetReadFile failed, error = %d (0x%x)/n",
				GetLastError(), GetLastError());
			break;
		}
		if (dwBytesRead == 0)
			break; // End of File.
		//printf("fill data%d",dwBytesRead);
		content->append(pMessageBody, dwBytesRead);
		free(pMessageBody);
	}
	auto h = ws2s(header);
	//printf("new header %s\n", h.c_str());
	Response rep(h, content);
	return rep;
}