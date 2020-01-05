// requests.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include "http.h"
using	namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	
	map<string, string>	header;
	//header["User-Agent"] = "User Defined Header ";
	int a;
	//Response rep = Post("http://47.106.162.182/post.php","name=lxw&age=24");
	//cout << rep["Server"] << endl;
	//cout << "-----------------------" << endl;
	//cout << rep.GetText() << endl;
	//cout << "-----------------------" << endl;
	cout << "---------HTTPS----------" << endl;
	Response bin = https_get("https://img12.360buyimg.com/n1/s450x450_jfs/t1/48975/24/16741/337144/5de0913eE206334a5/32c2af4c117024c3.jpg");
	header = bin.Header();
	cout << "File size: " << bin.size() << endl;
	cout << bin["status"] << endl;
	fstream png("baidu.jpg", ios::out | ios::binary);
	png.write((const char*)bin.GetBinary(), bin.size());
	png.close();
	cin >> a;
	return 0;
}

