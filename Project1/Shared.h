#pragma once
#ifndef SHARED_H
#define SHARED_H
#include <windows.h>
#include <iostream>
#include <string.h>

#include "resource.h"
#include "Physical.h"
#include "Session.h"

extern HANDLE hComm;
extern HWND hwnd;
extern LPCSTR lpszCommName;
extern BOOL commSetup;
extern OVERLAPPED osWrite;

#endif