#include "ui.h"


#include "../naiheqiao/nhq_data.h"
#include "../naiheqiao.h"
#include "../plugin.h"










void gui_display(nk_context *ctx){
	_REG reg = {0};
	nk_layout_row_static(ctx , 20 , 50 , 1);
	sprintf_s(reg._AX , "%s     %x" , XAX , g_context._AX);
	nk_text(ctx , reg._AX , strlen(reg._AX) , 100);

	sprintf_s(reg._BX , "%s     %x" , XBX , g_context._BX);
	nk_text(ctx , reg._BX , strlen(reg._BX) , 100);

	sprintf_s(reg._CX , "%s     %x" , XCX , g_context._CX);
	nk_text(ctx , reg._CX , strlen(reg._CX) , 100);

	sprintf_s(reg._DX , "%s     %x" , XDX , g_context._DX);
	nk_text(ctx , reg._DX , strlen(reg._DX) , 100);

	sprintf_s(reg._BP , "%s     %x" , XBP , g_context._BP);
	nk_text(ctx , reg._BP , strlen(reg._BP) , 100);

	sprintf_s(reg._SP , "%s     %x" , XSP , g_context._SP);
	nk_text(ctx , reg._SP , strlen(reg._SP) , 100);

	sprintf_s(reg._SI , "%s     %x" , XSI , g_context._SI);
	nk_text(ctx , reg._SI , strlen(reg._SI) , 100);

	sprintf_s(reg._DI , "%s     %x" , XDI , g_context._DI);
	nk_text(ctx , reg._DI , strlen(reg._DI) , 100);

#ifdef _WIN64
	sprintf_s(reg.R8 , "%s     %x" , X8 , g_context.R8);
	nk_text(ctx , reg.R8 , strlen(reg.R8) , 100);

	sprintf_s(reg.R9 , "%s     %x" , X9 , g_context.R9);
	nk_text(ctx , reg.R9 , strlen(reg.R9) , 100);

	sprintf_s(reg.R10 , "%s     %x" , X10 , g_context.R10);
	nk_text(ctx , reg.R10 , strlen(reg.R10) , 100);

	sprintf_s(reg.R11 , "%s     %x" , X11 , g_context.R11);
	nk_text(ctx , reg.R11 , strlen(reg.R11) , 100);

	sprintf_s(reg.R12 , "%s     %x" , X12 , g_context.R12);
	nk_text(ctx , reg.R12 , strlen(reg.R12) , 100);

	sprintf_s(reg.R13 , "%s     %x" , X13 , g_context.R13);
	nk_text(ctx , reg.R13 , strlen(reg.R13) , 100);

	sprintf_s(reg.R14 , "%s     %x" , X14 , g_context.R14);
	nk_text(ctx , reg.R14 , strlen(reg.R14) , 100);

	sprintf_s(reg.R15 , "%s     %x" , X15 , g_context.R15);
	nk_text(ctx , reg.R15 , strlen(reg.R15) , 100);
#endif
	sprintf_s(reg._IP , "%s     %x" , XIP , g_context._IP);
	nk_text(ctx , reg._IP , strlen(reg._IP) , 100);


#ifdef _WIN64
	nk_layout_row_dynamic(ctx , 20 , 50);
#else
	nk_layout_row_dynamic(ctx , 200 , 50);
#endif // _WIN64

	nk_layout_row_static(ctx , 20 , 365 , 1);


	if(nk_button_label(ctx , "inject dll")){
		if(!nhq_inject_dll()){
			MessageBox(0 , "inject fail" , 0 , 0);
		}
	}
}








static LRESULT CALLBACK
WindowProc(HWND wnd , UINT msg , WPARAM wparam , LPARAM lparam){
	switch(msg){
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	if(nk_gdi_handle_event(wnd , msg , wparam , lparam))
		return 0;

	return DefWindowProcW(wnd , msg , wparam , lparam);
}





int nhq_gui(void){
	GdiFont* font;
	struct nk_context *ctx;

	WNDCLASSW wc;
	ATOM atom;
	RECT rect = {0 , 0 , WINDOW_WIDTH , WINDOW_HEIGHT};
	DWORD style = WS_OVERLAPPEDWINDOW;
	DWORD exstyle = WS_EX_APPWINDOW;
	HWND wnd;
	HDC dc;
	int running = 1;
	int needs_refresh = 1;

	/* Win32 */
	memset(&wc , 0 , sizeof(wc));
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandleW(0);
	wc.hIcon = LoadIcon(NULL , IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL , IDC_ARROW);
	wc.lpszClassName = L"NuklearWindowClass";
	atom = RegisterClassW(&wc);

	AdjustWindowRectEx(&rect , style , FALSE , exstyle);
	wnd = CreateWindowExW(exstyle , wc.lpszClassName , L"дн╨нге" ,
						  style | WS_VISIBLE , CW_USEDEFAULT , CW_USEDEFAULT ,
						  rect.right - rect.left , rect.bottom - rect.top ,
						  NULL , NULL , wc.hInstance , NULL);
	dc = GetDC(wnd);

	/* GUI */
	font = nk_gdifont_create("Arial" , 14);
	ctx = nk_gdi_init(font , dc , WINDOW_WIDTH , WINDOW_HEIGHT);


	while(running){
		/* Input */
		MSG msg;
		nk_input_begin(ctx);
		if(needs_refresh == 0){
			if(GetMessageW(&msg , NULL , 0 , 0) <= 0)
				running = 0;
			else{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
			needs_refresh = 1;
		}
		else needs_refresh = 0;

		while(PeekMessageW(&msg , NULL , 0 , 0 , PM_REMOVE)){
			if(msg.message == WM_QUIT)
				running = 0;
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			needs_refresh = 1;
		}
		nk_input_end(ctx);

		/* GUI */
		if(nk_begin(ctx , "Register" , nk_rect(100 , 50 , 400 , 500) ,
		   NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
		   NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)){


			/******************************************************************************************************/
			gui_display(ctx);
			/******************************************************************************************************/

		}
		nk_end(ctx);
		if(nk_window_is_closed(ctx , "Register")) break;

		nk_gdi_render(nk_rgb(30 , 30 , 30));
	}
	ButtonAction.enjoy_it = false;
	nk_gdifont_del(font);
	ReleaseDC(wnd , dc);
	UnregisterClassW(wc.lpszClassName , wc.hInstance);
	return 0;
}





PLUG_EXPORT void CBMENUENTRY(CBTYPE cbType , PLUG_CB_MENUENTRY* info){
	switch(info->hEntry){
	case MENU_NAIHEQIAO:
		if(!ButtonAction.enjoy_it){
			ButtonAction.enjoy_it = true;
			nhq_gui();
		}
		break;
	}
}



