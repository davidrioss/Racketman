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

    void atualizaRaio(Mapa &mapa, std::function<void(int, int)> f){
        for(int k = 0; k < 4; k++){
            for(int i = x + dirs[k][0], j = y + dirs[k][1]; abs(i - x) <= raio && abs(j - y) <= raio; i += dirs[k][0], j += dirs[k][1]){
                auto a = mapa.getPos(i, j);
                if(a & BLOCO)
                    break;
                f(i, j);
                if(a & PAREDE || a & RAQUETE)
                    break;
            }
        }
    }

    bool atualiza(Mapa &mapa){
        if(--frames == -FPS / 3){
            mapa.grid[x][y] = 0;
            atualizaRaio(mapa, [&mapa](int x, int y){
                if(mapa.getPos(x, y) & PAREDE)
                    mapa.removePos(x, y, EXPLOSAO | PAREDE);
                else
                    mapa.grid[x][y] = 0;
            });
            return true;
        }

        if(frames > 0){
            if(!(mapa.getPos(x, y) & EXPLOSAO))
                return false;
            frames = 0;
        }
        
        // Explosão
        mapa.setPos(x, y, EXPLOSAO);
        atualizaRaio(mapa, [&mapa](int x, int y){
            mapa.setPos(x, y, EXPLOSAO);
        });
        return false;
    }
};