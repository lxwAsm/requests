#include <string>
#include "windows.h"
#include "wininet.h"
#pragma comment(lib,"wininet.lib")

using namespace std;
string https_get(const string &url);
