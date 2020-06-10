#pragma once
//#ifdef SYSHOOKPROC_EXPORTS
//#define SYSHOOKPROC_API __declspec(dllexport)
//#else
//#define SYSHOOKPROC_API __declspec(dllimport)
//#endif
#include<Windows.h>
#include<WinUser.h>
#include<iostream>
#include<Strsafe.h>
//extern "C" SYSHOOKPROC_API 
extern LRESULT CALLBACK SysCBTProc(int nCode, WPARAM wParam, LPARAM lParam);
