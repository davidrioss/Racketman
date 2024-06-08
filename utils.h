#pragma once

#define FPS 60

#include <math.h>

#define VIDAMAX 5
#define POTENCIAMAX 10
#define RAQUETESMAX 10
#define VELOCIDADEMAX 7

int largura, altura;
int dirs[][2] = {1,0, -1,0, 0,1, 0,-1};
int velocidades[] = {1, 2, 3, 4, 6, 10, 15, 20};

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

bool inteiro(int a){
	return a % FPS == 0;
}
