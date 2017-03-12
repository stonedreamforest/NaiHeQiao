// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "nhq_core.h"






#define STEP_OVER "STEP_OVER"
#define STEP_INTO "STEP_INTO"
#define RUN	"RUN"







LONG CALLBACK veh(_In_ PEXCEPTION_POINTERS ExceptionInfo){
	int iResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	while(1){
		MessageBoxA(0 , "发送消息" , 0 , 0);
		iResult = send(nhq_client.ConnectSocket , (char *)ExceptionInfo->ContextRecord , sizeof(CONTEXT) , 0);
		if(iResult == SOCKET_ERROR){
			MessageBoxA(0 , "client: send failed with error" , 0 , 0);
			closesocket(nhq_client.ConnectSocket);
			WSACleanup();
			return 1;
		}
		do{
			iResult = recv(nhq_client.ConnectSocket , recvbuf , recvbuflen , 0);
			if(iResult > 0){
				//MessageBoxA(0 , "EXCEPTION_CONTINUE_EXECUTION" , 0 , 0);
				return EXCEPTION_CONTINUE_EXECUTION;
				/*if(strcmp(STEP_OVER , recvbuf) == 0){
					return EXCEPTION_CONTINUE_EXECUTION;
				}
				else if(strcmp(STEP_INTO , recvbuf) == 0){
					return EXCEPTION_CONTINUE_EXECUTION;
				}
				else if(strcmp(RUN , recvbuf) == 0){
					return EXCEPTION_CONTINUE_EXECUTION;
				}
				else{
					MessageBoxA(0 , "error action" , 0 , 0);
				}*/
			}
			else if(iResult == 0){
				MessageBoxA(0 , "client: Connection closed" , 0 , 0);
			}
			else{
				MessageBoxA(0 , "client: recv failed with error" , 0 , 0);
			}
		} while(iResult > 0);
	}
	return EXCEPTION_CONTINUE_EXECUTION;
}

DWORD WINAPI ThreadFunc(VOID){
	init_NHQ_client();
	AddVectoredExceptionHandler(0 , veh);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule ,
					  DWORD  ul_reason_for_call ,
					  LPVOID lpReserved
){
	if(ul_reason_for_call == DLL_PROCESS_ATTACH){
		CreateThread(NULL , NULL , (LPTHREAD_START_ROUTINE)ThreadFunc , NULL , NULL , NULL);
	}
	return TRUE;
}

