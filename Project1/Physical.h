#pragma once
#ifndef PHYSICAL_H
#define PHYSICAL_H

#include "Shared.h"

DWORD WINAPI MyReadFileThread(LPVOID n);
void WriteToPort(WPARAM wParam);
#endif // !PHYSICAL_H
