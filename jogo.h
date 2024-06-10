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
    int fase = 0;
    std::list<Mosquito> mosquitos;
    Mapa mapa;
    Player player;
    std::list<int> setas;

    Jogo() : mapa(9, 9, 60){
        srand(time(NULL));
        int qtdMosquitos=1;
        mosquitos = std::list<Mosquito>();
        while(qtdMosquitos--){
            mosquitos.emplace_back(0, 0, mapa);
        }
    }

    void tecladoEspecial(int key, int x, int y){
        if(!player.vivo && setas.size() == 0)
            player.setaPos(1, 1);
        
        if(key == GLUT_KEY_RIGHT || key == GLUT_KEY_LEFT || key == GLUT_KEY_UP || key == GLUT_KEY_DOWN){
            for(auto &k : setas)
                if(k == key)
                    return;

            setas.push_back(key);
        }
    }

    void tecladoEspecialSolto(int key, int x, int y){
        if(key == GLUT_KEY_RIGHT || key == GLUT_KEY_LEFT || key == GLUT_KEY_UP || key == GLUT_KEY_DOWN){
            auto it = setas.begin();
            while(it != setas.end()){
                if(*it == key)
                    it = setas.erase(it);
                else
                    it++;
            }
        }
    }

    void comecaMovimento(){
        if(setas.empty())
            return;
        int key = setas.back();
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
        player.colocaRaquete(mapa);
    }

    void desenhaVidas(){
        int b = mapa.linhas - 1;
        for(int a = 1; a <= player.vidas; a++){
            desenhaTextura(T_CORACAO, a, b, a + 1, b + 1);
        }
    }
    
    void desenhaPontuacao(bool desenhaPlacar, float x, float y){
        if(desenhaPlacar)
            desenhaTextura(T_PLACAR, x, y, x + 2.6, y + 1);

        x += 1.8f;
        for(int k = 3, p = pontos; k != -1; k--, p /= 10, x -= 0.5f)
            desenhaTextura(p % 10, x + 0.05f, y + 0.10f, x + 0.45f, y + 0.90f);
    }

    void ajustaEscala(){
        float razaoLargura = (float) largura / mapa.colunas;
        float razaoAltura = (float) altura / mapa.linhas;
        if(razaoLargura <= razaoAltura){
            double sobra = (razaoAltura / razaoLargura - 1) * mapa.linhas / 2;
            gluOrtho2D(0.0, (double) mapa.colunas, -sobra, mapa.linhas + sobra);
        }else{
            double sobra = (razaoLargura / razaoAltura - 1) * mapa.colunas / 2;
            gluOrtho2D(-sobra, mapa.colunas + sobra, 0.0, (double) mapa.linhas);
        }
    }

    void desenha(){
		stopAudio(&audioPlayers[0]);
		stopAudio(&audioPlayers[1]);
		startAudio(&audioPlayers[2]);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        ajustaEscala();
        glClear(GL_COLOR_BUFFER_BIT);

        mapa.desenha();
        desenhaVidas();
        desenhaPontuacao(true, mapa.colunas - 3.3f, mapa.linhas - 1.0f);

        for(auto &mosquito : mosquitos)
		    mosquito.desenha();
        player.desenha();

        auto desenha = [](float x, float y, Texturas t, int q){
            desenhaTexturaQuadrado(t, x + 0.1f, y + 0.1f, 0.8f);
            desenhaNumero(q, x + 0.6f, y + 0.1f, 0.7f);
        };

        desenha(1.0, 0.0, T_RAQUETE, player.numRaquetes);
        desenha(2.0, 0.0, T_BATERIA, player.potenciaDaRaquete);
        desenha(3.0, 0.0, T_TENIS  , player.velocidade);
    }

    void morreu(){
        if(!player.vivo)
            return;
        
        player.vivo = false;
        player.vidas--;
    }

    void proximaFase(){
        fase++;
        player.reseta();
        int tamanho = minimo(9 + fase / 3 * 2, 15);
        mapa = Mapa(tamanho, tamanho, maximo(60 - fase, 20));
        
        int qtdMosquitos = minimo(fase + 1, 10);
        mosquitos.clear();
        while(qtdMosquitos--){
            mosquitos.emplace_back(minimo(fase / 3, VELOCIDADEMAX), 0, mapa);
        }
    }

    void atualiza(){
        // Atualiza as raquetes
        auto it = player.raquetes.begin();
        bool removido = false;
        while(it != player.raquetes.end()){
            if(it->atualiza(mapa)){
                it = player.raquetes.erase(it);
                removido = true;
                player.numRaquetes++;
            }else{
                it++;
            }
        }

        if(removido){
            for(auto &raquete : player.raquetes){
                if(raquete.frames > 0){
                    raquete.atualizaRaio(mapa, [&](int x, int y){
                        mapa.setPos(x, y, VAIEXPLO);
                    });
                }
            }
        }

        comecaMovimento();
        if(player.mover(mapa))
            proximaFase();
        // Colisão com a explosão
        auto a = mapa.getPosMov(player.x, player.y);
        if(a & EXPLOSAO)
            morreu();

        int x = player.x / FPS, y = player.y / FPS;
        if(!inteiro(player.x)){
            if(x % 2 == 0)
                x++;
        }else if(!inteiro(player.y)){
            if(y % 2 == 0)
                y++;
        }

        if(player.vivo){
            for(int k = 0; k < 4; k++){
                for(int i = x + dirs[k][0], j = y + dirs[k][1]; true; i += dirs[k][0], j += dirs[k][1]){
                    auto a = mapa.getPos(i, j);
                    if(a & (BLOCO | PAREDE | RAQUETE))
                        break;
                    if(!(a & MOSQUITO))
                        continue;
                    
                    for(auto &mosquito : mosquitos){
                        int dx = mosquito.x - i * FPS;
                        int dy = mosquito.y - j * FPS;
                        if((dx > -FPS && dx < FPS) && (dy > -FPS && dy < FPS)){
                            mosquito.viuPlayer(x, y, i, j, k);
                            break;
                        }
                    }
                    break;
                }
            }
        }

        // Movimenta mosquitos
        auto mosquito = mosquitos.begin();
        while(mosquito != mosquitos.end()){
            mosquito->mover(mapa);

            // Colisão com o player
            if(abs(mosquito->x - player.x) < FPS && abs(mosquito->y - player.y) < FPS)
                morreu();
            
            // Colisão com a explosão
            if(mapa.getPosMov(mosquito->x, mosquito->y) & EXPLOSAO){
                pontos += mosquito->velocidade + 1;
                mosquito->matar(mapa);
                mosquito = mosquitos.erase(mosquito);
            }else{
                mosquito++;
            }
        }

        if(mosquitos.empty()){
            mapa.setPos(1, 1, PORTA);
        }
    }
};