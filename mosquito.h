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
	bool espelha;
	bool movendo;

	Mosquito(int velocidade, int tipo, Mapa &mapa) : velocidade(velocidade), tipo(tipo){
		do{
			x = (rand() % (mapa.colunas - 3)) + 2;
			y = (rand() % (mapa.linhas - 3)) + 2;
		}while(x + y < 5 || mapa.getPos(x, y) & (BLOCO | MOSQUITO));
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
		espera = 0;
		espelha = false;
		frame = rand();
	}

	inline bool proximaDirecao0(int &i, int &j, Mapa &mapa){
		if(!(mapa.getPos(i + dx, j + dy) & (BLOCO | PAREDE | RAQUETE | MOSQUITO)))
			return true;

		if(movendo){
			dx *= -1;
			dy *= -1;
			espera = FPS;
			movendo = false;
			return false;
		}

		for(int k = 0; k < 4; k++){
			if(!(mapa.getPos(i + dirs[k][0], j + dirs[k][1]) & (BLOCO | PAREDE | RAQUETE | MOSQUITO))){
				dx = dirs[k][0];
				dy = dirs[k][1];
				return true;
			}
		}

		return false;
	}

	inline bool proximaDirecao1(int &i, int &j, Mapa &mapa){

	}

	void mover(Mapa &mapa){
		frame++;
		if(espera > 0){
			espera -= velocidades[velocidade];
			return;
		}

		if(inteiro(x) && inteiro(y)){
			int i = (float) x / FPS, j = (float) y / FPS;
			if(movendo)
				mapa.removePos(i - dx, j - dy, MOSQUITO);
			
			bool mover;
			switch (tipo)
			{
			case 0:
				mover = proximaDirecao0(i, j, mapa);
				break;
			case 1:
				mover = proximaDirecao1(i, j, mapa);
			default:
				break;
			}
			
			if(!mover)
				return;
			mapa.setPos(i + dx, j + dy, MOSQUITO);
		}

		if(dx > 0)
			espelha = false;
		else if(dx < 0)
			espelha = true;
		movendo = true;
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
		float Y = (float) y / FPS + sin(frame / 5) * 0.05;
		int t = T_MOSQUITO + (frame % 10) / 5;
		if(espelha)
			desenhaTexturaEspelhado(t, X, Y, X + 1, Y + 1);
		else
			desenhaTextura(t, X, Y, X + 1, Y + 1);
    }
};