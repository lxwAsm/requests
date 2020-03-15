// requests.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include "http.h"
using	namespace std;
using	namespace requests;

int _tmain(int argc, _TCHAR* argv[])
{
	
	map<string, string> header;
	map<string, string> options;
	//options["timeout"] = "3000";
	//options["proxy"] = "222.240.184.126:8086";
	int a;
	Response rep = Get("HTTPS://img12.360buyimg.com/n1/s450x450_jfs/t1/48975/24/16741/337144/5de0913eE206334a5/32c2af4c117024c3.jpg",header,options);
	cout << rep["cookies"] << endl;
	cout << rep.GetText() << endl;
	cout << rep.status << endl;
	cout << "-----------------------" << endl;
	//cout << "---------HTTPS----------" << endl;
	/*Response bin = https_get("https://img12.360buyimg.com/n1/s450x450_jfs/t1/48975/24/16741/337144/5de0913eE206334a5/32c2af4c117024c3.jpg");
	auto header = bin.Header();
	cout << "File size: " << bin.size() << endl;
	cout << bin["status"] << endl;
	fstream png("baidu.jpg", ios::out | ios::binary);
	png.write((const char*)bin.GetBinary(), bin.size());*/
	//png.close();
	cin >> a;
	return 0;
}

