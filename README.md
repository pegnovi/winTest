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

Results so far:

Total Elapsed Time With 10000 Loops: 16826.9 milliseconds
Average Elapsed Time Per Loop: 1.68269 milliseconds
CPU Usage: ~50%-55%

Total Elapsed Time With 50000 Loops: 94546.6 milliseconds
Average Elapsed Time Per Loop: 1.89093 milliseconds
CPU Usage: ~50%-55%