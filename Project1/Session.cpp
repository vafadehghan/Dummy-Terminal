#pragma once
/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:		Session.cpp - The Session layer of the serial port communication.
--
-- PROGRAM:			DummyTerminal
--
-- FUNCTIONS:		void commandMode()
--					void connectMode(HWND hWnd)
--					void exitProgram()
--
-- DATE:			October 3, 2017
--
--
-- DESIGNER:		Vafa Dehghan Saei
--
-- PROGRAMMER:		Vafa Dehghan Saei
--
-- NOTES:			This represents the Session layer of the serial port communication. This is where the computers connect through the serial port, and the settings are set.
----------------------------------------------------------------------------------------------------------------------*/
#include "Session.h"
#include "Physical.h"
int commSetup = false;
HANDLE hComm = NULL;
OVERLAPPED osWrite = { 0 };


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	commandMode
--
-- DATE:		October 3, 2017
--
--
-- DESIGNER:	Vafa Dehghan Saei
--
-- PROGRAMMER:	Vafa Dehghan Saei
--
-- INTERFACE:	void commandMode()
--
--
-- RETURNS:		void
--
-- NOTES:		This function opens a dialog and allows users to select their communication parameters.
--
----------------------------------------------------------------------------------------------------------------------*/
void commandMode() {
	COMMCONFIG cc;
	cc.dwSize = sizeof(COMMCONFIG);
	cc.wVersion = 0x100;
	DCB dcb = { 0 };

	GetCommConfig(hComm, &cc, &cc.dwSize);
	int response = CommConfigDialog(lpszCommName, hwnd, &cc);
	switch (response)
	{
	case IDOK:
		dcb.BaudRate = cc.dcb.BaudRate;
		dcb.ByteSize = cc.dcb.ByteSize;
		dcb.StopBits = cc.dcb.StopBits;
		dcb.Parity = cc.dcb.Parity;
		break;
	default:
		break;
	}
	SetCommState(hComm, &dcb);
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	connectMode
--
-- DATE:		October 3, 2017
--
--
-- DESIGNER:	Vafa Dehghan Saei
--
-- PROGRAMMER:	Vafa Dehghan Saei
--
-- INTERFACE:	void connectMode(HWND hWnd)
					HWND hWnd:A handle to the window
--
--
-- RETURNS:		void
--
-- NOTES:		This function enters connect mode where to users can communicate with each other in real time.
--
----------------------------------------------------------------------------------------------------------------------*/
void connectMode(HWND hWnd) {
	HANDLE hThread;
	DWORD threadId;
	hComm = CreateFile(lpszCommName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
	if (hComm == INVALID_HANDLE_VALUE) {
		MessageBox(hWnd, "Problem creating file", "", MB_OK);
	}

	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (osWrite.hEvent == NULL) {
		MessageBox(hWnd, "Error creating overlapped event handle", "", MB_OK);
	}

	commSetup = SetupComm(hComm, 2400, 2400);
	if (!commSetup) {
		MessageBox(hWnd, "Problem setting up comm", "", MB_OK);
	}
	else {
		commandMode();
	}
	if ((hThread = CreateThread(NULL, 0, MyReadFileThread, (LPVOID)hWnd, 0, &threadId)) == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, "Error creating READ thread", "", MB_OK);
		CloseHandle(hComm);
	}

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	exitProgram
--
-- DATE:		October 3, 2017
--
--
-- DESIGNER:	Vafa Dehghan Saei
--
-- PROGRAMMER:	Vafa Dehghan Saei
--
-- INTERFACE:	void exitProgram()
--
--
-- RETURNS:		void
--
-- NOTES:		This function exits the program.
--
----------------------------------------------------------------------------------------------------------------------*/
void exitProgram() {
	PostQuitMessage(0);
}
void disconnectPort() {
	CloseHandle(hComm);
}