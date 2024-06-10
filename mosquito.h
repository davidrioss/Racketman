#pragma once

#include "mapa.h"

#include <map>
#include <random>
#include <set>
#include <vector>

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
	int playerVisto = 0;
	std::vector<std::vector<int>> memoria;

	Mosquito(int velocidade, int tipo, Mapa &mapa) : velocidade(velocidade), tipo(rand() % 3){
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
		frame = rand() % 100 + 1'000'000;
		if(this->tipo == 2)
			memoria = std::vector<std::vector<int>>(mapa.colunas, std::vector<int>(mapa.linhas));
	}

	void viuPlayer(int xi, int yi, int xf, int yf, int d){
		if(tipo < 2)
			return;
		playerVisto++;
		while(xi != xf || yi != yf){
			memoria[xi][yi] = -playerVisto;
			xi += dirs[d][0];
			yi += dirs[d][1];
		}
		memoria[xf][yf] = -playerVisto;
	}

	void setaDirecao(int d){
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
		std::set<int> dirsLivres;
		bool livreDeExplosao = false;
		for(int k = 0; k < 4; k++){
			auto a = mapa.getPos(i + dirs[k][0], j + dirs[k][1]);
			if(!(a & (BLOCO | PAREDE | RAQUETE | MOSQUITO))){
				dirsLivres.insert(k);
				if(!(a & VAIEXPLO))
					livreDeExplosao = true;
			}
		}
		
		if(dirsLivres.empty() || (!livreDeExplosao && !(mapa.getPos(i, j) & VAIEXPLO))){
			if(movendo){
				movendo = false;
				espera = FPS;
			}

			return false;
		}

		if(livreDeExplosao){
			auto it = dirsLivres.begin();
			while(it != dirsLivres.end()){
				if(mapa.getPos(i + dirs[*it][0], j + dirs[*it][1]) & VAIEXPLO)
					it = dirsLivres.erase(it);
				else
					it++;
			}
		}

		if(movendo){
			dirsLivres.erase((dir + 2) % 4);
			if(dirsLivres.empty()){
				setaDirecao((dir + 2) % 4);
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

	bool proximaDirecao2(int &i, int &j, Mapa &mapa){
		std::map<int, int> dirsLivres;
		bool livreDeExplosao = false;
		for(int k = 0; k < 4; k++){
			int x = i + dirs[k][0], y = j + dirs[k][1];
			auto a = mapa.getPos(x, y);
			if(!(a & (BLOCO | PAREDE | RAQUETE | MOSQUITO))){
				dirsLivres[k] = (memoria[x][y] < 0? memoria[x][y]: frame - memoria[x][y]);
				if(!(a & VAIEXPLO))
					livreDeExplosao = true;
			}
		}
		
		if(dirsLivres.empty() || (!livreDeExplosao && !(mapa.getPos(i, j) & VAIEXPLO))){
			movendo = false;
			return false;
		}

		int mi = INT32_MAX, dirmi;
		if(livreDeExplosao){
			auto it = dirsLivres.begin();
			while(it != dirsLivres.end()){
				if(mapa.getPos(i + dirs[it->first][0], j + dirs[it->first][1]) & VAIEXPLO){
					it = dirsLivres.erase(it);
				}else{
					if(it->second < mi){
						mi = it->second;
						dirmi = it->first;
					}
					it++;
				}
			}
		}

		if(mi < 0){
			setaDirecao(dirmi);
			return true;
		}

		if(movendo){
			dirsLivres.erase((dir + 2) % 4);
			if(dirsLivres.empty()){
				setaDirecao((dir + 2) % 4);
				movendo = false;
				return true;
			}
		}

		int total = 0;
		for(auto &k : dirsLivres){
			total += k.second;
		}
		total = rand() % total;

		for(auto &k : dirsLivres){
			total -= k.second;
			if(total < 0){
				setaDirecao(k.first);
				return true;
			}
		}
		return false;
	}

	void mover(Mapa &mapa){
		frame++;
		if(espera > 0){
			espera -= velocidades[velocidade];
			return;
		}

		if(inteiro(x) && inteiro(y)){
			int i = x / FPS, j = y / FPS;
			if(tipo == 2)
				memoria[i][j] = frame;
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
			case 2:
				mover = proximaDirecao2(i, j, mapa);
			default:
				break;
			}
			
			if(!mover){
				return;
			}
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
		int t = T_MOSQUITO + (frame % 10) / 5 + (tipo) * 2;
		if(espelha)
			desenhaTexturaEspelhado(t, X, Y, X + 1, Y + 1);
		else
			desenhaTextura(t, X, Y, X + 1, Y + 1);
    }
};