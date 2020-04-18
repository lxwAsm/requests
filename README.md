>关闭预编译头：项目->属性->C/C++->预编译头->不使用预编译头  

[安装编译演示视频](https://www.bilibili.com/video/bv1gV411Z7fT)  
### 入门
>#include \<iostream>  
>#include \<string>  
>#include "http.h"  
>using	namespace std;  
>using	namespace requests;  
>int main(int argc)  
>{  
>&nbsp;&nbsp;&nbsp;&nbsp;Response resp = Get("https://baidu.com");  
>&nbsp;&nbsp;&nbsp;&nbsp;cout << resp.status << endl;  
>&nbsp;&nbsp;&nbsp;&nbsp;cout << resp.GetText() << endl;  
>&nbsp;&nbsp;&nbsp;&nbsp;return 0;  
> }  
### Get获取网页内容
>Response resp = Get("https://pan.baidu.com/disk/home");  
>cout << resp.GetText() << endl; `

### Post请求使用,上传json数据
>map<string, string> data;  
>data["name"] = "cpp";  
>data["age"] = "14";  
>Response resp = Post("http://47.106.162.182:8080/post.php", data);  
>cout << resp.status << endl;  
>cout << resp.GetText() << endl;  

### Get带cookie访问网页
>map<string, string> header;  
>string cookie = "BAIDUID=B066E871294A61BE394DE24FFA475653:FG=1; BIDUPSID=B066E871294A61BE9EF72E101F79BF87; PSTM=1578882286;";  
>Response resp = Get("https://pan.baidu.com/disk/home",header,cookie);  
>cout << resp.GetText() << endl;  
### 自定义Http Header
>map<string, string> header;  
>header["name"] = "cpp";  
>header["age"] = "14";
>header["User-Agent"] = "Unknown Brower"  
>Response resp = Get("http://47.106.162.182:8080/header.php",header);  
>cout << resp.status << endl;  
>cout << resp.GetText() << endl;  

### 上传文件
>map<string, string> header;  
>map<string, string> data;  
>map<string, string> files;  
>data["key"] = "something";  
>data["submit"] = "Submit"; //form表单的其他要上传的数据，如账号密码。   
>files["file"] = "C:\\stdafx.h";  //form表单中的文件字段。  
>Response resp = Post(url,data,files);  
>cout << resp.status << endl;  
>cout << resp.GetText() << endl;  
