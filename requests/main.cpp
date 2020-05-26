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
	
	map<string, string>	header;
	map<string, string> data;
	data["username"] = "username";
	data["password"] = "password";
	data["remember"] = "yes";
	data["login"] = "%E7%99%BB%E5%BD%95";
	Session	s;
	Response login = s.Post("http://47.106.162.182:8080/basic/login.php", data);
	cout << login.GetText() << endl;
	Response home = s.Get("http://47.106.162.182:8080/basic/index.php");
	cout << home.GetText() << endl;
	Response logout = s.Get("http://47.106.162.182:8080/basic/logout.php");
	cout << logout.GetText() << endl;
	getchar();
	return 0;
}

