#pragma once
/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:		Application.cpp - The main entry point of the program that houses WinMain and WndProc.
--
-- PROGRAM:			DummyTerminal
--
-- FUNCTIONS:		int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspzCmdParam, int nCmdShow) 
--					LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
--					INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
--
--
-- DATE:			October 3, 2017
--
--
-- DESIGNER:		Vafa Dehghan Saei
--
-- PROGRAMMER:		Vafa Dehghan Saei
--
-- NOTES:			This file is the entry point of DummyTerminal the creates the window that stores the terminal.
--					Once a key is setup is completed then users can communicate with another computer through the serial port.
----------------------------------------------------------------------------------------------------------------------*/

#include "Application.h"

LPCSTR lpszCommName = "COM1";
HWND hwnd;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	WinMain
--
-- DATE:		October 3, 2017
--
--
-- DESIGNER:	Vafa Dehghan Saei
--
-- PROGRAMMER:	Vafa Dehghan Saei
--
-- INTERFACE:	int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspzCmdParam, int nCmdShow) 
--					HINSTANCE hInst: A handle to the current instance of the application
--					HINSTANCE hprevInstance: A handle to the previous instance of the application.
--					LPSTR lspzCmdParam: The command line for the application.
--					int nCmdShow: Controls how the windows should be shown.
--
--
-- RETURNS:		If the function succeeds, and terminated successfully with WM_QUIT, it returns the exit value contained in WM_QUITs wParam. If terminated before the message loop, return a 0
--
-- NOTES:		This function creates the window and is the application entry point.
--			
----------------------------------------------------------------------------------------------------------------------*/
int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspzCmdParam, int nCmdShow) {

	MSG Msg;
	WNDCLASSEX Wcl;

	// Define a Window class
	Wcl.cbSize = sizeof(WNDCLASSEX);
	Wcl.style = 0; // default style
	Wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION); // large icon 
	Wcl.hIconSm = NULL; // use small version of large icon
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW);  // cursor style

	Wcl.lpfnWndProc = WndProc; // window function
	Wcl.hInstance = hInst; // handle to this instance
	Wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //white background
	Wcl.lpszClassName = Name; // window class name

	Wcl.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); // no class menu 
	Wcl.cbClsExtra = 0;      // no extra memory needed
	Wcl.cbWndExtra = 0;

	// Register the class
	if (!RegisterClassEx(&Wcl))
		return 0;

	hwnd = CreateWindow(
		Name, // name of window class
		Name, // title 
		WS_OVERLAPPEDWINDOW, // window style - normal
		CW_USEDEFAULT,	// X coord
		CW_USEDEFAULT, // Y coord
		CW_USEDEFAULT, // width
		CW_USEDEFAULT, // height
		NULL, // no parent window
		NULL, // no menu
		hInst, // instance handle
		NULL // no additional arguments
	);
	// Display the window
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	// Create the message loop
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg); // translate keybpard messages
		DispatchMessage(&Msg); // dispatch message and return control to windows
	}

	return Msg.wParam;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	WndProc
--
-- DATE:		October 3, 2017
--
--
-- DESIGNER:	Vafa Dehghan Saei
--
-- PROGRAMMER:	Vafa Dehghan Saei
--
-- INTERFACE:	LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
--					HWND hwnd: A handle to the window.
--					UINT Message: The message.
--					WPARAM wParam: Additional message information.
--					LPARAM lParam: Additional message information.
--
--
-- RETURNS:		The result of the message processing which depends on the message sent.
--
-- NOTES:		This function processes the messages sent to a window, and takes appropriate actions which is defined by the user.
--
----------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	
	switch (Message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_EXIT:
			exitProgram();
			break;
		case ID_HELP_ABOUT:
			//DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, About);
			break;
		case ID_FILE_NEWCONNECTION:
			connectMode(hwnd);
			break;
		case ID_FILE_SETTINGS:
			commandMode();
			break;
		case ID_FILE_DISCONNECT:
			disconnectPort();
			break;
		}
		break;
	case WM_CHAR:
		if (!commSetup) {
			MessageBox(hwnd, "Setup First", "", MB_OK);
		}
		else {
			if (wParam == VK_ESCAPE) {
				disconnectPort();
				commandMode();
			}
			else {
				WriteToPort(wParam);
			}
			}
		break;

	case WM_DESTROY:		// message to terminate the program
		PostQuitMessage(0);
		break;

	default: // Let Win32 process all other messages
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}




/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	About
--
-- DATE:		October 3, 2017
--
--
-- DESIGNER:	Vafa Dehghan Saei
--
-- PROGRAMMER:	Vafa Dehghan Saei
--
-- INTERFACE:	INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
--					HWND hDlg: A handle to the window.
--					UINT Message: The message.
--					WPARAM wParam: Additional message information.
--					LPARAM lParam: Additional message information.
--
--
-- RETURNS:		The result of the message processing which depends on the message sent.
--
-- NOTES:		This function processes the About Dialog and closes when OK is clicked on.
--
----------------------------------------------------------------------------------------------------------------------*/
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
