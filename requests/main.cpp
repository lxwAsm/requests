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
	
	map<string, string> header;
	map<string, string> options;
	map<string, string> data;
	map<string, string> files;
	int a;
	//options["timeout"] = "3000";
	data["key"] = "letmeupload";
	data["submit"] = "Submit";
	files["file"] = "C:\\Users\\jack\\Desktop\\git-for-windows.ico";
	Response resp = Post("http://47.106.162.182:8080/save.php", data, files);
	cout << resp.status << endl;
	cout << resp.GetText() << endl;
	cin >> a;
	return 0;
}

