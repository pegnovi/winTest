This project attempts to measure performance when querying of all Windows 10 windows via `EnumWindows` & `EnumChildWindows` during a loop.


### Steps To Run:

Download and run a Windows 10 VM.
https://developer.microsoft.com/en-us/windows/downloads/virtual-machines/

Install the Microsoft Visual C++ (MSVC) compiler toolset.
Can update it through Visual Studio to install C++ and update Windows 10 SDK.

Can use either Visual Studio 2019 or VSCode to compile and run the code.

For VSCode, follow the instructions here:
https://code.visualstudio.com/docs/cpp/config-mingw


### Test

Once winTest.exe is built, run it.
Average elapsed time per loop of EnumWindows & EnumChildWindows should be around 0.6 - 0.7 milliseconds with 10 windows open.
If logs are enabled, it increases to around 1.3 - 1.5 milliseconds per loop with 10 windows open.