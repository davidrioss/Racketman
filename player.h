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
	int framesAnimacao;

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
		framesAnimacao = framesProximo = 0;
	}

	bool movendo(){
		return !(inteiro(x) && inteiro(y));
	}

	void colocaRaquete(Mapa &mapa, bool botao){
		int i = x / FPS, j = y / FPS;
		if(!vivo || movendo() || numRaquetes == 0 || mapa.getPos(i, j) & RAQUETE){
			if(botao)
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
		if(!vivo){
			if(framesAnimacao)
				framesAnimacao--;
			return false;
		}

		x += velocidades[velocidade] * dx;
		y += velocidades[velocidade] * dy;
		if(framesProximo > 0)
			framesProximo--;

		if(inteiro(x) && inteiro(y)){
			framesAnimacao = 0;
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
				colocaRaquete(mapa, false);
			}
		}else{
			framesAnimacao++;
		}

		return false;
	}

	void desenha(){
		float X = (float) x / FPS;
		float Y = (float) y / FPS;
		int t;
		if(!vivo){
			if(framesAnimacao == 0)
				return;
			t = T_JOGADORMORRENDO + maximo(framesAnimacao / (FPS / 6) - 4, 0);
		}else{
			t = T_JOGADOR + (framesAnimacao % 15) / 5;
		}
		desenhaTextura(t, X, Y, X + 1, Y + 1, espelha);
	}
};