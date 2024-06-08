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
        player.mover();

        // Movimenta mosquitos
        for(auto &mosquito : mosquitos){
            mosquito.mover();

            // Checa se colide com player
            if(abs(mosquito.x - player.x) < FPS && abs(mosquito.y - player.y) < FPS)
                morreu();
        }

        // Remove raquetes
        auto it = raquetes.begin();
        while(it != raquetes.end()){
            if(it->frames-- == -FPS / 3){
                mapa.removePos(it->x, it->y, RAQUETE);
                it = raquetes.erase(it);
            }else{
                it++;
            }
        }

        //mata mosquitos verificando se esta vivo e posição
        // for(int n=0;n<6;n++){
        // 	if(mosquito[n][0] && ((mosquito[n][1]==raquete[1] && (mosquito[n][2]<=raquete[2]+potenciaDaRaquete && mosquito[n][2]>=raquete[2]-potenciaDaRaquete)) || 
        // (mosquito[n][2]==raquete[2] && (mosquito[n][1]<=raquete[1]+potenciaDaRaquete && mosquito[n][1]>=raquete[1]-potenciaDaRaquete)))){
        // 			mosquito[n][0]=0;
        // 			mapa[mosquito[n][1]][mosquito[n][2]]=0;
        // 			contadorMosquitos--;
        // 			pontos+=10;
        // 	}	
        // }
        
        // //destroi jogador
        // if((playerX==raquete[1] && (playerY<=raquete[2]+potenciaDaRaquete && playerY>=raquete[2]-potenciaDaRaquete)) || 
        // (playerY==raquete[2] && (playerX<=raquete[1]+potenciaDaRaquete && playerX>=raquete[2]-potenciaDaRaquete)))
        // 	Morreu();
        
        // //destruir paredes
        // for(int x=1;x<=potenciaDaRaquete;x++){
        // 	if(mapa[raquete[1]+x][raquete[2]]==1)
        // 		mapa[raquete[1]+x][raquete[2]]=0;
        // 	if(mapa[raquete[1]-x][raquete[2]]==1)
        // 		mapa[raquete[1]-x][raquete[2]]=0;	
        // 	if(mapa[raquete[1]][raquete[2]+x]==1)
        // 		mapa[raquete[1]][raquete[2]+x]=0;	
        // 	if(mapa[raquete[1]][raquete[2]-x]==1)
        // 		mapa[raquete[1]][raquete[2]-x]=0;
        // 	if(mapa[raquete[1]+x][raquete[2]]==5)
        // 		mapa[raquete[1]+x][raquete[2]]=6;
        // 	if(mapa[raquete[1]-x][raquete[2]]==5)
        // 		mapa[raquete[1]-x][raquete[2]]=6;	
        // 	if(mapa[raquete[1]][raquete[2]+x]==5)
        // 		mapa[raquete[1]][raquete[2]+x]=6;	
        // 	if(mapa[raquete[1]][raquete[2]-x]==5)
        // 		mapa[raquete[1]][raquete[2]-x]=6;
        // }
    }
};