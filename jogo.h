#pragma once

#include "mapa.h"
#include "mosquito.h"
#include "player.h"
#include "raquete.h"

#include <list>
#include <random>
#include <time.h>

class Jogo{
    public:
    int pontos = 0;
    int vidas = 3;
    std::list<Mosquito> mosquitos;
    std::list<Raquete> raquetes;
    Mapa mapa;
    Player player;

    Jogo() : mapa(15, 15, 55) {
        player = Player();
        
        srand(time(NULL));
        int qtdMosquitos=6;
        mosquitos = std::list<Mosquito>();
        while(qtdMosquitos--){
            mosquitos.emplace_back(1, 0, mapa);
        }
    }

    void teclado(int key, int x, int y){
        if(!player.vivo)
            player.setaPos(1, 1);

        if(key == GLUT_KEY_RIGHT)
        {
            player.comecaMovimento(1, 0, mapa);
        }

        if(key == GLUT_KEY_LEFT)
        {
            player.comecaMovimento(-1, 0, mapa);
        }
        
        if(key == GLUT_KEY_UP){
            player.comecaMovimento(0, 1, mapa);
        }
            
        if(key == GLUT_KEY_DOWN){
            player.comecaMovimento(0, -1, mapa);
        }
    }

    void colocaRaquete(){
        player.colocaRaquete(raquetes, mapa);
    }

    void desenhaVidas(){
        int b = 14;
        for(int a = 1; a <= vidas; a++){
            glColor3f(1.0f,0.0f,0.0f);
            glBegin( GL_POLYGON);
                glVertex3f(a+0.5, b, 0.0f);
                glVertex3f(a+1.0, b+0.6, 0.0f);
                glVertex3f(a+0.7, b+1, 0.0f);
                glVertex3f(a+0.5, b+0.8, 0.0f);
                glVertex3f(a+0.3, b+1, 0.0f);
                glVertex3f(a, b+0.6, 0.0f);
                glVertex3f(a+0.5, b, 0.0f);
            glEnd();
        }
    }
    
    void desenhaPontuacao(){
        
    }

    void desenha(){
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluOrtho2D(0, mapa.colunas, 0, mapa.linhas);
        glClear(GL_COLOR_BUFFER_BIT);

        mapa.desenha();
        desenhaVidas();
        desenhaPontuacao();

        for(auto &mosquito : mosquitos)
		    mosquito.desenha();
        for(auto &raquete : raquetes)
            raquete.desenha();
        player.desenha();
    }

    void morreu(){
        if(!player.vivo)
            return;
        
        player.vivo = false;
        vidas--;
    }

    void atualiza(){
        // Atualiza as raquetes
        auto it = raquetes.begin();
        while(it != raquetes.end()){
            if(it->atualiza(mapa)){
                it = raquetes.erase(it);
            }else{
                it++;
            }
        }

        player.mover(mapa);
        // Colisão com a explosão
        if(mapa.getPosMov(player.x, player.y) & EXPLOSAO)
            morreu();

        // Movimenta mosquitos
        auto mosquito = mosquitos.begin();
        while(mosquito != mosquitos.end()){
            mosquito->mover(mapa);

            // Colisão com o player
            if(abs(mosquito->x - player.x) < FPS && abs(mosquito->y - player.y) < FPS)
                morreu();
            
            // Colisão com a explosão
            if(mapa.getPosMov(mosquito->x, mosquito->y) & EXPLOSAO){
                mosquito->matar(mapa);
                mosquito = mosquitos.erase(mosquito);
            }else{
                mosquito++;
            }
        }
    }
};