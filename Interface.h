#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

/*
0 = Black
1 = Blue
2 = Green
3 = Aqua
4 = Red
5 = Purple
6 = Yellow
7 = White
8 = Gray
9 = Light Blue
10 = Light Green
11 = Light Aqua
12 = Light Red
13 = Light Purple
14 = Light Yellow
15 = Bright White
*/

/*
    AdjustWindowSize();
    ResetFont();
    Login(); - Se ocupa de cheie
    FilesManagement(); - Se ocupa de operatiuni
*/

//80x36
void AdjustWindowSize(){
    //DIMENSIUNEA BUFFER-ULUI
    _COORD coord;
    coord.X = MAX_PATH;
    coord.Y = 300;
    _SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    //DIMENSIUNEA FERESTREI
    Rect.Bottom = Height - 1;
    Rect.Right = Width - 1;
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleScreenBufferSize(Handle, coord);
    SetConsoleWindowInfo(Handle, TRUE, &Rect);
}
void gotoxy(int y,int x){
COORD coord={0,0};
coord.X=x;
coord.Y=y;
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
void ResetFont(){
    system("Color 00");
}
void FillLine(int TextUnits){
    int i;
    for(i=1;i<MAX_PATH-TextUnits;i++)
    cout<<" ";
}
void Login(){
    PasswordHint=new char[110];
    Selectare:
    pos=1;
    SetConsoleTextAttribute(hConsole, 7+4*16);
    cout<<"-Protector V1.39.1-"; FillLine(18);
    SetConsoleTextAttribute(hConsole, 15+0*16);
    cout<<"\n Introducere cheie\n Cheie noua";
    do{
        if(pos==1){
            gotoxy(2,0);
            cout<<" ";
            gotoxy(1,0);
            cout<<">\b";
        }else{
            gotoxy(1,0);
            cout<<" ";
            gotoxy(2,0);
            cout<<">\b";
        }
        c=getch();
        if(c==72)
            pos=1;
        else
            if(c==80)
                pos=-1;
    }while(c!=13);

    OptiuneaAleasa:
    system("cls");

    if(pos==1){
        SetConsoleTextAttribute(hConsole, 7+2*16);
        cout<<"-Introducere cheie-",FillLine(18);
        SetConsoleTextAttribute(hConsole, 7+0*16);
        cout<<">Aruncati fisierul '.pkey' in aceasta fereastra!\n";
        cout<<">DOAR ultima cale va fi aleasa.\n";
        cout<<">Calea se poate introduce si de la tastatura!\n\n";
        SetConsoleTextAttribute(hConsole, 15+0*16);
        DragFiles();
        if(Paths.Number==-10){
            system("cls");
            goto Selectare;
        }
        strcpy(ProtectedKeyPath,Paths.Name[Paths.Number]);
        //TEST DACA FISIERUL CU CHEIE ESTE TIPUL CORECT
        if(strstr(ProtectedKeyPath,".pkey")==NULL){
            gotoxy(Paths.Number+5,0);
            SetConsoleTextAttribute(hConsole, 12+15*16);
            cout<<endl<<"Tipul de fisier nu este acceptat!";
            Sleep(1000);
            SetConsoleTextAttribute(hConsole, 0+0*16);
            goto OptiuneaAleasa;
        }
        system("cls");
        SetConsoleTextAttribute(hConsole, 7+4*16);
        cout<<"-Introducere cheie-",FillLine(18);
        SetConsoleTextAttribute(hConsole, 7+0*16);
        cout<<">Aruncati fisierul '.pkey' in aceasta fereastra!\n";
        cout<<">DOAR ultima cale va fi aleasa.\n";
        cout<<">Calea se poate introduce si de la tastatura!\n\n";
        SetConsoleTextAttribute(hConsole, 15+0*16);
        cout<<Paths.Name[Paths.Number];
        IntroducereParola:
        gotoxy(6,0);
        cout<<"Parola:";
        GetPassword();
        if(GetProtectedKey(true)==false)
            goto IntroducereParola;
    }else{
        char Name[FILENAME_MAX],PasswordConfirm[35];
        SetConsoleTextAttribute(hConsole, 7+4*16);
        cout<<"-Cheie noua-", FillLine(11);
        SetConsoleTextAttribute(hConsole, 15+0*16);
        cout<<"\nNume*   :\n";
        cout<<"Parola  :\n";
        cout<<"Confrima:\n";
        cout<<"Indiciu :";
        gotoxy(1,9);  cin.getline(Name,FILENAME_MAX);
        {
            char AuxName[FILENAME_MAX];
            strcpy(AuxName,Name);
            strcat(AuxName,".pkey");
            file.open(AuxName);
            if(file.is_open()==true){
                gotoxy(5,0);
                SetConsoleTextAttribute(hConsole, 12+15*16);
                cout<<endl<<"Fisierul deja exista!";
                Sleep(1000);
                SetConsoleTextAttribute(hConsole, 0+0*16);
                file.close();
                goto OptiuneaAleasa;
            }
        }
        gotoxy(2,9);GetPassword();
        strcpy(PasswordConfirm,Password);
        gotoxy(3,9);GetPassword();
        if(strcmp(Password,PasswordConfirm)!=0){
            gotoxy(5,0);
            SetConsoleTextAttribute(hConsole, 12+15*16);
            cout<<endl<<"Parolele nu se aseamana!";
            Sleep(1000);
            SetConsoleTextAttribute(hConsole, 0+0*16);
            goto OptiuneaAleasa;
        }
        gotoxy(4,9);
        cin.getline(PasswordHint,100);
        {
            GenerateProtectedKey(Name);
            SetConsoleTextAttribute(hConsole, 10+15*16);
            cout<<"Cheie creata cu succes!";
            Sleep(1000);
            SetConsoleTextAttribute(hConsole, 0+0*16);
            system("cls");
        }
        goto Selectare;
    }
    delete[] PasswordHint;
    PasswordHint=NULL;
}
void FilesManagement_Encryption();
void FilesManagement_Access();
void FilesManagement_Decryption();
void FilesManagement_Descriptions(int pos);

void FilesManagement(){

    pos=0;
    Meniu:///GOTO MENIU
    system("cls");
    SetConsoleTextAttribute(hConsole, 7+4*16);
    cout<<"-Operatiuni fisiere-",FillLine(19);
    SetConsoleTextAttribute(hConsole, 15+0*16);
    cout<<"\n Criptare\n";
    cout<<" Accesare\n";
    cout<<" Decriptare\n";
    gotoxy(pos+1,0);
    cout<<">\b";
    do{
        FilesManagement_Descriptions(pos);
        c=getch();
        if(c==8){
            system("cls");
            return ;
        }
        if(c==80&&pos<2){
            gotoxy(pos+1,0);
            cout<<" ";
            pos++;
            gotoxy(pos+1,0);
            cout<<">\b";
        }else
            if(c==72&&pos>0){
                gotoxy(pos+1,0);
                cout<<" ";
                pos--;
                gotoxy(pos+1,0);
                cout<<">\b";
            }
    }while(c!=13);
    //Meniu Criptare
    system("cls");
    if(pos==0){
        FilesManagement_Encryption();
        goto Meniu;
    }else
        if(pos==1){
            FilesManagement_Access();
            goto Meniu;
        }else
            if(pos==2){
                FilesManagement_Decryption();
                goto Meniu;
            }
}
void FilesManagement_Encryption(){
    int Number=0;
    int i;///NU STERGE
    float t;
    char EncryptionDirectory[MAX_PATH],EncryptionFilePath[MAX_PATH];
    AruncaCalea:
    SetConsoleTextAttribute(hConsole, 7+6*16);
    cout<<"-Criptare fisiere-",FillLine(17);
    SetConsoleTextAttribute(hConsole, 7+0*16);
    cout<<">Aruncati directorul tinta in aceasta fereastra!\n";
    cout<<">DOAR ultima cale va fi aleasa.\n";
    cout<<">Calea se poate introduce si de la tastatura!\n";
    cout<<">Nu introduceti NIMIC pentru a proteja fisierele sursa.\n\n";
    SetConsoleTextAttribute(hConsole, 15+0*16);
    DragFiles();
    if(Paths.Number==-10)
        return ;
    strcpy(EncryptionDirectory,Paths.Name[Paths.Number]);
    if(Paths.Number>=0){
        file.open(EncryptionDirectory);
        if(file.is_open()==true||strchr(EncryptionDirectory,':')==NULL){
            gotoxy(Paths.Number+6,0);
            SetConsoleTextAttribute(hConsole, 12+15*16);
            cout<<endl<<"Calea nu indica un director!";
            Sleep(1000);
            SetConsoleTextAttribute(hConsole, 0+0*16);
            system("cls");
            file.close();
            goto AruncaCalea;
        }
    }else
        strcpy(EncryptionDirectory," ");
    system("cls");
    SetConsoleTextAttribute(hConsole, 7+2*16);
    cout<<"-Criptare fisiere-",FillLine(17);
    SetConsoleTextAttribute(hConsole, 7+0*16);
    cout<<">Aruncati fisirele in aceasta fereastra!\n";
    cout<<">Dublicatele vor fi eliminate dupa confirmare.\n";
    cout<<">Calea se poate introduce si de la tastatura!\n\n";
    SetConsoleTextAttribute(hConsole, 15+0*16);
    DragFiles();
    if(Paths.Number==-10)
        return ;
    system("cls");

    SetConsoleTextAttribute(hConsole, 7+4*16);
    cout<<"-Criptare fisiere-",FillLine(17);
    cout<<endl;
    SetConsoleTextAttribute(hConsole, 15+0*16);

    for(i=0,t=clock();i<=Paths.Number;i++){
        cout<<i+1<<"/"<<Paths.Number+1<<endl;

        GetProtectedKey(false);
        Protect(NULL,NULL,KeyString);
        if(InitFileRead(Paths.Name[i],false)==false){
            Number++;
            cout<<endl;
            continue;
        }
        if(strstr(Paths.Name[i],".prot")!=NULL){
            SetConsoleTextAttribute(hConsole, 12+0*16);
            cout<<"Tipul fisierului nu este acceptat:\n";
            cout<<Paths.Name[i]<<endl;
            SetConsoleTextAttribute(hConsole, 15+0*16);
            Number++;
            cout<<endl;
            file.close();
            continue;
        }
        if(strcmp(EncryptionDirectory," ")!=0){
            char *Vertical,AuxPath[MAX_PATH],EncrytionFileName[FILENAME_MAX];
            strcpy(AuxPath,Paths.Name[i]);
            strcat(AuxPath,".prot");

            Vertical=strrchr(AuxPath,92);
            strcpy(EncrytionFileName,Vertical);
            strcpy(EncryptionFilePath,EncryptionDirectory);
            strcat(EncryptionFilePath,EncrytionFileName);
        }else{
            strcpy(EncryptionFilePath,Paths.Name[i]);
            strcat(EncryptionFilePath,".prot");
        }
        file.open(EncryptionFilePath);
        if(file.is_open()==true){
            SetConsoleTextAttribute(hConsole, 12+0*16);
            cout<<"Fisierul deja exista:\n";
            cout<<EncryptionFilePath<<endl;
            SetConsoleTextAttribute(hConsole, 15+0*16);
            Number++;
            cout<<endl;
            file.close();
            continue;
        }
        cout<<EncryptionFilePath<<endl;
        if(strcmp(EncryptionDirectory," ")==0){//CREARE FISIER TEMPORAR PROTEJAT
            char TempName[MAX_PATH],TempPath[MAX_PATH];
            int CouldRename;
            GetTempPath(MAX_PATH,TempPath);
            tmpnam(TempName);
            strcat(TempPath,TempName+1);
            TempPath[strlen(TempPath)-1]='\0';
            InitFileRead(Paths.Name[i],false);
            InitFileWrite(TempPath,false);
            thread t (Progress,size);///PROGRESS

            for(;size>=0;size-=32){
                if(size>32)
                    FileRead(32);
                else
                    FileRead(size);
            if(size>32)
                    FileWrite(32);
                else
                    FileWrite(size);
            }
            t.join();
            CouldRename=rename(Paths.Name[i],EncryptionFilePath);
            if(CouldRename!=0){
                SetConsoleTextAttribute(hConsole, 12+0*16);
                cout<<"Fisierul nu a putut fi modificat:\n";
                cout<<Paths.Name[i]<<endl;
                SetConsoleTextAttribute(hConsole, 15+0*16);
                Number++;
                cout<<endl;
                continue;
            }
            strcpy(Paths.Name[i],TempPath);
        }
        InitFileRead(Paths.Name[i],false);
        InitFileWrite(EncryptionFilePath,true);
        thread p (Progress,(size/32 + 1)*32+64);
        for(;size>=0;size-=32){
            if(size>32)
                FileRead(32);
            else{
                FileRead(size);
                for(j=size;j<32;j++)//Pentru fisiere mici ca sa nu dea eroare la decriptare
                    Block[j]=32;
            }
            Protect(Block,true,NULL);
            FileWrite(32);
        }
        p.join();
        cout<<endl;
    }
    SetConsoleTextAttribute(hConsole, 10+0*16);
    cout<<"\n---------------------------------\n";
    cout<<"Criptarea s-a terminat in "<<(clock()-t)/CLK_TCK<<" secunde!\n";
    cout<<"Fisiere criptate cu succes: "<<Paths.Number-Number+1<<"/"<<Paths.Number+1;
    cout<<endl<<"Continuati?...";
    getch();
    SetConsoleTextAttribute(hConsole, 0+0*16);
}
void FilesManagement_Access(){
    int Number=0;
    int i;///NU STERGE
    SetConsoleTextAttribute(hConsole, 7+2*16);
    cout<<"-Accesare fisiere-",FillLine(17);
    SetConsoleTextAttribute(hConsole, 7+0*16);
    cout<<">Aruncati fisirele in aceasta fereastra!\n";
    cout<<">Dublicatele vor fi eliminate dupa confirmare.\n";
    cout<<">Calea se poate introduce si de la tastatura!\n\n";
    SetConsoleTextAttribute(hConsole, 15+0*16);
    DragFiles();
    if(Paths.Number==-10)
        return ;
    system("cls");

    SetConsoleTextAttribute(hConsole, 7+4*16);
    cout<<"-Accesare fisiere-",FillLine(17);
    cout<<endl;
    SetConsoleTextAttribute(hConsole, 15+0*16);

    for(i=0;i<=Paths.Number;i++){
        cout<<i+1<<"/"<<Paths.Number+1<<endl;
        if(strstr(Paths.Name[i],".prot")==NULL){
            SetConsoleTextAttribute(hConsole, 12+0*16);
            cout<<"Tipul fisierului nu este acceptat:\n";
            cout<<Paths.Name[i]<<endl;
            SetConsoleTextAttribute(hConsole, 15+0*16);
            Number++;
            cout<<endl;
            continue;
        }else
            if(InitFileRead(Paths.Name[i],true)==false){
                Number++;
                cout<<endl;
                continue;
            }
        cout<<Paths.Name[i]<<endl;
        if(FileAccess(Paths.Name[i])==false)
            Number++;
        cout<<endl;
    }
    SetConsoleTextAttribute(hConsole, 10+0*16);
    cout<<"\n---------------------------------\n";
    cout<<"Accesarea s-a terminat cu succes!\n";
    cout<<"Fisiere accesate cu succes: "<<Paths.Number-Number+1<<"/"<<Paths.Number+1;
    cout<<"\nContinuati?...";
    getch();
    SetConsoleTextAttribute(hConsole, 0+0*16);
}
void FilesManagement_Decryption(){
    int Number=0;
    float t;
    int i;///NU STERGE
    char DecryptionDirectory[MAX_PATH],DecryptionFilePath[MAX_PATH];

    AruncaCalea:
    //Preiau calea de decriptare
    SetConsoleTextAttribute(hConsole, 7+6*16);
    cout<<"-Decriptare fisiere-",FillLine(19);
    SetConsoleTextAttribute(hConsole, 7+0*16);
    cout<<">Aruncati directorul tinta in aceasta fereastra!\n";
    cout<<">DOAR ultima cale va fi aleasa.\n";
    cout<<">Calea se poate introduce si de la tastatura!\n";
    cout<<">Nu introduceti NIMIC pentru a elimina protectia fisierelor.\n\n";
    SetConsoleTextAttribute(hConsole, 15+0*16);
    DragFiles();
    if(Paths.Number==-10)
        return ;
    strcpy(DecryptionDirectory,Paths.Name[Paths.Number]);
    if(Paths.Number>=0){
            file.open(DecryptionDirectory);
            if(file.is_open()==true||strchr(DecryptionDirectory,':')==NULL){
                gotoxy(Paths.Number+6,0);
                SetConsoleTextAttribute(hConsole, 12+15*16);
                cout<<endl<<"Calea nu indica un director!";
                Sleep(1000);
                SetConsoleTextAttribute(hConsole, 0+0*16);
                system("cls");
                file.close();
                goto AruncaCalea;
            }
    }else
        strcpy(DecryptionDirectory," ");
    system("cls");

    SetConsoleTextAttribute(hConsole, 7+2*16);
    cout<<"-Decriptare fisiere-",FillLine(19);
    SetConsoleTextAttribute(hConsole, 7+0*16);
    cout<<">Aruncati fisirele in aceasta fereastra!\n";
    cout<<">Dublicatele vor fi eliminate dupa confirmare.\n";
    cout<<">Calea se poate introduce si de la tastatura!\n\n";
    SetConsoleTextAttribute(hConsole, 15+0*16);

    DragFiles();
    if(Paths.Number==-10)
        return ;
    system("cls");

    SetConsoleTextAttribute(hConsole, 7+4*16);
    cout<<"-Decriptare fisiere-",FillLine(19);
    cout<<endl;
    SetConsoleTextAttribute(hConsole, 15+0*16);

    for(i=0,t=clock();i<=Paths.Number;i++){
        cout<<i+1<<"/"<<Paths.Number+1<<endl;

        if(strstr(Paths.Name[i],".prot")==NULL){
            SetConsoleTextAttribute(hConsole, 12+0*16);
            cout<<"Tipul fisierului nu este acceptat:\n";
            cout<<Paths.Name[i]<<endl;
            SetConsoleTextAttribute(hConsole, 15+0*16);
            Number++;
            cout<<endl;
            continue;
        }
        if(strcmp(DecryptionDirectory," ")!=0){
            char *Vertical,*Dot,AuxPath[MAX_PATH],DecrytionFileName[FILENAME_MAX];
            strcpy(AuxPath,Paths.Name[i]);
            Dot=strstr(AuxPath,".prot");
            *Dot='\0';
            Vertical=strrchr(AuxPath,92);
            strcpy(DecrytionFileName,Vertical);
            strcpy(DecryptionFilePath,DecryptionDirectory);
            strcat(DecryptionFilePath,DecrytionFileName);
        }else{
            char *Dot;
            strcpy(DecryptionFilePath,Paths.Name[i]);
            Dot=strstr(DecryptionFilePath,".prot");
            *Dot='\0';
        }
        file.open(DecryptionFilePath);
        if(file.is_open()==true){
            SetConsoleTextAttribute(hConsole, 12+0*16);
            cout<<"Fisierul deja exista:\n";
            cout<<DecryptionFilePath<<endl;
            SetConsoleTextAttribute(hConsole, 15+0*16);
            Number++;
            cout<<endl;
            file.close();
            continue;
        }else
            if(InitFileRead(Paths.Name[i],true)==false){
                Number++;
                cout<<endl;
                continue;
            }
        cout<<DecryptionFilePath<<endl;
        //In caz ca se decripteaza fisierul sursa, se creaza mai intai copie temp a fisierului protejat,
        //aceasta urmand sa fie decriptata si sa inlocuiasca fisierul sursa
        if(strcmp(DecryptionDirectory," ")==0){
            char TempName[MAX_PATH],TempPath[MAX_PATH];
            int CouldRename;
            GetTempPath(MAX_PATH,TempPath);
            tmpnam(TempName);
            strcat(TempPath,TempName+1);
            TempPath[strlen(TempPath)-1]='\0';
            InitFileRead(Paths.Name[i],false);
            InitFileWrite(TempPath,false);
            thread t (Progress,size);///PROGRESS

            for(;size>=0;size-=32){
                if(size>32)
                    FileRead(32);
                else
                    FileRead(size);
            if(size>32)
                    FileWrite(32);
                else
                    FileWrite(size);
            }
            t.join();
            CouldRename=rename(Paths.Name[i],DecryptionFilePath);
            if(CouldRename!=0){
                SetConsoleTextAttribute(hConsole, 12+0*16);
                cout<<"Fisierul nu a putut fi modificat:\n";
                cout<<Paths.Name[i]<<endl;
                SetConsoleTextAttribute(hConsole, 15+0*16);
                Number++;
                cout<<endl;
                continue;
            }
            strcpy(Paths.Name[i],TempPath);
        }
        //REALIZEZ DECRIPTAREA
        InitFileRead(Paths.Name[i],true);
        InitFileWrite(DecryptionFilePath,false);
        thread t (Progress,size);
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
        //COPIEZ LA LOC FISIERUL TEMP PROTEJAT (DECRIPTARE ESUATA)
        if(TestDecryption()==false){
            char AuxPath[MAX_PATH];
            Number++;
            InitFileRead(Paths.Name[i],false);
            InitFileWrite(DecryptionFilePath,false);
            thread tt (Progress,size);///PROGRESS
            for(;size>=0;size-=32){
                if(size>32)
                    FileRead(32);
                else
                    FileRead(size);
            if(size>32)
                    FileWrite(32);
                else
                    FileWrite(size);
            }
            tt.join();
            strcpy(AuxPath,DecryptionFilePath);
            strcat(AuxPath,".prot");
            rename(DecryptionFilePath,AuxPath);
        }
        cout<<endl;
    }
    SetConsoleTextAttribute(hConsole, 10+0*16);
    cout<<"\n---------------------------------\n";
    cout<<"Decriptarea s-a terminat in "<<(clock()-t)/CLK_TCK<<" secunde!\n";
    cout<<"Fisiere decriptate cu succes: "<<Paths.Number-Number+1<<"/"<<Paths.Number+1;
    cout<<endl<<"Continuati?...";
    getch();
    SetConsoleTextAttribute(hConsole, 0+0*16);
}
void FilesManagement_Descriptions(int pos){
    int l=2,c=30;
    switch(pos){
        case 1:
            gotoxy(l,c);
            cout<<"-Creaza o copie protejata a fisierului      ";
            gotoxy(l+1,c);
            cout<<" sursa la calea definita;                   ";
            gotoxy(l+2,c);
            cout<<"-Protejeaza direct informatia din           ";
            gotoxy(l+3,c);
            cout<<" fisierul sursa;                            ";
        break;
        case 2:
            gotoxy(l,c);
            cout<<"-Acceseaza fisierul sursa protejat fara     ";
            gotoxy(l+1,c);
            cout<<" a-i elimina protectia;                     ";
            gotoxy(l+2,c);
            cout<<"-Editarea fisierului sursa este posibila;  ";
            gotoxy(l+3,c);
            cout<<"                                            ";
        break;
        case 3:
            gotoxy(l,c);
            cout<<"-Creaza o copie fara protectie a fisierului ";
            gotoxy(l+1,c);
            cout<<" sursa la calea definita;                   ";
            gotoxy(l+2,c);
            cout<<"-Elinima direct protectia fisierului sursa; ";
            gotoxy(l+3,c);
            cout<<"                                            ";
        break;
    }
    gotoxy(pos+1,0);
}
#endif // INTERFACE_H_INCLUDED
