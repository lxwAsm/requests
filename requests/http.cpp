#include "http.h"
using namespace requests;
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
			this->status = status2int(line[i]);
			continue;
		}
		string key = line[i].substr(0, p);
		string value = line[i].substr(p + 1, line[i].size() - 1 - p);
		header[key] = value;
		
	}
}

unsigned int Response::status2int(string &st){
	int s = st.find(' ');
	int e = st.rfind(' ');
	return atoi(st.substr(s + 1, e - s).c_str());
	
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
			this->status = status2int(line[i]);
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
Request::Request(std::string url, std::string method, map<string, string> &head, map<string, string> &options) :url(url)
{
	//http://www.baidu.com/hello?jack=123
	header["User-Agent"] = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.88 Safari/537.36\r\n";
	header["Connection"] = "Keep-Alive";
	map<std::string, std::string> default_options;
	default_options["timeout"] = "3000";
	default_options["proxy"] = "";
	this->method = method;//请求get post delete put
	if (header.size() > 0){
		map<string, string>::const_iterator	begin = head.cbegin();
		for (; begin != head.cend(); begin++){
			//cookie用InternetSetCookie设置
			header[begin->first] = begin->second;//替换默认header的设置
		}
	}
	if (options.size() > 0){
		map<std::string,std::string>::iterator	begin = options.begin();
		for (; begin != options.end(); begin++){
			default_options[begin->first] = begin->second;//替换默认options的设置
		}
	}
	this->timeout = atoi(default_options["timeout"].c_str());
	this->proxy = default_options["proxy"];
	int pos = url.find("://");
	if (pos == -1) return;
	int end = url.find('/', pos + 3);
	if (end == -1){//没有参数
		domain = url.substr(pos + 3);
		if (url.substr(0, 5) == "http:"){
			port = 80;
		}
		else{
			port = 443;
		}
		param = "/";
	}
	else{//有请求参数
		std::string domain_port = url.substr(pos + 3, end - pos - 3);
		int port_pos = domain_port.find(":");
		if (port_pos!= -1){//不是默认端口
			domain = domain_port.substr(0, port_pos);
			port = atoi(domain_port.substr(port_pos + 1).c_str());
		}
		else{
			if (url.substr(0, 5) == "http:"){
				port = 80;
			}
			else{
				port = 443;
			}
			domain = domain_port;
		}
		
		param = url.substr(end);
	}
}
void  Request::SetPostHeader(BinaryData &data){
	if (method == "POST "){
		header["Content-Length"] = to_string(data.size());
		if (data.find("--216378052142052079582464804396") != -1){
			header["Content-Type"] = "multipart/form-data; boundary=---------------------------216378052142052079582464804396";
		}
		else{
			header["Content-Type"] = "application/x-www-form-urlencoded; charset=UTF-8";
		}
		
	}
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




std::string requests::GetIpByDomainName(const char *szHost)
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

Response	DoSend(std::string url, map<string, string> &head, string method = "GET ", string data = ""){
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
			string strSend = "";//with post data
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
Response	requests::Get(std::string url, map<string, string> &head,std::string cookie, map<string, string> &options)
{
	return https_get(url, head,cookie,options);
}

Response	requests::Post(std::string url, BinaryData &data, map<string, string> &head, map<string, string> &options){
	return requests::https_post(url, data, head,options);
}

Response requests::Post(string url, map<string, string> &data,map<string,string> files, map<string, string> &head, map<string, string> &options){
	return https_post(url,data,files,head,options);
}

Response	requests::request(string method, string url, BinaryData &data, map<string, string> &head,std::string cookie, map<string, string> &options){
	DWORD	flags;
	if (to_lower(url.substr(0, 6)) == "https:"){
		 flags = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
			INTERNET_FLAG_KEEP_CONNECTION |
			INTERNET_FLAG_NO_AUTH |
			//INTERNET_FLAG_NO_COOKIES |
			INTERNET_FLAG_NO_UI |
			//设置启用HTTPS
			INTERNET_FLAG_SECURE |
			INTERNET_FLAG_RELOAD;
		return requests::https_send(method, url, INTERNET_DEFAULT_HTTPS_PORT, flags,data, head,cookie,options);
	}
	else{
		flags = INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD;
		return requests::https_send(method, url, INTERNET_DEFAULT_HTTP_PORT,flags, data, head,cookie,options);
	}
}

Response requests::https_get(string url, map<string, string> &head,std::string cookie, map<string, string> &options){
	BinaryData db;
	return requests::request("GET", url,db, head,cookie,options); //request函数里面处理http和https
}

Response requests::https_post(string url, BinaryData &data, map<string, string> &head, map<string, string> &options){
	return requests::request("POST", url, data, head,"",options);
}

Response	requests::https_post(string url, map<string, string> &data, map<string, string> files, map<string, string> &head, map<string, string> &options){
	string up_str;
	BinaryData up_data;
	if (files.size() > 0){//要上传文件，加boundary
		for (auto &f : files){
			up_data.append("-----------------------------216378052142052079582464804396\r\n");
			int pos = f.second.rfind("\\");
			std::string filename;
			if (pos != -1){
				filename = f.second.substr(pos + 1);
			}
			else{
				filename = f.second;
			}
			up_data.append("Content-Disposition: form-data; name=\"" + f.first + "\"" + "; filename=\"" + filename + "\"\r\n");
			up_data.append("Content-Type: text/x-python-script\r\n\r\n");
			char * buffer;
			long size;
			ifstream in(filename, ios::in | ios::binary | ios::ate);
			size = in.tellg();
			in.seekg(0, ios::beg);
			buffer = new char[size];
			in.read(buffer, size);
			in.close();
			up_data.append((byte*)buffer, size);
			delete[] buffer;
		}
		for (auto &i : data){
			up_data.append("\r\n-----------------------------216378052142052079582464804396\r\n");
			up_data.append("Content-Disposition: form-data; name=\""+i.first+"\"\r\n\r\n");
			up_data.append(i.second);
		}
		up_data.append("\r\n-----------------------------216378052142052079582464804396--");
	}
	else{
		for (auto &i : data){
			up_str += i.first + "=";
			up_str += i.second + "&";
		}
		up_str.erase(up_str.end() - 1);
		up_data.append(up_str);
	}
	return requests::request("POST", url, up_data, head);
}


Response	requests::https_send(string method, string url, int port, DWORD flags, BinaryData &data, map<string, string> &head,std::string cookie, map<string, string> &options){
	Request req(url,method+" ", head,options);
	LPCTSTR lpszAgent = L"WinInetGet/0.1";
	HINTERNET hInternet = InternetOpen(lpszAgent,
		INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	auto domain = s2ws(req.domain);
	//printf("domain:%S", domain.c_str());
	LPCTSTR lpszServerName = domain.c_str();
	INTERNET_PORT nServerPort = req.port; // HTTPS端口443
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
	LPCTSTR lpszVerb=NULL;
	if (req.method == "GET "){
		lpszVerb = L"GET";
	}
	if (req.method == "POST "){
		lpszVerb = L"POST";
	}
	auto param = s2ws(req.param);
	//printf("param:%S", param.c_str());
	LPCTSTR lpszObjectName = param.c_str();
	LPCTSTR lpszVersion = L"HTTP/1.1";    // 默认.
	LPCTSTR lpszReferrer = NULL;   // 没有引用页
	LPCTSTR *lplpszAcceptTypes = NULL; // Accpet所有类型.
	DWORD dwOpenRequestFlags = flags;
	DWORD dwOpenRequestContext = 0;
	//初始化Request
	HINTERNET hRequest = HttpOpenRequest(hConnect, lpszVerb, lpszObjectName, lpszVersion,
		lpszReferrer, lplpszAcceptTypes,
		dwOpenRequestFlags, dwOpenRequestContext);
	//发送Request
again:
	/*
	set internet option here
	*/
	DWORD dwTimeOut = req.timeout;
	InternetSetOption(hRequest, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeOut, sizeof(dwTimeOut));
	if (req.proxy.size() > 0){
		INTERNET_PROXY_INFO proxy_info;
		proxy_info.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
		proxy_info.lpszProxy = s2ws(req.proxy).c_str();
		proxy_info.lpszProxyBypass = s2ws(req.proxy).c_str();
		InternetSetOption(hRequest, INTERNET_OPTION_PROXY, &proxy_info, sizeof(proxy_info));
	}
	//Set Cookies Header
	if (cookie.size()>0){
		vector<string> name_value = SplitString(cookie, ";");
		for (std::string v : name_value){
			int pos = v.find("=");
			if (pos == -1) continue;
			std::string name = s_trim(v.substr(0, pos));
			std::string value = s_trim(v.substr(pos + 1));
			if (InternetSetCookieA(("https://" + req.domain).c_str(), name.c_str(),value.c_str())){
				printf("Cookies Add Ok");
			}
		}
		
	}
	req.SetPostHeader(data);
	DWORD dwError = 0;
	auto he = s2ws(req.HeaderToString());
	printf("https:%S\ntimeout:%d\nproxy:%s\n%s", he.c_str(),req.timeout,req.proxy.c_str(),data.to_string().c_str());
	//printf("post data:%s", data.to_string().c_str());
	if (!HttpSendRequest(hRequest, he.c_str(),he.size(),(LPVOID)data.raw_buffer(),data.size()))
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
			shared_ptr<BinaryData> err_info(new BinaryData(500));
			err_info->append("HttpQueryInfo Timeout");
			std::string err_header;
			Response	resp(err_header,err_info);
			resp.status = 0;
 			return resp;
		}
	}
	pInfoBuffer[dwInfoBufferLength] = '/0';
	pInfoBuffer[dwInfoBufferLength + 1] = '/0';
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
		content->append(pMessageBody, dwBytesRead);
		free(pMessageBody);
	}
	//Free handles
	//CloseHandle(hRequest);
	//CloseHandle(hConnect);
	//CloseHandle(hInternet);
	auto h = ws2s(header);
	Response rep(h, content);
	return rep;
}