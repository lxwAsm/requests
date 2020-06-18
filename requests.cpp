#include "requests.h"
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
Response::Response(std::string h,std::string origin_domain,shared_ptr<BinaryData> data){
	string head;
	this->req_domain = origin_domain;
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
		if (key == s_trim("Set-Cookie")){
			vector<string> name_value;
			SplitString(value, name_value, ";");
			for (std::string v : name_value){
				int pos = v.find("=");
				if (pos == -1) continue;
				std::string name = s_trim(v.substr(0, pos));
				std::string value = s_trim(v.substr(pos + 1));
				cookie[name] = value;//save cookie dic
			}
			this->vec_cookie.push_back(value);//save multi cookie str
		}
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

Request::Request(std::string url, std::string method, const map<string, string> &head, const map<string, string> &options) :url(url)
{
	//http://www.baidu.com/hello?jack=123
	header["User-Agent"] = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.88 Safari/537.36";
	header["Connection"] = "Keep-Alive";
	default_options["timeout"] = "3000";
	default_options["proxy"] = "";
	this->method = method;//请求get post delete put
	if (head.size() > 0){
		map<string, string>::const_iterator	begin = head.cbegin();
		for (; begin != head.cend(); begin++){
			header[begin->first] = begin->second;//替换默认header的设置
		}
	}
	if (options.size() > 0){
		map<std::string, std::string>::const_iterator	begin = options.cbegin();
		for (; begin != options.end(); begin++){
			default_options[begin->first] = begin->second;//替换默认options的设置
		}
	}
	this->timeout = atoi(default_options["timeout"].c_str());
	this->proxy = default_options["proxy"];
	int pos = url.find("://");
	if (pos == -1){
		url.insert(0, "http://");
		this->prefix = "http://";
		pos = url.find("://");
	}
	else{
		this->prefix = url.substr(0, pos + 3);
	}
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
		if (port_pos != -1){//不是默认端口
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
	if (method == "POST" && data.size()>0){
		header["Content-Length"] = to_string(data.size());
		if (data.find("--216378052142052079582464804396") != -1){
			header["Content-Type"] = "multipart/form-data; boundary=---------------------------216378052142052079582464804396";
		}
		else{
			header["Content-Type"] = "application/x-www-form-urlencoded; charset=UTF-8";
		}
		return;
	}
	if (method == "PUT"){
		header["Content-Length"] = to_string(data.size());
		std::string filetype = get_content_type(data.user_data);
		header["Content-Type"] = filetype;
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

Session::Session(){
	
}

Session::~Session(){
	
}

bool	 Session::InstallCookie(Response &r){
	bool	isSuccess = FALSE;
	for (auto i : r.cookie){
		cookies[i.first] = i.second;
	}
	if (r.vec_cookie.size() > 0){
		this->vec_cookie = r.vec_cookie;
	}
	for (std::string &c : r.vec_cookie){
		isSuccess=InternetSetCookieA(r.req_domain.c_str(), NULL, c.c_str())==TRUE;
	}
	return isSuccess;
}
Response Session::Get(std::string url,const map<string, string> &head, std::string cookie_arg,const map<string, string> &options){
	BinaryData db;
	Response r = requests::request("GET", url, db, head, cookie_arg, options);
	this->InstallCookie(r);
	return r;
}

Response Session::Post(std::string url, map<string, string> &data,const map<string, string> files,const map<string, string> &head, std::string cookie_arg,const map<string, string> &options){
	Response r = https_post(url, data, files, head, cookie_arg, options);
	this->InstallCookie(r);
	return r;
}

//----------BinaryData-----------
BinaryData::BinaryData()
{

}

BinaryData::BinaryData(int maxsize)
{
	data.reserve(maxsize);
}


void BinaryData::append(byte n){
	data.push_back(n);
}

int	 BinaryData::size(){
	return data.size();
}
void BinaryData::append(const std::string n){
	data.insert(data.end(), n.begin(), n.end());

}

int  BinaryData::find(const char *s){
	return std::string(data.begin(), data.end()).find(s);
}

std::string BinaryData::substr(int start, int end){
	return std::string(data.begin() + start, data.begin() + end);
}

void BinaryData::erase(int start, int end){
	data.erase(data.begin() + start, data.begin() + end);
}
const byte* BinaryData::raw_buffer(){
	return data.data();
}
void BinaryData::append(byte *buffer, int size){
	data.insert(data.end(), buffer, buffer + size);
}
std::string BinaryData::to_string(){
	return std::string(data.begin(), data.end());
}
BinaryData::~BinaryData()
{

}

//-----------EndBinary----------------------

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
//************************************
// Method:    Get
// FullName:  requests::Get
// Access:    public 
// Returns:   requests::Response
// Qualifier:
// Parameter: std::string url
// Parameter: map<string
// Parameter: string> & head
// Parameter: std::string cookie
// Parameter: map<string
// Parameter: string> & options
//************************************
Response	requests::Get(std::string url,const map<string, string> &head,std::string cookie,const map<string, string> &options)
{
	BinaryData db;
	return requests::request("GET", url, db, head, cookie, options);

}

Response	requests::Delete(std::string url, const map<string, string> &head, std::string cookie, const map<string, string> &options){
	BinaryData db;
	return requests::request("DELETE", url, db, head, cookie, options); 
}

Response	requests::Head(std::string url, const map<string, string> &head, std::string cookie, const map<string, string> &options){
	BinaryData db;
	return requests::request("HEAD", url, db, head, cookie, options); 
}

Response	requests::Options(std::string url, const map<string, string> &head, std::string cookie, const map<string, string> &options){
	BinaryData db;
	return requests::request("OPTIONS", url, db, head, cookie, options);

}

Response	requests::Put(std::string url, const std::string filepath, const map<string, string> &head, std::string cookie, const map<string, string> &options){
	ifstream in(filepath, ios::in | ios::binary | ios::ate);
	BinaryData up_data;
	if (!in.is_open()){
		throw "file open failed";
	}
	char * buffer;
	long size;
	size = in.tellg();
	in.seekg(0, ios::beg);
	buffer = new char[size];
	in.read(buffer, size);
	in.close();
	up_data.append((byte*)buffer, size);
	delete[] buffer;
	up_data.user_data = filepath;
	return requests::request("PUT", url, up_data, head, cookie, options);
}
Response	requests::Post(std::string url, BinaryData &data,const map<string, string> &head, std::string cookie,const map<string, string> &options){
	return requests::https_post(url, data, head,cookie,options);
}

Response requests::Post(string url, map<string, string> &data,const map<string,string> files,const map<string, string> &head,std::string cookie,const map<string, string> &options){
	return https_post(url,data,files,head,cookie,options);
}

Response requests::Post(std::string url, std::string str_data, const map<string, string> &head, std::string cookie, const map<string, string> &options){
	BinaryData bd;
	bd.append(str_data);
	return requests::https_post(url, bd, head, cookie, options);
}

Response	requests::request(string method, string url, BinaryData &data,const map<string, string> &head,std::string cookie,const map<string, string> &options){
	DWORD	flags;
	if (to_lower(url.substr(0, 6)) == "https:"){
		 flags = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
			INTERNET_FLAG_KEEP_CONNECTION |
			//INTERNET_FLAG_NO_AUTH |
			//INTERNET_FLAG_NO_COOKIES |
			INTERNET_FLAG_NO_UI |
			INTERNET_FLAG_SECURE |
			INTERNET_FLAG_RELOAD;
		return requests::https_send(method, url, INTERNET_DEFAULT_HTTPS_PORT, flags,data, head,cookie,options);
	}
	else{
		flags = INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD;
		return requests::https_send(method, url, INTERNET_DEFAULT_HTTP_PORT,flags, data, head,cookie,options);
	}
}


Response requests::https_post(string url, BinaryData &data,const map<string, string> &head,std::string cookie,const map<string, string> &options){
	return requests::request("POST", url, data, head,cookie,options);
}

std::string requests::get_content_type(string &filename){
	std::map<std::string, std::string> map_type = {//
			{ ".pdf", "application/pdf" },
			{ ".js", "application/ecmascript" },
			{ ".json", "application/json" },
			{ ".zip", "application/x-zip-compressed" },
			{ ".txt", "text/plain" },
			{ ".mp3", "audio/mpeg" },
			{ ".gif", "image/gif" },
			{ ".c", "text/plain" },
			{ ".h", "text/plain" },
			{ ".py", "text/x-python-script" },
			{ ".jpg", "image/jpeg" }
	};
	int index = filename.rfind('.');
	if (index == -1){
		return "application/octet-stream";
	}
	else{
		std::string file_type = filename.substr(index);
		std::string result = map_type[file_type];
		if (result.size() > 0){
			return result;
		}
		else{
			return "application/octet-stream";//unkonw file type
		}
	}
}

Response	requests::https_post(string url, map<string, string> &data,const map<string, string> files,const map<string, string> &head,std::string cookie,const  map<string, string> &options){
	string up_str;
	BinaryData up_data;
	if (files.size() > 0){//
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
			std::string file_type = get_content_type(filename);
			up_data.append("Content-Type: "+file_type+"\r\n\r\n");
			char * buffer;
			long size;
			ifstream in(f.second, ios::in | ios::binary | ios::ate);
			if (!in.is_open()){
				throw "file open failed";
			}

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
	return requests::request("POST", url, up_data, head,cookie,options);
}

std::string requests::http_err2str(DWORD code){
	static map<DWORD, std::string> err_code = {
			{ 12001, "ERROR_INTERNET_OUT_OF_HANDLES No more handles could be generated at this time." },
			{ 12002, "ERROR_INTERNET_TIMEOUT The request has timed out." },
			{ 12003, "ERROR_INTERNET_EXTENDED_ERROR An extended error was returned from the server., This is typically a string or buffer containing a verbose error message. Call InternetGetLastResponseInfo to retrieve the error text." },
			{ 12004, "ERROR_INTERNET_INTERNAL_ERROR An internal error has occurred." },
			{ 12005, "ERROR_INTERNET_INVALID_URL The URL is invalid." },
			{ 12006, "ERROR_INTERNET_UNRECOGNIZED_SCHEME The URL scheme could not be recognized or is not supported." },
			{ 12007, "ERROR_INTERNET_NAME_NOT_RESOLVED The server name could not be resolved." },
			{ 12008, "ERROR_INTERNET_PROTOCOL_NOT_FOUND The requested protocol could not be located." },
			{ 12009, "ERROR_INTERNET_INVALID_OPTION A request to InternetQueryOption or InternetSetOption specified an invalid option value." },
			{ 12010, "ERROR_INTERNET_BAD_OPTION_LENGTH The length of an option supplied to InternetQueryOption or InternetSetOption is incorrect for the type of option specified." },
			{ 12011, "ERROR_INTERNET_OPTION_NOT_SETTABLE The request option cannot be set, only queried." },
			{ 12012, "ERROR_INTERNET_SHUTDOWN The Win32 Internet function support is being shut down or unloaded." },
			{ 12013, "ERROR_INTERNET_INCORRECT_USER_NAME The request to connect and log on to an FTP server could not be completed because the supplied user name is incorrect." },
			{ 12014, "ERROR_INTERNET_INCORRECT_PASSWORD The request to connect and log on to an FTP server could not be completed because the supplied password is incorrect." },
			{ 12015, "ERROR_INTERNET_LOGIN_FAILURE The request to connect to and log on to an FTP server failed." },
			{ 12016, "ERROR_INTERNET_INVALID_OPERATION The requested operation is invalid." },
			{ 12017, "ERROR_INTERNET_OPERATION_CANCELLED The operation was canceled, usually because the handle on which the request was operating was closed before the operation completed." },
			{ 12018, "ERROR_INTERNET_INCORRECT_HANDLE_TYPE The type of handle supplied is incorrect for this operation." },
			{ 12019, "ERROR_INTERNET_INCORRECT_HANDLE_STATE The requested operation cannot be carried out because the handle supplied is not in the correct state." },
			{ 12020, "ERROR_INTERNET_NOT_PROXY_REQUEST The request cannot be made via a proxy." },
			{ 12021, "ERROR_INTERNET_REGISTRY_VALUE_NOT_FOUND A required registry value could not be located." },
			{ 12022, "ERROR_INTERNET_BAD_REGISTRY_PARAMETER A required registry value was located but is an incorrect type or has an invalid value." },
			{ 12023, "ERROR_INTERNET_NO_DIRECT_ACCESS Direct network access cannot be made at this time." },
			{ 12024, "ERROR_INTERNET_NO_CONTEXT An asynchronous request could not be made because a zero context value was supplied." },
			{ 12025, "ERROR_INTERNET_NO_CALLBACK An asynchronous request could not be made because a callback function has not been set." },
			{ 12026, "ERROR_INTERNET_REQUEST_PENDING The required operation could not be completed because one or more requests are pending." },
			{ 12027, "ERROR_INTERNET_INCORRECT_FORMAT The format of the request is invalid." },
			{ 12028, "ERROR_INTERNET_ITEM_NOT_FOUND The requested item could not be located." },
			{ 12029, "ERROR_INTERNET_CANNOT_CONNECT The attempt to connect to the server failed." },
			{ 12030, "ERROR_INTERNET_CONNECTION_ABORTED The connection with the server has been terminated." },
			{ 12031, "ERROR_INTERNET_CONNECTION_RESET The connection with the server has been reset." },
			{ 12032, "ERROR_INTERNET_FORCE_RETRY Calls for the Win32 Internet function to redo the request." },
			{ 12033, "ERROR_INTERNET_INVALID_PROXY_REQUEST The request to the proxy was invalid." },
			{ 12036, "ERROR_INTERNET_HANDLE_EXISTS The request failed because the handle already exists." },
			{ 12037, "ERROR_INTERNET_SEC_CERT_DATE_INVALID SSL certificate date that was received from the server is bad.The certificate is expired."},  
			{ 12038, "ERROR_INTERNET_SEC_CERT_CN_INVALID SSL certificate common name (host name field) is incorrect., For example, if you entered www.server.com and the common name on the certificate says www.,different.com."},  
			{ 12039, "ERROR_INTERNET_HTTP_TO_HTTPS_ON_REDIR The application is moving from a non-SSL to an SSL connection because of a redirect."},
			{ 12040, "ERROR_INTERNET_HTTPS_TO_HTTP_ON_REDIR The application is moving from an SSL to an non-SSL connection because of a redirect." },
			{ 12041, "ERROR_INTERNET_MIXED_SECURITY Indicates that the content is not entirely secure. Some of the content being viewed may have come from unsecured servers."},   
			{ 12042, "ERROR_INTERNET_CHG_POST_IS_NON_SECURE The application is posting and attempting to change multiple lines of text on a server that is not secure."},
			{ 12043, "ERROR_INTERNET_POST_IS_NON_SECURE The application is posting data to a server that is not secure." },
			{ 12110, "ERROR_FTP_TRANSFER_IN_PROGRESS The requested operation cannot be made on the FTP session handle because an operation is already in progress." },
			{ 12111, "ERROR_FTP_DROPPED The FTP operation was not completed because the session was aborted." },
			{ 12130, "ERROR_GOPHER_PROTOCOL_ERROR An error was detected while parsing data returned from the gopher server." },
			{ 12131, "ERROR_GOPHER_NOT_FILE The request must be made for a file locator." },
			{ 12132, "ERROR_GOPHER_DATA_ERROR An error was detected while receiving data from the gopher server." },
			{ 12133, "ERROR_GOPHER_END_OF_DATA The end of the data has been reached." },
			{ 12134, "ERROR_GOPHER_INVALID_LOCATOR The supplied locator is not valid." },
			{ 12135, "ERROR_GOPHER_INCORRECT_LOCATOR_TYPE The type of the locator is not correct for this operation." },
			{ 12136, "ERROR_GOPHER_NOT_GOPHER_PLUS The requested operation can only be made against a Gopher+ server or with a locator that specifies a Gopher+ operation." },
			{ 12137, "ERROR_GOPHER_ATTRIBUTE_NOT_FOUND The requested attribute could not be located." },
			{ 12138, "ERROR_GOPHER_UNKNOWN_LOCATOR The locator type is unknown." },
			{ 12150, "ERROR_HTTP_HEADER_NOT_FOUND The requested header could not be located." },
			{ 12151, "ERROR_HTTP_DOWNLEVEL_SERVER The server did not return any headers." },
			{ 12152, "ERROR_HTTP_INVALID_SERVER_RESPONSE The server response could not be parsed." },
			{ 12153, "ERROR_HTTP_INVALID_HEADER The supplied header is invalid." },
			{ 12154, "ERROR_HTTP_INVALID_QUERY_REQUEST The request made to HttpQueryInfo is invalid." },
			{ 12155, "ERROR_HTTP_HEADER_ALREADY_EXISTS The header could not be added because it already exists." },
			{ 12156, "ERROR_HTTP_REDIRECT_FAILED The redirection failed because either the scheme changed (for example, HTTP to FTP) or all attempts made to redirect failed (default is five attempts)." },
	};
	return err_code[code];
}

Response	requests::https_send(string method, string url, int port, DWORD flags, BinaryData &data,const map<string, string> &head,std::string cookie,const map<string, string> &options){
	Request req(url,method, head,options);
	LPCTSTR lpszAgent = L"WinInetGet/CppRequests";
	std::wstring proxy = s2ws(req.proxy);
	LPCTSTR	lpszProxy = proxy.c_str();
	HINTERNET hInternet = NULL;
	if (proxy.size() > 0){
		hInternet = InternetOpen(lpszAgent,INTERNET_OPEN_TYPE_PROXY, lpszProxy, NULL, 0);
	}
	else{
		hInternet = InternetOpen(lpszAgent,INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	}	
	auto domain = s2ws(req.domain);
	//printf("domain:%S", domain.c_str());
	LPCTSTR lpszServerName = domain.c_str();
	INTERNET_PORT nServerPort = req.port; //
	LPCTSTR lpszUserName = NULL;//s2ws(req.default_options["username"]).c_str(); //
	LPCTSTR lpszPassword = NULL;//s2ws(req.default_options["password"]).c_str(); //
	DWORD dwConnectFlags = 0;
	DWORD dwConnectContext = 0;
	DWORD dwError = 0;
	HINTERNET hConnect = InternetConnect(hInternet,
		lpszServerName, nServerPort,
		lpszUserName, lpszPassword,
		INTERNET_SERVICE_HTTP,
		dwConnectFlags, dwConnectContext);

	//使用Get
	auto http_method = s2ws(s_trim(req.method));
	LPCTSTR lpszVerb = http_method.c_str();
	auto param = s2ws(req.param);
	//printf("param:%S", param.c_str());
	LPCTSTR lpszObjectName = param.c_str();
	LPCTSTR lpszVersion = L"HTTP/1.1";    // 
	LPCTSTR lpszReferrer = NULL;   // 
	LPCTSTR *lplpszAcceptTypes = NULL; //
	DWORD dwOpenRequestFlags = flags;
	DWORD dwOpenRequestContext = 0;
	HINTERNET hRequest = HttpOpenRequest(hConnect, lpszVerb, lpszObjectName, lpszVersion,
		lpszReferrer, lplpszAcceptTypes,
		dwOpenRequestFlags, dwOpenRequestContext);

	if (hRequest == NULL){
		char err_buffer[200] = { 0 };
		dwError = GetLastError();
		std::string err_info = http_err2str(dwError);
		_snprintf(err_buffer, 200, "HttpOpenRequest failed,error = %d (%s)\n",
			dwError, err_info.c_str());
		throw err_buffer;
	}
	//Set Internet Timeout here
	DWORD dwTimeOut = req.timeout;
	InternetSetOption(hRequest, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeOut, sizeof(dwTimeOut));
	//Set Cookies Header
	auto header_cookie=head.find("Cookie");
	if (header_cookie!=head.end()){
		cookie += ";" + header_cookie->second;
	}
 	if (cookie.size()>0){
		vector<string> name_value = SplitString(cookie, ";");
		for (std::string v : name_value){
			int pos = v.find("=");
			if (pos == -1) continue;
			std::string name = s_trim(v.substr(0, pos));
			std::string value = s_trim(v.substr(pos + 1));
			if (!InternetSetCookieA((req.prefix + req.domain).c_str(), name.c_str(),value.c_str())){
				//printf("Cookies Add Ok");
			}
		}
		
	}
	req.SetPostHeader(data);
	auto he = s2ws(req.HeaderToString());
	//printf("https:%S\ntimeout:%d\nproxy:%s\n%s", he.c_str(),req.timeout,req.proxy.c_str(),data.to_string().c_str());
	//printf("post data:%s", data.to_string().c_str());
	//Ignore 12057 Error
	DWORD dwFlags;
	DWORD dwBuffLen = sizeof(dwFlags);
	InternetQueryOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlags, &dwBuffLen);
	dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
	dwFlags |= SECURITY_FLAG_IGNORE_REVOCATION;
	InternetSetOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));
	if (!HttpSendRequest(hRequest, he.c_str(), he.size(), (LPVOID)data.raw_buffer(), data.size()))
	{
		char err_buffer[200] = { 0 };
		dwError = GetLastError();
		std::string err_info = http_err2str(dwError);
		_snprintf(err_buffer, 200, "HttpSendRequest failed,error = %d (%s)\n",
			dwError, err_info.c_str());
		throw err_buffer;
	}
	//
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
			char err_buffer[200] = { 0 };
			dwError = GetLastError();
			std::string err_info = http_err2str(dwError);
			_snprintf(err_buffer, 200, "HttpQueryInfo failed,error = %d (%s)\n",
				dwError, err_info.c_str());
			throw err_buffer;
		}
	}
	pInfoBuffer[dwInfoBufferLength] = '/0';
	pInfoBuffer[dwInfoBufferLength + 1] = '/0';
	wstring header = (WCHAR*)pInfoBuffer;
	free(pInfoBuffer);
	//HTTP Response
	DWORD dwBytesAvailable;
	BYTE *pMessageBody = NULL;
	shared_ptr<BinaryData> content(new BinaryData(1000));
	while (InternetQueryDataAvailable(hRequest, &dwBytesAvailable, 0, 0)) {
		pMessageBody = (BYTE*)malloc(dwBytesAvailable + 1);
		DWORD dwBytesRead;
		BOOL bResult = InternetReadFile(hRequest, pMessageBody,
			dwBytesAvailable, &dwBytesRead);
		if (!bResult) {
			char err_buffer[200] = { 0 };
			free(pMessageBody);
			_snprintf(err_buffer,200,"InternetReadFile failed, error = %d (0x%x)/n",
				GetLastError(), GetLastError());
			throw err_buffer;
		}
		if (dwBytesRead == 0) {
			free(pMessageBody);
			break; // End of File.
		}
		content->append(pMessageBody, dwBytesRead);
		free(pMessageBody);
	}
	//Free handles
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);
	auto h = ws2s(header);
	Response rep(h,req.prefix+req.domain, content);
	return rep;
}
