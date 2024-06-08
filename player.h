#pragma once

#include "raquete.h"
#include "mapa.h"
#include "utils.h"

#include <list>

#include <GL/glut.h>

class Player{
	public:
	int x;
	int y;
	int dx;
	int dy;
	int velocidade;
    int potenciaDaRaquete;
    int numRaquetes;
	bool vivo;

	Player(){
        x = y = FPS;
		potenciaDaRaquete = numRaquetes= velocidade = 1;
		vivo = true;
	}

	void setaPos(int nx, int ny){
		x = nx * FPS;
		y = ny * FPS;
		dx = 0;
		dy = 0;
		vivo = true;
	}

	bool movendo(){
		return dx || dy;
	}

	void colocaRaquete(std::list<Raquete> &raquetes, Mapa &mapa){
		int i = x / FPS, j = y / FPS;
		if(movendo() || raquetes.size() == numRaquetes || mapa.getPos(i, j) & RAQUETE)
			return;
		
		mapa.setPos(i, j, RAQUETE);
		raquetes.emplace_back(i, j, potenciaDaRaquete, 5 * FPS);
	}

	void comecaMovimento(int ndx, int ndy, Mapa mapa){
		int i = x / FPS, j = y / FPS;
        if(movendo() || mapa.getPos(i + ndx, j + ndy) & (BLOCO | PAREDE | RAQUETE))
            return;
		dx = ndx;
		dy = ndy;
	}

	void mover(){
		x += velocidade * dx;
		y += velocidade * dy;

		if(inteiro(x) && inteiro(y)){
			dx = 0;
			dy = 0;
		}
	}

	void desenha(){
		float X = (float) x / FPS;
		float Y = (float) y / FPS;
		glColor3f(0.329400f,0.329400f,0.329400f); //CALÇA
		glRectf(X + 0.38f, Y + 0.05f, X + 0.47f, Y + 0.30f);
		glRectf(X + 0.62f, Y + 0.05f, X + 0.71f, Y + 0.30f);
		glRectf(X + 0.38f, Y + 0.30f, X + 0.71f, Y + 0.40f);

		glColor3f(1.0f,1.0f,1.0f); //camisa
		glRectf(X + 0.45f, Y + 0.40f, X + 0.55f, Y + 0.67f);
		glRectf(X + 0.33f, Y + 0.55f, X + 0.38f, Y + 0.67f);
		glRectf(X + 0.71f, Y + 0.55f, X + 0.76f, Y + 0.67f);

		glColor3f(0.0f,0.0f,1.0f); //colete e bone
		glRectf(X + 0.38f, Y + 0.40f, X + 0.45f, Y + 0.67f);
		glRectf(X + 0.55f, Y + 0.40f, X + 0.71f, Y + 0.67f);
		glRectf(X + 0.40f, Y + 0.90f, X + 0.60f, Y + 1.00f);
		glRectf(X + 0.60f, Y + 0.90f, X + 0.65f, Y + 0.93f);

		glColor3f(0.858800f,0.576500f,0.439200f); //rosto e braços
		glRectf(X + 0.33f, Y + 0.55f, X + 0.38f, Y + 0.35f);
		glRectf(X + 0.71f, Y + 0.55f, X + 0.76f, Y + 0.35f);
		glRectf(X + 0.40f, Y + 0.90f, X + 0.60f, Y + 0.67f);

		glColor3f(0.0f,0.0f,0.3f); //pes
		glRectf(X + 0.38f, Y + 0.05f, X + 0.50f, Y + 0.00f);
		glRectf(X + 0.62f, Y + 0.05f, X + 0.75f, Y + 0.00f);
	}
};