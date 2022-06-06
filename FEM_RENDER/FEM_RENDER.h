#pragma once

#include "resource.h"
#include <windows.h>
#include "..\\FEM\\FEM_SCENE.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);              	                                                                                  
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);