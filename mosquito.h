#pragma once

#include "mapa.h"

#include <map>
#include <random>
#include <set>
#include <vector>

enum Cor{
	PRETO,
	VERDE,
	VERMELHO
};

class Mosquito{
	public:
	int x, y, dx, dy;       // Coordenas vezes UNIDADES de espaço
	int dir;                // Direção do movimento
	int velocidade;
	Cor cor;
	int espera;             // Frames até começar a se mover de novo
	int frame;              // Frame atual, usado para deslocar o mosquito na vertical
	bool espelha;
	bool movendo;
	int playerVisto = 0;    // Quantas vezes já viu o player
	bool vivo = true;
	int framesAnimacao = 0; // Frame atual da animação de morte
	std::vector<std::vector<int>> memoria;

	Mosquito(int velocidade, Cor cor, Mapa &mapa) : velocidade(velocidade), cor(cor){
		do{
			x = (rand() % (mapa.colunas - 3)) + 2;
			y = (rand() % (mapa.linhas - 3)) + 2;
		}while(x + y < 5 || mapa.getPos(x, y) & (BLOCO | MOSQUITO));
		mapa.grid[x][y] = MOSQUITO;
		
		setaDirecao(rand() % 4);

		x *= UNIDADE;
		y *= UNIDADE;
		movendo = false;
		espera = 0;
		espelha = false;
		frame = rand() % 100 + 1'000'000;
		if(this->cor == VERMELHO)
			memoria = std::vector<std::vector<int>>(mapa.colunas, std::vector<int>(mapa.linhas));
	}

	// velocidade vezes 1, 4 ou 16 se o mosquito for preto, verde ou vermelho repectivamente
	int pontos(){
		return (velocidade + 1) * (1 << (2 * cor));
	}

	void viuPlayer(int xi, int yi, int xf, int yf, int d){
		if(cor < VERMELHO)
			return;
		playerVisto++;
		// Guarda menos a quantidadede de player para seguir o menor valor que é a ultima posição onde viu o player
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

	// Retorna true se o mosquito deve se mover e falso se ele deve ficar parado
	bool proximaDirecaoPreto(int &i, int &j, Mapa &mapa){
		// Continua se movendo se o proximo for livre
		if(!(mapa.getPos(i + dx, j + dy) & (BLOCO | PAREDE | RAQUETE | MOSQUITO)))
			return true;

		// Se estava se movendo, para por uma UNIDADE de tempo e inverte a direção
		if(movendo){
			setaDirecao((dir + 2) % 4);
			espera = UNIDADE;
			movendo = false;
			return false;
		}

		// Parado por 2 frames consecutivos (bloqueado nas duas direções), escolhe a primeira direção livre
		for(int k = 0; k < 4; k++){
			if(!(mapa.getPos(i + dirs[k][0], j + dirs[k][1]) & (BLOCO | PAREDE | RAQUETE | MOSQUITO))){
				setaDirecao(k);
				return true;
			}
		}

		// Totalmente bloqueado, continua parado
		return false;
	}

	// Retorna verdadeiro se o mosquito deve se mover e falso se ele deve ficar parado
	bool proximaDirecaoVerde(int &i, int &j, Mapa &mapa){
		// Se já parou para recaular a direção e a próxima direção continua livre, continua se movendo nela
		if(!movendo && !(mapa.getPos(i + dx, j + dy) & (BLOCO | PAREDE | RAQUETE | MOSQUITO | VAIEXPLO)))
			return true;

		
		std::set<int> dirsLivres;
		bool livreDeExplosao = false;
		// Procura por posições livres e se está seguro de explosões em alguma delas
		for(int k = 0; k < 4; k++){
			auto a = mapa.getPos(i + dirs[k][0], j + dirs[k][1]);
			if(!(a & (BLOCO | PAREDE | RAQUETE | MOSQUITO))){
				dirsLivres.insert(k);
				if(!(a & VAIEXPLO))
					livreDeExplosao = true;
			}
		}
		
		// Se está totalmente bloqueado ou se todas as posições estão no alcance de uma bomba e ele está numa posição
		// segura fica parado e escolhe uma nova direção aleatoria
		if(dirsLivres.empty() || (!livreDeExplosao && !(mapa.getPos(i, j) & VAIEXPLO))){
			setaDirecao(rand() % 4);
			if(movendo){
				// Se estava se movendo coloca para esperar por uma UNIDADE de tempo
				movendo = false;
				espera = UNIDADE;
			}

			return false;
		}

		// Se tem posições seguras remove todas as que estão no alcance da bomba da lista
		if(livreDeExplosao){
			auto it = dirsLivres.begin();
			while(it != dirsLivres.end()){
				if(mapa.getPos(i + dirs[*it][0], j + dirs[*it][1]) & VAIEXPLO)
					it = dirsLivres.erase(it);
				else
					it++;
			}
		}

		// Se estava se movendo remove a direção oposta ao movimento da lista
		if(movendo){
			dirsLivres.erase((dir + 2) % 4);
			// Se for a unica direção livre escolhe ela, fica parado e espera por uma UNIDADE de tempo
			if(dirsLivres.empty()){
				setaDirecao((dir + 2) % 4);
				espera = UNIDADE;
				movendo = false;
				return false;
			}
		}

		// Escolhe uma direção aleatoria da lista
		auto it = dirsLivres.begin();
		advance(it, rand() % dirsLivres.size());
		int nd = *it;
		// Se for a mesma direção continua se movendo
		if(nd == dir)
			return true;
		
		// Se for outra direção seta a direção nova
		setaDirecao(nd);
		if(movendo){
			// Se estava se movendo fica parado e espera por uma UNIDADE de tempo
			espera = UNIDADE;
			movendo = false;
			return false;
		}
		return true;
	}
	
	// Retorna true se o mosquito deve se mover e falso se ele deve ficar parado
	bool proximaDirecaoVermelho(int &i, int &j, Mapa &mapa){
		std::map<int, int> dirsLivres;
		bool livreDeExplosao = false;
		// Procura por posições livres e se está seguro de explosões em alguma delas
		for(int k = 0; k < 4; k++){
			int x = i + dirs[k][0], y = j + dirs[k][1];
			auto a = mapa.getPos(x, y);
			if(!(a & (BLOCO | PAREDE | RAQUETE | MOSQUITO))){
				// Mapeia a direção para o valor na memoria se for negativo (quando ele viu o player) e para o frame
				// menos o valor da memoria se positivo (quantos frames se passou desde a ultima vez que ele visitou)
				dirsLivres[k] = (memoria[x][y] < 0? memoria[x][y]: frame - memoria[x][y]);
				if(!(a & VAIEXPLO))
					livreDeExplosao = true;
			}
		}
		
		// Se está totalmente bloqueado ou se todas as posições estão no alcance de uma bomba e ele está numa posição
		// segura fica parado
		if(dirsLivres.empty() || (!livreDeExplosao && !(mapa.getPos(i, j) & VAIEXPLO))){
			movendo = false;
			return false;
		}

		int mi = INT32_MAX, dirmi;
		// Se tem posições seguras remove todas as que estão no alcance da bomba da lista e procura se em alguma delas
		// leva em direção ao player
		if(livreDeExplosao){
			auto it = dirsLivres.begin();
			while(it != dirsLivres.end()){
				if(mapa.getPos(i + dirs[it->first][0], j + dirs[it->first][1]) & VAIEXPLO){
					it = dirsLivres.erase(it);
				}else{
					// Guarda o menor valor em mi e a menor direção em dirmi
					if(it->second < mi){
						mi = it->second;
						dirmi = it->first;
					}
					it++;
				}
			}

			// Se leva em direção ao player escolhe essa direção
			if(mi < 0){
				setaDirecao(dirmi);
				return true;
			}
		}

		// Se estava se movendo remove a direção oposta ao movimento da lista
		if(movendo){
			dirsLivres.erase((dir + 2) % 4);
			// Se for a unica direção livre escolhe ela
			if(dirsLivres.empty()){
				setaDirecao((dir + 2) % 4);
				movendo = false;
				return true;
			}
		}

		int total = 0;
		// Acumula os pesos de todas as direções livres
		for(auto &k : dirsLivres){
			if(k.second < 0)
				k.second = 1;
			total += k.second;
		}

		// Escolhe a direção usando a quantidade de tempo desde aultima visita na posição como peso
		// Faz com que ele prefira esplorar mais o mapa ao invés de ficar repetindo as mesmas posições
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

	// Retorana verdade se o mosquito deve ser removido da lista
	bool mover(Mapa &mapa){
		if(!vivo){
			return --framesAnimacao == 0;
		}

		frame++;
		if(espera > 0){
			espera -= velocidades[velocidade];
			return false;
		}

		// Se chegou num valor inteiro chama as funções que recalcula a direção do movimento
		if(inteiro(x) && inteiro(y)){
			int i = x / UNIDADE, j = y / UNIDADE;
			if(cor == VERMELHO)
				memoria[i][j] = frame;
			if(movendo)
				mapa.removePos(i - dx, j - dy, MOSQUITO);
			
			bool mover = false;
			switch (cor)
			{
			case PRETO:
				mover = proximaDirecaoPreto(i, j, mapa);
				break;
			case VERDE:
				mover = proximaDirecaoVerde(i, j, mapa);
				break;
			case VERMELHO:
				mover = proximaDirecaoVermelho(i, j, mapa);
			default:
				break;
			}
			
			if(!mover){
				return false;
			}
			mapa.setPos(i + dx, j + dy, MOSQUITO);
		}

		if(dx > 0)
			espelha = false;
		else if(dx < 0)
			espelha = true;
		movendo = true;
		// Atualiza as coordenadas
		x += velocidades[velocidade] * dx;
		y += velocidades[velocidade] * dy;
		return false;
	}

	// Remove o mosquito do mapa
	void matar(Mapa &mapa){
		int i = x / UNIDADE, j = y / UNIDADE;
		mapa.removePos(i, j, MOSQUITO);
		if(!inteiro(x))
			mapa.removePos(i + 1, j, MOSQUITO);
		if(!inteiro(y))
			mapa.removePos(i, j + 1, MOSQUITO);
	}

    void desenha(){
		float X = (float) x / UNIDADE;
		float Y = (float) y / UNIDADE + sin(frame / 5) * 0.05; // Usa o frame pra delocar o mosquito na vertical
		int t = T_MOSQUITO + (frame % 10) / 5 + cor * 2; // Escolhe a textura atual
		desenhaTextura(t, X, Y, X + 1, Y + 1, espelha);
		// Se está morrendo desenha a textura de morte por cima
		if(!vivo && framesAnimacao < 9){
			t = T_MOSQUITOMORRENDO + (framesAnimacao - 1) / 2;
			desenhaTextura(t, X, Y, X + 1, Y + 1, espelha);
		}
    }
};