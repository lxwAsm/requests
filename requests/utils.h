#ifndef utils996
#define utils996
#include <string>
#include <algorithm>
#include <Windows.h>//

std::wstring s2ws(const std::string& str);
std::string  ws2s(const std::wstring &str);
std::string to_lower(std::string str);

class Base64
{
public:
	/*编码
	DataByte
	[in]输入的数据长度,以字节为单位
	*/
	std::string Encode(char* Data, int DataByte);
	/*解码
	DataByte
	[in]输入的数据长度,以字节为单位
	OutByte
	[out]输出的数据长度,以字节为单位,请不要通过返回值计算
	输出数据的长度
	*/
	std::string Decode(const char* Data, int DataByte, int& OutByte);
};
#endif