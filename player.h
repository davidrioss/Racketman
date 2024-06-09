#pragma once

#include "raquete.h"
#include "mapa.h"
#include "utils.h"

#include <list>

class Player{
	public:
	int x;
	int y;
	int dx;
	int dy;
	int velocidade;
    int potenciaDaRaquete;
    int numRaquetes;
	int vidas;
	bool vivo;
	bool espelha = true;

	Player(){
        x = y = FPS;
		potenciaDaRaquete = numRaquetes = 1;
		velocidade = 1;
		dx = dy = 0;
		vidas = 3;
		vivo = true;
	}

	void setaPos(int nx, int ny){
		x = nx * FPS;
		y = ny * FPS;
		dx = 0;
		dy = 0;
		vivo = true;
	}

	bool movendo(){
		return dx || dy;
	}

	void colocaRaquete(std::list<Raquete> &raquetes, Mapa &mapa){
		int i = x / FPS, j = y / FPS;
		if(!vivo || movendo() || raquetes.size() == numRaquetes || mapa.getPos(i, j) & RAQUETE)
			return;
		
		mapa.setPos(i, j, RAQUETE);
		raquetes.emplace_back(i, j, potenciaDaRaquete, 3 * FPS);
	}

	void comecaMovimento(int ndx, int ndy, Mapa mapa){
		int i = x / FPS, j = y / FPS;
        if(movendo() || mapa.getPos(i + ndx, j + ndy) & (BLOCO | PAREDE | RAQUETE))
            return;
		
		if(ndx > 0)
			espelha = true;
		else if(ndx < 0)
			espelha = false;
		dx = ndx;
		dy = ndy;
	}

	void mover(Mapa &mapa){
		x += velocidades[velocidade] * dx;
		y += velocidades[velocidade] * dy;

		if(inteiro(x) && inteiro(y)){
			dx = 0;
			dy = 0;
			int i = x / FPS, j = y / FPS;
			auto a = mapa.getPos(i, j);
			if(a & VIDA){
				if(vidas < VIDAMAX)
					vidas++;
				mapa.removePos(i, j, VIDA);
			}else if(a & RAQPODER){
				if(potenciaDaRaquete < POTENCIAMAX)
					potenciaDaRaquete++;
				mapa.removePos(i, j, RAQPODER);
			}else if(a & RAQBONUS){
				if(potenciaDaRaquete < RAQUETESMAX)
					numRaquetes++;
				mapa.removePos(i, j, RAQBONUS);
			}else if(a & BOTA){
				if(velocidade < VELOCIDADEMAX)
					velocidade++;
				mapa.removePos(i, j, BOTA);
			}
		}
	}

	void desenha(){
		if(!vivo)
			return;
		float X = (float) x / FPS;
		float Y = (float) y / FPS;
		if(espelha)
			desenhaTexturaEspelhado(T_JOGADOR, X, Y, X + 1, Y + 1);
		else
			desenhaTextura(T_JOGADOR, X, Y, X + 1, Y + 1);
	}
};