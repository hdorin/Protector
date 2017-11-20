#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED
HANDLE  hConsole;

//Interface.h
int Width = 80;
int Height = 20;
ifstream file;
int pos;
char c;

//authentication.h
char ProtectedKeyPath[MAX_PATH];
char *Password;
char *ProtectedKeyString;
int PasswordLen;
char *KeyString,*TestKey;
char *PasswordHint;

//SwapEncryption.h
int add[35];
int add_next[35];
char *Block;
int i,j;

//Drag&Drop
struct {
    int Number;
    char Name[50][MAX_PATH];
}Paths;

//files.h
fstream fIn;
fstream fOut;
streampos size,TestOut,TestIn;
char EndTest[35]="Test criptare terminata cu succ!";
    struct tm* Time;//PENTRU IsModified
    struct stat attrib;
    int Sec,Min,Hr,Day;

#endif // GLOBALS_H_INCLUDED
