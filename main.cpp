#include<iostream>
#include<fstream>
#include<string.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include<thread>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;

#include"globals.h"
#include"Drag&Drop.h"
#include"authentication.h"
#include"SwapEncryption.h"
#include"files.h"
#include"Interface.h"

int main(){
    while(true){
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTitle("Protector");
        AdjustWindowSize();
        ResetFont();
            Password=new char[40];
            Block = new char [35];
        Login();
        FilesManagement();
            delete[] Password;
            delete[] Block;
            Password=Block=NULL;
    }
return 0;}
