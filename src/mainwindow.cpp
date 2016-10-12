#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "custommask.h"
#include <iostream>
#include <stdio.h>
#include <QtMath>
#include <stdlib.h>
#include <windows.h>
#include <QLabel>
#include <QQueue>
#include <QVBoxLayout>
#define byte unsigned char
#define b16 unsigned short int
#define b32 unsigned int

unsigned char type[2];

typedef struct{                   //2 bytes
    unsigned int size;                          //4 bytes
    unsigned short int reserved1, reserved2;    //4 bytes   14 bytes
    unsigned int offset;                        //4 bytes
} HEADER;

typedef struct{
    unsigned int size;
    unsigned int width, height;
    unsigned short int planes;
    unsigned short int bits;
    unsigned int compression;
    unsigned int imagesize;
    unsigned int xresolution, yresolution;
    unsigned int ncolours;
    unsigned int importancolours;
} INFOHEADER;

enum Op{
    Filtro,
    bSobel,
    pLaplace,
    Sgaus,
    Smedia,
    bLaplace,
    bPrewitt,
    Umbra,
    Negative,
    Eq,
    Ilumination,
    Contrast,
    MirrorX,
    MirrorY,
    Escal,
    Zoom,
    Rotates
};

OPENFILENAME ofn;
HEADER header;
INFOHEADER info;
QImage Image, Img, SaveS, SaveH, Iundo, Iredo; //Img Copia!
QString fileName;

bool ops[20];
int div3, div5, div7, div9, div11, div13, div15;
int r[256], g[256], b[256],  NM;
int M3x3[3][3], M5x5[5][5], M7x7[7][7], M9x9[9][9];
int M11x11[11][11], M13x13[13][13];
long long int  M15x15[15][15];
int Ailumination, resolution, Sw, Sh, ro, undo, redo;
int ancho, altura, umbral, t, tam, Rancho, Ralto, Uancho, Ualto;
double AzoomI, AzoomO, contrast, filtro[1000][1000], zoom;
bool bolS, bolA, fillH;

int clamp(int num){ // -  - - - - - - - - - NEW
    if(num > 255)
        return 255;
    else if(num < 0)
        return 0;
    return num;
}

void Rotate90(){
    int w, h;
    int t = info.width-1;
    QImage Img2(info.height, info.width, QImage::Format_RGB888);

    for(unsigned int i = 0; i < info.width; i++){
        for(unsigned int j = 0; j < info.height; j++)
            Img2.setPixel(j, t, Image.pixel(i, j));
        t--;
    }

    w = info.width; h = info.height;
    info.width = h; info.height = w;

    w = info.xresolution; h = info.yresolution;
    info.xresolution = h; info.yresolution = w;
    Image = Img2;
}

void Rotate180(){
    int t = info.width-1;
    int z;
    QImage Img2(info.width, info.height, QImage::Format_RGB888);

    for(unsigned int i = 0; i < info.width; i++){
        z = info.height-1;
        for(unsigned int j = 0; j < info.height; j++){
            Img2.setPixel(t, z, Image.pixel(i, j));
            z--;
        }
        t--;
    }
    Image = Img2;
}

void Rotate270(){
    int w, h;
    int t;
    QImage Img2(info.height, info.width, QImage::Format_RGB888);

    for(unsigned int i = 0; i < info.width; i++){
        t = info.height-1;
        for(unsigned int j = 0; j < info.height; j++){
            Img2.setPixel(t, i, Image.pixel(i, j));
            t--;
        }
    }

    w = info.width; h = info.height;
    info.width = h; info.height = w;

    w = info.xresolution; h = info.yresolution;
    info.xresolution = h; info.yresolution = w;
    Image = Img2;
}

void GenerateM(){
    int v3[3], v5[5], v7[7], v9[9], v11[11], v13[13], v15[15];
    div3 = div5 = div7 = div9 = div11 = div13 = div15 = 0;

    v3[0] = 1; v3[1] = 2; v3[2] = 1;
    v5[0] = 1; v5[1] = 4; v5[2] = 6; v5[3] = 4; v5[4] = 1;
    v7[0] = 1; v7[1] = 6; v7[2] = 15; v7[3] = 20; v7[4] = 15; v7[5] = 6; v7[6] = 1;
    v9[0] = 1; v9[1] = 8; v9[2] = 28; v9[3] = 56; v9[4] = 70; v9[5] = 56; v9[6] = 28; v9[7] = 8; v9[8] = 1;
    v11[0] = 1; v11[1] = 10; v11[2] = 45; v11[3] = 120; v11[4] = 210; v11[5] = 252; v11[6] = 210; v11[7] = 120; v11[8] = 45; v11[9] = 10; v11[10] = 1;
    v13[0] = 1; v13[1] = 12; v13[2] = 66; v13[3] = 220; v13[4] = 495; v13[5] = 792; v13[6] = 924; v13[7] = 792; v13[8] = 495; v13[9] = 220; v13[10] = 66; v13[11] = 12; v13[12] = 1;
    v15[0] = 1; v15[1] = 14; v15[2] = 91; v15[3] = 364; v15[4] = 1001; v15[5] = 2002; v15[6] = 3003; v15[7] = 3432; v15[8] = 3003; v15[9] = 2002; v15[10] = 1001; v15[11] = 364; v15[12] = 91; v15[13] = 14; v15[14] = 1;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++)
            M3x3[i][j]=v3[i]*v3[j];
        div3 += v3[i];
    }
    div3 *= div3;

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++)
            M5x5[i][j]=v5[i]*v5[j];
        div5 += v5[i];
    }
    div5 *= div5;

    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 7; j++)
            M7x7[i][j]=v7[i]*v7[j];
        div7 += v7[i];
    }
    div7 *= div7;

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++)
            M9x9[i][j]=v9[i]*v9[j];
        div9 += v9[i];
    }
    div9 *= div9;

    for(int i = 0; i < 11; i++){
        for(int j = 0; j < 11; j++)
            M11x11[i][j]=v11[i]*v11[j];
        div11 += v11[i];
    }
    div11 *= div11;

    for(int i = 0; i < 13; i++){
        for(int j = 0; j < 13; j++)
            M13x13[i][j] = v13[i]*v13[j];
        div13 += v13[i];
    }
    div13 *= div13;

    for(int i = 0; i < 15; i++){
        for(int j = 0; j < 15; j++)
            M15x15[i][j]=v15[i]*v15[j];
        div15 += v15[i];
    }
    div15 *= div15;
}

void Suavizado_Media(){
    if(!bolS){
        QImage Img(info.width, info.height, QImage::Format_RGB888);
        Img = Image;

        unsigned int PosX, PosY;
        int PromR, PromG, PromB, div;
        div = PromR = PromG = PromB = 0;

        for(int i = 0; i < Image.width(); i++){
            for(int j = 0; j < Image.height(); j++){
                for(int m = -t/2; m <= t/2; m++){
                    for(int n = -t/2; n <= t/2; n++){
                        PosX = i + m; PosY = j + n;
                        if(PosX >= 0 && PosY >= 0 && PosX < info.width && PosY < info.height){
                           QColor clrCurrent(Img.pixel(PosX, PosY));
                           PromR += clrCurrent.red(); PromG += clrCurrent.green(); PromB += clrCurrent.blue();
                           div++;
                        }
                    }
                }
                PromR = PromR/div; PromG = PromG/div; PromB = PromB/div;
                Image.setPixel(i, j, qRgb(PromR, PromG, PromB));
                div = PromR = PromG = PromB = 0;
            }
        }
        SaveS = Image;
        bolS = false;
        bolA = true;
    }
}

void Suavizado_Gausiano(){
    if(!bolS){

        QImage Img(info.width, info.height, QImage::Format_RGB888);
        Img = Image;
        int indexX = 0, indexY = 0;
        unsigned int PosX, PosY;
        int PromR, PromG, PromB, t;
        PromR = PromG = PromB = 0;

        if(tam == 3){
            t = tam;
            for(int i = 0; i < Image.width(); i++){
                for(int j = 0; j < Image.height(); j++){
                    indexY = 0;
                    for(int m = -t/2; m <= t/2; m++){
                        indexX = 0;
                        for(int n = -t/2; n <= t/2; n++){
                            PosX = i + m; PosY = j + n;
                            if(PosX >= 0 && PosY >= 0 && PosX < info.width && PosY < info.height){
                               QColor clrCurrent(Img.pixel(PosX, PosY));
                               PromR += clrCurrent.red()*M3x3[indexX][indexY]; PromG += clrCurrent.green()*M3x3[indexX][indexY]; PromB += clrCurrent.blue()*M3x3[indexX][indexY];
                            }
                            indexX++;
                        }
                        indexY++;
                    }
                    PromR = PromR/div3; PromG = PromG/div3; PromB = PromB/div3;
                    Image.setPixel(i, j, qRgb(PromR, PromG, PromB));
                    PromR = PromG = PromB = 0;
                }
            }

        }else if(tam == 5){
            t = tam;
            for(int i = 0; i < Image.width(); i++){
                for(int j = 0; j < Image.height(); j++){
                    indexY = 0;
                    for(int m = -t/2; m <= t/2; m++){
                        indexX = 0;
                        for(int n = -t/2; n <= t/2; n++){
                            PosX = i + m; PosY = j + n;
                            if(PosX >= 0 && PosY >= 0 && PosX < info.width && PosY < info.height){
                               QColor clrCurrent(Img.pixel(PosX, PosY));
                               PromR += clrCurrent.red()*M5x5[indexX][indexY]; PromG += clrCurrent.green()*M5x5[indexX][indexY]; PromB += clrCurrent.blue()*M5x5[indexX][indexY];
                            }
                            indexX++;
                        }
                        indexY++;
                    }
                    PromR = PromR/div5; PromG = PromG/div5; PromB = PromB/div5;
                    Image.setPixel(i, j, qRgb(PromR, PromG, PromB));
                    PromR = PromG = PromB = 0;
                }
            }
        }else if(tam == 7){
            t = tam;
            for(int i = 0; i < Image.width(); i++){
                for(int j = 0; j < Image.height(); j++){
                    indexY = 0;
                    for(int m = -t/2; m <= t/2; m++){
                        indexX = 0;
                        for(int n = -t/2; n <= t/2; n++){
                            PosX = i + m; PosY = j + n;
                            if(PosX >= 0 && PosY >= 0 && PosX < info.width && PosY < info.height){
                               QColor clrCurrent(Img.pixel(PosX, PosY));
                               PromR += clrCurrent.red()*M7x7[indexX][indexY]; PromG += clrCurrent.green()*M7x7[indexX][indexY]; PromB += clrCurrent.blue()*M7x7[indexX][indexY];
                            }
                            indexX++;
                        }
                        indexY++;
                    }
                    PromR = PromR/div7; PromG = PromG/div7; PromB = PromB/div7;
                    Image.setPixel(i, j, qRgb(PromR, PromG, PromB));
                    PromR = PromG = PromB = 0;
                }
            }
        }else if(tam == 9){
            t = tam;
            for(int i = 0; i < Image.width(); i++){
                for(int j = 0; j < Image.height(); j++){
                    indexY = 0;
                    for(int m = -t/2; m <= t/2; m++){
                        indexX = 0;
                        for(int n = -t/2; n <= t/2; n++){
                            PosX = i + m; PosY = j + n;
                            if(PosX >= 0 && PosY >= 0 && PosX < info.width && PosY < info.height){
                               QColor clrCurrent(Img.pixel(PosX, PosY));
                               PromR += clrCurrent.red()*M9x9[indexX][indexY]; PromG += clrCurrent.green()*M9x9[indexX][indexY]; PromB += clrCurrent.blue()*M9x9[indexX][indexY];
                            }
                            indexX++;
                        }
                        indexY++;
                    }
                    PromR = PromR/div9; PromG = PromG/div9; PromB = PromB/div9;
                    Image.setPixel(i, j, qRgb(PromR, PromG, PromB));
                    PromR = PromG = PromB = 0;
                }
            }
        }else if(tam == 11){
            t = tam;
            for(int i = 0; i < Image.width(); i++){
                for(int j = 0; j < Image.height(); j++){
                    indexY = 0;
                    for(int m = -t/2; m <= t/2; m++){
                        indexX = 0;
                        for(int n = -t/2; n <= t/2; n++){
                            PosX = i + m; PosY = j + n;
                            if(PosX >= 0 && PosY >= 0 && PosX < info.width && PosY < info.height){
                               QColor clrCurrent(Img.pixel(PosX, PosY));
                               PromR += clrCurrent.red()*M11x11[indexX][indexY]; PromG += clrCurrent.green()*M11x11[indexX][indexY]; PromB += clrCurrent.blue()*M11x11[indexX][indexY];
                            }
                            indexX++;
                        }
                        indexY++;
                    }
                    PromR = PromR/div11; PromG = PromG/div11; PromB = PromB/div11;
                    Image.setPixel(i, j, qRgb(PromR, PromG, PromB));
                    PromR = PromG = PromB = 0;
                }
            }
        }else if(tam == 13){
            t = tam;
            for(int i = 0; i < Image.width(); i++){
                for(int j = 0; j < Image.height(); j++){
                    indexY = 0;
                    for(int m = -t/2; m <= t/2; m++){
                        indexX = 0;
                        for(int n = -t/2; n <= t/2; n++){
                            PosX = i + m; PosY = j + n;
                            if(PosX >= 0 && PosY >= 0 && PosX < info.width && PosY < info.height){
                               QColor clrCurrent(Img.pixel(PosX, PosY));
                               PromR += clrCurrent.red()*M13x13[indexX][indexY];
                               PromG += clrCurrent.green()*M13x13[indexX][indexY];
                               PromB += clrCurrent.blue()*M13x13[indexX][indexY];
                            }
                            indexX++;
                        }
                        indexY++;
                    }
                    PromR = PromR/div13; PromG = PromG/div13; PromB = PromB/div13;
                    Image.setPixel(i, j, qRgb(PromR, PromG, PromB));
                    PromR = PromG = PromB = 0;
                }
            }
        }else if(tam == 15){
            long long int PromR, PromG, PromB;
            PromR = PromG = PromB = 0;
            t = tam;
            for(int i = 0; i < Image.width(); i++){
                for(int j = 0; j < Image.height(); j++){
                    indexY = 0;
                    for(int m = -t/2; m <= t/2; m++){
                        indexX = 0;
                        for(int n = -t/2; n <= t/2; n++){
                            PosX = i + m; PosY = j + n;
                            if(PosX >= 0 && PosY >= 0 && PosX < info.width && PosY < info.height){
                               QColor clrCurrent(Img.pixel(PosX, PosY));
                               PromR += clrCurrent.red()*M15x15[indexX][indexY];
                               PromG += clrCurrent.green()*M15x15[indexX][indexY];
                               PromB += clrCurrent.blue()*M15x15[indexX][indexY];
                            }
                            indexX++;
                        }
                        indexY++;
                    }
                    PromR = PromR/div15; PromG = PromG/div15; PromB = PromB/div15;
                    Image.setPixel(i, j, qRgb(PromR, PromG, PromB));
                    PromR = PromG = PromB = 0;
                }
            }
        }
        SaveS = Image;
        bolS = false; bolA = true;
    }
}

void Borde_Laplaciano () {  // CALCULO DEL BORDE LAPLACIANO
    if(!bolS){

        QImage Img(info.width, info.height, QImage::Format_RGB888);
        Img = Image;

        int *A = new int[tam*tam];
        int pivote = ((tam/2)*tam) + (tam/2);
        for(int i = 0; i < tam*tam; i++){ // se llena el kernel dependiendo del tamaño que se escogio.
            if(i == pivote){
                A[i]=(tam*tam)-1;
            }else{
            A[i] = -1;
            }
        }

        unsigned int PosX, PosY;
        int AcumR, AcumG, AcumB, t=tam/2, index;

        AcumR = AcumG = AcumB = 0;

        for(int i = 0; i < Image.width(); i++){ // Se busca la imagen resultante Laplaciana
            for(int j = 0; j < Image.height(); j++){
                for(int m = -t; m <= t; m++){
                    for(int n = -t; n <= t; n++){
                        PosX = i + m; PosY = j + n;
                        if(PosX >= 0 && PosY >= 0 && PosX < info.width && PosY < info.height){
                           QColor clrCurrent(Img.pixel(PosX, PosY));
                           index = ((n+t)*tam)+(m+t); // indice de la mascara.

                           AcumR += clrCurrent.red() * A[index];
                           AcumG += clrCurrent.green() * A[index];
                           AcumB += clrCurrent.blue() * A[index];
                        }
                    }
                }
                AcumR = clamp(AcumR); AcumG = clamp(AcumG); AcumB = clamp(AcumB);
                Image.setPixel(i, j, qRgb(AcumR, AcumG, AcumB));
                AcumR = AcumG = AcumB = 0;
            }
        }
        SaveS = Image;
        bolS = false; bolA = true;
        delete[] A;
    }
}

void XGradiente(int* Acumx, int i, int j, int op){ // CALCULO DE LA GRADIENTE HORIZONTAL PARA EL BORDE

    int AcumR=0, AcumG=0, AcumB=0, tam2=t/2;

    if(op == 1){ // KERNEL DE SOBEL  X

        int* A = new int[t*t];

        //MASCARAS de X
        switch(t){

        case 3:
            A[0]=1; A[1]=0; A[2]=-1;
            A[3]=2; A[4]=0; A[5]=-2;
            A[6]=1; A[7]=0; A[8]=-1;
        break;

        case 5:
            A[0]=1; A[1]=2; A[2]=0; A[3]=-2; A[4]=-1;
            A[5]=4; A[6]=8; A[7]=0; A[8]=-8; A[9]=-4;
            A[10]=6; A[11]=12; A[12]=0; A[13]=-12; A[14]=-6;
            A[15]=4; A[16]=8; A[17]=0; A[18]=-8; A[19]=-4;
            A[20]=1; A[21]=2; A[22]=0; A[23]=-2; A[24]=-1;
        break;

        case 7:
        break;

        }
        int PosX, PosY;
        int index2;

        for(int m = -tam2; m <= tam2; m++){
            for(int n = -tam2; n <= tam2; n++){
                PosX = i + m; PosY = j + n;
                if(PosX >= 0 && PosY >= 0 && PosX < Image.width() && PosY < Image.height()){
                   QColor clrCurrent(Img.pixel(PosX, PosY));
                   index2 = ((n+tam2)*t)+(m+tam2); // indice de la mascara.

                   AcumR = (clrCurrent.red() * A[index2]) + AcumR;
                   AcumG = (clrCurrent.green() * A[index2]) + AcumG;
                   AcumB = (clrCurrent.blue() * A[index2]) + AcumB;
                }
            }
        }

    }else{ // KERNEL DE PREWITT X

        int* A = new int[t*t];


        //MASCARAS de X
        switch(t){

        case 3: {
            int index = 0;
            for(int i=0; i<t;i++){
                A[index++] = 1;
                A[index++] = 0;
                A[index++] = -1;
            }
        break;}

        case 5: {
            int index = 0;
            for(int i=0; i<t;i++){
                A[index++] = 2;
                A[index++] = 1;
                A[index++] = 0;
                A[index++] = -1;
                A[index++] = -2;
            }
        break;}

        case 7:{
            int index = 0;
            for(int i=0; i<t;i++){
                A[index++] = 3;
                A[index++] = 2;
                A[index++] = 1;
                A[index++] = 0;
                A[index++] = -1;
                A[index++] = -2;
                A[index++] = -3;
            }
        break;}

        case 9:{
            int index = 0;
            for(int i=0; i<t;i++){
                A[index++] = 4;
                A[index++] = 3;
                A[index++] = 2;
                A[index++] = 1;
                A[index++] = 0;
                A[index++] = -1;
                A[index++] = -2;
                A[index++] = -3;
                A[index++] = -4;
            }
        break;}

        case 11:{
            int index = 0;
            for(int i=0; i<t;i++){
                A[index++] = 5;
                A[index++] = 4;
                A[index++] = 3;
                A[index++] = 2;
                A[index++] = 1;
                A[index++] = 0;
                A[index++] = -1;
                A[index++] = -2;
                A[index++] = -3;
                A[index++] = -4;
                A[index++] = -5;
            }
        break;}

        case 13:{
            int index = 0;
            for(int i=0; i<t;i++){
                A[index++] = 6;
                A[index++] = 5;
                A[index++] = 4;
                A[index++] = 3;
                A[index++] = 2;
                A[index++] = 1;
                A[index++] = 0;
                A[index++] = -1;
                A[index++] = -2;
                A[index++] = -3;
                A[index++] = -4;
                A[index++] = -5;
                A[index++] = -6;
            }
        break;}

        case 15:{
            int index = 0;
            for(int i=0; i<t;i++){
                A[index++] = 7;
                A[index++] = 6;
                A[index++] = 5;
                A[index++] = 4;
                A[index++] = 3;
                A[index++] = 2;
                A[index++] = 1;
                A[index++] = 0;
                A[index++] = -1;
                A[index++] = -2;
                A[index++] = -3;
                A[index++] = -4;
                A[index++] = -5;
                A[index++] = -6;
                A[index++] = -7;
            }
        break;}

        }

        int PosX, PosY;
        int index2;

        for(int m = -tam2; m <= tam2; m++){
            for(int n = -tam2; n <= tam2; n++){
                PosX = i + m; PosY = j + n;
                if(PosX >= 0 && PosY >= 0 && PosX < Image.width() && PosY < Image.height()){
                   QColor clrCurrent(Img.pixel(PosX, PosY));
                   index2 = ((n+tam2)*t)+(m+tam2); // indice de la mascara.

                   AcumR = (clrCurrent.red() * A[index2]) + AcumR;
                   AcumG = (clrCurrent.green() * A[index2]) + AcumG;
                   AcumB = (clrCurrent.blue() * A[index2]) + AcumB;
                }
            }
        }

    }


    Acumx[0] = AcumR;
    Acumx[1] = AcumG;
    Acumx[2] = AcumB;

}




void YGradiente(int* Acumy, int i, int j, int op){ // CALCULO DE LA GRADIENTE VERTICAL PARA EL BORDE

    int AcumR=0, AcumG=0, AcumB=0, tam2=t/2;

    if(op == 1){ // KERNEL DE SOBEL  Y



        int* A = new int[t*t];

        //MASCARAS de Y
        switch(t){

            case 3:
                A[0]=-1; A[1]=-2; A[2]=-1;
                A[3]=0; A[4]=0; A[5]=0;
                A[6]=1; A[7]=2; A[8]=1;
            break;

            case 5:
                A[0]=-1; A[1]=-4; A[2]=-6; A[3]=-4; A[4]=-1;
                A[5]=-2; A[6]=-8; A[7]=-12; A[8]=-8; A[9]=-2;
                A[10]=0; A[11]=0; A[12]=0; A[13]=0; A[14]=0;
                A[15]=2; A[16]=8; A[17]=12; A[18]=8; A[19]=2;
                A[20]=1; A[21]=4; A[22]=6; A[23]=4; A[24]=1;
            break;

        }
        int PosX, PosY;
        int index2;

        for(int m = -tam2; m <= tam2; m++){
            for(int n = -tam2; n <= tam2; n++){
                PosX = i + m; PosY = j + n;
                if(PosX >= 0 && PosY >= 0 && PosX < Image.width() && PosY < Image.height()){
                   QColor clrCurrent(Img.pixel(PosX, PosY));
                   index2 = ((n+tam2)*t)+(m+tam2); // indice de la mascara.

                   AcumR = (clrCurrent.red() * A[index2]) + AcumR;
                   AcumG = (clrCurrent.green() * A[index2]) + AcumG;
                   AcumB = (clrCurrent.blue() * A[index2]) + AcumB;
                }
            }
        }

    }else{ // KERNEL DE PREWITT  Y
        int* A = new int[t*t];


        //MASCARAS de Y
        switch(t){

        case 3: {
                int index = 0;
                int aux[3]={-1,0,1};
                for(int i=0;i<t;i++){
                    A[index++] = aux[i];
                    A[index++] = aux[i];
                    A[index++] = aux[i];
                }

            break;}

        case 5: {
                int index = 0;
                int aux[5]={-2,-1,0,1,2};
                for(int i=0;i<t;i++){
                    A[index++] = aux[i];
                    A[index++] = aux[i];
                    A[index++] = aux[i];
                    A[index++] = aux[i];
                    A[index++] = aux[i];
                }

            break;}

        case 7: {
                int index = 0;
                int aux[7]={-3,-2,-1,0,1,2,3};
                for(int i=0;i<t;i++){
                    A[index++] = aux[i];
                    A[index++] = aux[i];
                    A[index++] = aux[i];
                    A[index++] = aux[i];
                    A[index++] = aux[i];
                    A[index++] = aux[i];
                    A[index++] = aux[i];
                }

            break;}

        case 9: {
            int index = 0;
            int aux[9]={-4,-3,-2,-1,0,1,2,3,4};
            for(int i=0;i<t;i++){
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
            }

            break;}

        case 11: {
            int index = 0;
            int aux[11]={-5,-4,-3,-2,-1,0,1,2,3,4,5};
            for(int i=0;i<t;i++){
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
            }

            break;}

        case 13: {
            int index = 0;
            int aux[13]={-7,-5,-4,-3,-2,-1,0,1,2,3,4,5,7};
            for(int i=0;i<t;i++){
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
            }

            break;}

        case 15: {
            int index = 0;
            int aux[15]={-8,-7,-5,-4,-3,-2,-1,0,1,2,3,4,5,7,8};
            for(int i=0;i<t;i++){
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
                A[index++] = aux[i];
            }

            break;}

        }
        int PosX, PosY;
        int index2;

        for(int m = -tam2; m <= tam2; m++){
            for(int n = -tam2; n <= tam2; n++){
                PosX = i + m; PosY = j + n;
                if(PosX >= 0 && PosY >= 0 && PosX < Image.width() && PosY < Image.height()){
                   QColor clrCurrent(Img.pixel(PosX, PosY));
                   index2 = ((n+tam2)*t)+(m+tam2); // indice de la mascara.

                   AcumR = (clrCurrent.red() * A[index2]) + AcumR;
                   AcumG = (clrCurrent.green() * A[index2]) + AcumG;
                   AcumB = (clrCurrent.blue() * A[index2]) + AcumB;
                }
            }
        }


    }

    Acumy[0] = AcumR;
    Acumy[1] = AcumG;
    Acumy[2] = AcumB;

}

void Buscar_Bordes (int op) {  // CALCULO DEL BORDE LOS BORDES PREWITT Y SOBEL
    if(!bolS){
        QImage Img(info.width, info.height, QImage::Format_RGB888);
        Img = Image;

        int *Acumx = new int[3];
        int *Acumy = new int[3];
        int sumR, sumG, sumB;

        for(unsigned int i = 0; i < Image.width(); i++){
            for(unsigned int j = 0; j < Image.height(); j++){


                     XGradiente(Acumx, i, j, op);
                     YGradiente(Acumy, i, j, op);

                     sumR = clamp(abs(Acumx[0]) + abs(Acumy[0]));
                     sumG = clamp(abs(Acumx[1]) + abs(Acumy[1]));
                     sumB = clamp(abs(Acumx[2]) + abs(Acumy[2]));

                    Image.setPixel(i, j, qRgb(sumR, sumG, sumB));

            }
        }

        SaveS = Image;
        bolS = true;
        delete[] Acumx;
        delete[] Acumy;
    }

}

void Perfilado_Laplaciano () {  // CALCULO DEL PERFILADO LAPLACIANO
    if(!bolS){

        QImage Img(info.width, info.height, QImage::Format_RGB888);
        Img = Image;
        int div = tam*2;

        int *A = new int[tam*tam];
        int pivote = ((tam/2)*tam) + (tam/2);
        for(int i = 0; i < tam*tam; i++){ // se llena el kernel dependiendo del tamaño que se escogio.
            if(i == pivote){
                A[i]=(tam*tam)-1;
            }else{
            A[i] = -1;
            }
        }

        unsigned int PosX, PosY;
        int AcumR, AcumG, AcumB, t=tam/2, index;

        AcumR = AcumG = AcumB = 0;

        for(int i = 0; i < Image.width(); i++){ // Se busca la imagen resultante Laplaciana
            for(int j = 0; j < Image.height(); j++){
                for(int m = -t; m <= t; m++){
                    for(int n = -t; n <= t; n++){
                        PosX = i + m; PosY = j + n;
                        if(PosX >= 0 && PosY >= 0 && PosX < info.width && PosY < info.height){
                           QColor clrCurrent(Img.pixel(PosX, PosY));
                           index = ((n+t)*tam)+(m+t); // indice de la mascara.

                           AcumR += clrCurrent.red() * A[index];
                           AcumG += clrCurrent.green() * A[index];
                           AcumB += clrCurrent.blue() * A[index];
                        }
                    }
                }
                AcumR = clamp(AcumR/div); AcumG = clamp(AcumG/div); AcumB = clamp(AcumB/div);
                Image.setPixel(i, j, qRgb(AcumR, AcumG, AcumB));
                AcumR = AcumG = AcumB = 0;
            }
        }

        for(int i = 0; i < Image.width(); i++){ // Se une la imagen original con la Laplaciana
            for(int j = 0; j < Image.height(); j++){
                QColor clrLaplace(Image.pixel(i, j));
                QColor clrOriginal(Img.pixel(i, j));

                AcumR = clamp(clrLaplace.red() + clrOriginal.red()) ;
                AcumG = clamp(clrLaplace.green() + clrOriginal.green());
                AcumB = clamp(clrLaplace.blue() + clrOriginal.blue());

                Image.setPixel(i, j, qRgb(AcumR, AcumG, AcumB));

            }
        }
        SaveS = Image;
        bolS = bolA = true;
        delete[] A;
    }
}

void MainWindow::ZoomNN (double zoom){ // -  - - - - - - - - - NEW


    int oheight = info.height, owidth = info.width;
    int nheight = (int)(oheight * zoom);
    int nwidth = (int)(owidth * zoom);
    info.height = nheight; info.width = nwidth;
    QImage Img2(nwidth, nheight, QImage::Format_RGB888);

    if(!ui->Bilinear->isChecked()) { // -  - - - - - - - - - VECINOS CERCANOS

        double x_ratio = owidth/(double)nwidth, y_ratio = oheight/(double)nheight, px, py;

        for(int i=0; i<nwidth; i++) {
            for(int j=0; j<nheight; j++){

                px = (int)(i*x_ratio);
                py = (int)(j*y_ratio);

                Img2.setPixel(i,j,Image.pixel(px,py));
            }
        }

    } else{  // -  - - - - - - - - - BILINEAR

        double x_ratio = (owidth-1)/(double)nwidth, y_ratio = (oheight-1)/(double)nheight;
        int x, y, x_diff, y_diff;//indexa, indexb, indexc, indexd;

        for(int i=0; i<nwidth; i++) {
            for(int j=0; j<nheight; j++){

                x = (int)(x_ratio * i);
                y = (int)(y_ratio * j);
                x_diff = (x_ratio * i) - x;
                y_diff = (y_ratio * j) - y;


                // Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + D(wh)
                QColor clrPixelA(Image.pixel(x, y));
                QColor clrPixelB(Image.pixel(x+1, y));
                QColor clrPixelC(Image.pixel(x, y+1));
                QColor clrPixelD(Image.pixel(x+1, y+1));

                QRgb value = qRgb(
                            clamp(clrPixelA.red()*(1-x_diff)*(1-y_diff) + clrPixelB.red()*(x_diff)*(1-y_diff) + clrPixelC.red()*(y_diff)*(1-x_diff) + clrPixelD.red()*(x_diff*y_diff)) ,
                            clamp(clrPixelA.green()*(1-x_diff)*(1-y_diff) + clrPixelB.green()*(x_diff)*(1-y_diff) + clrPixelC.green()*(y_diff)*(1-x_diff) + clrPixelD.green()*(x_diff*y_diff)),
                            clamp(clrPixelA.blue()*(1-x_diff)*(1-y_diff) + clrPixelB.blue()*(x_diff)*(1-y_diff) + clrPixelC.blue()*(y_diff)*(1-x_diff) + clrPixelD.blue()*(x_diff*y_diff))
                           );
                Img2.setPixel(i,j,value);
            }
        }
    }
    Image = Img2;
}

void MainWindow::fillHist(){
    for(int i=0; i <256 ; i++){
        r[i] = 0; g[i]=0; b[i]=0;
    }

    for(int q=0; q < SaveH.width(); q++){
        for(int c=0; c < SaveH.height(); c++){
           QColor clrCurrent(SaveH.pixel(q, c));
           r[clrCurrent.red()]++;
           g[clrCurrent.green()]++;
           b[clrCurrent.blue()]++;
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QVBoxLayout * lbl = new QVBoxLayout(this);
    lbl->addWidget(ui->MyLabel);
    ui->scrollAreaWidgetContents->setLayout(lbl);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionCargar_triggered()
{
   fileName = QFileDialog::getOpenFileName(this, this->tr("Open File"), QString(), this->tr("BMP (*.bmp)"));
   if(!fileName.isEmpty()){
        QByteArray ba = fileName.toLatin1();
        const char *ofn = ba.data();

        FILE *f;
        f = fopen(ofn, "rb+");
        if(!f){
            printf("No se pudo abrir el archivo\n");
            return;
        }
        GenerateM();
//LECTURA DE LAS CABECERAS
        fread(&type, 2, 1, f);
        fread(&header, 12 , 1, f);
        fread(&info, 40, 1, f);

        if(type[0]!='B' || type[1]!='M'){
            printf ("La imagen debe tener un forma BMP.\n");
            return;
        }
//LECTURA DEL BITMAP
        Image = QImage(fileName, "BMP");
//CARGANDO DATOS PARA EL HISTOGRAMA
        NM = info.width*info.height;
        Sw = info.width; Sh = info.height;

        for(int i = 0; i < 20; i++)
            ops[i] = false;
        AzoomI = 0; AzoomO = 0; Ailumination = 0; umbral = 0; undo = redo = -1;
        Img = SaveH = SaveS = Image; contrast = 0.0; bolS = false; tam = 0; ro = 0;
        fillHist(); hist.LoadHist(r, g, b, Image);
        fillH = bolA = false;

        resolution = (int)(ceil(sqrt((info.height*info.height)+(info.width*info.width))));
// DATOS DE LA IMAGEN

        ui->info_ancho->setText(QString::number(Image.width()));
        ui->info_alto->setText(QString::number(Image.height()));
        ui->info_bits->setText(QString::number(info.bits));

        if(info.bits == 24){

            ui->info_tam->setText(QString::number(((Image.width() * Image.height() * 3)+54)));
        }else if (info.bits == 8){
            ui->info_tam->setText(QString::number(((Image.width() * Image.height() )+54)+  (info.ncolours * 4)));
        }else if (info.bits == 4) {
            ui->info_tam->setText(QString::number((((Image.width() * Image.height())/2 )+54)+  (info.ncolours * 4)));
        }else{
            ui->info_tam->setText(QString::number((((Image.width() * Image.height())/8 )+54)+  (info.ncolours * 4)));
        }

        ui->MyLabel->setPixmap(QPixmap::fromImage(Image));
    }
}

void MainWindow::on_actionGuardar_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, this->tr("Save File"), QString(), this->tr("BMP (*.bmp)"));
        if(!fileName.isEmpty())
            Image.save(fileName, "BMP");
}

void MainWindow::SendOp(int op){
    switch(op){
        case Umbra:
            ops[Umbra] = 1;
            fillH = true;
        break;
        case Negative:
            ops[Negative] = !ops[Negative];
            fillH = true;
        break;
        case Eq:
            ops[Eq] = !ops[Eq];
            fillH = true;
        break;
        case MirrorX:
            ops[op] = !ops[op];
            fillH = false;
        break;
        case MirrorY:
            ops[op] = !ops[op];
            fillH = false;
        break;
        case Zoom:
            ops[Zoom] = 1;
            fillH = false;
        break;
        case Escal:
            ops[Escal] = 1;
            fillH = true;
        break;
        case Ilumination:
            ops[Ilumination] = 1;
            fillH = true;
        break;
        case Contrast:
            ops[Contrast] = 1;
            fillH = true;
        break;
        case Sgaus:
            ops[Sgaus] = 1;
            fillH = true;
        break;
        case Smedia:
            ops[Smedia] = 1;
            fillH = true;
        break;
        case bLaplace:
            ops[bLaplace] = 1;
            fillH = true;
        break;
        case bPrewitt:
            ops[bPrewitt] = 1;
            fillH = true;
        break;
        case bSobel:
            ops[bSobel] = 1;
            fillH = true;
        break;
        case pLaplace:
            ops[pLaplace] = 1;
            fillH = true;
        break;
        case Rotates:
            QString angle = ui->GetAngle->text();
            ro += angle.toInt();
            ops[Rotates] = 1;
            fillH = false;
        break;
    }
    undo = op;
    DoOperations();
}

void MainWindow::DoOperations(){

    Iundo = SaveS;
    Image = SaveH = Img;
    info.width = Sw; info.height = Sh;
    fillHist();

    if(bolS || bolA){
        Image = SaveS;
    }
    for(int i = 0; i < 20; i++){
        if(ops[i]){
            switch(i){
                case Umbra:
                    Umbral();
                    SaveH = Image;
                break;
                case Negative:
                    Negativo();
                    SaveH = Image;
                break;
                case Eq:
                    Ecualizar();
                    SaveH = Image;
                break;
                case MirrorX:
                    EspejoX();
                break;
                case MirrorY:
                    EspejoY();
                break;
                case Zoom:
                    zoom = 1 + AzoomI - AzoomO;
                    if(zoom == 0){
                        AzoomO -= 0.25;
                        zoom += 0.25;
                    }else if(zoom == 3){
                        AzoomI -= 0.25;
                        zoom -= 0.25;
                    }

                    ZoomNN(zoom);
                break;
                case Escal:
                    if(ancho != info.width || altura != info.height)
                        Escals();
                break;
                case Ilumination:
                    vIlumination(Ailumination);
                    SaveH = Image;
                break;
                case Contrast:
                    if(contrast != 1){
                        Contraste();
                        SaveH = Image;
                    }
                break;
                case Sgaus:
                    if(info.bits == 24){
                        Suavizado_Gausiano();
                        SaveH = Image;
                    }
                break;
                case Smedia:
                    if(info.bits == 24){
                        Suavizado_Media();
                        SaveH = Image;
                    }
                break;
                case bLaplace:
                    if(info.bits == 24){
                       Borde_Laplaciano();
                       SaveH = Image;
                    }
                break;
                case bPrewitt:
                    if(info.bits == 24){
                        Buscar_Bordes(2);
                        SaveH = Image;
                    }
                break;
                case bSobel:
                    if(info.bits == 24){
                        Buscar_Bordes(1);
                        SaveH = Image;
                    }
                break;
                case pLaplace:
                    if(info.bits == 24){
                        Perfilado_Laplaciano();
                        SaveH = Image;
                    }
                break;
                case Rotates:
                    Rotate(ro);
                break;
            }
        }
    }
    fillHist();
    hist.LoadHist(r, g, b, Image);
    ui->MyLabel->setPixmap(QPixmap::fromImage(Image));
}

void MainWindow::Negativo(){

    if(info.bits == 24){
        for(int r=0; r < Image.width(); r++){
            for(int c=0; c < Image.height(); c++){
                QColor clrCurrent(Image.pixel(r, c));
                QRgb value = qRgb(255 - clrCurrent.red(), 255 - clrCurrent.green() , 255 - clrCurrent.blue());
                Image.setPixel(r, c, value);
            }
        }
    }else if(info.bits < 24){
        for(unsigned int i = 0; i < info.ncolours; i++){
            QColor clrCurrent(Image.color(i));
            QRgb value = qRgb(255 - clrCurrent.red(), 255 - clrCurrent.green(), 255 - clrCurrent.blue());
            Image.setColor(i, value);
        }
    }
}

void MainWindow::Ecualizar(){
    int eqB[256], eqG[256], eqR[256];
    int iAcumB = b[0], iAcumG = g[0], iAcumR = r[0];

    eqB[0] = 0; eqG[0] = 0; eqR[0] = 0;
    eqB[255] = 255; eqG[255] = 255; eqR[255] = 255;

    for(int i=1; i<255; i++){
        eqB[i] = (iAcumB * 255) / NM;
        iAcumB += b[i];

        eqG[i] = (iAcumG * 255) / NM;
        iAcumG += g[i];

        eqR[i] = (iAcumR * 255) / NM;
        iAcumR += r[i];
    }

    for(int row=0; row < Image.width(); row++){
        for(int c=0; c < Image.height(); c++){
            QColor clrCurrent(Image.pixel(row, c));
            QRgb value = qRgb(eqR[clrCurrent.red()], eqG[clrCurrent.green()] ,eqB[clrCurrent.blue()]);
            Image.setPixel(row, c, value);
        }
    }
}

void MainWindow::EspejoX(){
    int t;
    QImage Img2(info.width, info.height, QImage::Format_RGB888);

    t=info.width-1;

    for(unsigned int i=0; i<info.width; i++){
        for(unsigned int j=0; j<info.height; j++)
            Img2.setPixel(t,j,Image.pixel(i,j));
        t--;
    }
    Image = Img2;
}

void MainWindow::EspejoY(){
    int t;
    QImage Img2(info.width, info.height, QImage::Format_RGB888);

        for(unsigned int i=0; i<info.width; i++){
            t=info.height-1;
            for(unsigned int j=0; j<info.height; j++){
                Img2.setPixel(i,t, Image.pixel(i,j));
                    t--;
                }
              }
    Image = Img2;
}

void MainWindow::Escals(){
    if(ancho==0 || altura == 0){
            //mensaje de error
            exit(0);
        }

        int oheight = info.height, owidth = info.width;
        int nheight = altura;
        int nwidth = ancho;

        QImage Img2(nwidth, nheight, QImage::Format_RGB888);

        if(!ui->Bilinear->isChecked()) { // -  - - - - - - - - - VECINOS CERCANOS

            double x_ratio = owidth/(double)nwidth, y_ratio = oheight/(double)nheight, px, py;

            for(int i=0; i<nwidth; i++) {
                for(int j=0; j<nheight; j++){

                    px = (int)(i*x_ratio);
                    py = (int)(j*y_ratio);

                    Img2.setPixel(i,j,Image.pixel(px,py));
                }
            }

        } else{  // -  - - - - - - - - - BILINEAR

            double x_ratio = (owidth-1)/(double)nwidth, y_ratio = (oheight-1)/(double)nheight;
            int x, y, x_diff, y_diff;//indexa, indexb, indexc, indexd;

            for(int i=0; i<nwidth; i++) {
                for(int j=0; j<nheight; j++){

                    x = (int)(x_ratio * i);
                    y = (int)(y_ratio * j);
                    x_diff = (x_ratio * i) - x;
                    y_diff = (y_ratio * j) - y;


                    // Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + D(wh)
                    QColor clrPixelA(Image.pixel(x, y));
                    QColor clrPixelB(Image.pixel(x+1, y));
                    QColor clrPixelC(Image.pixel(x, y+1));
                    QColor clrPixelD(Image.pixel(x+1, y+1));

                    QRgb value = qRgb(
                                clamp(clrPixelA.red()*(1-x_diff)*(1-y_diff) + clrPixelB.red()*(x_diff)*(1-y_diff) + clrPixelC.red()*(y_diff)*(1-x_diff) + clrPixelD.red()*(x_diff*y_diff)) ,
                                clamp(clrPixelA.green()*(1-x_diff)*(1-y_diff) + clrPixelB.green()*(x_diff)*(1-y_diff) + clrPixelC.green()*(y_diff)*(1-x_diff) + clrPixelD.green()*(x_diff*y_diff)),
                                clamp(clrPixelA.blue()*(1-x_diff)*(1-y_diff) + clrPixelB.blue()*(x_diff)*(1-y_diff) + clrPixelC.blue()*(y_diff)*(1-x_diff) + clrPixelD.blue()*(x_diff*y_diff))
                               );

                    Img2.setPixel(i,j,value);
                }
            }

        }


        info.width = nwidth;
        info.height = nheight;
        Image = Img2;

        // DATOS DE LA IMAGEN

                ui->info_ancho->setText(QString::number(Image.width()));
                ui->info_alto->setText(QString::number(Image.height()));
                ui->info_bits->setText(QString::number(info.bits));

                if(info.bits == 24){

                    ui->info_tam->setText(QString::number(((Image.width() * Image.height() * 3)+54)));
                }else if (info.bits == 8){
                    ui->info_tam->setText(QString::number(((Image.width() * Image.height() )+54)+  (info.ncolours * 4)));
                }else if (info.bits == 4) {
                    ui->info_tam->setText(QString::number((((Image.width() * Image.height())/2 )+54)+  (info.ncolours * 4)));
                }else{
                    ui->info_tam->setText(QString::number((((Image.width() * Image.height())/8 )+54)+  (info.ncolours * 4)));
                }
}

void MainWindow::vIlumination(int light){
    QImage Img2(info.width, info.height, QImage::Format_RGB888);

        if(info.bits == 24){
            for(int r=0; r < Image.width(); r++){
                for(int c=0; c < Image.height(); c++){
                    QColor clrCurrent(Image.pixel(r, c));
                    QRgb value = qRgb(clamp(clrCurrent.red() + light), clamp(clrCurrent.green() + light) ,clamp(clrCurrent.blue() + light));
                    Img2.setPixel(r, c, value);
                }
            }
        }else if(info.bits < 24){
            for(unsigned int i = 0; i < info.ncolours; i++){
                QColor clrCurrent(Image.color(i));
                QRgb value = qRgb(clamp(light + clrCurrent.red()), clamp(light + clrCurrent.green()), clamp(light + clrCurrent.blue() ));
                Img2.setColor(i, value);
            }
        }
        Image = Img2;
}

void MainWindow::Contraste(){
    QImage Img2(info.width, info.height, QImage::Format_RGB888);

    if(info.bits == 24){
        for(int r=0; r < Image.width(); r++){
            for(int c=0; c < Image.height(); c++){
                QColor clrCurrent(Image.pixel(r, c));
                QRgb value = qRgb(clamp((contrast * (clrCurrent.red() - 128)) + 128), clamp((contrast * (clrCurrent.green() - 128)) + 128) ,clamp((contrast * (clrCurrent.blue() - 128)) + 128));
                Img2.setPixel(r, c, value);
            }
        }
    }else if(info.bits < 24){
        for(unsigned int i = 0; i < info.ncolours; i++){
            QColor clrCurrent(Image.color(i));
            QRgb value = qRgb(clamp((contrast * (clrCurrent.red() - 128)) + 128), clamp((contrast * (clrCurrent.green() - 128)) + 128) ,clamp((contrast * (clrCurrent.blue() - 128)) + 128));
            Img2.setColor(i, value);
        }
    }
    Image = Img2;
}

void MainWindow::Umbral(){
    if(umbral >= 0){
        QImage Img2(info.width, info.height, QImage::Format_RGB888);
        int aux;
        if(info.bits == 24){
            for(int r=0; r < Image.width(); r++){
                for(int c=0; c < Image.height(); c++){
                    QColor clrCurrent(Image.pixel(r, c));
                    aux =(clrCurrent.red() + clrCurrent.green() + clrCurrent.blue())/3;
                    if(aux > umbral){
                        aux = 255;
                    }else{
                        aux = 0;
                    }
                    QRgb value = qRgb(aux,aux,aux);
                    Img2.setPixel(r, c, value);
                }
            }
        }else if(info.bits < 24){
            for(unsigned int i = 0; i < info.ncolours; i++){
                QColor clrCurrent(Image.color(i));
                aux =(clrCurrent.red() + clrCurrent.green() + clrCurrent.blue())/3;
                if(aux > umbral){
                    aux = 255;
                }else{
                    aux = 0;
                }
                QRgb value = qRgb(aux,aux,aux);
                Img2.setColor(i, value);
            }
        }
        Image = Img2;
    }
}

void MainWindow::Rotate(int Angle){

    QImage Rotate(resolution, resolution, QImage::Format_RGB888);

    int im, resto = Angle;

    while(resto >= 90){
        int N[4]; N[0]= resto/360, N[1] = resto/270, N[2] = resto/180; N[3] = resto/90;
        int menor = 999999;

        for(int i = 0; i < 4; i++){
           if(N[i] < menor && N[i] != 0){
               menor = N[i];
               im = i;
           }
        }

        if(im == 0){

            resto -= 360*menor;
        }else if(im == 1){
                Rotate270();
            resto -= 270*menor;
        }else if(im == 2){
                 Rotate180();
             resto -= 180*menor;
        }else if(im == 3){
                 Rotate90();
             resto -= 90*menor;
        }
    }
    if(resto > 0){

        Angle = resto;
        double seno = sin((Angle*3.1416)/180.0);
        double tang = tan(((Angle/2)*3.1416)/180.0);

        int x0 = (int)(floor(info.width/2));
        int y0 = (int)(floor(info.height/2));
        int x2 = 0;
        int y2 = 0;

        for(int i = 0; i < info.width; i++){
            for(int j = 0; j < info.height; j++){
                x2 = (int)((i-x0) - tang*(y0-j));
                y2 = (int)(y0-j);

                y2 = (int)((seno*x2) + y2);

                x2 = (int)((x2) - tang*(y2))+((resolution/2));
                y2 = (int)(-y2)+((resolution/2));

                if(x2 < resolution && y2 < resolution && x2 > -1 && y2 > -1){
                    Rotate.setPixel(x2, y2, Image.pixel(i, j));
                }
                x2 = 0;
                y2 = 0;
            }
        }
        Image = Rotate;
    }
}

void MainWindow::on_pushButton_clicked()  //NEGATIVO
{
    SendOp(Negative);
}

void MainWindow::on_bRotate_clicked()   //ROTACION EN SENTIDO HORARIO
{
    SendOp(Rotates);
}

void MainWindow::on_Equal_clicked()
{
    SendOp(Eq);
}

void MainWindow::on_bHist_clicked()
{
   fillHist();
   hist.LoadHist(r, g, b, Image);
   hist.show();
}

void MainWindow::on_bZoomOut_clicked()
{
    AzoomO += 0.25;
    SendOp(Zoom);
}

void MainWindow::on_bZoomIn_clicked()
{
    AzoomI += 0.25;
    SendOp(Zoom);
}

void MainWindow::on_bMirrorX_clicked()
{
    SendOp(MirrorX);
}

void MainWindow::on_bMirrorY_clicked()
{
    SendOp(MirrorY);
}

void MainWindow::on_sIuminacion_valueChanged(int light)
{
    Ailumination =+ light;
    SendOp(Ilumination);
}

void MainWindow::on_sbContraste_valueChanged(double cont)
{
    contrast = cont;
    SendOp(Contrast);
}

void MainWindow::on_sUmbral_valueChanged(int umbra)
{
    umbral = umbra;
    SendOp(Umbra);
}

void MainWindow::on_bEscalar_clicked()
{
    Ualto = altura; Uancho = ancho;
    ancho = (ui->Ancho->text()).toInt();
    altura = (ui->Altura->text()).toInt();
    SendOp(Escal);
}

void MainWindow::on_action3x3_triggered()
{
    bolS = false;
    t = 3;
    SendOp(Smedia);
}

void MainWindow::on_action5x5_triggered()
{
    bolS = false;
    t = 5;
    SendOp(Smedia);
}

void MainWindow::on_action7x7_triggered()
{
    bolS = false;
    t = 7;
    SendOp(Smedia);
}

void MainWindow::on_action9x9_triggered()
{
    bolS = false;
    t = 9;
    SendOp(Smedia);
}

void MainWindow::on_action11x11_triggered()
{
    bolS = false;
    t = 11;
    SendOp(Smedia);
}

void MainWindow::on_action13x13_triggered()
{
    bolS = false;
    t = 13;
    SendOp(Smedia);
}

void MainWindow::on_action15x15_triggered()
{
    bolS = false;
    t = 15;
    SendOp(Smedia);
}

void MainWindow::on_bReset_clicked()
{
    Image = SaveS = SaveH = Img;
    ro = 0;
    AzoomI = 0; AzoomO = 0; Ailumination = 0; umbral = 0;
    Img = Image; contrast = 0.0; bolS = bolA = false; undo = redo = -1;
    for(int i = 0; i < 13; i++)
        ops[i] = 0;
    info.width = Sw; info.height = Sh;
    fillHist(); hist.LoadHist(r, g, b, Image);
    ui->MyLabel->setPixmap(QPixmap::fromImage(Image));
}

void MainWindow::on_action3x3_2_triggered()
{
    bolS = false;
    tam = 3;
    SendOp(Sgaus);
}

void MainWindow::on_action5x5_2_triggered()
{
    bolS = false;
    tam = 5;
    SendOp(Sgaus);
}

void MainWindow::on_action7x7_2_triggered()
{
    bolS = false;
    tam = 7;
    SendOp(Sgaus);
}

void MainWindow::on_action9x9_2_triggered()
{
    bolS = false;
    tam = 9;
    SendOp(Sgaus);
}

void MainWindow::on_action11x11_2_triggered()
{
    bolS = false;
    tam = 11;
    SendOp(Sgaus);
}

void MainWindow::on_action13x13_2_triggered()
{
    bolS = false;
    tam = 13;
    SendOp(Sgaus);
}

void MainWindow::on_action15x15_2_triggered()
{
    bolS = false;
    tam = 15;
    SendOp(Sgaus);
}

void MainWindow::on_action3x3_3_triggered()
{
    bolS = false;
    tam = 3;
    SendOp(bLaplace);
}

void MainWindow::on_action5x5_3_triggered()
{
    bolS = false;
    tam = 5;
    SendOp(bLaplace);
}

void MainWindow::on_action7x7_3_triggered()
{
    bolS = false;
    tam = 7;
    SendOp(bLaplace);
}

void MainWindow::on_action9x9_3_triggered()
{
    bolS = false;
    tam = 9;
    SendOp(bLaplace);
}

void MainWindow::on_action11x11_3_triggered()
{
    bolS = false;
    tam = 11;
    SendOp(bLaplace);
}

void MainWindow::on_action13x13_3_triggered()
{
    bolS = false;
    tam = 13;
    SendOp(bLaplace);
}

void MainWindow::on_action15x15_3_triggered()
{
    bolS = false;
    tam = 15;
    SendOp(bLaplace);
}

void MainWindow::on_action3x3_4_triggered()
{
    bolS = false;
    tam = 3;
    SendOp(pLaplace);
}

void MainWindow::on_action5x5_4_triggered()
{
    bolS = false;
    tam = 5;
    SendOp(pLaplace);
}

void MainWindow::on_action7x7_4_triggered()
{
    bolS = false;
    tam = 7;
    SendOp(pLaplace);
}

void MainWindow::on_action9x9_4_triggered()
{
    bolS = false;
    tam = 9;
    SendOp(pLaplace);
}

void MainWindow::on_action11x11_4_triggered()
{
    bolS = false;
    tam = 11;
    SendOp(pLaplace);
}

void MainWindow::on_action13x13_4_triggered()
{
    bolS = false;
    tam = 13;
    SendOp(pLaplace);
}

void MainWindow::on_action15x15_4_triggered()
{
    bolS = false;
    tam = 15;
    SendOp(pLaplace);
}

void MainWindow::on_bUndo_clicked()
{
    if(undo >= 0){
        if(undo == Sgaus || undo == Smedia || undo == bLaplace || undo == bPrewitt || undo == bSobel || undo == pLaplace || undo == Filtro){
            Iredo = SaveS;
            SaveS = Iundo;
            ops[undo] = 0;
        }else
            ops[undo] = !ops[undo];
        redo = undo;
        if(!ops[Sgaus] && !ops[Smedia] && !ops[bLaplace] && !ops[bPrewitt] && !ops[bSobel] && !ops[pLaplace] && !ops[Filtro])
            bolA = false;
        DoOperations();
    }else{
        std::cout << "No hay nada que deshacer" << std::endl;
    }
    undo = -1;
}

void MainWindow::on_bRehacer_clicked()
{
    if(redo >= 0){
        if(redo == Sgaus || redo == Smedia || redo == bLaplace || redo == bPrewitt || redo == bSobel || redo == pLaplace || redo == Filtro){
            SaveS = Iredo;
            Iundo = Iredo;
            ops[redo] = 0;
            bolA = true;
        }else
            ops[redo] = !ops[redo];
        undo = redo;
        DoOperations();
    }else{
        std::cout << "No hay nada que rehacer" << std::endl;
    }
    redo = -1;
}

void MainWindow::on_actionCrear_Filtro_triggered()
{
    if(info.bits == 24){
        int res;
        CustomMask v;
        v.setWindowTitle("Crear Filtro");
        res = v.exec();
        if(res == QDialog::Rejected)
            return;

        //
        for (int k = 0; k < v.tam; k++){
            for (int c = 0; c < v.tam; c++){
                filtro[k][c] = v.getT(k, c);

            }
        }

        QImage Img;
        Img = Image;
        int x, y, Ix, Iy;
        unsigned int PosX, PosY;
        long long int PromR, PromG, PromB, T;
        PromR = PromG = PromB = 0;
        T = v.tam - 1;
        x = v.cX; y = v.cY;

        for(int i = 0; i < Image.width(); i++){
            for(int j = 0; j < Image.height(); j++){
                Iy = 0;
                for(int m = -x; m <= T-x; m++){
                    Ix = 0;
                    for(int n = -y; n <= T-y; n++){
                        PosX = i + m; PosY = j + n;
                        if(PosX >= 0 && PosY >= 0 && PosX < info.width && PosY < info.height){
                           QColor clrCurrent(Img.pixel(PosX, PosY));
                           PromR += clrCurrent.red()*filtro[Ix][Iy]; PromG += clrCurrent.green()*filtro[Ix][Iy]; PromB += clrCurrent.blue()*filtro[Ix][Iy];
                        }
                        Ix++;
                    }
                    Iy++;
                }
                //PromR = PromR/div; PromG = PromG/div; PromB = PromB/div;
                Image.setPixel(i, j, qRgb(clamp(PromR), clamp(PromG), clamp(PromB)));
                PromR = PromG = PromB = 0;
            }
        }
        SaveS = Image;
        //bolS = false;
        bolA = true;
        //ops[Smedia] = 1;
        fillH = true;
        undo = Filtro;
        DoOperations();
    }
}

void MainWindow::on_action3x3_5_triggered()
{
    bolS = false;
    t = 3;
    SendOp(bPrewitt);
}


void MainWindow::on_action5x5_5_triggered()
{
    bolS = false;
    t = 5;
    SendOp(bPrewitt);
}

void MainWindow::on_action7x7_5_triggered()
{
    bolS = false;
    t = 7;
    SendOp(bPrewitt);
}

void MainWindow::on_action9x9_5_triggered()
{
    bolS = false;
    t = 9;
    SendOp(bPrewitt);
}

void MainWindow::on_action11x11_5_triggered()
{
    bolS = false;
    t = 11;
    SendOp(bPrewitt);
}

void MainWindow::on_action13x13_5_triggered()
{
    bolS = false;
    t = 13;
    SendOp(bPrewitt);
}

void MainWindow::on_action15x15_5_triggered()
{
    bolS = false;
    t = 15;
    SendOp(bPrewitt);
}

void MainWindow::on_action3x3_6_triggered()
{
    bolS = false;
    t = 3;
    SendOp(bSobel);
}

void MainWindow::on_action5x5_6_triggered()
{
    bolS = false;
    t = 5;
    SendOp(bSobel);
}
