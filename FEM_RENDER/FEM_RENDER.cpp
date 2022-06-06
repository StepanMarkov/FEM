#include "stdafx.h"
#include "FEM_RENDER.h" 
#include "resource.h"

using namespace DirectX;

CSCENE SCENE;

//============================================================================================================================================================//
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)  
{
	ShowCursor(0);
	UNREFERENCED_PARAMETER(hPrevInstance);														   
	UNREFERENCED_PARAMETER(lpCmdLine);															   																								   	
	if (FAILED(InitWindow(hInstance, nCmdShow)))   return -1;	
	SCENE.Initialization();																						   
	MSG msg = { 0 };

	while (WM_QUIT != msg.message)																   
	{																							   
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))										   
		{																						   
			TranslateMessage(&msg);																   
			DispatchMessage(&msg);																   
		}																						   
		else																					   
		{																						   
			SCENE.Render();
		}																						   
	}																							   
																								   
	SCENE.~CSCENE();
																								   
	return (int)msg.wParam;																		   
}																								   
//===========================================================================================================================================================//
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)                                              
{																								   
	//------------------------------------------------------------------//						   
	{
		WNDCLASSEX wcex;

		wcex.cbSize        = sizeof(WNDCLASSEX);
		wcex.style         = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc   = WndProc;
		wcex.cbClsExtra    = 0;
		wcex.cbWndExtra    = 0;
		wcex.hInstance     = hInstance;
		wcex.hIcon         = nullptr;
		wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName  = nullptr;
		wcex.lpszClassName = L"SCENE";
		wcex.hIconSm = nullptr;

		if (!RegisterClassEx(&wcex))
			return E_FAIL;
	}
	//----------------------------------------------------------------//						   
																								   
	SCENE.g_hInst = hInstance;
	RECT rc = { 0, 0, LONG(SCENE.width), LONG(SCENE.height) };
	AdjustWindowRect(&rc, WS_POPUP | WS_EX_TOPMOST, FALSE);
	SCENE.g_hWnd = CreateWindow(L"SCENE", L"MESH", WS_POPUP | WS_EX_TOPMOST,	0, 0, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
	if (!SCENE.g_hWnd) return E_FAIL;																			   																								   
	ShowWindow(SCENE.g_hWnd, nCmdShow);															   
																								   
	return S_OK;																				   
}																								   
//===========================================================================================================================================================//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)                    
{																								   
																				                   																								   //
	switch (message)																			   
	{																			                   
																								   
	case WM_DESTROY:																			   
		PostQuitMessage(0);																		   
		break;																					   
																								   
	default:																					   
		return DefWindowProc(hWnd, message, wParam, lParam);									   
	}																							   
	
	return 0;																					   
}																								   																												  
//==========================================================================================================================================================//
