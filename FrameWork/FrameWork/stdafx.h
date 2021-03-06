// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once



#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>

// 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

extern HWND g_hWnd;

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#define Singletone(class_name)\
	private:\
	class_name(void);\
	~class_name(void);\
	public:\
	static class_name* GetInstance()\
	{\
		static class_name instance;\
	return &instance;\
	}

#define SafeUpdate(p)	{if(p) p->Update();}
#define SafeRender(p)	{if(p) p->Render();}

#define SafeRelease(p)	{if(p) p->Release(); p = NULL;}
#define SafeDelete(p) {if(p) delete p; p = NULL;}
#define SafeDeleteArray(p) {if(p) delete[] p; p = NULL;}
#define Synthesize(varType,varName,FunName)\
	protected: varType varName; \
	public: inline varType Get##FunName(void) const{return varName;}\
	public: inline void Set##FunName(varType var){varName = var;}

extern HWND g_hWnd;
extern bool updateblock;
#include "cDeviceManage.h"
#include "Logger.h"
#include "EventManager.h"
#include "SceneManager.h"

#pragma region json
#ifndef _MSC_VER // json헤더에서 요구
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "json parson.h"
#include "json additional code.h"
#pragma endregion


inline bool STRCMP(char* str1, char* str2)
{
	return strcmp(str1, str2) == 0 ? true : false;
}

