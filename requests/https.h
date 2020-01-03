#include <string>
#include "BinaryData.h"
#include "windows.h"
#include "wininet.h"
#pragma comment(lib,"wininet.lib")

using namespace std;
BinaryData https_get(const string &url);
