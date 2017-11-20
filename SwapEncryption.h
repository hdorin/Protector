#ifndef SWAPENCRYPTION_H_INCLUDED
#define SWAPENCRYPTION_H_INCLUDED
/*
1.Generare cheie noua - GenerateNewKey();
2.Initializare functia Protect - Protect(NULL,NULL,<cheie>)
3.Criptare block: Protect(<string>,true,NULL)
  Decriptare block: Protect(<string>,false,NULL)
*/

//convert
unsigned char ch1;
int sum;
//sort
bool g;
int len;
int SwapAux;
int add_s[32],id[32],aux;
char ch_s[32];
int convert(unsigned char ch){
    sum=0;
    while(ch>1){
        if(ch>100)   sum+=1;
        if(ch%10==0) sum+=5;
        if(ch%7==0)  sum+=2;
        if(ch%5==0)  sum+=5;
        if(ch%3==0)  sum+=7;
        if(ch%2==0)  sum+=3;

        ch1=ch;
        ch=0;
        while(ch1>10){
            ch+=ch1%10;
            ch1/=10;
        }
    }
    return sum;
}
inline void sort_encrypt(char *Block){
    len=16;
    do{
        g=false;
        for(i=0;i<32-len;i++)
            if(add[i]<add[i+len]){
                SwapAux=add[i];
                add[i]=add[i+len];
                add[i+len]=SwapAux;
                g=true;
                SwapAux=Block[i];
                Block[i]=Block[i+len];
                Block[i+len]=SwapAux;
            }
        if(g==false)
            len/=2;
    }while(len>0);
}
inline void sort_restore(char *Block){
    len=16;
    for(i=0;i<32;i++){
        id[i]=i;
        ch_s[i]=Block[i];
        add_s[i]=add[i];
    }
    do{
        g=false;
        for(i=0;i<32-len;i++)
            if(add_s[i]<add_s[i+len]){
                SwapAux=id[i];
                id[i]=id[i+len];
                id[i+len]=SwapAux;
                g=true;
                SwapAux=add_s[i];
                add_s[i]=add_s[i+len];
                add_s[i+len]=SwapAux;
            }
        if(g==false)
            len/=2;
    }while(len>0);
    for(i=0;i<32;i++)
        Block[id[i]]=ch_s[i];
}
inline void sort_add(){
    len=16;
    do{
        g=false;
        for(i=0;i<32-len;i++)
            if(add[i]<add[i+len]){
                SwapAux=add[i];
                add[i]=add[i+len];
                add[i+len]=SwapAux;
                g=true;
            }
        if(g==false)
            len/=2;
    }while(len>0);
}
inline void block_encrypt(char *Block){
    for(i=0;i<32;i++)
        add_next[i]=convert(Block[i]);
    for(i=0;i<32;i++)
        Block[i]+=add[i];

    sort_encrypt(Block);

    for(i=0;i<32;i++){
        add[i]+=add_next[i];
        if(add[i]>100)
            add[i]/=10;
    }
}
inline void block_decrypt(char *Block){
    sort_restore(Block);
    for(i=0;i<32;i++)
        Block[i]-=add[i];

    sort_add();
    for(i=0;i<32;i++){
        add[i]+=convert(Block[i]);
        if(add[i]>100)
            add[i]/=10;
    }
}
inline void Protect(char *Block,bool EncryptBlock,char *KeyString){
    if(KeyString!=NULL){
        for(i=0;i<32;i++)
            add[i]=convert(KeyString[i]);
        return ;
    }
    if(EncryptBlock==true)
        block_encrypt(Block);
    else
        block_decrypt(Block);
}
#endif // SWAPENCRYPTION_H_INCLUDED
