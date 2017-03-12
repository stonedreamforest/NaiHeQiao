
#include "plugin.h"
#include "naiheqiao/nhq_data.h"

#include "naiheqiao/nhq_server.h"











static void Adler32Menu(int hWindow){
	if(!DbgIsDebugging()){
		_plugin_logputs("[" PLUGIN_NAME "] You need to be debugging to use this command");
		return;
	}
	SELECTIONDATA sel;
	GuiSelectionGet(hWindow , &sel);
	duint len = sel.end - sel.start + 1;
	unsigned char* data = new unsigned char[len];
	if(DbgMemRead(sel.start , data , len)){
		DWORD a = 1 , b = 0;
		for(duint index = 0; index < len; ++index){
			a = (a + data[index]) % 65521;
			b = (b + a) % 65521;
		}
		delete[] data;
		DWORD checksum = (b << 16) | a;
		_plugin_logprintf("[" PLUGIN_NAME "] Adler32 of %p[%X] is: %08X\n" , sel.start , len , checksum);
	}
	else
		_plugin_logputs("[" PLUGIN_NAME "] DbgMemRead failed...");
}

static bool cbTestCommand(int argc , char* argv[]){
	_plugin_logputs("[" PLUGIN_NAME "] Test command!");
	char line[GUI_MAX_LINE_SIZE] = "";
	if(!GuiGetLineWindow("test" , line))
		_plugin_logputs("[" PLUGIN_NAME "] Cancel pressed!");
	else
		_plugin_logprintf("[" PLUGIN_NAME "] Line: \"%s\"\n" , line);
	return true;
}

static duint exprZero(int argc , duint* argv , void* userdata){
	return 0;
}

PLUG_EXPORT void CBINITDEBUG(CBTYPE cbType , PLUG_CB_INITDEBUG* info){
   // _plugin_logprintf("[" PLUGIN_NAME "] Debugging of %s started!\n", info->szFileName);
}

PLUG_EXPORT void CBSTOPDEBUG(CBTYPE cbType , PLUG_CB_STOPDEBUG* info){
   // _plugin_logputs("[" PLUGIN_NAME "] Debugging stopped!");
}

PLUG_EXPORT void CBEXCEPTION(CBTYPE cbType , PLUG_CB_EXCEPTION* info){
   // _plugin_logprintf("[" PLUGIN_NAME "] ExceptionRecord.ExceptionCode: %08X\n", info->Exception->ExceptionRecord.ExceptionCode);
}

PLUG_EXPORT void CBDEBUGEVENT(CBTYPE cbType , PLUG_CB_DEBUGEVENT* info){
   /* if(info->DebugEvent->dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
	{
		_plugin_logprintf("[" PLUGIN_NAME "] DebugEvent->EXCEPTION_DEBUG_EVENT->%.8X\n", info->DebugEvent->u.Exception.ExceptionRecord.ExceptionCode);
	}*/
}



//Initialize your plugin data here.
bool pluginInit(PLUG_INITSTRUCT* initStruct){


	ServerThreadHandle = CreateThread(NULL , NULL , (LPTHREAD_START_ROUTINE)init_NHQ_server , NULL , NULL , NULL);
	if(!_plugin_registercommand(pluginHandle , PLUGIN_NAME , cbTestCommand , true))
		_plugin_logputs("[" PLUGIN_NAME "] Error registering the \"" PLUGIN_NAME "\" command!");

	if(!_plugin_registerexprfunction(pluginHandle , PLUGIN_NAME ".zero" , 0 , exprZero , nullptr))
		_plugin_logputs("[" PLUGIN_NAME "] Error registering the \"" PLUGIN_NAME ".zero\" expression function!");

	return true; //Return false to cancel loading the plugin.
}

//Deinitialize your plugin data here (clearing menus optional).
bool pluginStop(){
	CloseHandle(ServerThreadHandle);
	_plugin_unregistercommand(pluginHandle , PLUGIN_NAME);
	_plugin_menuclear(hMenu);
	_plugin_menuclear(hMenuDisasm);
	_plugin_menuclear(hMenuDump);
	_plugin_menuclear(hMenuStack);
	return true;
}

//Do GUI/Menu related things here.
void pluginSetup(){
	_plugin_menuaddentry(hMenu , MENU_NAIHEQIAO , "&enjoy it");
   // _plugin_menuaddentry(hMenuDisasm, MENU_DISASM_ADLER32, "&Adler32 Selection");
  //  _plugin_menuaddentry(hMenuDump, MENU_DUMP_ADLER32, "&Adler32 Selection");
  //  _plugin_menuaddentry(hMenuStack, MENU_STACK_ADLER32, "&Adler32 Selection");
}
