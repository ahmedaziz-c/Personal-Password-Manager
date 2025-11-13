@echo off
echo Building Password Manager for Windows...
g++ -std=c++17 -Wall -Wextra -O2 -c main.cpp -o main.o
g++ -std=c++17 -Wall -Wextra -O2 -c PasswordManager.cpp -o PasswordManager.o
g++ -std=c++17 -Wall -Wextra -O2 -c PasswordManagerTUI.cpp -o PasswordManagerTUI.o
g++ main.o PasswordManager.o PasswordManagerTUI.o -o password_manager.exe -luser32
echo Build complete! Run password_manager.exe
pause
