#pragma once

#define FPS 60

#include <math.h>

#define POTENCIAMAX 5

int dirs[][2] = {1,0, -1,0, 0,1, 0,-1};

enum Entidades{
	BLOCO    = 1,
	PAREDE   = 2,
    PLAYER   = 1 << 2,
	RAQUETE  = 1 << 3,
	EXPLOSAO = 1 << 4,
    MOSQUITO = 1 << 5,
	BONUS    = 1 << 6,
};

bool inteiro(int a){
	return a % FPS == 0;
}
