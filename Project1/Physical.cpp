#pragma once
/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:		Physical.cpp - The Physical layer of the serial port communication.
--
-- PROGRAM:			DummyTerminal
--
-- FUNCTIONS:		DWORD WINAPI MyReadFileThread(LPVOID n)
--					void WriteToPort(WPARAM wParam)
--
--
-- DATE:			October 3, 2017
--
--
-- DESIGNER:		Vafa Dehghan Saei
--
-- PROGRAMMER:		Vafa Dehghan Saei
--
-- NOTES:			This represents the physical layer of the serial port communication. This is where the serial port is read and written.
----------------------------------------------------------------------------------------------------------------------*/
#include "Physical.h"


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	MyReadFileThread
--
-- DATE:		October 3, 2017
--
--
-- DESIGNER:	Vafa Dehghan Saei
--
-- PROGRAMMER:	Vafa Dehghan Saei
--
-- INTERFACE:	DWORD WINAPI MyReadFileThread(LPVOID n)
--					LPVOID n: The arguments of this thread
--
--
-- RETURNS:		If the function succeeds it returns a 1.
--
-- NOTES:		This function is a thread that reads in from the serial port and displays it on to the screen.
--
----------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI MyReadFileThread(LPVOID n) {
	OVERLAPPED osRead = { 0 };
	osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	char output[2] = { NULL, NULL };
	BOOL readThreadDone = false;
	char str[100];
	HDC hdc;
	static unsigned k = 0;
	DWORD dwRead;

	while (!readThreadDone) {
		if (!ReadFile(hComm, output, 2, &dwRead, &osRead))
		{
			if ((GetLastError()) == ERROR_IO_PENDING)
			{
				if (GetOverlappedResult(hComm, &osRead, &dwRead, TRUE))
				{
					if (dwRead) {
						output[dwRead] = '/0';
						hdc = GetDC(hwnd);
						sprintf_s(str, "%c", (char)output[0]);
						TextOut(hdc, 10 * k, 0, str, strlen(str));
						k++;
						ReleaseDC(hwnd, hdc);
					}
				}
			}
		}
		else {
			if (dwRead) {
				output[dwRead] = '/0';
				hdc = GetDC(hwnd);
				sprintf_s(str, "%c", (char)output[0]);
				TextOut(hdc, 10 * k, 0, str, strlen(str));
				k++;
				ReleaseDC(hwnd, hdc);
			}
		}
	}
	return 1;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	WriteToPort
--
-- DATE:		October 3, 2017
--
--
-- DESIGNER:	Vafa Dehghan Saei
--
-- PROGRAMMER:	Vafa Dehghan Saei
--
-- INTERFACE:	void WriteToPort(WPARAM wParam)
--					WPARAM wParam: the wParam of the keyboard button pressed.
--
--
-- RETURNS:		void
--
-- NOTES:		This function takes in the keyboard inputs wParam and sends it through the serial port.
--
----------------------------------------------------------------------------------------------------------------------*/
void WriteToPort(WPARAM wParam) {
	WriteFile(hComm, (LPBYTE)&wParam, 1, NULL, &osWrite);
}