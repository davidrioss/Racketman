#pragma once

#include "mapa.h"

#include <functional>

class Raquete{
    public:
	int x;
    int y;
    int raio;
    int frames;

    Raquete(int x, int y, int raio, int frames, Mapa &mapa) : x(x), y(y), raio(raio), frames(frames){
        atualizaRaio(mapa, [&mapa](int x, int y){
            mapa.setPos(x, y, VAIEXPLO);
        });
    }

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
        // Limpa explosão
        if(--frames == -FPS / 4){
            mapa.grid[x][y] = 0;
            atualizaRaio(mapa, [&mapa](int x, int y){
                auto a = mapa.getPos(x, y);
                if(!(a & EXPLOSAO)){
                    mapa.removePos(x, y, VAIEXPLO);
                    return;
                }
                if(a & PAREDE)
                    mapa.removePos(x, y, EXPLOSAO | PAREDE | VAIEXPLO);
                else
                    mapa.grid[x][y] = 0;
            });
            return true;
        }

        if(frames > 0)
            return false;
        
        // Explosão
        mapa.setPos(x, y, EXPLOSAO);
        atualizaRaio(mapa, [&mapa](int x, int y){
            mapa.setPos(x, y, EXPLOSAO);
        });
        return false;
    }

    void desenha(){
        if(frames < 0)
            return;
        float s = 0.95 + cos(frames * M_PI / (FPS / 3)) * 0.05;
        float d = (1 - s) / 2;
        desenhaTexturaQuadrado(T_RAQUETE, x + d, y + d, s);
    }
};