#pragma once

#include <math.h>

bool inteiro(float a){
	return abs(a - round(a)) < 1e-3;
}