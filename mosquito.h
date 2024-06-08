#pragma once

#include "mapa.h"

#include <random>
#include <GL/glut.h>

class mosquito{
	public:
	float x, y;
	int dx, dy, v, t;

	mosquito(int v, int t, mapa &m) : v(v), t(t){
		do{
			x = (rand() % (m.linhas - 1)) + 1;
			y = (rand() % (m.colunas - 1)) + 1;
		}while(m.getPos(x, y));
		
		dx = 1;
		dy = 0;
	}

	void mover(){
		if(abs(x - (int) x) < 1e-4 && abs(y - (int) y) < 1e-4){
			switch (t)
			{
			case 0:
				dx *= -1;
				dy *= -1;
				return;
			default:
				return;
			}
		}

		x += v * dx;
		y += v * dy;
	}

    void desenha(){
        glColor3f(0.0f,0.0f,0.0f); //corpo
        glRectf(x+0.1, y+0.3, x+0.25, y+0.5);
        glRectf(x+0.25, y+0.3, x+0.5, y+0.6);
        glRectf(x+0.5, y+0.35, x+1, y+0.55);
        glRectf(x, y, x+0.1, y+0.4);
        glRectf(x+0.25, y+0.1, x+0.3, y+0.3);
        glRectf(x+0.35, y+0.1, x+0.4, y+0.3);
        glRectf(x+0.45, y+0.1, x+0.5, y+0.3);
        glRectf(x+0.55, y+0.15, x+0.6, y+0.35);
        glRectf(x+0.65, y+0.15, x+0.7, y+0.35);
        glRectf(x+0.75, y+0.15, x+0.8, y+0.35);
        glColor3f(1.0f,1.0f,1.0f); //listras
        glRectf(x+0.6, y+0.35, x+0.65, y+0.55);
        glRectf(x+0.75, y+0.35, x+0.8, y+0.55);
        glRectf(x+0.9, y+0.35, x+0.95, y+0.55);
        glColor3f(1.0f,0.0f,0.0f); //olho
        glRectf(x+0.1, y+0.4, x+0.15, y+0.48);
        glColor3f(0.678400f,0.917600f,0.917600f); //asas
        glRectf(x+0.1, y+0.6, x+0.4, y+1.0);
        glRectf(x+0.5, y+0.6, x+0.95, y+0.85);	
    }
};