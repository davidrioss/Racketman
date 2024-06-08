#pragma once

#include "utils.h"

#include <random>
#include <vector>

#include <GL/glut.h>

class Mapa{
	public:
	std::vector<std::vector<short int>> grid;
	int linhas;
	int colunas;

	Mapa(int linhas, int colunas, int paredes) : linhas(linhas), colunas(colunas){
		grid = std::vector<std::vector<short int>>(linhas, std::vector<short int>(colunas));

        for(int i = 0; i < linhas; i++) for(int j = 0; j < colunas; j++){
            if(i == 0 || i == linhas - 1 || j == 0 || j== colunas - 1 || ( i % 2 == 0 && j % 2 == 0)){
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
        int i = x / FPS, j = y / FPS;
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

    void desenha(){
        for(int i = 0; i < linhas; i++){
            for(int j = 0; j < colunas; j++){
                if(grid[i][j] & BLOCO){
                    glColor3f(0.329400f,0.329400f,0.329400f);
                    glRectf(i, j, i+1, j+1);
                    
                    glColor3f(0.184300f,0.184300f,0.309800f);
                    glBegin( GL_LINE_LOOP);
                        glVertex2f(i, j);
                        glVertex2f(i, j+1);
                        glVertex2f(i+1, j+1);
                        glVertex2f(i+1, j);
                    glEnd();
                }else if(grid[i][j] & PAREDE){
                    glColor3f(0.556900f,0.419600f,0.137300f);
                    glRectf(i, j, i+1, j+1);
                    
                    glColor3f(0.435300f,0.258800f,0.258800f);
                    glBegin( GL_LINE_LOOP);
                        glVertex2f(i, j);
                        glVertex2f(i, j+1);
                        glVertex2f(i+1, j+1);
                        glVertex2f(i+1, j);
                    glEnd();
                }else if(grid[i][j] & VIDA){
                    glColor3f(1.0, 0.0, 0.0);
                    glRectf(i + 0.2, j + 0.2, i + 0.8, j + 0.8);
                }else if(grid[i][j] & RAQPODER){
                    glColor3f(0.196100f,0.800000f,0.196100f);
                    glRectf(i+0.2, j, i+0.8, j+0.7);
                    glColor3f(0.10f,0.10f,0.10f);
                    glRectf(i+0.3, j+0.9, i+0.7, j+1);
                    glRectf(i+0.2, j+0.7, i+0.8, j+0.9);
                    glBegin( GL_LINE_LOOP);
                        glVertex2f(i+0.2, j);
                        glVertex2f(i+0.8, j);
                        glVertex2f(i+0.8, j+0.7);
                        glVertex2f(i+0.2, j+0.7);
                    glEnd();
                    glColor3f(1.0f,1.0f,0.0f);
                    glLineWidth(3.0);
                    glBegin( GL_LINE_STRIP);
                        glVertex2f(i+0.6, j+0.2);
                        glVertex2f(i+0.4, j+0.4);
                        glVertex2f(i+0.6, j+0.4);
                        glVertex2f(i+0.4, j+0.6);
                    glEnd();
                    glLineWidth(1.0);
                }else if(grid[i][j] & RAQBONUS){
                    glColor3f(0.0, 1.0, 1.0);
                    glRectf(i + 0.2, j + 0.2, i + 0.8, j + 0.8);
                }else if(grid[i][j] & BOTA){
                    glColor3f(1.0, 1.0, 1.0);
                    glRectf(i + 0.2, j + 0.2, i + 0.8, j + 0.8);
                }else if(grid[i][j] & ESCADA){
                    glColor3f(0.0, 0.0, 0.0);
                    glRectf(i + 0.3, j + 0.2, i + 0.7, j + 0.8);
                }
            }
        }
    }
};