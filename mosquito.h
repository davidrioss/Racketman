#pragma once

#include "mapa.h"

#include <random>
#include <GL/glut.h>

class Mosquito{
	public:
	int x, y;
	int dx, dy;
	int velocidade;
	int tipo;

	Mosquito(int velocidade, int tipo, Mapa &mapa) : velocidade(velocidade), tipo(tipo){
		do{
			x = (rand() % (mapa.linhas - 3)) + 2;
			y = (rand() % (mapa.colunas - 3)) + 2;
		}while(mapa.getPos(x, y));
		
		dx = 1;
		dy = 0;

		x *= FPS;
		y *= FPS;
	}

	void mover(){
		//verifica bloqueio na horizontal 
		// if((mapa[mosquito[n][1]+1][mosquito[n][2]]!=0 && mapa[mosquito[n][1]-1][mosquito[n][2]]==0) || (mapa[mosquito[n][1]+1][mosquito[n][2]]==0 && mapa[mosquito[n][1]-1][mosquito[n][2]]!=0))
		// mosquito[n][3] = -mosquito[n][3];

		// //verifica bloqueio na vertical
		// if((mapa[mosquito[n][1]][mosquito[n][2]+1]==0 && mapa[mosquito[n][1]][mosquito[n][2]-1])!=0 || (mapa[mosquito[n][1]][mosquito[n][2]+1]!=0 && mapa[mosquito[n][1]][mosquito[n][2]-1]==0))
		// mosquito[n][4] = -mosquito[n][4];

		// //inverte eixo de movidentação do mosquito se eixo bloqueado
		// if((mapa[mosquito[n][1]+1][mosquito[n][2]] && mapa[mosquito[n][1]-1][mosquito[n][2]] && mosquito[n][3]) || (mapa[mosquito[n][1]][mosquito[n][2]+1]!=0 && mapa[mosquito[n][1]][mosquito[n][2]-1]!=0 && mosquito[n][4])){
		// 	int aux = mosquito[n][3];
		// 	mosquito[n][3] = mosquito[n][4];
		// 	mosquito[n][4] = aux;
		// } 

		// //movimenta mosquito
		// if(mosquito[n][3] && (mapa[mosquito[n][1] + mosquito[n][3]][mosquito[n][2]]==0)){
		// 	mapa[mosquito[n][1]][mosquito[n][2]]=0;
		// 	mosquito[n][1] += mosquito[n][3];
		// 	mapa[mosquito[n][1]][mosquito[n][2]]=4;
		// 	if(playerX==mosquito[n][1] && playerY==mosquito[n][2])
		// 		Morreu();
		// }
		// else{
		// 	if(mosquito[n][4] && (mapa[mosquito[n][1]][mosquito[n][2] + mosquito[n][4]]==0)){
		// 		mapa[mosquito[n][1]][mosquito[n][2]]=0;
		// 		mosquito[n][2] += mosquito[n][4];
		// 		mapa[mosquito[n][1]][mosquito[n][2]]=4;
		// 		if(playerX==mosquito[n][1] && playerY==mosquito[n][2])
		// 			Morreu();
		// 	}
		// }
		// if(abs(x - (int) x) < 1e-4 && abs(y - (int) y) < 1e-4){
		// 	switch (tipo)
		// 	{
		// 	case 0:
		// 		dx *= -1;
		// 		dy *= -1;
		// 		return;
		// 	default:
		// 		return;
		// 	}
		// }

		x += velocidade * dx;
		y += velocidade * dy;
	}

    void desenha(){
		float X = (float) x / FPS;
		float Y = (float) y / FPS;
        glColor3f(0.0f,0.0f,0.0f); //corpo
        glRectf(X+0.1, Y+0.3, X+0.25, Y+0.5);
        glRectf(X+0.25, Y+0.3, X+0.5, Y+0.6);
        glRectf(X+0.5, Y+0.35, X+1, Y+0.55);
        glRectf(X, Y, X+0.1, Y+0.4);
        glRectf(X+0.25, Y+0.1, X+0.3, Y+0.3);
        glRectf(X+0.35, Y+0.1, X+0.4, Y+0.3);
        glRectf(X+0.45, Y+0.1, X+0.5, Y+0.3);
        glRectf(X+0.55, Y+0.15, X+0.6, Y+0.35);
        glRectf(X+0.65, Y+0.15, X+0.7, Y+0.35);
        glRectf(X+0.75, Y+0.15, X+0.8, Y+0.35);
        glColor3f(1.0f,1.0f,1.0f); //listras
        glRectf(X+0.6, Y+0.35, X+0.65, Y+0.55);
        glRectf(X+0.75, Y+0.35, X+0.8, Y+0.55);
        glRectf(X+0.9, Y+0.35, X+0.95, Y+0.55);
        glColor3f(1.0f,0.0f,0.0f); //olho
        glRectf(X+0.1, Y+0.4, X+0.15, Y+0.48);
        glColor3f(0.678400f,0.917600f,0.917600f); //asas
        glRectf(X+0.1, Y+0.6, X+0.4, Y+1.0);
        glRectf(X+0.5, Y+0.6, X+0.95, Y+0.85);	
    }
};