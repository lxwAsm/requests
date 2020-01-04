// requests.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "http.h"
using	namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	
	map<string, string>	header;
	//header["User-Agent"] = "User Defined Header ";
	int a;
	Response rep = Post("http://47.106.162.182/post.php","name=lxw&age=24");
	cout << rep["Server"] << endl;
	cout << "-----------------------" << endl;
	cout << rep.GetText() << endl;
	cout << "-----------------------" << endl;
	cout << "---------HTTPS----------" << endl;
	Response bin = https_get("https://www.cnblogs.com/wuyepeng/p/9741241.html");
	cout << bin["Content-Type"] << endl;
	cout << bin["Date"] << endl;
	cin >> a;
	return 0;
}

