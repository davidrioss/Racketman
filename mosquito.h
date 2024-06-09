#pragma once

#include "mapa.h"

#include <random>
#include <set>

class Mosquito{
	public:
	int x, y;
	int dx, dy, dir;
	int velocidade;
	int tipo;
	int espera;
	int frame;
	bool espelha;
	bool movendo;

	Mosquito(int velocidade, int tipo, Mapa &mapa) : velocidade(velocidade), tipo(1){
		do{
			x = (rand() % (mapa.colunas - 3)) + 2;
			y = (rand() % (mapa.linhas - 3)) + 2;
		}while(x + y < 5 || mapa.getPos(x, y) & (BLOCO | MOSQUITO));
		mapa.grid[x][y] = MOSQUITO;
		
		setaDirecao(rand() % 4);

		x *= FPS;
		y *= FPS;
		movendo = false;
		espera = 0;
		espelha = false;
		frame = rand();
	}

	void setaDirecao(int d){
		if(d < 0 || d > 3){
			printf("ERROR\n");
			exit(1);
		}
		dir = d;
		dx = dirs[dir][0];
		dy = dirs[dir][1];
	}

	bool proximaDirecao0(int &i, int &j, Mapa &mapa){
		if(!(mapa.getPos(i + dx, j + dy) & (BLOCO | PAREDE | RAQUETE | MOSQUITO)))
			return true;

		if(movendo){
			setaDirecao((dir + 2) % 4);
			espera = FPS;
			movendo = false;
			return false;
		}

		for(int k = 0; k < 4; k++){
			if(!(mapa.getPos(i + dirs[k][0], j + dirs[k][1]) & (BLOCO | PAREDE | RAQUETE | MOSQUITO))){
				setaDirecao(k);
				return true;
			}
		}

		return false;
	}

	bool proximaDirecao1(int &i, int &j, Mapa &mapa){
		printf("proximaDirecao1 %d %d\n", i, j);
		if(!movendo && !(mapa.getPos(i + dx, j + dy) & (BLOCO | PAREDE | RAQUETE | MOSQUITO)))
			return true;
		
		std::set<int> dirsLivres;
		for(int k = 0; k < 4; k++)
			if(!(mapa.getPos(i + dirs[k][0], j + dirs[k][1]) & (BLOCO | PAREDE | RAQUETE | MOSQUITO)))
				dirsLivres.insert(k);
		
		if(dirsLivres.empty()){
			if(movendo){
				setaDirecao(rand() % 4);
				movendo = false;
				espera = FPS;
			}

			return false;
		}

		if(movendo){
			dirsLivres.erase((dir + 2) % 4);
			if(dirsLivres.empty()){
				dir = (dir + 2) % 4;
				dx *= -1;
				dy *= -1;
				espera = FPS;
				movendo = false;
				return false;
			}
		}
		auto it = dirsLivres.begin();
		advance(it, rand() % dirsLivres.size());
		int nd = *it;
		if(nd == dir)
			return true;
		
		setaDirecao(nd);
		if(movendo){
			espera = FPS;
			movendo = false;
		}
	}

	void mover(Mapa &mapa){
		frame++;
		if(espera > 0){
			espera -= velocidades[velocidade];
			return;
		}

		if(inteiro(x) && inteiro(y)){
			int i = x / FPS, j = y / FPS;
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
				break;
			default:
				break;
			}
			
			if(!mover)
				return;
			printf("SET %d %d\n", i + dx, j + dy);
			mapa.setPos(i + dx, j + dy, MOSQUITO);
		}

		if(dx > 0)
			espelha = false;
		else if(dx < 0)
			espelha = true;
		movendo = true;
		x += velocidades[velocidade] * dx;
		y += velocidades[velocidade] * dy;
		printf("%.3lf %.3lf\n", x / 60.0, y / 60.0);
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