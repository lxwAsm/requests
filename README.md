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

