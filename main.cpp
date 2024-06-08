#include "jogo.h"

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <string>

using namespace std;

enum Tela{
	MENU,
	GAMEOVER,
	PAUSE,
	JOGO
};

int menuItem = 0;
Tela tela = MENU;
Jogo jogo = Jogo();

void resetaJogo(){
	jogo = Jogo();
	tela = JOGO;
}

void Teclado1(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	
	//botão espaço para por raquete	
	if(key == 32){
		jogo.colocaRaquete();
	}

	//botão p para pausar
	if(key == 'p'){
		if(tela == PAUSE)
			tela = JOGO;
		else if(tela == JOGO)
			tela = PAUSE;
	}

	//botão enter para começar
	if(key == 13){
		if(tela == MENU || tela == GAMEOVER){
			resetaJogo();
		}
	}
}

void Teclado2(int key, int x, int y){
	if (key == GLUT_KEY_HOME){
		resetaJogo();
		return;
	}

	if(tela == JOGO){
		jogo.teclado(key, x, y);
		return;
	}
	
	if(key == GLUT_KEY_UP || key == GLUT_KEY_DOWN)
		menuItem = !menuItem;
}

// Função callback chamada pela GLUT a cada intervalo de tempo
void DinamicaDoJogo(int frame)
{
	if(tela == JOGO){
		jogo.atualiza();
		if(jogo.player.vidas == 0)
			tela = GAMEOVER;
	}

	//chamada recursiva para gerar dinamica do jogo  
	glutPostRedisplay();
	glutTimerFunc(1000.0 / FPS, DinamicaDoJogo, frame + 1); 
}

void DesenhaMenu(){
	glColor3f(1.0f,1.0f,1.0f);
	glBegin(GL_TRIANGLES);
		glVertex2f(13.5f, 13.45f - menuItem);
		glVertex2f(12.5f, 13.0f - menuItem);
		glVertex2f(13.5f, 12.55f - menuItem);
	glEnd();
	// DesenhaTexto(estado == pausado ? "continuar" : "start", 13.5f, 13.5f, 1.0f, false);
	// DesenhaTexto("sair", 13.5f, 12.5f, 1.0f, false);
}

void DesenhaGameover(){
	DesenhaMenu();
	// DesenhaTexto("Gameover", 7.5f, 7.5f, 2.0, true);
	glColor4f(1.0f,1.0f,1.0f, 1.0f);
}

void DesenhaPausado(){
	jogo.desenha();
	glColor3f(1.0f,1.0f,1.0f);
	glRectf(4.5f, 5.5f, 6.5f, 9.5f);
	glRectf(8.5f, 5.5f,10.5f, 9.5f);
}

void DesenhaTela(){
	glClearColor(0.576500f, 0.858800f, 0.439200f, 0.0f);
	system("cls");
	printf("\nTela: %d", tela);
	printf("\nPontuacao do jogador: %d", jogo.pontos);
	printf("\nAltura %d Largura %d", altura, largura);
	printf("\nVelocidade %d", jogo.player.velocidade);
	printf("\nVidas %d", jogo.player.vidas);
	printf("\nNumRaquetes %d", jogo.player.numRaquetes);
	printf("\nPoder %d", jogo.player.potenciaDaRaquete);
	switch (tela)
	{
	case MENU:
		DesenhaMenu();
		break;
	case GAMEOVER:
		DesenhaGameover();
		break;
	case PAUSE:
		DesenhaPausado();
		break;
	case JOGO:
		jogo.desenha();
		break;
	}

	glFlush();
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Atualiza as variáveis
	largura = w;
	altura = h;

	// Especifica as dimensões da Viewport
	glViewport(0, 0, largura, altura);
}

main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  
	glutInitWindowPosition(5,5);     
	glutInitWindowSize(500,500);
	glutCreateWindow("Racketman");
	
	glutReshapeFunc(AlteraTamanhoJanela);
	glutDisplayFunc(DesenhaTela);
	glutSpecialFunc(Teclado2);    
	glutKeyboardFunc(Teclado1); 
	glutTimerFunc(0, DinamicaDoJogo, 1); //inicio do loop de dinamica do jogo
	
	glutMainLoop();
	
	return 0;
}