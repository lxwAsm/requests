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
	header["User-Agent"] = "User Defined Header ";
	int a;
	Response rep = Get("http://47.106.162.182/112358.php",header);
	cout << rep["Server"] << endl;
	cout << "-----------------------" << endl;
	cout << rep.GetText() << endl;
	cout << "-----------------------" << endl;
	cin >> a;
	return 0;
}

