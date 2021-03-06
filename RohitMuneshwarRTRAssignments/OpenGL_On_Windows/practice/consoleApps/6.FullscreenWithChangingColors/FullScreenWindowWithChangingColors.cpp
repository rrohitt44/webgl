#include<Windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
bool gbDone = false;
bool gbFullScreen = false;
HWND ghwnd;
DWORD dwStyle;
int giPaintFlag = -1;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLIne, int iCmdShow){
	WNDCLASSEX wndclass;
	TCHAR AppName[] = TEXT("Window Custom");
	TCHAR WinName[] = TEXT("Window At Centre");
	HWND hwnd;
	MSG msg;
	RECT rect;
	//initialize window class
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = AppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	//initialization complete
	
	RegisterClassEx(&wndclass); //Register Class
	
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	int width = (rect.right-rect.left)/2 - WIN_WIDTH/2;
	int height = (rect.bottom - rect.top)/2 - WIN_HEIGHT/2;
	//int width = GetSystemMetrics(SM_CXSCREEN)/2 - WIN_WIDTH/2;
	//int height = GetSystemMetrics(SM_CYSCREEN)/2 - WIN_HEIGHT/2;
	//create window in memory
	hwnd = CreateWindow(
	AppName,
	WinName,
	WS_OVERLAPPEDWINDOW,
	width,
	height,
	WIN_WIDTH,
	WIN_HEIGHT,
	NULL,
	NULL,
	hInstance,
	NULL
	);
	ghwnd = hwnd;
	ShowWindow(hwnd, iCmdShow); //Show Window
	//ShowWindow(hwnd, SW_MAXIMIZE); //Show Window
	UpdateWindow(hwnd); // color/paint the background of the window
	
	//message loop
	while(gbDone==false){
		
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if(msg.message == WM_QUIT){
				gbDone = true;
			}else{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}else{
			//rendering starts here
		}
	}
	return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam){
	void ToggleFullscreen();
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rc;
	HBRUSH hBrush;
	switch(iMsg){
		case WM_PAINT:
			hdc = BeginPaint(hwnd,&ps);
			GetClientRect(hwnd,&rc);
			if(giPaintFlag == 1){
				hBrush = CreateSolidBrush(RGB(255,0,0));
			}else if(giPaintFlag == 2){
				hBrush = CreateSolidBrush(RGB(0,255,0));
			}else if(giPaintFlag == 3){
				hBrush = CreateSolidBrush(RGB(0,0,255));
			}else if(giPaintFlag == 4){
				hBrush = CreateSolidBrush(RGB(0,255,255));
			}else if(giPaintFlag == 5){
				hBrush = CreateSolidBrush(RGB(255,0,255));
			}else if(giPaintFlag == 6){
				hBrush = CreateSolidBrush(RGB(255,255,0));
			}else if(giPaintFlag == 7){
				hBrush = CreateSolidBrush(RGB(0,0,0));
			}else if(giPaintFlag == 8){
				hBrush = CreateSolidBrush(RGB(255,255,255));
			}
			FillRect(hdc,&rc,hBrush);
			EndPaint(hwnd,&ps);
		break;
		case WM_KEYDOWN:
			switch(wParam){
				case VK_ESCAPE:
					DestroyWindow(hwnd);
					break;
				break;
				case 0x46: //F or f key
						ToggleFullscreen();
				break;
				case 0x52: //R
					giPaintFlag = 1;
					InvalidateRect(hwnd, NULL, TRUE); //calls WM_PAINT
				break;
				case 0x47: //G
					giPaintFlag = 2;
					InvalidateRect(hwnd, NULL, TRUE); //calls WM_PAINT
				break;
				case 0x42: //B
					giPaintFlag = 3;
					InvalidateRect(hwnd, NULL, TRUE); //calls WM_PAINT
				break;
				case 0x43: //C
					giPaintFlag = 4;
					InvalidateRect(hwnd, NULL, TRUE);
				break;
				case 0x4D: //M
					giPaintFlag = 5;
					InvalidateRect(hwnd, NULL, TRUE);
				break;
				case 0x59: //Y
					giPaintFlag = 6;
					InvalidateRect(hwnd, NULL, TRUE);
				break;
				case 0x4B: //K
					giPaintFlag = 7;
					InvalidateRect(hwnd, NULL, TRUE);
				break;
				case 0x57: //W
					giPaintFlag = 8;
					InvalidateRect(hwnd, NULL, TRUE);
				break;
				default:
					TCHAR str[255];
					wsprintf(str,TEXT("There is no color for this key %d"),wParam);
					MessageBox(hwnd,str,TEXT("To Rohit"),MB_OK);
				break;
			}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void ToggleFullscreen(){
	HMONITOR hMonitor;
	//check kara window fullscreen ahe ka
	if(gbFullScreen == false){
		//get current window style
		 dwStyle = GetWindowLong(ghwnd,GWL_STYLE);
		 
		 //does it includes overlapped window
		if(dwStyle & WS_OVERLAPPEDWINDOW){
			
			//evaluate current window placement
			bool bWindowPlacement = GetWindowPlacement(ghwnd,&wpPrev);
			
			//take the information of the window which you want to make full screen
			hMonitor = MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY);
			MONITORINFO mi;
			mi.cbSize = sizeof(MONITORINFO);
			bool bMonitorInfo = GetMonitorInfo(hMonitor, &mi);
			
			if(bWindowPlacement && bMonitorInfo){
				//set window style accordingly
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				//set window position accordingly
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, (mi.rcMonitor.right - mi.rcMonitor.left), 
				(mi.rcMonitor.bottom - mi.rcMonitor.top), SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			//hide the cursor
			ShowCursor(FALSE);
			gbFullScreen = true;
		}
	}else{
		//code to restore the full screen
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle & WS_OVERLAPPEDWINDOW);
		//set window placement back to previous
		SetWindowPlacement(ghwnd, &wpPrev);
		//set window position
		SetWindowPos(ghwnd, HWND_TOP, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		//show cursor
		ShowCursor(TRUE);
		gbFullScreen = false;
	}
}