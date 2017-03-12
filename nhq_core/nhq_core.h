// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 NHQ_CORE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// NHQ_CORE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef NHQ_CORE_EXPORTS
#define NHQ_CORE_API __declspec(dllexport)
#else
#define NHQ_CORE_API __declspec(dllimport)
#endif

// 此类是从 nhq_core.dll 导出的
class NHQ_CORE_API Cnhq_core {
public:
	Cnhq_core(void);
	// TODO:  在此添加您的方法。
};

extern NHQ_CORE_API int nnhq_core;

NHQ_CORE_API int fnnhq_core(void);



/*****************************---------nai he qiao client-----------*********************************************/

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

 struct NHQ_CLIENT{
	SOCKET ConnectSocket;
};
extern  NHQ_CLIENT nhq_client;
bool init_NHQ_client();