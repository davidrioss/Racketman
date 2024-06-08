#include "mapa.h"
#include "utils.h"
#include "mosquito.h"
#include "player.h"
#include "raquete.h"

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <string>
#include <vector>
#include <list>
using namespace std;

#define FPS 60

enum Estado{
	menu,
	gameover,
	pausado,
	jogando
};

int pontos=0, vidas=3, potenciaDaRaquete=1, contadorMosquitos=0, numRaquetes = 1;
list<mosquito> mosquitos;
list<raquete> raquetes;
mapa m = mapa();
player p = player();
int menuItem = 0;
Estado estado = menu;

void DesenhaVidas(){
	int a, b=14;
	if(vidas>=1){
		a=13;
		glColor3f(1.0f,0.0f,0.0f);
		glBegin( GL_POLYGON);
		    glVertex3f(a+0.5, b, 0.0f);
		    glVertex3f(a+1.0, b+0.6, 0.0f);
		    glVertex3f(a+0.7, b+1, 0.0f);
		    glVertex3f(a+0.5, b+0.8, 0.0f);
		    glVertex3f(a+0.3, b+1, 0.0f);
		    glVertex3f(a, b+0.6, 0.0f);
		    glVertex3f(a+0.5, b, 0.0f);
	    glEnd();
	}
	if(vidas>=2){
		a=12;
		glColor3f(1.0f,0.0f,0.0f);
		glBegin( GL_POLYGON);
		    glVertex3f(a+0.5, b, 0.0f);
		    glVertex3f(a+1.0, b+0.6, 0.0f);
		    glVertex3f(a+0.7, b+1, 0.0f);
		    glVertex3f(a+0.5, b+0.8, 0.0f);
		    glVertex3f(a+0.3, b+1, 0.0f);
		    glVertex3f(a, b+0.6, 0.0f);
		    glVertex3f(a+0.5, b, 0.0f);
	    glEnd();
	}
	if(vidas==3){
		a=11;
		glColor3f(1.0f,0.0f,0.0f);
		glBegin( GL_POLYGON);
		    glVertex3f(a+0.5, b, 0.0f);
		    glVertex3f(a+1.0, b+0.6, 0.0f);
		    glVertex3f(a+0.7, b+1, 0.0f);
		    glVertex3f(a+0.5, b+0.8, 0.0f);
		    glVertex3f(a+0.3, b+1, 0.0f);
		    glVertex3f(a, b+0.6, 0.0f);
		    glVertex3f(a+0.5, b, 0.0f);
	    glEnd();
	}

}

void DesenhaPontuacao(){
	
}

void InicializaMatriz(){
	//1=paredes, 2=blocos fixos, 3=raquete, 4=mosquitos, 5=parede bonus, 6=bonus
	//desenha blocos fixos
	srand(time(NULL));
	int paredes=55, qtdMosquitos=6;
	m = mapa(15, 15, paredes);
	mosquitos = list<mosquito>();
	while (qtdMosquitos--){
		mosquitos.emplace_back(1, 0, m);
	}	
}

void ResetaJogo(){
	p = player();
	pontos = 0;
	vidas = 3;
	InicializaMatriz();
	estado = jogando;
}

void Morreu(){
	if(vidas--){
		p.setaPos(1, 1);
	}else{
		estado = gameover;
		p.setaPos(-1, -1);
	}
}

void Teclado1(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	
	//botão espaço para por raquete	
	if(key == 32){
		if (!p.movendo() && raquetes.size() < numRaquetes){
			raquetes.emplace_back(p.x + 1, p.y, potenciaDaRaquete, 5 * FPS);
		}
	}

	//botão p para pausar
	if(key == 'p'){
		if(estado == pausado)
			estado = jogando;
		else if(estado == jogando)
			estado = pausado;
	}

	//botão enter para começar
	if(key == 13){
		if(estado == menu || estado == gameover){
			ResetaJogo();
		}
	}
}

void Teclado2(int key, int x, int y){
	if(estado != jogando){
		if(key == GLUT_KEY_UP || key == GLUT_KEY_DOWN)
			menuItem = !menuItem;
		
		// glutPostRedisplay();
		return;
	}


	if(key == GLUT_KEY_LEFT)
	{
		p.comecaMovimento(1, 0, m);
	}
	
	if(key == GLUT_KEY_RIGHT)
	{
		p.comecaMovimento(-1, 0, m);
	}     
	
	if(key == GLUT_KEY_UP){
		p.comecaMovimento(0, 1, m);
	}
		
	if(key == GLUT_KEY_DOWN){
		p.comecaMovimento(0, -1, m);
	}                    
	
	
	if (key == GLUT_KEY_HOME){
		ResetaJogo();
	}
	                                   
	// glutPostRedisplay();
}


void Explosao(){
	//mata mosquitos verificando se esta vivo e posição
	// for(int n=0;n<6;n++){
	// 	if(mosquito[n][0] && ((mosquito[n][1]==raquete[1] && (mosquito[n][2]<=raquete[2]+potenciaDaRaquete && mosquito[n][2]>=raquete[2]-potenciaDaRaquete)) || 
	// (mosquito[n][2]==raquete[2] && (mosquito[n][1]<=raquete[1]+potenciaDaRaquete && mosquito[n][1]>=raquete[1]-potenciaDaRaquete)))){
	// 			mosquito[n][0]=0;
	// 			mapa[mosquito[n][1]][mosquito[n][2]]=0;
	// 			contadorMosquitos--;
	// 			pontos+=10;
	// 	}	
	// }
	
	// //destroi jogador
	// if((playerX==raquete[1] && (playerY<=raquete[2]+potenciaDaRaquete && playerY>=raquete[2]-potenciaDaRaquete)) || 
	// (playerY==raquete[2] && (playerX<=raquete[1]+potenciaDaRaquete && playerX>=raquete[2]-potenciaDaRaquete)))
	// 	Morreu();
	
	// //destruir paredes
	// for(int x=1;x<=potenciaDaRaquete;x++){
	// 	if(mapa[raquete[1]+x][raquete[2]]==1)
	// 		mapa[raquete[1]+x][raquete[2]]=0;
	// 	if(mapa[raquete[1]-x][raquete[2]]==1)
	// 		mapa[raquete[1]-x][raquete[2]]=0;	
	// 	if(mapa[raquete[1]][raquete[2]+x]==1)
	// 		mapa[raquete[1]][raquete[2]+x]=0;	
	// 	if(mapa[raquete[1]][raquete[2]-x]==1)
	// 		mapa[raquete[1]][raquete[2]-x]=0;
	// 	if(mapa[raquete[1]+x][raquete[2]]==5)
	// 		mapa[raquete[1]+x][raquete[2]]=6;
	// 	if(mapa[raquete[1]-x][raquete[2]]==5)
	// 		mapa[raquete[1]-x][raquete[2]]=6;	
	// 	if(mapa[raquete[1]][raquete[2]+x]==5)
	// 		mapa[raquete[1]][raquete[2]+x]=6;	
	// 	if(mapa[raquete[1]][raquete[2]-x]==5)
	// 		mapa[raquete[1]][raquete[2]-x]=6;
	// }	
}



// Função callback chamada pela GLUT a cada intervalo de tempo
void DinamicaDoJogo(int frame)
{
	if(estado != pausado){
		p.mover(FPS);
		for(auto &k : mosquitos)
			k.mover();
	}

	//contagem do tempo da raquete, a explosão acontece em -1
	// if(raquete[0]==-1){
	// 	raquete[0]=0;
	// 	mapa[raquete[1]][raquete[2]]=0;
	// 	Explosao();
	// }
	// if(raquete[0]<-1)
	// raquete[0]++;

	
	//movimentação dos mosquitos
    // for(int n=0;n<6;n++){
	// 	if(mosquito[n][0]){
    // 		//verifica bloqueio na horizontal 
    // 		if((mapa[mosquito[n][1]+1][mosquito[n][2]]!=0 && mapa[mosquito[n][1]-1][mosquito[n][2]]==0) || (mapa[mosquito[n][1]+1][mosquito[n][2]]==0 && mapa[mosquito[n][1]-1][mosquito[n][2]]!=0))
	// 		mosquito[n][3] = -mosquito[n][3];

	// 		//verifica bloqueio na vertical
    // 		if((mapa[mosquito[n][1]][mosquito[n][2]+1]==0 && mapa[mosquito[n][1]][mosquito[n][2]-1])!=0 || (mapa[mosquito[n][1]][mosquito[n][2]+1]!=0 && mapa[mosquito[n][1]][mosquito[n][2]-1]==0))
	// 		mosquito[n][4] = -mosquito[n][4];

	// 		//inverte eixo de movidentação do mosquito se eixo bloqueado
    // 		if((mapa[mosquito[n][1]+1][mosquito[n][2]] && mapa[mosquito[n][1]-1][mosquito[n][2]] && mosquito[n][3]) || (mapa[mosquito[n][1]][mosquito[n][2]+1]!=0 && mapa[mosquito[n][1]][mosquito[n][2]-1]!=0 && mosquito[n][4])){
	// 			int aux = mosquito[n][3];
	// 			mosquito[n][3] = mosquito[n][4];
    // 			mosquito[n][4] = aux;
	// 		} 

	// 		//movimenta mosquito
	// 		if(mosquito[n][3] && (mapa[mosquito[n][1] + mosquito[n][3]][mosquito[n][2]]==0)){
	// 			mapa[mosquito[n][1]][mosquito[n][2]]=0;
	// 			mosquito[n][1] += mosquito[n][3];
	// 			mapa[mosquito[n][1]][mosquito[n][2]]=4;
	// 			if(playerX==mosquito[n][1] && playerY==mosquito[n][2])
	// 				Morreu();
	// 		}
	// 		else{
	// 			if(mosquito[n][4] && (mapa[mosquito[n][1]][mosquito[n][2] + mosquito[n][4]]==0)){
	// 				mapa[mosquito[n][1]][mosquito[n][2]]=0;
	// 				mosquito[n][2] += mosquito[n][4];
	// 				mapa[mosquito[n][1]][mosquito[n][2]]=4;
	// 				if(playerX==mosquito[n][1] && playerY==mosquito[n][2])
	// 					Morreu();
	// 			}
	// 		}
				
	// 	}
		
	// }
	
	
	//chamada recursiva para gerar dinamica do jogo
	glutTimerFunc(1000.0 / FPS, DinamicaDoJogo, frame + 1);   
	glutPostRedisplay();
}

void DesenhaMapa(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(15.0f, 0.0f, 0.0f, 15.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m.desenha();
	
	DesenhaVidas();
	DesenhaPontuacao();
	for(auto &k : mosquitos)
		k.desenha();
	auto it = raquetes.begin();
	while(it != raquetes.end()){
		it->desenha();
		if(it->t == -FPS / 3)
			it = raquetes.erase(it);
		else
			it++;
	}
	if(estado == jogando || estado == pausado)
		p.desenha();
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
	DesenhaMenu();
	glColor3f(1.0f,1.0f,1.0f);
	glRectf(4.5f, 5.5f, 6.5f, 9.5f);
	glRectf(8.5f, 5.5f,10.5f, 9.5f);
}

void DesenhaTela(){
	glClearColor(0.576500f, 0.858800f, 0.439200f, 0.0f);
	system("cls");
	printf("\nEstado: %d", estado);
	printf("\nPontuacao do jogador: %d", pontos);
	DesenhaMapa();
	if(estado == menu){
		DesenhaMenu();
	}else if(estado == gameover){
		DesenhaGameover();
	}else{
		if(estado == pausado)
			DesenhaPausado();
	}

	glFlush();
}

main(int argc, char** argv){
	
	InicializaMatriz(); 
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  
	glutInitWindowPosition(5,5);     
	glutInitWindowSize(500,500);  
	glutCreateWindow("Racketman");
	
	glutDisplayFunc(DesenhaTela);
	glutSpecialFunc(Teclado2);    
	glutKeyboardFunc(Teclado1); 
	
	glutTimerFunc(500, DinamicaDoJogo,1); //inicio do loop de dinamica do jogo
	
	glutMainLoop();
	
	return 0;
}