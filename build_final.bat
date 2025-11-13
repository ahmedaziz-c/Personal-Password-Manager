@echo off
chcp 65001 > nul
echo Building Password Manager (Statically Linked)...
echo.

echo Cleaning previous builds...
del /Q *.o password_manager.exe 2>nul

echo Compiling with static linking...
g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -c PasswordManager.cpp -o PasswordManager.o
g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -c SimplePasswordManager.cpp -o SimplePasswordManager.o
g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -c main.cpp -o main.o

echo Linking with static libraries...
g++ -static -static-libgcc -static-libstdc++ main.o PasswordManager.o SimplePasswordManager.o -o password_manager.exe

echo.
echo ==================================================
echo BUILD COMPLETE!
echo ==================================================
echo.
echo Checking dependencies...
dir password_manager.exe
echo.
echo File size should be larger (static linking) but no DLL dependencies!
echo.
pause