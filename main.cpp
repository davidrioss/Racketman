#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <string>
#include <vector>
using namespace std;

#define playerXinicial 13
#define playerYinicial 1

enum Estado{
	menu,
	gameover,
	pausado,
	jogando
};

int mapa[15][15];
int playerX= -1;
int playerY= -1;
int mosquito[6][5]; //colunas - ativo, x, y, deslocamento x, deslocamento y
int raquete[3]={0,0,0};  //colunas - ativo , x, y
int pontos=0, vidas=3, potenciaDaRaquete=1, contadorMosquitos=0;
int menuItem = 0;
Estado estado = menu;

void DesenhaBloco(float i, float j){
	GLfloat a=i, b=j;
	
	glColor3f(0.329400f,0.329400f,0.329400f);
	glRectf(a, b, a+1, b+1);
	
	glColor3f(0.184300f,0.184300f,0.309800f);
	glBegin( GL_LINE_LOOP);
	    glVertex3f(a, b, 0.0f);
	    glVertex3f(a, b+1, 0.0f);
	    glVertex3f(a+1, b+1, 0.0f);
	    glVertex3f(a+1, b, 0.0f);
    glEnd();
}


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

void DesenhaParede(float i, float j){
	GLfloat a=i, b=j;
	
	glColor3f(0.556900f,0.419600f,0.137300f);
	glRectf(a, b, a+1, b+1);
	
	glColor3f(0.435300f,0.258800f,0.258800f);
	glBegin( GL_LINE_LOOP);
	    glVertex3f(a, b, 0.0f);
	    glVertex3f(a, b+1, 0.0f);
	    glVertex3f(a+1, b+1, 0.0f);
	    glVertex3f(a+1, b, 0.0f);
    glEnd();
}

void DesenhaBonus(float i, float j){
	GLfloat a=i, b=j;
	
	glColor3f(0.196100f,0.800000f,0.196100f);
	glRectf(a+0.2, b, a+0.8, b+0.7);
	glColor3f(0.10f,0.10f,0.10f);
	glRectf(a+0.3, b+0.9, a+0.7, b+1);
	glRectf(a+0.2, b+0.7, a+0.8, b+0.9);
	glBegin( GL_LINE_LOOP);
	    glVertex3f(a+0.2, b, 0.0f);
	    glVertex3f(a+0.8, b, 0.0f);
	    glVertex3f(a+0.8, b+0.7, 0.0f);
	    glVertex3f(a+0.2, b+0.7, 0.0f);
    glEnd();
	glColor3f(1.0f,1.0f,0.0f);
	glLineWidth(3.0);
	glBegin( GL_LINE_STRIP);
	    glVertex3f(a+0.6, b+0.2, 0.0f);
	    glVertex3f(a+0.4, b+0.4, 0.0f);
	    glVertex3f(a+0.6, b+0.4, 0.0f);
	    glVertex3f(a+0.4, b+0.6, 0.0f);
    glEnd();
    glLineWidth(1.0);
	
}

void DesenhaPersonagem(int a, int b){
		
	glColor3f(0.329400f,0.329400f,0.329400f); //CALÇA
	glRectf(a+0.38, b+0.05, a+0.47, b+0.3);
	glRectf(a+0.62, b+0.05, a+0.71, b+0.3);
	glRectf(a+0.38, b+0.3, a+0.71, b+0.4);
	glColor3f(1.0f,1.0f,1.0f); //camisa
	glRectf(a+0.45, b+0.40, a+0.55, b+0.67);
	glRectf(a+0.33, b+0.55, a+0.38, b+0.67);
	glRectf(a+0.71, b+0.55, a+0.76, b+0.67);
	glColor3f(0.0f,0.0f,1.0f); //colete e bone
	glRectf(a+0.38, b+0.4, a+0.45, b+0.67);
	glRectf(a+0.55, b+0.4, a+0.71, b+0.67);
	glRectf(a+0.4, b+0.9, a+0.6, b+1.0);
	glRectf(a+0.6, b+0.9, a+0.65, b+0.93);
	glColor3f(0.858800f,0.576500f,0.439200f); //rosto e braços
	glRectf(a+0.33, b+0.55, a+0.38, b+0.35);
	glRectf(a+0.71, b+0.55, a+0.76, b+0.35);
	glRectf(a+0.40, b+0.90, a+0.60, b+0.67);
	glColor3f(0.0f,0.0f,0.3f); //pes
	glRectf(a+0.38, b+0.05, a+0.50, b);
	glRectf(a+0.62, b+0.05, a+0.75, b);

}

void DesenhaRaquete(){	
	if(raquete[0]){
		glColor3f(0.1f,0.1f,0.1f);
		glRectf(raquete[1]+0.4, raquete[2], raquete[1]+0.6, raquete[2]+0.4);
		glColor3f(0.5f,0.5f,0.5f);
		glRectf(raquete[1]+0.2, raquete[2]+0.4, raquete[1]+0.8, raquete[2]+1);
		glColor3f(1.0f,1.0f,0.5f);
		glBegin( GL_LINES);
		    glVertex3f(raquete[1]+0.4, raquete[2]+0.4, 0.0f);
		    glVertex3f(raquete[1]+0.4, raquete[2]+1, 0.0f);
		    glVertex3f(raquete[1]+0.6, raquete[2]+0.4, 0.0f);
		    glVertex3f(raquete[1]+0.6, raquete[2]+1, 0.0f);
		    glVertex3f(raquete[1]+0.2, raquete[2]+0.6, 0.0f);
		    glVertex3f(raquete[1]+0.8, raquete[2]+0.6, 0.0f);
		    glVertex3f(raquete[1]+0.2, raquete[2]+0.8, 0.0f);
		    glVertex3f(raquete[1]+0.8, raquete[2]+0.8, 0.0f);   
	    glEnd();
	}
	
	//raio da explosao
	if(raquete[0]==-1){
	glColor3f(0.196100f,0.196100f,0.800000f);
	glRectf(raquete[1]-potenciaDaRaquete, raquete[2]+0.2, raquete[1]+1+potenciaDaRaquete, raquete[2]+0.8);
	glRectf(raquete[1]+0.2, raquete[2]-potenciaDaRaquete, raquete[1]+0.8, raquete[2]+1+potenciaDaRaquete);
	glColor3f(0.0f,0.498000f,1.0f);
	glRectf(raquete[1]-potenciaDaRaquete, raquete[2]+0.3, raquete[1]+1+potenciaDaRaquete, raquete[2]+0.7);
	glRectf(raquete[1]+0.3, raquete[2]-potenciaDaRaquete, raquete[1]+0.7, raquete[2]+1+potenciaDaRaquete);	
	glColor3f(0.749000f,0.847100f,0.847100f);
	glRectf(raquete[1]-potenciaDaRaquete, raquete[2]+0.4, raquete[1]+1+potenciaDaRaquete, raquete[2]+0.6);
	glRectf(raquete[1]+0.4, raquete[2]-potenciaDaRaquete, raquete[1]+0.6, raquete[2]+1+potenciaDaRaquete);	}
}

void DesenhaMosquitos(){
	for(int x=0;x<6;x++){
		if(mosquito[x][0]){
			glColor3f(0.0f,0.0f,0.0f); //corpo
			glRectf(mosquito[x][1]+0.1, mosquito[x][2]+0.3, mosquito[x][1]+0.25, mosquito[x][2]+0.5);
			glRectf(mosquito[x][1]+0.25, mosquito[x][2]+0.3, mosquito[x][1]+0.5, mosquito[x][2]+0.6);
			glRectf(mosquito[x][1]+0.5, mosquito[x][2]+0.35, mosquito[x][1]+1, mosquito[x][2]+0.55);
			glRectf(mosquito[x][1], mosquito[x][2], mosquito[x][1]+0.1, mosquito[x][2]+0.4);
			glRectf(mosquito[x][1]+0.25, mosquito[x][2]+0.1, mosquito[x][1]+0.3, mosquito[x][2]+0.3);
			glRectf(mosquito[x][1]+0.35, mosquito[x][2]+0.1, mosquito[x][1]+0.4, mosquito[x][2]+0.3);
			glRectf(mosquito[x][1]+0.45, mosquito[x][2]+0.1, mosquito[x][1]+0.5, mosquito[x][2]+0.3);
			glRectf(mosquito[x][1]+0.55, mosquito[x][2]+0.15, mosquito[x][1]+0.6, mosquito[x][2]+0.35);
			glRectf(mosquito[x][1]+0.65, mosquito[x][2]+0.15, mosquito[x][1]+0.7, mosquito[x][2]+0.35);
			glRectf(mosquito[x][1]+0.75, mosquito[x][2]+0.15, mosquito[x][1]+0.8, mosquito[x][2]+0.35);
			glColor3f(1.0f,1.0f,1.0f); //listras
			glRectf(mosquito[x][1]+0.6, mosquito[x][2]+0.35, mosquito[x][1]+0.65, mosquito[x][2]+0.55);
			glRectf(mosquito[x][1]+0.75, mosquito[x][2]+0.35, mosquito[x][1]+0.8, mosquito[x][2]+0.55);
			glRectf(mosquito[x][1]+0.9, mosquito[x][2]+0.35, mosquito[x][1]+0.95, mosquito[x][2]+0.55);
			glColor3f(1.0f,0.0f,0.0f); //olho
			glRectf(mosquito[x][1]+0.1, mosquito[x][2]+0.4, mosquito[x][1]+0.15, mosquito[x][2]+0.48);
			glColor3f(0.678400f,0.917600f,0.917600f); //asas
			glRectf(mosquito[x][1]+0.1, mosquito[x][2]+0.6, mosquito[x][1]+0.4, mosquito[x][2]+1.0);
			glRectf(mosquito[x][1]+0.5, mosquito[x][2]+0.6, mosquito[x][1]+0.95, mosquito[x][2]+0.85);				
		}
	}
}

void DesenhaPontuacao(){
	
}

void InicializaMatriz(){
	//1=paredes, 2=blocos fixos, 3=raquete, 4=mosquitos, 5=parede bonus, 6=bonus
	//desenha blocos fixos
	int paredes=55, mosquitos=6, paredebonus=1;
	for(int i=0;i<15;i++){
		for(int j=0;j<15;j++){
			if(i==0 || i==14 ||j==0 || j==14 || (i%2==0 && j%2==0)){
				mapa[i][j]=2;
			}else{
				mapa[i][j]=0;
			}
		}
	}
	//marca local inicial do jogador
	mapa[12][1]=3;
	mapa[13][1]=3;
	mapa[13][2]=3;
	
	//distribui paredes aleatoriamente e bonus
	srand(time(NULL));
	while(paredes){
		int aux1=rand()%15; 
		int aux2=rand()%15;
		if(mapa[aux1][aux2]==0){
			mapa[aux1][aux2]=1;
			paredes--;
		}
	}
	
	while(paredebonus){
		int aux1=rand()%15; 
		int aux2=rand()%15;
		if(mapa[aux1][aux2] == 0){
			mapa[aux1][aux2]=5;
			paredebonus--;
		}
	}
	
	//distribui mosquitos aleatoriamente
	while(mosquitos){
		int aux1=rand()%15;
		int aux2=rand()%15;
		if(mapa[aux1][aux2]==0){
			mosquito[mosquitos-1][0]=1;
			mosquito[mosquitos-1][1]=aux1;
			mosquito[mosquitos-1][2]=aux2;
			mosquito[mosquitos-1][3]=1;
			mosquito[mosquitos-1][4]=0;
			mapa[aux1][aux2]=4;
			mosquitos--;
			contadorMosquitos++;
		}
	}
	
	//libera area inicial do jogador
	mapa[12][1]=0;
	mapa[13][1]=0;
	mapa[13][2]=0;
	
}

void ResetaJogo(){
	playerX= playerXinicial;
	playerY= playerYinicial;
	pontos = 0;
	vidas = 3;
	InicializaMatriz();
	estado = jogando;
}

void Morreu(){
	if(vidas--){
		playerX= playerXinicial;
		playerY= playerYinicial;
	}else{
		estado = gameover;
		playerX = -1;
		playerY = -1;
	}
}

void Teclado1(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	
	//botão espaço para por raquete	
	if(key == 32){
		if ( raquete[0]==0){
			raquete[0]=-7;
			raquete[1]=playerX;
			raquete[2]=playerY;
			mapa[playerX][playerY]=3;
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
		
		glutPostRedisplay();
		return;
	}


	if(key == GLUT_KEY_LEFT)
	{
		if ( mapa[playerX+1][playerY]==0)
		playerX++; 
		else{
		if ( mapa[playerX+1][playerY]==4)
			Morreu();
		if 	( mapa[playerX+1][playerY]==6){
			mapa[playerX+1][playerY]=0;
			potenciaDaRaquete++;
			playerX++;
			}
		}
	}
	
	if(key == GLUT_KEY_RIGHT)
	{
		if ( mapa[playerX-1][playerY]==0)
			playerX--;  
		else{
		if ( mapa[playerX-1][playerY]==4)
			Morreu();
		if 	( mapa[playerX-1][playerY]==6){
			mapa[playerX-1][playerY]=0;
			potenciaDaRaquete++;
			playerX--;
			}	
		}
	}     
	
	if(key == GLUT_KEY_UP){
		if ( mapa[playerX][playerY+1]==0)
			playerY++;
		else{
		if ( mapa[playerX][playerY+1]==4)
			Morreu();
		if 	( mapa[playerX][playerY+1]==6){
			mapa[playerX][playerY+1]=0;
			potenciaDaRaquete++;
			playerY++;
			}	
		}
	}
		
	if(key == GLUT_KEY_DOWN){
		if ( mapa[playerX][playerY-1]==0)
			playerY--; 
		else{
		if ( mapa[playerX][playerY-1]==4)
			Morreu();
		if 	( mapa[playerX][playerY-1]==6){
			mapa[playerX][playerY-1]=0;
			potenciaDaRaquete++;
			playerY--;
			}	
		}
	}                    
	
	
	if (key == GLUT_KEY_HOME){
		ResetaJogo();
	}
	                                   
	glutPostRedisplay();
}


void Explosao(){
	//mata mosquitos verificando se esta vivo e posição
	for(int n=0;n<6;n++){
		if(mosquito[n][0] && ((mosquito[n][1]==raquete[1] && (mosquito[n][2]<=raquete[2]+potenciaDaRaquete && mosquito[n][2]>=raquete[2]-potenciaDaRaquete)) || 
	(mosquito[n][2]==raquete[2] && (mosquito[n][1]<=raquete[1]+potenciaDaRaquete && mosquito[n][1]>=raquete[1]-potenciaDaRaquete)))){
				mosquito[n][0]=0;
				mapa[mosquito[n][1]][mosquito[n][2]]=0;
				contadorMosquitos--;
				pontos+=10;
		}	
	}
	
	//destroi jogador
	if((playerX==raquete[1] && (playerY<=raquete[2]+potenciaDaRaquete && playerY>=raquete[2]-potenciaDaRaquete)) || 
	(playerY==raquete[2] && (playerX<=raquete[1]+potenciaDaRaquete && playerX>=raquete[2]-potenciaDaRaquete)))
		Morreu();
	
	//destruir paredes
	for(int x=1;x<=potenciaDaRaquete;x++){
		if(mapa[raquete[1]+x][raquete[2]]==1)
			mapa[raquete[1]+x][raquete[2]]=0;
		if(mapa[raquete[1]-x][raquete[2]]==1)
			mapa[raquete[1]-x][raquete[2]]=0;	
		if(mapa[raquete[1]][raquete[2]+x]==1)
			mapa[raquete[1]][raquete[2]+x]=0;	
		if(mapa[raquete[1]][raquete[2]-x]==1)
			mapa[raquete[1]][raquete[2]-x]=0;
		if(mapa[raquete[1]+x][raquete[2]]==5)
			mapa[raquete[1]+x][raquete[2]]=6;
		if(mapa[raquete[1]-x][raquete[2]]==5)
			mapa[raquete[1]-x][raquete[2]]=6;	
		if(mapa[raquete[1]][raquete[2]+x]==5)
			mapa[raquete[1]][raquete[2]+x]=6;	
		if(mapa[raquete[1]][raquete[2]-x]==5)
			mapa[raquete[1]][raquete[2]-x]=6;
	}	
}



// Função callback chamada pela GLUT a cada intervalo de tempo
void DinamicaDoJogo(int value)
{
	if(estado == pausado){
		//chamada recursiva para gerar dinamica do jogo
		glutTimerFunc(500, DinamicaDoJogo,1);   
		glutPostRedisplay();
		return;
	}

	//contagem do tempo da raquete, a explosão acontece em -1
	if(raquete[0]==-1){
		raquete[0]=0;
		mapa[raquete[1]][raquete[2]]=0;
		Explosao();
	}
	if(raquete[0]<-1)
	raquete[0]++;

	
	//movimentação dos mosquitos
    for(int n=0;n<6;n++){
		if(mosquito[n][0]){
    		//verifica bloqueio na horizontal 
    		if((mapa[mosquito[n][1]+1][mosquito[n][2]]!=0 && mapa[mosquito[n][1]-1][mosquito[n][2]]==0) || (mapa[mosquito[n][1]+1][mosquito[n][2]]==0 && mapa[mosquito[n][1]-1][mosquito[n][2]]!=0))
			mosquito[n][3] = -mosquito[n][3];

			//verifica bloqueio na vertical
    		if((mapa[mosquito[n][1]][mosquito[n][2]+1]==0 && mapa[mosquito[n][1]][mosquito[n][2]-1])!=0 || (mapa[mosquito[n][1]][mosquito[n][2]+1]!=0 && mapa[mosquito[n][1]][mosquito[n][2]-1]==0))
			mosquito[n][4] = -mosquito[n][4];

			//inverte eixo de movidentação do mosquito se eixo bloqueado
    		if((mapa[mosquito[n][1]+1][mosquito[n][2]] && mapa[mosquito[n][1]-1][mosquito[n][2]] && mosquito[n][3]) || (mapa[mosquito[n][1]][mosquito[n][2]+1]!=0 && mapa[mosquito[n][1]][mosquito[n][2]-1]!=0 && mosquito[n][4])){
				int aux = mosquito[n][3];
				mosquito[n][3] = mosquito[n][4];
    			mosquito[n][4] = aux;
			} 

			//movimenta mosquito
			if(mosquito[n][3] && (mapa[mosquito[n][1] + mosquito[n][3]][mosquito[n][2]]==0)){
				mapa[mosquito[n][1]][mosquito[n][2]]=0;
				mosquito[n][1] += mosquito[n][3];
				mapa[mosquito[n][1]][mosquito[n][2]]=4;
				if(playerX==mosquito[n][1] && playerY==mosquito[n][2])
					Morreu();
			}
			else{
				if(mosquito[n][4] && (mapa[mosquito[n][1]][mosquito[n][2] + mosquito[n][4]]==0)){
					mapa[mosquito[n][1]][mosquito[n][2]]=0;
					mosquito[n][2] += mosquito[n][4];
					mapa[mosquito[n][1]][mosquito[n][2]]=4;
					if(playerX==mosquito[n][1] && playerY==mosquito[n][2])
						Morreu();
				}
			}
				
		}
		
	}
	
	
	//chamada recursiva para gerar dinamica do jogo
	glutTimerFunc(500, DinamicaDoJogo,1);   
	glutPostRedisplay();
}

void DesenhaMapa(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(15.0f, 0.0f, 0.0f, 15.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for(int i=0;i<15;i++){
		for(int j=0;j<15;j++){
			if(mapa[i][j]==2)
				DesenhaBloco(i,j);
			if(mapa[i][j]==1 || mapa[i][j]==5)
				DesenhaParede(i,j);
			if(mapa[i][j]==6)
				DesenhaBonus(i,j);
		}
	}
	
	DesenhaVidas();
	DesenhaPontuacao();
	DesenhaMosquitos();
	DesenhaRaquete();
	if(estado == jogando || estado == pausado)
		DesenhaPersonagem(playerX, playerY);
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

