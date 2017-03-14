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

		iResult = send(nhq_client.ConnectSocket , (char *)ExceptionInfo->ContextRecord , sizeof(CONTEXT) , 0);
		if(iResult == SOCKET_ERROR){
			MessageBoxA(0 , "client: send failed with error" , 0 , 0);
			closesocket(nhq_client.ConnectSocket);
			WSACleanup();
			return 1;
		}
		MessageBoxA(0 , "回车单步" , "单步" , 0);
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	return EXCEPTION_CONTINUE_EXECUTION;
}

DWORD WINAPI ThreadFunc(VOID){
	init_NHQ_client();
	AddVectoredExceptionHandler(1 , veh);
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

