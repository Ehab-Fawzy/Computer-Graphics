#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <iostream>
using namespace std;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

bool firstClick = true;
int x1 , y1 , x2 , y2;

void drawLine( HDC hcd , COLORREF color){
    if ( x1 == x2 && y1 == y2 ){
        SetPixel(hcd , x1 , y1 , color);
        return;
    }

    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    long double slope = (1.0 * deltaY) / deltaX;

    if ( abs(deltaX) >= abs(deltaY) ){
        if ( x1 > x2 ){
            swap(x1 , x2);
            swap(y1 , y2);
        }
        for ( int x = x1; x <= x2; ++x ){
            long double y = slope * (x - x1) + y1;
            SetPixel(hcd , x , (int)y , color);
        }
    }
    else{
        if ( y1 > y2 ){
            swap(x1 , x2);
            swap(y1 , y2);
        }
        for ( int y = y1; y <= y2; ++y ){
            long double x = ( (1.0 * (y - y1)) / slope ) + x1;
            SetPixel(hcd , (int)x , y , color);
        }
    }
}

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                BeginPaint(hwnd , &ps);
                drawLine(ps.hdc , RGB(0,0,0));
                EndPaint(hwnd , &ps);
            }
            break;
        case WM_LBUTTONDOWN:
            {
                if ( firstClick ){
                    x1 = LOWORD(lParam);
                    y1 = HIWORD(lParam);
                    firstClick = false;
                }
                else{
                    x2 = LOWORD(lParam);
                    y2 = HIWORD(lParam);
                    firstClick = true;
                    InvalidateRect(hwnd , NULL , false);
                }
            }
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
