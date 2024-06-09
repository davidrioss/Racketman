#pragma once

#define FPS 60

#include "audio.h"

#include <math.h>

#include <GL/glut.h>

#define maximo(a, b) ((a) > (b) ? (a) : (b))
#define minimo(a, b) ((a) < (b) ? (a) : (b))

#define VIDAMAX 5
#define POTENCIAMAX 10
#define RAQUETESMAX 10
#define VELOCIDADEMAX 7

int largura, altura;
int dirs[][2] = {1,0, -1,0, 0,1, 0,-1};
int velocidades[] = {1, 2, 3, 4, 6, 10, 15, 20};
GLuint textureID[40];

enum Entidades{
	BLOCO    = 1,
	PAREDE   = 2,
	ESCADA   = 1 << 2,
	RAQUETE  = 1 << 3,
	EXPLOSAO = 1 << 4,
    MOSQUITO = 1 << 5,
	RAQPODER = 1 << 6,
	RAQBONUS = 1 << 7,
	BOTA     = 1 << 8,
	VIDA     = 1 << 9
};

enum Texturas{
	T_MENU = 10,
	T_GAMEOVER,
	T_GRAMA,
	T_PEDRA,
	T_PAREDE,
	T_JOGADOR,
	T_MOSQUITO,
	T_RAQUETE,
	T_CORACAO,
	T_RAIO,
	T_BATERIA,
	T_PAUSE,
	T_PLACAR,
	T_MOSQUITINHO,
	T_TENIS,
	T_RAQUETEBONUS
};

bool inteiro(int a){
	return a % FPS == 0;
}

void desenhaTextura(int textura, float xi, float yi, float xf, float yf){
	glBindTexture(GL_TEXTURE_2D, textureID[textura]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(xi, yf);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(xf, yf);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(xf, yi);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(xi, yi);
    glEnd();
}

void desenhaTexturaEspelhado(int textura, float xi, float yi, float xf, float yf){
	glBindTexture(GL_TEXTURE_2D, textureID[textura]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(xf, yf);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(xi, yf);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(xi, yi);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(xf, yi);
    glEnd();
}
