#include "nhq_server.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>


#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"


#include "../naiheqiao.h"
#include "nhq_data.h"



bool init_NHQ_server(){
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// 初始化 Winsock
	iResult = WSAStartup(MAKEWORD(2 , 2) , &wsaData);
	if(iResult != 0){
		MessageBoxA(0 , "server: WSAStartup failed with error" , 0 , 0);
		return 1;
	}

	ZeroMemory(&hints , sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL , DEFAULT_PORT , &hints , &result);
	if(iResult != 0){
		MessageBoxA(0 , "server: getaddrinfo failed with error" , 0 , 0);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family , result->ai_socktype , result->ai_protocol);
	if(ListenSocket == INVALID_SOCKET){
		MessageBoxA(0 , "server: socket failed with error" , 0 , 0);
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket , result->ai_addr , (int)result->ai_addrlen);
	if(iResult == SOCKET_ERROR){
		MessageBoxA(0 , "server: bind failed with error" , 0 , 0);
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket , SOMAXCONN);
	if(iResult == SOCKET_ERROR){
		MessageBoxA(0 , "server: listen failed with erro" , 0 , 0);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket , NULL , NULL);
	if(ClientSocket == INVALID_SOCKET){
		MessageBoxA(0 , "server: accept failed with error" , 0 , 0);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// No longer need server socket
	closesocket(ListenSocket);

	// Receive until the peer shuts down the connection

	do{
		iResult = recv(ClientSocket , recvbuf , recvbuflen , 0);
		nhq_recv_data(recvbuf);
		if(iResult > 0){
			//....等待开发
		}
		else if(iResult == 0){
			MessageBoxA(0 , "Connection closing..." , 0 , 0);
		}
		else{
			MessageBoxA(0 , "recv failed with error" , 0 , 0);
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

	} while(iResult > 0);

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket , SD_SEND);
	if(iResult == SOCKET_ERROR){
		MessageBoxA(0 , "shutdown failed with error" , 0 , 0);
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}
