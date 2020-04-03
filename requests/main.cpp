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
	//options["proxy"] = "222.240.184.126:8086";
	string cookie = "BAIDUID=B066E871294A61BE394DE24FFA475653:FG=1; BIDUPSID=B066E871294A61BE9EF72E101F79BF87; PSTM=1578882286; PANWEB=1; BDCLND=F11qEnP39lJDQ2kzlIZZeuXq%2B13S8VTeUN1U90stlMU%3D; BDUSS=G1OOTF1TGFsbEtLeWQ4TWtYemc1all5bkp5d3Q4akd2aWpXM1d3Z3pjck5XNUZlRVFBQUFBJCQAAAAAAAAAAAEAAAAWxccwbDE5NDE1ODkxAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAM3OaV7NzmleW; SCRC=14fb9e50e6e5d6c4a78f794d5a26795e; STOKEN=7058b56fab0665e0cfc62d33f139a14b0111aad2920b16e21c9751bfe5a4ceff; cflag=13%3A3; BDORZ=FFFB88E999055A3F8A630C64834BD6D0; delPer=0; PSINO=7; H_PS_PSSID=1420_31118_21110_30903_31085; BDRCVFR[Fc9oatPmwxn]=mk3SLVN4HKm";
	int a;

	/*Response resp = Get("https://pan.baidu.com/disk/home",header,cookie);
	cout << resp.status << endl;
	fstream png("baidu.html", ios::out | ios::binary);
	png.write((const char*)resp.GetBinary(), resp.size());
	png.close();*/
	header["name"] = "cpp";
	header["age"] = "14";
	header["User-Agent"] = "XiaoMi Brower";
	BinaryData post_data;
	post_data.append("PostData");
	Response resp = Get("https://baidu.com/");
	cout << resp.status << endl;
	for (auto i : resp.cookie){
		cout << i.first << ":" << i.second << endl;
	}
	cout << "--------------------";
	
	//cout << resp.GetText() << endl;
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

