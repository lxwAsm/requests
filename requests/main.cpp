// requests.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include "http.h"
using	namespace std;
using	namespace requests;

string generate_key(string &name){
	string info = "{\"openid\":\"oIaII0UXKpdR7x1r901Jz9ipIFzA\",\"key\":\"%s\",\"page\":1,\"timestamp\":%d}";
	char	buffer[200] = { 0 };
	time_t	t = time(0);
	sprintf_s(buffer, 200, info.c_str(), name.c_str(), t);
	Base64 b64;
	return b64.Encode(buffer, strlen(buffer));
}
int _tmain(int argc, _TCHAR* argv[])
{
	int a;
	map<string, string> options;
	map<string, string> header;
	//header["kk"] = "dd";
	//options["proxy"] = "http=http://122.51.49.88:8888";
	try{
		Response resp = Get("http://47.106.162.182:8080/ip.php");
		cout << resp.status << endl;
	}
	catch (const char *msg){
		cout << msg << endl;
	}
	

	cin >> a;
	return 0;
}

