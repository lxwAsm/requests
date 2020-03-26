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
	map<string, string> data;
	map<string, string> files;
	//options["timeout"] = "3000";
	data["key"] = "letmeupload";
	data["submit"] = "Submit";
	files["file"] = "C:\\Users\\jack\\Desktop\\stdafx.h";
	//options["proxy"] = "222.240.184.126:8086";
	int a;

	Response resp = Post("http://47.106.162.182:8080/save.php",data,files);
	cout << resp.status << endl;
	cout << resp.GetText() << endl;
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

