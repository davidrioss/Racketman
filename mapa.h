#pragma once

#include "utils.h"

#include <random>
#include <vector>

#include <GL/glut.h>

class Mapa{
	public:
	std::vector<std::vector<char>> grid;
	int linhas;
	int colunas;

	Mapa(int linhas, int colunas, int paredes) : linhas(linhas), colunas(colunas){
		grid = std::vector<std::vector<char>>(linhas, std::vector<char>(colunas));

        for(int i = 0; i < linhas; i++)
            for(int j = 0; j < colunas; j++)
                if(i == 0 || i == linhas - 1 || j == 0 || j== colunas - 1 || ( i % 2 == 0 && j % 2 == 0))
                    grid[i][j] = BLOCO;
        
        //distribui paredes aleatoriamente e bonus
        while(paredes){
            int aux1 = (rand() % (linhas - 3)) + 2; 
            int aux2 = (rand() % (colunas - 3)) + 2;
            if(grid[aux1][aux2])
                continue;
            grid[aux1][aux2] = PAREDE;
            // if(rand() % 10 == 0)
                grid[aux1][aux2] |= BONUS;
            paredes--;
        }
	}

	char getPos(int x, int y){
		return grid[x][y];
	}

	char getPosMov(int x, int y){
        int i = x / FPS, j = y / FPS;
		if(!inteiro(x))
			return getPos(i, j) | getPos(i + 1, j);
		if(!inteiro(y))
			return getPos(i, j) | getPos(i, j + 1);
		return getPos(i, j);
	}

	void setPos(int x, int y, char e){
		grid[x][y] |= e;
	}

	void removePos(int x, int y, char e){
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
                }else if(grid[i][j] & BONUS){
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
                }
            }
        }
    }
};