#ifndef DRAGDROP_H_INCLUDED
#define DRAGDROP_H_INCLUDED

/*DragFiles - completeaza matricea Paths
    Paths.Number -> Numar fisiere
    Paths.Name[i] -> Calea unui fisier
*/
/*
    -Arunci fisiere in fereastra, acestea vor fi afisate
    -Doar cele distincte vor fi memorate
    -Dupa ce apesi Enter vor fi afisate numai cele memorate
    -Pentru folosire doar se apeleaza "DragFiles();"
*/
/*
Paths.Number==10 -> S-a apasat Backspace
*/

bool SamePath(int line);

void DragFiles(){
    int ssize;
    bool dot=false/*intalneste punctul de la extensie, se apropie de sfarsit calea*/;
    bool space=false/*calea in scriere are spatiu*/;
    bool prevspace=true/*calea prec are spatiu*/;
    char aux,aux1;
    Paths.Number=0;

    for(i=0; ;i++){
        aux=getch();
            if(aux=='.')//ca sa nu ramana primul rand gol
                dot=true;
            if(aux==8){
                Paths.Number=-10;
                return ;
            }else
            if(aux==13){
                Paths.Name[Paths.Number][i]='\0';
                if(prevspace==true||SamePath(Paths.Number)==1)
                    Paths.Number--;
                return ;
            }else
                if(aux==34||space==true){//FACUT PENTRU CAZUL SPATIU TEST DE REPETITIE
                    if(space==false&&aux==34){
                        space=true;
                        if(prevspace==false){
                            if(SamePath(Paths.Number)==0)
                                Paths.Number++;
                            cout<<endl;
                        }
                        i=-1;
                    }else
                        if(space==true&&aux==34){
                            space=false;
                            dot=false;
                            prevspace=true;
                            Paths.Name[Paths.Number][i]='\0';
                                cout<<Paths.Name[Paths.Number];
                                cout<<endl;
                            if(SamePath(Paths.Number)==0)
                                Paths.Number++;
                            i=-1;
                        }else{
                            Paths.Name[Paths.Number][i]=aux;
                            Paths.Name[Paths.Number][i+1]='\0';
                        }
                }else
                    if(aux==':'&&dot==true){
                        cout<<"\b \n"<<aux1<<aux;
                        i=0;
                        dot=false;
                        ssize=strlen(Paths.Name[Paths.Number]);
                        Paths.Name[Paths.Number][ssize-1]='\0';
                        prevspace=false;

                        if(SamePath(Paths.Number)==0)
                            Paths.Number++;
                        Paths.Name[Paths.Number][i++]=aux1;
                        Paths.Name[Paths.Number][i]=aux;
                        Paths.Name[Paths.Number][i+1]='\0';
                    }else{
                        Paths.Name[Paths.Number][i]=aux;
                        Paths.Name[Paths.Number][i+1]='\0';
                        aux1=aux;
                        if(space==false)
                            prevspace=false;
                        cout<<aux;
                    }
    }
}
bool SamePath(int line){
    for(i=0;i<line;i++)
        if(strcmp(Paths.Name[i],Paths.Name[line])==0)
            return 1;
    return 0;
}
#endif // DRAG&DROP_H_INCLUDED
