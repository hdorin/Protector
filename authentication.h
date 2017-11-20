#ifndef AUTHENTICATION_H_INCLUDED
#define AUTHENTICATION_H_INCLUDED
/*
GetPassword - citeste parola de pe ecran
GenerateProtectedKey - creaza o cheie protejara de parola
      (pt criptare/decriptare se folosesti fisierul de cheie)
      (fisierul cheie se genereaza/incarca la inceput)
GetProtectedKey - Dupa ce se citeste parola se cauta cheia in fisierul de cheie
       (cheia se gaseste in KeyString)
Dupa fiecare operatie cheia devine 0
*/

///strcpy(ProtectedKeyPath,Paths.Name[0]);
//ADRESA DE UNDE SE IA CHEIEA (cu GetProtectedKey) DE FIECARE DATA!!!


inline void Protect(char *Block,bool EncryptBlock,char *KeyString);
void gotoxy(int y,int x);

void GenerateNewKey(){
    srand (time(NULL));
            for(i=0;i<32;i++)
                do
                    KeyString[i]=rand()%224+32;
                while(i&&KeyString[i]==KeyString[i-1]);
            KeyString[32]='\0';
}
void GetPassword(){
    PasswordLen=0;
    for(i=1;i<=32;i++)
        cout<<"_";
    for(i=1;i<=32;i++)
        cout<<'\b';
    while(true){
        c=getch();
        if(c>=33&&c<=126){
            if(PasswordLen<32){
                Password[PasswordLen++]=c;
                cout<<'*';
            }else
                Beep(500,100);
        }else
            if(c==8&&PasswordLen>0){
                cout<<'\b'<<'_'<<'\b';
                PasswordLen--;
            }else
                if(c==13){
                    Password[PasswordLen]='\0';
                    return ;
                }else
                    Beep(500,100);
    }
}
//PARAMETRUL TREBUIE SA FIE VARIABILA!!!
void GenerateProtectedKey(char FileName[FILENAME_MAX]){
    int pos;
    char aux,AuxFileName[FILENAME_MAX];
    TestKey=new char[40];
    KeyString=new char[40];
    ProtectedKeyString=new char[260];
    strcpy(AuxFileName,FileName);

    GenerateNewKey();
    srand(time(NULL));

    for(i=0;i<256;i++)
        ProtectedKeyString[i]=rand()%224+32;
    for(i=0;i<256;i++)
        ProtectedKeyString[i]=rand()%224+32;
    ProtectedKeyString[256]='\0';

    for(i=0,j=PasswordLen-1;i<32;i++,j--){
        if(j==-1)
            j=PasswordLen-1;
        aux=Password[j];
        pos=0;
        while(aux){
            if(aux%2==0)
                pos++;
            aux/=2;
        }
        ProtectedKeyString[i*8+pos]=KeyString[i]+Password[j]+PasswordLen;
    }
    strcat(AuxFileName,".pkey");//pkey = Protected Key
    fOut.open(AuxFileName,ios::out|ios::binary|ios::ate);
    fOut.write(ProtectedKeyString,256);
    //Adaug cheia (protejata de parola) la sfarsit pentru verificare
    TestKey=new char[40];
    strncpy(TestKey,KeyString,32);
    Protect(NULL,true,KeyString);
    Protect(TestKey,true,NULL);
    fOut.write(TestKey,32);
    fOut.write(PasswordHint,strlen(PasswordHint));
    fOut.close();

    delete[] ProtectedKeyString;
    delete[] KeyString;
    delete[] TestKey;
    KeyString=ProtectedKeyString=TestKey=NULL;
}
void GetProtectedKeyThread(){
    int pos;
    char aux;
    for(i=0,j=PasswordLen-1;i<32;i++,j--){
        if(j==-1)
            j=PasswordLen-1;
        aux=Password[j];
        pos=0;
        while(aux){
            if(aux%2==0)
                pos++;
            aux/=2;
        }
        KeyString[i]=ProtectedKeyString[i*8+pos]-Password[j]-PasswordLen;
    }
    KeyString[32]='\0';
    Sleep(5);
    delete[] ProtectedKeyString;
    delete[] KeyString;
    KeyString=ProtectedKeyString=NULL;
}
bool GetProtectedKey(bool TestPassword){
    KeyString=new char[40];
    ProtectedKeyString=new char[260];
    bool RetValue=true;

    ifstream f(ProtectedKeyPath,ios::binary|ios::ate);
    if(f.is_open()==false){
        SetConsoleTextAttribute(hConsole, 12+0*16);
        cout<<"Cheia nu a mai putut fi gasita!\n";
        cout<<"Process terminated.\n";
        while(true){
            getch();
            Sleep(1);
        }
    }
    SetConsoleTextAttribute(hConsole, 15+0*16);
    f.seekg (0, ios::beg);
    f.read(ProtectedKeyString,256);

    thread (GetProtectedKeyThread).detach();
    Sleep(2);
    //TESTEZ cheia gasita da parola data CU cheia de la sfarsit deprotejata de parola
    if(TestPassword==true){
        TestKey=new char[40];
        PasswordHint=new char[110];
        f.read(TestKey,32);
        Protect(NULL,false,KeyString);
        Protect(TestKey,false,NULL);
        if(strncmp(TestKey,KeyString,32)!=0){
            f.get(PasswordHint,100);
            gotoxy(7,0);
            SetConsoleTextAttribute(hConsole, 12+15*16);
            cout<<"Parola este incorecta!";
            cout<<"\nIndiciu:"<<PasswordHint;
            SetConsoleTextAttribute(hConsole, 15+0*16);
            Sleep(1000);
            gotoxy(7,0);
            cout<<"                      \n";
            cout<<"                                                                                ";
            RetValue=false;
        }
        delete[] TestKey;
        delete[] PasswordHint;
        TestKey=PasswordHint=NULL;
    }
    f.close();
    return RetValue;
}
#endif // AUTHENTICATION_H_INCLUDED
