@echo off
echo Building Password Manager for Windows...
echo.

echo Step 1: Compiling PasswordManager.cpp...
g++ -std=c++17 -Wall -Wextra -O2 -c PasswordManager.cpp -o PasswordManager.o
if errorlevel 1 (
    echo Error compiling PasswordManager.cpp
    pause
    exit /b 1
)

echo Step 2: Compiling PasswordManagerTUI.cpp...
g++ -std=c++17 -Wall -Wextra -O2 -c PasswordManagerTUI.cpp -o PasswordManagerTUI.o
if errorlevel 1 (
    echo Error compiling PasswordManagerTUI.cpp
    pause
    exit /b 1
)

echo Step 3: Compiling main.cpp...
g++ -std=c++17 -Wall -Wextra -O2 -c main.cpp -o main.o
if errorlevel 1 (
    echo Error compiling main.cpp
    pause
    exit /b 1
)

echo Step 4: Linking everything together...
g++ main.o PasswordManager.o PasswordManagerTUI.o -o password_manager.exe -luser32
if errorlevel 1 (
    echo Error during linking
    pause
    exit /b 1
)

echo.
echo ===============================================
echo BUILD SUCCESSFUL!
echo ===============================================
echo Run the program with: password_manager.exe
echo.
pause
