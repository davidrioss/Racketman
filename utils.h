#pragma once

#define FPS 60

#include <math.h>

enum Entidades{
	BLOCO    = 1,
	PAREDE   = 2,
    PLAYER   = 1 << 2,
	RAQUETE  = 1 << 3,
    MOSQUITO = 1 << 4,
	BONUS    = 1 << 5,
};

bool inteiro(int a){
	return a % FPS == 0;
}