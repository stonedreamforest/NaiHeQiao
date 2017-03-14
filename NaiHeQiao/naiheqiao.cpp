#include "naiheqiao.h"
#include "windows.h"
#include "naiheqiao/nhq_data.h"
#include "plugin.h"

using namespace Script::Debug;

std::vector<duint> GetAddresses(duint start , duint count){
	std::vector<duint> result;
	std::vector<unsigned char> data(count * 16);
	if(DbgMemRead(start , data.data() , data.size())){
		BASIC_INSTRUCTION_INFO info;
		for(duint i = 0; count; count--){
			i += DbgFunctions()->DisasmFast(data.data() + i , start + i , &info) ? info.size : 1;
			result.push_back(start + i);
		}
	}
	return result;
}


void nhq_recv_data(char *context){
	if(g_SR_count % 2 == 0 && *context != NULL){
		memcpy(&g_context , context , sizeof(CONTEXT));
		nhq_step_over(g_context._IP);
		//ButtonAction.setp_over = false;
	}
	g_SR_count++;
}

void nhq_step_over(duint CurrentAdress){
	DeleteBreakpoint(CurrentAdress);
	SetBreakpoint(GetAddresses(CurrentAdress , 1)[0]);
}




bool nhq_inject_dll(){
	if(ButtonAction.dll_inject){
		_plugin_logprintf("dll already inject");
		return true;
	}
	char DllPath[MAX_PATH] = "";
	GetCurrentDirectory(MAX_PATH , DllPath);
	strcat_s(DllPath , DLLPATH);
	_plugin_logprintf("%s" , DllPath);
	HANDLE process = DbgGetProcessHandle();
	if(!process){
		_plugin_logprintf("get process handle error");
		return false;
	}
	LPVOID memory = LPVOID(VirtualAllocEx(process , nullptr , strlen(DllPath) + 1 , MEM_COMMIT | MEM_RESERVE , PAGE_READWRITE));
	if(!memory){
		_plugin_logprintf("get memory handle error");
		return false;
	}
	if(!WriteProcessMemory(process , memory , DllPath , strlen(DllPath) + 1 , nullptr)){
		_plugin_logprintf("WriteProcessMemory error");
		return false;
	}
	if(!CreateRemoteThread(process , nullptr , NULL , LPTHREAD_START_ROUTINE(GetProcAddress(GetModuleHandleA("kernel32.dll") , "LoadLibraryA")) , memory , NULL , nullptr)){
		_plugin_logprintf("CreateRemoteThread error");
		return false;
	}
	ButtonAction.dll_inject = true;
	return ButtonAction.dll_inject;
}
