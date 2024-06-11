#pragma once

#include "raquete.h"
#include "mapa.h"
#include "utils.h"

#include <list>

class Player{
	public:
	int x, y, dx, dy; // Coordenadas vezes UNIDADES de espaço
	int velocidade;
    int potenciaDaRaquete;
    int numRaquetes;
	int vidas;
	bool vivo;
	bool espelha;
    std::list<Raquete> raquetes{};
	int framesProximo;  // Frames em que aceita colocar a raquete ainda
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
		x = nx * UNIDADE;
		y = ny * UNIDADE;
		dx = 0;
		dy = 0;
		vivo = true;
		framesAnimacao = framesProximo = 0;
	}

	bool movendo(){
		return !(inteiro(x) && inteiro(y));
	}

	void colocaRaquete(Mapa &mapa, bool botao){
		int i = x / UNIDADE, j = y / UNIDADE;
		if(!vivo || movendo() || numRaquetes == 0 || mapa.getPos(i, j) & RAQUETE){
			if(botao)
				framesProximo = UNIDADE / 6; // Se não pode colcar a raquete, tenta colocar novamente nos proximos frames
			return;
		}
		
		mapa.setPos(i, j, RAQUETE);
		raquetes.emplace_back(i, j, potenciaDaRaquete, 3 * UNIDADE, mapa);
		numRaquetes--;
		framesProximo = 0;
	}

	void comecaMovimento(int ndx, int ndy, Mapa &mapa){
		int i = x / UNIDADE, j = y / UNIDADE;
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
			int i = x / UNIDADE, j = y / UNIDADE;
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
				if(numRaquetes + raquetes.size() < RAQUETESMAX)
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
		float X = (float) x / UNIDADE;
		float Y = (float) y / UNIDADE;
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