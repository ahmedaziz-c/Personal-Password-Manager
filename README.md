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
- g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -c PasswordManager.cpp -o PasswordManager.o
- g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -c SimplePasswordManager.cpp -o SimplePasswordManager.o
- g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -c main.cpp -o main.o

# Run
- password_manager.exe
- https://github.com/ahmedaziz-c/Personal-Password-Manager/releases
