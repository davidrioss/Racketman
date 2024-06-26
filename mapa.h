#pragma once

#include "utils.h"

#include <random>
#include <vector>

class Mapa{
	public:
	std::vector<std::vector<short int>> grid;   // Grid com bitmask dos itens que ocupam esse lugar
	int linhas;
	int colunas;

    Mapa(){
        linhas = 0;
        colunas = 0;
    }

	Mapa(int linhas, int colunas, int paredes) : linhas(linhas), colunas(colunas){
		grid = std::vector<std::vector<short int>>(colunas, std::vector<short int>(linhas));

        for(int i = 0; i < colunas; i++) for(int j = 0; j < linhas; j++){
            if(i == 0 || i == colunas - 1 || j == 0 || j == linhas - 1 || ( i % 2 == 0 && j % 2 == 0)){
                grid[i][j] = BLOCO;
            }else if(rand() % 100 < paredes){
                grid[i][j] = PAREDE;

                // Distribui bonus
                int bonus = rand() % 100;
                if(bonus == 0)
                    grid[i][j] |= VIDA;
                else if(bonus <= 6)
                    grid[i][j] |= RAQPODER;
                else if(bonus <= 11)
                    grid[i][j] |= RAQBONUS;
                else if(bonus <= 15)
                    grid[i][j] |= BOTA;
            }
        }
        grid[1][1] = grid[1][2] = grid[2][1] = 0;
        grid[3][1] = grid[1][3] = PAREDE;
	}

	int getPos(int x, int y){
		return grid[x][y];
	}

	int getPosMov(int x, int y){
        int i = x / UNIDADE, j = y / UNIDADE;
		if(!inteiro(x))
			return getPos(i, j) | getPos(i + 1, j);
		if(!inteiro(y))
			return getPos(i, j) | getPos(i, j + 1);
		return getPos(i, j);
	}
    
	void setPos(int x, int y, int e){
		grid[x][y] |= e;
	}

	void removePos(int x, int y, int e){
		grid[x][y] &= ~e;
	}

    void desenhaDebug(int i, int j){
    }

    void desenha(){
        for(int i = 0; i < colunas; i++){
            for(int j = 0; j < linhas; j++){
                if(grid[i][j] & BLOCO){
                    desenhaTextura(T_PEDRA, i, j, i + 1, j + 1);
                }else if(grid[i][j] & PAREDE){
                    desenhaTextura(T_PAREDE, i, j, i + 1, j + 1);
                }else{
                    desenhaTextura(T_GRAMA, i, j, i + 1, j + 1);
                    if(grid[i][j] & PORTA){
                        desenhaTextura(T_PORTA, i, j, i + 1, j + 1);
                    }
                    if(grid[i][j] & VIDA){
                        desenhaTextura(T_CORACAO, i + 0.1f, j + 0.1f, i + 0.9f, j + 0.9f);
                    }
                    if(grid[i][j] & RAQPODER){
                        desenhaTextura(T_BATERIA, i + 0.1f, j + 0.1f, i + 0.9f, j + 0.9f);
                    }
                    if(grid[i][j] & RAQBONUS){
                        desenhaTextura(T_RAQUETEBONUS, i + 0.1f, j + 0.1f, i + 0.9f, j + 0.9f);
                    }
                    if(grid[i][j] & BOTA){
                        desenhaTextura(T_TENIS, i + 0.1f, j + 0.1f, i + 0.9f, j + 0.9f);
                    }
                    if(grid[i][j] & EXPLOSAO){
                        desenhaTextura(T_RAIO, i, j, i + 1, j + 1);
                    }
                }
#ifdef DEBUG
                int entidades = __builtin_popcount(grid[i][j] & ~(BLOCO | PAREDE));
                if(entidades)
                    desenhaNumero(entidades, i, j, 0.5);
#endif
            }
        }
    }
};