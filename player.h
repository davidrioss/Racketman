#pragma once

#include "utils.h"
#include "mapa.h"

#include <GL/glut.h>

class player{
	public:
	float x, y;
	int dx, dy, v;

	player(){
		v = 1;
        x = y = 1;
	}

	void setaPos(int nx, int ny){
		x = nx;
		y = ny;
		dx = 0;
		dy = 0;
	}

	bool movendo(){
		return dx || dy;
	}

	void comecaMovimento(int ndx, int ndy, mapa m){
        if(movendo())
            return;
		dx = ndx;
		dy = ndy;
	}

	void mover(float fps){
		x += v * dx / fps;
		y += v * dy / fps;

        printf("%.3lf: %.3lf, %.3lf: %.3lf\n", x, abs(x - (int) x), y, abs(y - (int) y));
		if(inteiro(x) && inteiro(y)){
			dx = 0;
			dy = 0;
		}
	}

	void desenha(){
		glColor3f(0.329400f,0.329400f,0.329400f); //CALÇA
		glRectf(x + 0.38f, y + 0.05f, x + 0.47f, y + 0.30f);
		glRectf(x + 0.62f, y + 0.05f, x + 0.71f, y + 0.30f);
		glRectf(x + 0.38f, y + 0.30f, x + 0.71f, y + 0.40f);

		glColor3f(1.0f,1.0f,1.0f); //camisa
		glRectf(x + 0.45f, y + 0.40f, x + 0.55f, y + 0.67f);
		glRectf(x + 0.33f, y + 0.55f, x + 0.38f, y + 0.67f);
		glRectf(x + 0.71f, y + 0.55f, x + 0.76f, y + 0.67f);

		glColor3f(0.0f,0.0f,1.0f); //colete e bone
		glRectf(x + 0.38f, y + 0.40f, x + 0.45f, y + 0.67f);
		glRectf(x + 0.55f, y + 0.40f, x + 0.71f, y + 0.67f);
		glRectf(x + 0.40f, y + 0.90f, x + 0.60f, y + 1.00f);
		glRectf(x + 0.60f, y + 0.90f, x + 0.65f, y + 0.93f);

		glColor3f(0.858800f,0.576500f,0.439200f); //rosto e braços
		glRectf(x + 0.33f, y + 0.55f, x + 0.38f, y + 0.35f);
		glRectf(x + 0.71f, y + 0.55f, x + 0.76f, y + 0.35f);
		glRectf(x + 0.40f, y + 0.90f, x + 0.60f, y + 0.67f);

		glColor3f(0.0f,0.0f,0.3f); //pes
		glRectf(x + 0.38f, y + 0.05f, x + 0.50f, y + 0.00f);
		glRectf(x + 0.62f, y + 0.05f, x + 0.75f, y + 0.00f);
	}
};