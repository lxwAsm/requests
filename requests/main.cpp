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
	int a=0;
	stringstream info;
	string name = "2016212073";
	string url = "http://47.106.162.182:8080/post.php";//"https://we.cqu.pt/api/get_student_info.php";
	//cout << "输入名字或者学号:";
	//cin >> name;
	string key = generate_key(name);
	data["name"] = key;
	data["age"] = "14";
	Response html = Post(url,data);
	cout << html.status << endl;
	cout << html.GetText() << endl;
		/*CJsonObject json(html.GetText());
		json.Get("status", a);
		if (a == 200){
			info << "姓名:" << json["data"]["rows"][0]("xm") << "\n";
			info << "出生日期:" << json["data"]["rows"][0]("csrq") << "\n";
			info << "学号:" << json["data"]["rows"][0]("xh") << "\n";
			info << "学院:" << json["data"]["rows"][0]("yxm") << "\n";
			info << "专业:" << json["data"]["rows"][0]("zymEn") << "\n";
			info << "入学时间:" << json["data"]["rows"][0]("nj") << "\n";
			info << "状态:" << json["data"]["rows"][0]("xjzt") << "\n";
			info << "班级代号:" << json["data"]["rows"][0]("bj") << "\n";
			cout << info.str();
		}*/
	cin >> a;
	return 0;
}

