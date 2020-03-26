// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <dwmapi.h>

// https://stackoverflow.com/questions/1739259/how-to-use-queryperformancecounter
double PCFreq = 0.0;
__int64 CounterStart = 0;

// Sets the PCFreq in counts per millisecond.
void setupPCFreq()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
    {
        std::cout << "QueryPerformanceFrequency failed!\n" << std::endl;
    }

    // convert li.QuadPart, which is in counts per second (counts/second), to be counts/millisecond by doing 
    // counts/second * second/milliseconds = counts/millisecond
    // where 1 second per 1000 milliseconds (1/1000)
    PCFreq = double(li.QuadPart) / 1000.0;
    std::cout << "PCFreq: " << PCFreq << " counts per millisecond" << std::endl << std::endl;
}

// Sets the counts so far into a start counter.
// Call this first before calling GetCounter.
void StartCounter()
{
    if (!PCFreq)
    {
        setupPCFreq();
    }

    LARGE_INTEGER li;
    QueryPerformanceCounter(&li); //counts so far
    CounterStart = li.QuadPart;
}

// Gets the counts so far and does calculation with start counter to get milliseconds elapsed.
// Call this after calling StartCounter.
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    // 1. current counts so far - previous counts so far = counts (over some duration)
    // 2. counts (over some duration) / counts per millisecond = milliseconds elapsed
    //    counts / (counts/millisecond) = milliseconds
    return double(li.QuadPart - CounterStart) / PCFreq;
}

// https://stackoverflow.com/questions/7277366/why-does-enumwindows-return-more-windows-than-i-expected
BOOL isVisibleOnDesktop(HWND hwnd)
{
    TITLEBARINFO ti;
    HWND hwndTry, hwndWalk = NULL;

    if (!IsWindowVisible(hwnd))
        return FALSE;

    if (IsIconic(hwnd))
        return FALSE;

    // hwndTry = GetAncestor(hwnd, GA_ROOTOWNER);
    // while (hwndTry != hwndWalk)
    // {
    //     hwndWalk = hwndTry;
    //     hwndTry = GetLastActivePopup(hwndWalk);
    //     if (IsWindowVisible(hwndTry))
    //         break;
    // }
    // if (hwndWalk != hwnd)
    //     return FALSE;

    BOOL dwmCompositionEnabled;
    DwmIsCompositionEnabled(&dwmCompositionEnabled);
    if (dwmCompositionEnabled)
    {
        // https://stackoverflow.com/questions/43927156/enumwindows-returns-closed-windows-store-applications
        BOOL isCloaked{ FALSE };
        DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &isCloaked, sizeof(isCloaked));
        if (isCloaked)
        {
            return FALSE;
        }
    }

    // the following removes some task tray programs and "Program Manager"
    ti.cbSize = sizeof(ti);
    GetTitleBarInfo(hwnd, &ti);
    if (ti.rgstate[0] & STATE_SYSTEM_INVISIBLE ||
        ti.rgstate[0] & STATE_SYSTEM_UNAVAILABLE ||
        ti.rgstate[0] & STATE_SYSTEM_OFFSCREEN)
        return FALSE;

    return TRUE;
}

BOOL CALLBACK EnumChildProc(HWND hWnd, long lParam)
{
    char buff[255];

    if (isVisibleOnDesktop(hWnd)) {
        // std::cout << "-- Child hWnd: " << hWnd << std::endl;
        GetWindowTextA(hWnd, (LPSTR)buff, 254);
        // printf("Window Text: %s\n", buff);
    }

    return TRUE;
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam)
{
    char buff[255];

    if (isVisibleOnDesktop(hWnd)) {
        // std::cout << "hWnd: " << hWnd << std::endl;
        GetWindowTextA(hWnd, (LPSTR)buff, 254);
        // printf("Window Text: %s\n", buff);
        // GetClassNameA(hWnd, (LPSTR)buff, 254);
        // printf("Class Name: %s\n", buff);
    }

    // Child windows
    EnumChildWindows(hWnd, (WNDENUMPROC)EnumChildProc, 0);

    return TRUE;
}

void dummyCalculations()
{
    int counter{ 0 };
    int odds{ 0 };
    int evens{ 0 };
    while ( counter < 1000 )
    {
        int randomNumber{ rand() };
        if (randomNumber % 2 == 0)
        {
            evens++;
        }
        else
        {
            odds++;
        }
        counter++;
    }
}

int main()
{
    int maxLoop{ 10000 };
    double elapsedTimes{ 0.0 };
    int whileCounter{ 0 };

    while (whileCounter < maxLoop)
    {
        StartCounter();
        
        EnumWindows((WNDENUMPROC)EnumWindowsProc, 0);
        dummyCalculations();

        double elapsedTime{ GetCounter() };
        elapsedTimes += elapsedTime;
        // std::cout << "Elapsed Time: " << elapsedTime << std::endl << std::endl;
        whileCounter++;

    }

    std::cout << "Total Elapsed Time With " << maxLoop << " Loops: " << elapsedTimes << " milliseconds" << std::endl;
    std::cout << "Average Elapsed Time Per Loop: " << elapsedTimes / maxLoop << " milliseconds" << std::endl;
}
