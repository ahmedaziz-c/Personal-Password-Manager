# Step 1: Install Compiler
- Download MinGW-w64 from winlibs.com
- Extract to C:\mingw64
- Add C:\mingw64\bin to your System PATH

# Step 2: Create Files
- Create the 7 files listed above in a new folder.

# Step 3: Build & Run (cmd)
- Method 1: Using build script:
- build.bat

Method 2: Manual compilation:
- g++ -std=c++17 -c main.cpp -o main.o
- g++ -std=c++17 -c PasswordManager.cpp -o PasswordManager.o  
- g++ -std=c++17 -c PasswordManagerTUI.cpp -o PasswordManagerTUI.o
- g++ main.o PasswordManager.o PasswordManagerTUI.o -o password_manager.exe -luser32

# Run
- password_manager.exe
