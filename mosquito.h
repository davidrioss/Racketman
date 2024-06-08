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
	int espera;
	int frame;
	int frameCaminhando;
	bool movendo;

	Mosquito(int velocidade, int tipo, Mapa &mapa) : velocidade(velocidade), tipo(tipo){
		do{
			x = (rand() % (mapa.linhas - 3)) + 2;
			y = (rand() % (mapa.colunas - 3)) + 2;
		}while(x + y < 5 || mapa.getPos(x, y) & BLOCO);
		mapa.grid[x][y] = MOSQUITO;
		
		if(mapa.getPos(x + 1, y) & BLOCO && mapa.getPos(x - 1, y)){
			dx = 0;
			dy = 1;
		}else if(mapa.getPos(x, y + 1) & BLOCO && mapa.getPos(x, y - 1)){
			dx = 1;
			dy = 0;
		}else{
			dx = rand() % 2;
			dy = !dx;
		}

		x *= FPS;
		y *= FPS;
		movendo = false;
		frameCaminhando = frame = espera = 0;
	}

	void mover(Mapa &mapa){
		frame++;
		if(espera){
			espera -= velocidades[velocidade];
			return;
		}

		if(inteiro(x) && inteiro(y)){
			int i = (float) x / FPS, j = (float) y / FPS;
			if(movendo)
				mapa.removePos(i - dx, j - dy, MOSQUITO);

			if(mapa.getPos(i + dx, j + dy) & (BLOCO | PAREDE | RAQUETE | MOSQUITO)){
				dx *= -1;
				dy *= -1;
				if(movendo){
					espera = FPS;
					movendo = false;
				}else{
					for(int k = 0; k < 4; k++){
						if(!(mapa.getPos(i + dirs[k][0], j + dirs[k][1]) & (BLOCO | PAREDE | RAQUETE | MOSQUITO))){
							dx = dirs[k][0];
							dy = dirs[k][1];
							break;
						}
					}
				}
				return;
			}

			mapa.setPos(i + dx, j + dy, MOSQUITO);
		}

		movendo = true;
		frameCaminhando++;
		x += velocidades[velocidade] * dx;
		y += velocidades[velocidade] * dy;
	}

	void matar(Mapa &mapa){
		int i = x / FPS, j = y / FPS;
		mapa.removePos(i, j, MOSQUITO);
		if(!inteiro(x))
			mapa.removePos(i + 1, j, MOSQUITO);
		if(!inteiro(y))
			mapa.removePos(i, j + 1, MOSQUITO);
	}

    void desenha(){
		float X = (float) x / FPS;
		float Y = (float) y / FPS;
        glColor3f(0.0f,0.0f,0.0f); //corpo
        glRectf(X+0.1, Y+0.3, X+0.25, Y+0.5);
        glRectf(X+0.25, Y+0.3, X+0.5, Y+0.6);
        glRectf(X+0.5, Y+0.35, X+1, Y+0.55);
        glRectf(X, Y + 0.03 + sin(frame/6.0) * 0.02, X+0.1, Y+0.4);
		float pe = Y + 0.12 + sin(frameCaminhando / 5) * 0.02;
		float pd = Y + 0.17 + cos(frameCaminhando / 5) * 0.02;
        glRectf(X+0.25, pe, X+0.3, Y + 0.30);
        glRectf(X+0.35, pe, X+0.4, Y + 0.30);
        glRectf(X+0.45, pe, X+0.5, Y + 0.30);
        glRectf(X+0.55, pd, X+0.6, Y + 0.35);
        glRectf(X+0.65, pd, X+0.7, Y + 0.35);
        glRectf(X+0.75, pd, X+0.8, Y + 0.35);
        glColor3f(1.0f,1.0f,1.0f); //listras
        glRectf(X+0.6, Y+0.35, X+0.65, Y+0.55);
        glRectf(X+0.75, Y+0.35, X+0.8, Y+0.55);
        glRectf(X+0.9, Y+0.35, X+0.95, Y+0.55);
        glColor3f(1.0f,0.0f,0.0f); //olho
        glRectf(X+0.1, Y+0.4, X+0.15, Y+0.48);
        glColor3f(0.678400f,0.917600f,0.917600f); //asas
        glRectf(X+0.1, Y+0.6, X+0.40, Y+0.95+sin(frame/2.0)*0.05);
        glRectf(X+0.5, Y+0.6, X+0.95, Y+0.80+cos(frame/2.0)*0.05);	
    }
};