#pragma once
#ifndef _DATA_H_
#define _DATA_H_



#include "windows.h"




enum{
	MENU_NAIHEQIAO ,
	MENU_DISASM_ADLER32 ,
	MENU_DUMP_ADLER32 ,
	MENU_STACK_ADLER32
};






extern HANDLE  ServerThreadHandle;



struct BUTTONACTION{
	bool setp_over;
	bool setp_into;
	bool run;
	bool dll_inject;
	bool enjoy_it;
};
extern BUTTONACTION ButtonAction;











#ifdef _WIN64
#define DLLPATH "\\plugins\\nhq_core_x64.dll"
#else
#define DLLPATH "\\plugins\\nhq_core_x86.dll"
#endif









#ifdef _WIN64
#define XAX "RAX"
#define XBX "RBX"
#define XCX "RCX"
#define XDX "RDX"
#define XBP "RBP"
#define XSP "RSP"
#define XSI "RSI"
#define XDI "RDI"
#define X8  "R8"
#define X9  "R9"
#define X10 "R10"
#define X11 "R11"
#define X12 "R12"
#define X13 "R13"
#define X14 "R14"
#define X15 "R15"
#define XIP "RIP"

//
#define _AX Rax
#define _BX Rbx
#define _CX Rcx
#define _DX Rdx
#define _BP Rbp
#define _SP Rsp
#define _SI Rsi
#define _DI Rdi
#define _IP Rip


#else
#define XAX "EAX"
#define XBX "EBX"
#define XCX "ECX"
#define XDX "EDX"
#define XBP "EBP"
#define XSP "ESP"
#define XSI "ESI"
#define XDI "EDI"
#define XIP "EIP"

//
#define _AX Eax
#define _BX Ebx
#define _CX Ecx
#define _DX Edx
#define _BP Ebp
#define _SP Esp
#define _SI Esi
#define _DI Edi
#define _IP Eip
#endif // _WIN64




typedef struct _REG{
	char _AX[100];
	char _BX[100];
	char _CX[100];
	char _DX[100];
	char _BP[100];
	char _SP[100];
	char _SI[100];
	char _DI[100];
	char _IP[100];

#ifdef _WIN64
	char R8[100];
	char R9[100];
	char R10[100];
	char R11[100];
	char R12[100];
	char R13[100];
	char R14[100];
	char R15[100];
#endif
};

extern CONTEXT g_context;
















extern int g_SR_count;






























































#endif