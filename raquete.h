#pragma once

#include "mapa.h"

#include <functional>

#include <GL/glut.h>

class Raquete{
    public:
	int x;
    int y;
    int raio;
    int frames;

    Raquete(int x, int y, int raio, int frames) : x(x), y(y), raio(raio), frames(frames){}

    void desenha(){
        glColor3f(0.1f,0.1f,0.1f);
        glRectf(x + 0.4, y + 0.0, x + 0.6, y + 0.4);
        glColor3f(0.5f,0.5f,0.5f);
        glRectf(x + 0.2, y + 0.4, x + 0.8, y + 1.0);
        glColor3f(1.0f,1.0f,0.5f);
        glBegin( GL_LINES);
            glVertex2f(x + 0.4, y + 0.4);
            glVertex2f(x + 0.4, y + 1.0);
            glVertex2f(x + 0.6, y + 0.4);
            glVertex2f(x + 0.6, y + 1.0);
            glVertex2f(x + 0.2, y + 0.6);
            glVertex2f(x + 0.8, y + 0.6);
            glVertex2f(x + 0.2, y + 0.8);
            glVertex2f(x + 0.8, y + 0.8);   
        glEnd();
        
        //raio da explosao
        if(frames > 0)
            return;
        
        glColor3f(0.196100f,0.196100f,0.800000f);
        glRectf(x - raio, y + 0.2, x + 1 + raio, y+0.8);
        glRectf(x + 0.2, y - raio, x +0.8, y + 1 + raio);
        glColor3f(0.0f,0.498000f,1.0f);
        glRectf(x - raio, y + 0.3, x + 1 + raio, y + 0.7);
        glRectf(x + 0.3, y - raio, x+0.7, y + 1 + raio);
        glColor3f(0.749000f,0.847100f,0.847100f);
        glRectf(x - raio, y+0.4, x + 1 + raio, y+0.6);
        glRectf(x+0.4, y - raio, x+0.6, y + 1 + raio);
    }

    void atualizaRaio(Mapa &mapa, std::function<void(int, int)> f){
        for(int k = 0; k < 4; k++){
            for(int i = x + dirs[k][0], j = y + dirs[k][1]; abs(i - x) <= raio && abs(j - y) <= raio; i += dirs[k][0], j += dirs[k][1]){
                auto a = mapa.getPos(i, j);
                if(a & BLOCO)
                    break;
                f(i, j);
                if(a & PAREDE)
                    break;
            }
        }
    }

    bool atualiza(Mapa &mapa){
        if(--frames == -FPS / 3){
            mapa.removePos(x, y, RAQUETE);
            atualizaRaio(mapa, [&mapa](int x, int y){
                if(mapa.getPos(x, y) & PAREDE)
                    mapa.removePos(x, y, EXPLOSAO | PAREDE);
                else
                    mapa.grid[x][y] = 0;
            });
            return true;
        }

        if(frames)
            return false;
        
        // Explosão
        atualizaRaio(mapa, [&mapa](int x, int y){
            mapa.setPos(x, y, EXPLOSAO);
        });
        return false;
    }
};