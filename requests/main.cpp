// requests.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
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
	string name = "2016212073";
	string url = "https://we.cqu.pt/api/get_student_info.php";
	//cout << "输入名字或者学号:";
	//cin >> name;
	string key = generate_key(name);
	data["key"] = key;
	Response html = Get("https://github.com/lxwAsm/requests/blob/master/README.md");
	cout << html.status << endl;
	cout << html.GetText() << endl;
	//cout << "---------HTTPS----------" << endl;
	/*Response bin = Get("https://img12.360buyimg.com/n1/s450x450_jfs/t1/48975/24/16741/337144/5de0913eE206334a5/32c2af4c117024c3.jpg");
	auto header = bin.Header();
	cout << "File size: " << bin.size() << endl;
	cout << bin["status"] << endl;
	fstream png("baidu.jpg", ios::out | ios::binary);
	png.write((const char*)bin.GetBinary(), bin.size());*/
	//png.close();
	cin >> a;
	return 0;
}

