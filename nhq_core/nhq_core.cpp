// nhq_core.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "nhq_core.h"



// 这是导出变量的一个示例
NHQ_CORE_API int nnhq_core = 0;

// 这是导出函数的一个示例。
NHQ_CORE_API int fnnhq_core(void){
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 nhq_core.h
Cnhq_core::Cnhq_core(){
	return;
}






/*****************************---------nai he qiao client-----------*********************************************/


NHQ_CLIENT nhq_client = {0};

bool init_NHQ_client(){

	WSADATA wsaData;
	nhq_client.ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL ,
		*ptr = NULL ,
		hints;

	int iResult;


	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2 , 2) , &wsaData);
	if(iResult != 0){
		MessageBoxA(0 , "client:WSAStartup failed with error" , 0 , 0);
		return 1;
	}

	ZeroMemory(&hints , sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;


	//get ip
	char hostname[MAX_PATH];
	addrinfo   *res;
	gethostname(hostname , MAX_PATH);  
	getaddrinfo(hostname , NULL , &hints , &res);
	char ip[MAX_PATH];
	for(ptr = res; ptr != NULL; ptr = ptr->ai_next){
		switch(ptr->ai_family){
		case AF_INET:
			unsigned char *up = (unsigned char *)ptr->ai_addr->sa_data;
			sprintf_s(ip , "%d.%d.%d.%d" , up[2] , up[3] , up[4] , up[5]);
			break;
		}
	}

	// Resolve the server address and port
	iResult = getaddrinfo(ip , DEFAULT_PORT , &hints , &result);
	if(iResult != 0){
		MessageBoxA(0 , "client: getaddrinfo failed with error" , 0 , 0);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for(ptr = result; ptr != NULL; ptr = ptr->ai_next){

		// Create a SOCKET for connecting to server
		nhq_client.ConnectSocket = socket(ptr->ai_family , ptr->ai_socktype ,
										  ptr->ai_protocol);
		if(nhq_client.ConnectSocket == INVALID_SOCKET){
			MessageBoxA(0 , "client: socket failed with error" , 0 , 0);
			WSACleanup();
			return 1;
		}
		// Connect to server.
		iResult = connect(nhq_client.ConnectSocket , ptr->ai_addr , (int)ptr->ai_addrlen);
		if(iResult == SOCKET_ERROR){
			closesocket(nhq_client.ConnectSocket);
			nhq_client.ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);

	if(nhq_client.ConnectSocket == INVALID_SOCKET){
		MessageBoxA(0 , "client: Unable to connect to server!" , 0 , 0);
		WSACleanup();
		return 1;
	}
	return 0;
}






