#pragma once

#include <GL/glut.h>

class raquete{
    public:
	int x, y, r, t;

    raquete(int x, int y, int r, int t) : x(x), y(y), r(r), t(t){}

    void desenha(){
        glColor3f(0.1f,0.1f,0.1f);
        glRectf(x + 0.4, y + 0.0, x + 0.6, y + 0.4);
        glColor3f(0.5f,0.5f,0.5f);
        glRectf(x + 0.2, y + 0.4, x + 0.8, y + 1.0);
        glColor3f(1.0f,1.0f,0.5f);
        glBegin( GL_LINES);
            glVertex2f(x + 0.4, y + 0.4);
            glVertex2f(x + 0.4, y + 1.0);
            glVertex2f(x + 0.6, y + 0.4);
            glVertex2f(x + 0.6, y + 1.0);
            glVertex2f(x + 0.2, y + 0.6);
            glVertex2f(x + 0.8, y + 0.6);
            glVertex2f(x + 0.2, y + 0.8);
            glVertex2f(x + 0.8, y + 0.8);   
        glEnd();
        
        //raio da explosao
        if(--t > 0)
            return;
        
        glColor3f(0.196100f,0.196100f,0.800000f);
        glRectf(x - r  , y + 0.2, x +1+r, y+0.8);
        glRectf(x + 0.2, y - r  , x +0.8, y+1+r);
        glColor3f(0.0f,0.498000f,1.0f);
        glRectf(x-r, y+0.3, x+1+r, y+0.7);
        glRectf(x+0.3, y-r, x+0.7, y+1+r);	
        glColor3f(0.749000f,0.847100f,0.847100f);
        glRectf(x-r, y+0.4, x+1+r, y+0.6);
        glRectf(x+0.4, y-r, x+0.6, y+1+r);
    }
};