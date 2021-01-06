#include <iostream>
#include<cstdlib>
#include<cmath>
#include<fstream>
#include <windows.h>
#include <winable.h>
#include<vector>

#include "gl\glut.h"
#include "SOIL/SOIL.h"
#include "ImageClass.h"



using namespace std;
ImageClass Image, NewImage;
int tomPreto=5, intervaloX=0, intervaloY=0;
ofstream arq;

void criarArquivo(){

  arq.open("coordenadas.txt",ios::in);
  if(!arq){
    arq.open("coordenadas.txt");
  }else{
    arq.close();
    remove("coordenadas.txt");
    arq.open("coordenadas.txt");
  }
}
 void escreverXYjunto(vector<int>& vetx, vector<int>& vety){
     int contador=0;
     bool subCaneta = false;
    //arq<<"XY ";
    for(int i=0;i<vetx.size();i++){
        contador++;
        if(i==vetx.size()-1){
            arq<<"+"<<vetx[i];
            arq<<","<<vety[i]<<",";
        }else{
            arq<<vetx[i]<<",";
            arq<<vety[i]<<",";
        }
        if(contador>=300){arq<<"\n"; contador=0;}
    }

 }
void escreverArquivo(vector<int>& vetx, vector<int>& vety){
  arq<<"X {";
  for(int i=0;i<vetx.size();i++){
    if(i == vetx.size()-1){
       arq<<vetx[i];
    }else{
         arq<<vetx[i]<<", ";
    }
  }
  arq<<"} \n";
  arq<<"Y {";
  for(int j=0;j<vety.size();j++){
    if(j == vety.size()-1){
        arq<<vety[j];
    }else{
        arq<<vety[j]<<", ";
    }
  }
  arq<<"} \n";
}

void fecharArquivo(){
    arq.close();
}

int verificarVet(vector<int>& vetx, vector<int>& vety, int x, int y){
    for(int i=0; i<vetx.size();i++){
        if(vetx[i]==x && vety[i]==y){
           return i;
        }
    }
    return 0;
}

int intervaloVetor(int x, int y){
    int a=0;
    if((intervaloX-x)<-1 || (intervaloX-x)>1 || (intervaloY-y)<-1 || (intervaloY-y)>1){
        a=1;
    }
    intervaloX=x;
    intervaloY=y;
    return a;
}

void adicionarXY(int x, int y, vector<int>& vetx, vector<int>& vety){
    if(intervaloVetor(x,y)==1){
        vetx.push_back(300);
        vety.push_back(300);
        vetx.push_back(x);
        vety.push_back(y);
        cout<<"X "<<300<<" Y "<<300<<endl;
        cout<<"X "<<x<<" Y "<<y<<endl;
    }else{
        vetx.push_back(x);
        vety.push_back(y);
        cout<<"X "<<x<<" Y "<<y<<endl;
    }

}


void verifPixelPreto(int x, int y, vector<int>& vetx, vector<int>& vety, int maxX, int maxY){
      int pixel;
      if(x<maxX && y<maxY && y>0 && x>0){
          pixel = Image.GetPointIntensity(x,y+1);
          if(pixel <= tomPreto){
            if(verificarVet(vetx,vety,x,y+1)==0){
                adicionarXY(x,y+1,vetx,vety);
                verifPixelPreto(x,y+1,vetx,vety,maxX,maxY);
            }
          }else{
            pixel = Image.GetPointIntensity(x+1,y+1);
            if(pixel <= tomPreto){
                if(verificarVet(vetx,vety,x+1,y+1)==0){
                    adicionarXY(x+1,y+1,vetx,vety);
                    verifPixelPreto(x+1,y+1,vetx,vety,maxX,maxY);
                }
            }else{
                pixel = Image.GetPointIntensity(x+1,y);
                if(pixel <= tomPreto){
                    if(verificarVet(vetx,vety,x+1,y)==0){
                        adicionarXY(x+1,y,vetx,vety);
                        verifPixelPreto(x+1,y,vetx,vety,maxX,maxY);
                    }
                }else{
                    pixel = Image.GetPointIntensity(x+1,y-1);
                    if(pixel <= tomPreto){
                        if(verificarVet(vetx,vety,x+1,y-1)==0){
                            adicionarXY(x+1,y-1,vetx,vety);
                        }
                    }
                }
            }
         }
      }
}


void verifPixels(int maxX, int maxY, int* vet, vector<int>& vetx, vector<int>& vety){
   int pixel;

    for(int x=0; x<= maxX; x++){
        for(int y=0; y<=maxY; y++){
            pixel=Image.GetPointIntensity(x,y);
            if(pixel<=tomPreto){//preto
                vet[1]+=1;
                if(x<maxX && y<maxY && y>0 && x>0){
                    if(verificarVet(vetx,vety,x,y)==0){
                        adicionarXY(x,y,vetx,vety);
                        verifPixelPreto(x,y,vetx,vety,maxX,maxY);
                    }
                }
            }else{
                vet[0]+=1;
            }
        }
    }
}


void arrumarCoordenadas(vector<int>& vetx, vector<int>& vety){
    vector <int>x,y;
    x.push_back(321);
    x.push_back(vetx[1]);
    y.push_back(321);
    y.push_back(vety[1]);
    int resp, anteriorx=vetx[1], anteriory = vety[1];

    for(int i=2;i<vetx.size();i++){
        if(vetx[i]!=300){
            resp = anteriorx - vetx[i];
            if(resp>0){
                resp = resp*(-1);
                x.push_back(resp);
            }else if(resp<0){
                resp = resp*(-1);
                x.push_back(resp);
            }else{
               x.push_back(resp);
            }
        anteriorx = vetx[i];
        }else if(vetx[i] == 300){
            x.push_back(321);
        }
    }
    x.push_back(vetx[vetx.size()-1]);

        for(int i=2;i<vety.size();i++){
        if(vety[i]!=300){
            resp = anteriory - vety[i];
            if(resp>0){
                resp = resp*(-1);
                y.push_back(resp);
            }else if(resp<0){
                resp = resp*(-1);
                y.push_back(resp);
            }else{
                y.push_back(resp);
            }
        anteriory = vety[i];
        }else if(vety[i] == 300){
            y.push_back(321);
        }
    }
      y.push_back(vety[vety.size()-1]);
    escreverXYjunto(x,y);
}
//###################################################################################

int procCaracter(char letra){

    switch(letra){
        case '0': return 0x60;
        break;
        case '1': return 0x61;
        break;
        case '2': return 0x62;
        break;
        case '3': return 0x63;
        break;
        case '4': return 0x64;
        break;
        case '5': return 0x65;
        break;
        case '6': return 0x66;
        break;
        case '7': return 0x67;
        break;
        case '8': return 0x68;
        break;
        case '9': return 0x69;
        break;
        case ',': return 0x6E;
        break;
        case '-': return 0x6D;
        break;
        case '+': return 0x6B;
        break;
    }
}

void digitar(vector<int> vet){
    KEYBDINPUT kbin;
    INPUT in;

    kbin.dwFlags=0;
    kbin.time=0;
    kbin.wScan=0;
    kbin.dwExtraInfo=0;

    in.type=1;

   Sleep(5000);
    for(int i=0;i<vet.size();i++){
        kbin.wVk=vet[i];
        in.ki=kbin;
        if(vet[i]==0x0D){
            SendInput(1,&in,sizeof(in));
            Sleep(60);
        }else{
            SendInput(1,&in,sizeof(in));
            Sleep(60);
        }
    }

}

void explode(const string& s, const char& c){
    string buff="";
    vector<string> vet_separado;
    vector<int> vet_convertido;
    int countVirgula = 0;
    cout<<s;
    for(int i=0;i<=s.size();i++){
            if(s[i]==c){countVirgula++;}

            if(countVirgula<=1){
                    buff+=s[i];
                    vet_convertido.push_back(procCaracter(s[i]));
            }
            else{
                vet_separado.push_back(buff);
                vet_separado.push_back("\n");
                vet_convertido.push_back(procCaracter(s[i]));
                vet_convertido.push_back(0x0D);
                countVirgula=0;
                buff="";
            }
        }

    if(buff!=""){
        vet_separado.push_back(buff);
    }

    for(auto s:vet_separado){
        cout<<s;
    }
    digitar(vet_convertido);
}

string pegarlinha(){
    string line;

    ifstream file("coordenadas.txt");
    if(file.is_open()){
        while(!file.eof()){
            getline(file,line);
            explode(line,',');

            line="";
        }
        file.close();
    }
    return line;
}


void init(){
    int result;
    string nome;
    vector<int> vetx;
    vector<int> vety;
    vetx.reserve(50000);
    vety.reserve(50000);
    unsigned char r,g,b;


    nome = "10x10.jpg";
    result = Image.Load(nome.c_str());
    if(!result){
            cout<<"Erro ao carregar a imagem. \n"<<endl;
        exit(1);
    }else{
        int x=1;
        int y=1;
        cout<<"Imagem carregada. \n"<<endl;

        cout<<Image.SizeX()<<endl;
        cout<<Image.SizeY()<<endl;

    int vetor[2];
    verifPixels(Image.SizeX()-1,Image.SizeY()-1,vetor,vetx,vety);

    arrumarCoordenadas(vetx,vety);
    fecharArquivo();
    pegarlinha();
    }
}



int main ( int argc, char** argv )
{

    criarArquivo();
    init();

    system("pause");
    return 0;
}


