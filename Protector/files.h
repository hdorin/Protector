#ifndef FILES_H_INCLUDED
#define FILES_H_INCLUDED
//Block - retine informatia din fisier (numai 32 de caractere)
///CRIPTARE
/*
GetProtectedKey();
Protect(NULL,NULL,KeyString);
InitFileRead("",false);
InitFileWrite("",true);
for(;size>=0;size-=32){
    if(size>32)
        FileRead(32);
    else
        FileRead(size);
    Protect(Block,true,NULL);
    FileWrite(32);///IN CAZ CA SCRIE NECRIPTAT -> 32 este size sau 32 daca size>32
}
*/
///DECRIPTARE
/*
InitFileRead("",true);
InitFileWrite("",false);
GetProtectedKey();
Protect(NULL,NULL,KeyString);
for(;size>=0;size-=32){
     FileRead(32);
     Protect(Block,false,NULL);
    if(size>32)
        FileWrite(32);
    else{
        FileWrite(size);
    }
}
*/
///AFISARE PROGRES
//thread (Progress,size).detach();


void Progress(streampos size_aux){
    streampos size_prev_aux=size;
    int step=250;
    int convert=1048576;
    char unit[5]="MB";
    if((float)size_aux/convert<0.1&&convert==1048576){
        convert/=1024;
        strcpy(unit,"KB");
    }
    while(size>0){
        if(step<247)
            step++;
        else{
            cout.precision(3);
            cout<<(float)size/convert<<"/"<<(float)size_aux/convert<<" "<<unit<<"   ";
            cout.precision(3);
            cout<<(float)((size_prev_aux-size)*3.95)/convert<<" "<<unit<<"/s                   ";
            cout<<'\r';
            step=0;
            size_prev_aux=size;
        }
        Sleep(1);
    }
        cout.precision(3);
        cout<<"0/"<<(float)size_aux/convert<<" "<<unit<<"                                                           ";
        cout<<endl;
}
//Encrypted - ce citeste (nu) este criptat
bool InitFileRead(char Path[MAX_PATH],bool Encrypted){
    fIn.close();
    fIn.open(Path,ios::in|ios::binary|ios::ate);
    if(fIn.is_open()==false){
        SetConsoleTextAttribute(hConsole, 12+0*16);
        cout<<"Adresa invalida:\n"<<Path<<endl;
        SetConsoleTextAttribute(hConsole, 15+0*16);
        return false;
    }
    if(Encrypted==false){
        size=fIn.tellg();
        fIn.seekg (0, ios::beg);
        TestIn=0;
    }else{
        TestIn=32;
        fIn.seekg (0, ios::beg);
        int id;
        size=0;
        for(id=0;id<32;id++)
            Block[id]=0;
        fIn.read(Block,32);
        for(id=0;id<32;id++)
            size=size*100+Block[id]%100;
        fIn.seekg (0, ios::beg);
        fIn.read(Block,32);
    }
    return true;
}
bool IsModified(char Path[MAX_PATH],bool CompareFiles){
    if(CompareFiles==false){
        stat(Path, &attrib);     // get the attributes of afile.txt
        Time = localtime(&(attrib.st_mtime));
        Sec=Time->tm_sec;
        Min=Time->tm_min;
        Hr=Time->tm_hour;
        Day=Time->tm_mday;
    }else{
        stat(Path, &attrib);
        Time = localtime(&(attrib.st_mtime));
        if(Time->tm_sec!=Sec)
            return true;
        if(Time->tm_min!=Min)
            return true;
        if(Time->tm_hour!=Hr)
            return true;
        if(Time->tm_mday!=Day)
            return true;
    }
    return false;
}
bool TestDecryption(){
    if(TestIn>0){
        fIn.read(Block,TestIn);
        fIn.close();
        Protect(Block,false,NULL);
        if(strncmp(Block,EndTest,32)!=0){
            SetConsoleTextAttribute(hConsole, 12+0*16);
            cout<<"Decriptarea a esuat, cheie incorecta!\n";
            SetConsoleTextAttribute(hConsole, 15+0*16);
            return false;
        }
        TestIn=0;
    }
    return true;
}
inline void FileRead(streampos BlockLen){
        fIn.read(Block,BlockLen);
        if(size<=32&&TestIn==0)
            fIn.close();
}
//Encrypted - ce scrie (nu) este criptat
void InitFileWrite(char Path[MAX_PATH],bool Encrypted){
        fOut.close();
        fOut.open(Path,ios::out|ios::binary|ios::ate);//CREEZ FISIERUL FINAL DIN FISIERUL TEMP MODIFICAT
        if(Encrypted==true){
            int id;
            streampos aux=size;
            for(id=32-1;id>=0;id--){
                Block[id]=aux%100;
                aux=aux/100;
            }
            fOut.write(Block,32);
            TestOut=32;
        }else
            TestOut=0;
}
inline void FileWrite(streampos BlockLen){
        fOut.write(Block,BlockLen);
        if(size<32){
            if(TestOut>0){
                strcpy(Block,EndTest);
                Protect(Block,true,NULL);
                fOut.write(Block,TestOut);
                TestOut=0;
            }
            fOut.close();
        }
}
//Action:"open","edit"
int FileAccess(char Path[MAX_PATH]){
    int i;
    char TempName[MAX_PATH],TempPath[MAX_PATH],Extension[FILENAME_MAX],*Dot;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    strcpy(TempPath,Path);
    Dot=strrchr(TempPath,'.');
    *Dot='\0';
    Dot=strrchr(TempPath,'.');
    strcpy(Extension,Dot);
    GetTempPath(MAX_PATH,TempPath);
    tmpnam(TempName);
    Dot=strrchr(TempName,'.');
    strcpy(Dot,Extension);
    strcat(TempPath,TempName+1);

    InitFileRead(Path,true);
    InitFileWrite(TempPath,false);
    thread t (Progress,size);///PROGRESS
    GetProtectedKey(false);
    Protect(NULL,NULL,KeyString);
    for(;size>=0;size-=32){
        FileRead(32);
        Protect(Block,false,NULL);
        if(size>32)
            FileWrite(32);
        else
            FileWrite(size);
    }
    t.join();
    if(TestDecryption()==false)
        return false;
    IsModified(TempPath,false);

    SHELLEXECUTEINFO ShExecInfo = {0};//PORNESC FISIERUL
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = "open";
    ShExecInfo.lpFile = TempPath;
    ShExecInfo.lpParameters = NULL;
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_MAXIMIZE;
    ShExecInfo.hInstApp = NULL;
    ShellExecuteEx(&ShExecInfo);
    WaitForSingleObject(ShExecInfo.hProcess,INFINITE);

    SetConsoleTextAttribute(hConsole, 11+0*16);
    cout<<"Modificarile se pot salva acum! Continuati?...\r";
    SetConsoleTextAttribute(hConsole, 15+0*16);
    getch();

    if(IsModified(TempPath,true)==true){
        cout<<"Se salveaza modificarile...                          \n";
        InitFileRead(TempPath,false);
        InitFileWrite(Path,true);

        thread tt (Progress,(size/32 + 1)*32+64);///PROGRESS

        GetProtectedKey(false);
        Protect(NULL,NULL,KeyString);
        for(;size>=0;size-=32){
            if(size>32)
                FileRead(32);
            else{
                FileRead(size);
                for(i=size;i<32;i++)//Pentru fisiere mici ca sa nu dea eroare la decriptare
                    Block[i]=32;
            }
                Protect(Block,true,NULL);
                FileWrite(32);
            }
        tt.join();
    }else{
        cout<<"Fisierul nu a fost modificat.                        \n";
    }
    remove(TempPath);//Sterge fisierul temp
    return true;
}
#endif // FILES_H_INCLUDED
