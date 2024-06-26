#pragma once

#define FPS     60 // taxa de atualização da tela
#define UNIDADE 60 // 1 unidade de espaço/tempo

#include "audio.h"

#include <math.h>

#include <GL/glut.h>

#define maximo(a, b) ((a) > (b) ? (a) : (b))
#define minimo(a, b) ((a) < (b) ? (a) : (b))

#define VIDAMAX 5
#define POTENCIAMAX 9
#define RAQUETESMAX 9
#define VELOCIDADEMAX 7

int largura, altura;
int dirs[][2] = {1,0, 0,1, -1,0, 0,-1};
int velocidades[] = {1, 2, 3, 4, 6, 10, 15, 20};
int qtdMosquitos[][3] = {
	1, 0, 0,
	2, 0, 0,
	2, 1, 0,
	3, 1, 0,
	3, 2, 0,
	3, 2, 1,
	3, 3, 1,
	3, 3, 2,
	3, 4, 2,
	3, 4, 3,
	2, 5, 3,
	2, 4, 4,
	1, 5, 4,
	1, 4, 5,
	0, 5, 5,
	0, 4, 6,
	0, 3, 7,
	0, 2, 8,
	0, 1, 9,
	0, 0,10
};
GLuint textureID[50];

enum Entidades{
	BLOCO    = 1,
	PAREDE   = 2,
	PORTA    = 1 << 2,
	RAQUETE  = 1 << 3,
	EXPLOSAO = 1 << 4,
    MOSQUITO = 1 << 5,
	RAQPODER = 1 << 6,
	RAQBONUS = 1 << 7,
	BOTA     = 1 << 8,
	VIDA     = 1 << 9,
	VAIEXPLO = 1 << 10
};

enum Texturas{
	T_MENU = 10,
	T_GAMEOVER,
	T_GRAMA,
	T_PEDRA,
	T_PAREDE,
	T_JOGADOR,
	T_MOSQUITO = 18,
	T_RAQUETE = 24,
	T_CORACAO,
	T_RAIO,
	T_BATERIA,
	T_PAUSE,
	T_PLACAR,
	T_MOSQUITINHO,
	T_TENIS,
	T_RAQUETEBONUS,
	T_PORTA,
	T_JOGADORMORRENDO,
	T_MOSQUITOMORRENDO = 37,
	T_BOTAOINFO = 41,
	T_CONTROLES
};

bool inteiro(int a){
	return a % UNIDADE == 0;
}

void desenhaTextura(int textura, float xi, float yi, float xf, float yf, bool espelha = false){
	glBindTexture(GL_TEXTURE_2D, textureID[textura]);
	glBegin(GL_QUADS);
		if(espelha){
			glTexCoord2f(0.0f, 0.0f); glVertex2f(xf, yf);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(xi, yf);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(xi, yi);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(xf, yi);
		}else{
			glTexCoord2f(0.0f, 0.0f); glVertex2f(xi, yf);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(xf, yf);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(xf, yi);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(xi, yi);
		}
    glEnd();
}

void desenhaTexturaQuadrado(int textura, float x, float y, float t){
	desenhaTextura(textura, x, y, x + t, y + t);
}

void desenhaNumero(int numero, float x, float y, float t){
	desenhaTextura(numero, x, y, x + t / 2, y + t);
}
