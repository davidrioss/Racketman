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
	bool espelha;
    std::list<Raquete> raquetes{};
	int framesProximo;
	int framesAndando;

	Player(){
		setaPos(1, 1);
		vidas = 3;
		potenciaDaRaquete = numRaquetes = velocidade = 1;
		espelha = true;
	}

	void reseta(){
		setaPos(1, 1);
		numRaquetes += raquetes.size();
		raquetes.clear();
	}

	void setaPos(int nx, int ny){
		x = nx * FPS;
		y = ny * FPS;
		dx = 0;
		dy = 0;
		vivo = true;
		framesAndando = framesProximo = 0;
	}

	bool movendo(){
		return !(inteiro(x) && inteiro(y));
	}

	void colocaRaquete(Mapa &mapa){
		int i = x / FPS, j = y / FPS;
		if(!vivo || movendo() || numRaquetes == 0 || mapa.getPos(i, j) & RAQUETE){
			framesProximo = FPS / 6;
			return;
		}
		
		mapa.setPos(i, j, RAQUETE);
		raquetes.emplace_back(i, j, potenciaDaRaquete, 3 * FPS, mapa);
		numRaquetes--;
		framesProximo = 0;
	}

	void comecaMovimento(int ndx, int ndy, Mapa &mapa){
		int i = x / FPS, j = y / FPS;
        if(!vivo || movendo() || mapa.getPos(i + ndx, j + ndy) & (BLOCO | PAREDE | RAQUETE))
            return;
		
		if(ndx > 0)
			espelha = true;
		else if(ndx < 0)
			espelha = false;
		dx = ndx;
		dy = ndy;
	}

	bool mover(Mapa &mapa){
		x += velocidades[velocidade] * dx;
		y += velocidades[velocidade] * dy;
		if(framesProximo)
			framesProximo--;

		if(inteiro(x) && inteiro(y)){
			framesAndando = 0;
			dx = 0;
			dy = 0;
			int i = x / FPS, j = y / FPS;
			auto a = mapa.getPos(i, j);
			if(a & VIDA){
				if(vidas < VIDAMAX)
					vidas++;
				mapa.removePos(i, j, VIDA);
			}
			if(a & RAQPODER){
				if(potenciaDaRaquete < POTENCIAMAX)
					potenciaDaRaquete++;
				mapa.removePos(i, j, RAQPODER);
			}
			if(a & RAQBONUS){
				if(potenciaDaRaquete < RAQUETESMAX)
					numRaquetes++;
				mapa.removePos(i, j, RAQBONUS);
			}
			if(a & BOTA){
				if(velocidade < VELOCIDADEMAX - 1)
					velocidade++;
				mapa.removePos(i, j, BOTA);
			}
			if(a & PORTA){
            	return true;
			}

			if(framesProximo > 0){
				colocaRaquete(mapa);
			}
		}else{
			framesAndando++;
		}

		return false;
	}

	void desenha(){
		if(!vivo)
			return;
		float X = (float) x / FPS;
		float Y = (float) y / FPS;
		int t = T_JOGADOR + (framesAndando % 15) / 5;
		if(espelha)
			desenhaTexturaEspelhado(t, X, Y, X + 1, Y + 1);
		else
			desenhaTextura(t, X, Y, X + 1, Y + 1);
	}
};