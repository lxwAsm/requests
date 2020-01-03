// requests.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "http.h"
#include "https.h"
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
	BinaryData bin = https_get("www.baidu.com");
	cout << bin.size() << endl;
	cout << bin.to_string() << endl;
	cin >> a;
	return 0;
}

