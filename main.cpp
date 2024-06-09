#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <string>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#define NUM_AUDIOS 3

using namespace std;

#define playerXinicial 13
#define playerYinicial 1

enum Estado{
	menu,
	gameover,
	pausado,
	jogando
};

typedef struct {
    ma_decoder decoder;
    ma_device device;
} AudioPlayer;

int mapa[15][15];
int playerX= -1;
int playerY= -1;
int mosquito[6][5]; //colunas - ativo, x, y, deslocamento x, deslocamento y
int raquete[3]={0,0,0};  //colunas - ativo , x, y
int pontos=0, vidas=3, potenciaDaRaquete=1, contadorMosquitos=0, mouseX, mouseY;
int menuItem = 0;
Estado estado = menu;
GLuint textureID[40];
AudioPlayer audioPlayers[NUM_AUDIOS];

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    (void)pInput;
}

void initAudio(AudioPlayer* player, const char* filepath) {
    ma_result result;

    result = ma_decoder_init_file(filepath, NULL, &player->decoder);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize decoder for %s.\n", filepath);
        return;
    }

    ma_device_config deviceConfig;
    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = player->decoder.outputFormat;
    deviceConfig.playback.channels = player->decoder.outputChannels;
    deviceConfig.sampleRate        = player->decoder.outputSampleRate;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = &player->decoder;

    result = ma_device_init(NULL, &deviceConfig, &player->device);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize playback device for %s.\n", filepath);
        ma_decoder_uninit(&player->decoder);
        return;
    }
}

void startAudio(AudioPlayer* player) {
    ma_device_start(&player->device);
}

void stopAudio(AudioPlayer* player) {
    ma_device_stop(&player->device);
}

void initializeAudios() {
    // Inicialize os diferentes áudios
    initAudio(&audioPlayers[0], "menu.mp3");
    initAudio(&audioPlayers[1], "gameover.mp3");
    initAudio(&audioPlayers[2], "game.mp3");
}

void cleanupAudios() {
    for (int i = 0; i < NUM_AUDIOS; i++) {
        ma_device_uninit(&audioPlayers[i].device);
        ma_decoder_uninit(&audioPlayers[i].decoder);
    }
}

// Função para carregar a imagem e criar uma textura
void loadTexture(const char* filename, int n) { 
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        // Determina o formato da textura com base no número de canais da imagem
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else {
            printf("Número de canais não suportado\n");
            stbi_image_free(data);
            return;
        }

        // Gera e vincula a textura
        glGenTextures(1, &textureID[n]);
        glBindTexture(GL_TEXTURE_2D, textureID[n]);
        
        // Carrega a textura na GPU
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        
        // Configura os parâmetros de textura
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Libera a memória da imagem carregada
        stbi_image_free(data);
    } else {
        printf("Falha ao carregar a textura\n");
    }
}

// Função de inicialização das texturas
void InicializarTexturas () {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    loadTexture("0.png", 0);
    loadTexture("1.png", 1);
    loadTexture("2.png", 2);
    loadTexture("3.png", 3);
    loadTexture("4.png", 4);
    loadTexture("5.png", 5);
    loadTexture("6.png", 6);
    loadTexture("7.png", 7);
    loadTexture("8.png", 8);
    loadTexture("9.png", 9);
    loadTexture("menu.jpg",10);
    loadTexture("gameover.jpg", 11);
    loadTexture("grama.jpg", 12);
    loadTexture("pedra.jpg", 13);
    loadTexture("parede.png", 14);
    loadTexture("jogador.png", 15);
    loadTexture("mosquito.png", 16);
    loadTexture("raquete.png", 17);
    loadTexture("coracao.png", 18);
    loadTexture("raio.png", 19);
    loadTexture("bateria.png", 20);
    loadTexture("pause.png", 21);
    loadTexture("placar.png", 22);
    loadTexture("mosquitinho.png", 23);
}


void DesenhaBloco(float i, float j){
	GLfloat a=i, b=j;
	glBindTexture(GL_TEXTURE_2D, textureID[13]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(a+1, b+1);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(a, b+1);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(a, b);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(a+1, b);
    glEnd();
}

void DesenhaVidas(){
	int a, b=14;
	if(vidas>=1){
		a=13;
		glBindTexture(GL_TEXTURE_2D, textureID[18]);
	    glBegin(GL_QUADS);
	    glTexCoord2f(0.0f, 0.0f); glVertex2f(a+1, b+1);
	    glTexCoord2f(1.0f, 0.0f); glVertex2f(a, b+1);
	    glTexCoord2f(1.0f, 1.0f); glVertex2f(a, b);
	    glTexCoord2f(0.0f, 1.0f); glVertex2f(a+1, b);
	    glEnd();
	}
	if(vidas>=2){
		a=12;
		glBindTexture(GL_TEXTURE_2D, textureID[18]);
	    glBegin(GL_QUADS);
	    glTexCoord2f(0.0f, 0.0f); glVertex2f(a+1, b+1);
	    glTexCoord2f(1.0f, 0.0f); glVertex2f(a, b+1);
	    glTexCoord2f(1.0f, 1.0f); glVertex2f(a, b);
	    glTexCoord2f(0.0f, 1.0f); glVertex2f(a+1, b);
	    glEnd();
	}
	if(vidas==3){
		a=11;
		glBindTexture(GL_TEXTURE_2D, textureID[18]);
	    glBegin(GL_QUADS);
	    glTexCoord2f(0.0f, 0.0f); glVertex2f(a+1, b+1);
	    glTexCoord2f(1.0f, 0.0f); glVertex2f(a, b+1);
	    glTexCoord2f(1.0f, 1.0f); glVertex2f(a, b);
	    glTexCoord2f(0.0f, 1.0f); glVertex2f(a+1, b);
	    glEnd();
	}
}

void DesenhaPontos(){
	
	if(estado==jogando || estado==pausado){
		glBindTexture(GL_TEXTURE_2D, textureID[22]);
	    glBegin(GL_QUADS);
	    glTexCoord2f(0.0f, 0.0f); glVertex2f(3.3f, 15.0f);
	    glTexCoord2f(1.0f, 0.0f); glVertex2f(0.7f, 15.0f);
	    glTexCoord2f(1.0f, 1.0f); glVertex2f(0.7f, 14.0f);
	    glTexCoord2f(0.0f, 1.0f); glVertex2f(3.3f, 14.0f);
	    glEnd();
	}
    
	glBindTexture(GL_TEXTURE_2D, textureID[pontos%10]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(1.5f, 15.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(01.0f, 15.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(01.0f, 14.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(1.5f, 14.0f);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, textureID[(pontos / 10) % 10]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(2.0f, 15.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.5f, 15.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.5f, 14.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(2.0f, 14.0f);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, textureID[(pontos / 100) % 10]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(2.5f, 15.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(2.0f, 15.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(2.0f, 14.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(2.5f, 14.0f);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, textureID[(pontos / 1000) % 10]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(3.0f, 15.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(2.5f, 15.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(2.5f, 14.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(3.0f, 14.0f);
    glEnd();
	
}

void DesenhaParede(float i, float j){
	GLfloat a=i, b=j;
    glBindTexture(GL_TEXTURE_2D, textureID[14]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(a+1, b+1);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(a, b+1);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(a, b);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(a+1, b);
    glEnd();
}

void DesenhaBonus(float i, float j){
	GLfloat a=i, b=j;
	glBindTexture(GL_TEXTURE_2D, textureID[20]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(a+1, b+1);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(a, b+1);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(a, b);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(a+1, b);
    glEnd();
	
}

void DesenhaPersonagem(int i, int j){
	GLfloat a=i, b=j;
	glBindTexture(GL_TEXTURE_2D, textureID[15]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(a+1, b+1);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(a, b+1);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(a, b);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(a+1, b);
    glEnd();
}

void DesenhaRaquete(){	
	GLfloat a=raquete[1], b=raquete[2];
	if(raquete[0]){
		glBindTexture(GL_TEXTURE_2D, textureID[17]);
	    glBegin(GL_QUADS);
	    glTexCoord2f(0.0f, 0.0f); glVertex2f(a+1, b+1);
	    glTexCoord2f(1.0f, 0.0f); glVertex2f(a, b+1);
	    glTexCoord2f(1.0f, 1.0f); glVertex2f(a, b);
	    glTexCoord2f(0.0f, 1.0f); glVertex2f(a+1, b);
	    glEnd();
	}
	
	//raio da explosao
	glBindTexture(GL_TEXTURE_2D, textureID[19]);
	if(raquete[0]==-1){
		for(int x=1;x<=potenciaDaRaquete;x++){
		glBegin(GL_QUADS);
    	glTexCoord2f(0.0f, 0.0f); glVertex2f(x+a+1, b+1);
    	glTexCoord2f(1.0f, 0.0f); glVertex2f(x+a, b+1);
    	glTexCoord2f(1.0f, 1.0f); glVertex2f(x+a, b);
    	glTexCoord2f(0.0f, 1.0f); glVertex2f(x+a+1, b);
    	glEnd();	
    	
    	glBegin(GL_QUADS);
    	glTexCoord2f(0.0f, 0.0f); glVertex2f(a+1-x, b+1);
    	glTexCoord2f(1.0f, 0.0f); glVertex2f(a-x, b+1);
    	glTexCoord2f(1.0f, 1.0f); glVertex2f(a-x, b);
    	glTexCoord2f(0.0f, 1.0f); glVertex2f(a+1-x, b);
    	glEnd();
    	
    	glBegin(GL_QUADS);
    	glTexCoord2f(0.0f, 0.0f); glVertex2f(a+1, b+1-x);
    	glTexCoord2f(1.0f, 0.0f); glVertex2f(a, b+1-x);
    	glTexCoord2f(1.0f, 1.0f); glVertex2f(a, b-x);
    	glTexCoord2f(0.0f, 1.0f); glVertex2f(a+1, b-x);
    	glEnd();
    	
    	glBegin(GL_QUADS);
    	glTexCoord2f(0.0f, 0.0f); glVertex2f(a+1, x+b+1);
    	glTexCoord2f(1.0f, 0.0f); glVertex2f(a, x+b+1);
    	glTexCoord2f(1.0f, 1.0f); glVertex2f(a, x+b);
    	glTexCoord2f(0.0f, 1.0f); glVertex2f(a+1, x+b);
    	glEnd();
		}		
	}
}

void DesenhaMosquitos(){
	for(int x=0;x<6;x++){
		if(mosquito[x][0]){
			GLfloat a=mosquito[x][1], b=mosquito[x][2];
			glBindTexture(GL_TEXTURE_2D, textureID[16]);
		    glBegin(GL_QUADS);
		    glTexCoord2f(0.0f, 0.0f); glVertex2f(a+1, b+1);
		    glTexCoord2f(1.0f, 0.0f); glVertex2f(a, b+1);
		    glTexCoord2f(1.0f, 1.0f); glVertex2f(a, b);
		    glTexCoord2f(0.0f, 1.0f); glVertex2f(a+1, b);
		    glEnd();
		}
	}
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
	vidas = 3;
	playerX= playerXinicial;
	playerY= playerYinicial;
	contadorMosquitos=0;
	InicializaMatriz();
	estado = jogando;
}

void Morreu(){
	if(--vidas){
		playerX= playerXinicial;
		playerY= playerYinicial;
	}else{
		estado = gameover;
		
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

}

void Teclado2(int key, int x, int y){

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
	                                   
	glutPostRedisplay();
}

void GerenciaMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON){
        if (state == GLUT_UP) {     
	        if (x>=238 && x<=466 && y>=510 && y<=565 && (estado==menu || estado==gameover)){
	         	pontos=0;
				ResetaJogo();
			}
	        if (x>=300 && x<=400 && y>=575 && y<=630 && (estado==menu || estado==gameover))
	         	exit(1);      
	        }
	}    
    glutPostRedisplay();
}

void passiveMouseMotion(int x, int y) {
    mouseX=x;
    mouseY=y;
	  
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
				pontos++;
		}	
	}
	
	if(contadorMosquitos==0){
		ResetaJogo();
	}
	
	//destroi jogador
	if((playerX==raquete[1] && (playerY<=raquete[2]+potenciaDaRaquete && playerY>=raquete[2]-potenciaDaRaquete)) || 
	(playerY==raquete[2] && (playerX<=raquete[1]+potenciaDaRaquete && playerX>=raquete[1]-potenciaDaRaquete)))
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
	
	glBindTexture(GL_TEXTURE_2D, textureID[12]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(15.0f, 15.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(0.0f, 15.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(0.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(15.0f, 0.0f);
    glEnd();
	
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
	DesenhaPontos();
	DesenhaMosquitos();
	DesenhaRaquete();
	DesenhaPersonagem(playerX, playerY);
}

void DesenhaMenu(){
	glBindTexture(GL_TEXTURE_2D, textureID[10]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(15.0f, 15.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(0.0f, 15.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(0.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(15.0f, 0.0f);
    glEnd();
    
    
	if (mouseX>=238 && mouseX<=466 && mouseY>=510 && mouseY<=565){
    	glBindTexture(GL_TEXTURE_2D, textureID[23]);
	    glBegin(GL_QUADS);
	    glTexCoord2f(0.0f, 0.0f); glVertex2f(5.1f, 4.05f);
	    glTexCoord2f(1.0f, 0.0f); glVertex2f(3.9f, 4.05f);
	    glTexCoord2f(1.0f, 1.0f); glVertex2f(3.9f, 2.85f);
	    glTexCoord2f(0.0f, 1.0f); glVertex2f(5.1f, 2.85f);
	    glEnd();
	}
    if (mouseX>=300 && mouseX<=400 && mouseY>=575 && mouseY<=630){
    	glBindTexture(GL_TEXTURE_2D, textureID[23]);
	    glBegin(GL_QUADS);
	    glTexCoord2f(0.0f, 0.0f); glVertex2f(6.45f, 2.7f);
	    glTexCoord2f(1.0f, 0.0f); glVertex2f(5.25f, 2.7f);
	    glTexCoord2f(1.0f, 1.0f); glVertex2f(5.25f, 1.5f);
	    glTexCoord2f(0.0f, 1.0f); glVertex2f(6.45f, 1.5f);
	    glEnd();
	}   	      

}

void DesenhaGameover(){

	glBindTexture(GL_TEXTURE_2D, textureID[11]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(15.0f, 15.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(0.0f, 15.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(0.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(15.0f, 0.0f);
    glEnd();
    DesenhaPontos();
    
    if (mouseX>=238 && mouseX<=466 && mouseY>=510 && mouseY<=565){
    	glBindTexture(GL_TEXTURE_2D, textureID[23]);
	    glBegin(GL_QUADS);
	    glTexCoord2f(0.0f, 0.0f); glVertex2f(5.1f, 4.05f);
	    glTexCoord2f(1.0f, 0.0f); glVertex2f(3.9f, 4.05f);
	    glTexCoord2f(1.0f, 1.0f); glVertex2f(3.9f, 2.85f);
	    glTexCoord2f(0.0f, 1.0f); glVertex2f(5.1f, 2.85f);
	    glEnd();
	}
    if (mouseX>=300 && mouseX<=400 && mouseY>=575 && mouseY<=630){
    	glBindTexture(GL_TEXTURE_2D, textureID[23]);
	    glBegin(GL_QUADS);
	    glTexCoord2f(0.0f, 0.0f); glVertex2f(6.45f, 2.7f);
	    glTexCoord2f(1.0f, 0.0f); glVertex2f(5.25f, 2.7f);
	    glTexCoord2f(1.0f, 1.0f); glVertex2f(5.25f, 1.5f);
	    glTexCoord2f(0.0f, 1.0f); glVertex2f(6.45f, 1.5f);
	    glEnd();
	} 
	
}

void DesenhaPausado(){
	glBindTexture(GL_TEXTURE_2D, textureID[21]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(10.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(05.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(05.0f, 05.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(10.0f, 05.0f);
    glEnd();
}

void DesenhaTela(){
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	DesenhaMapa();
	if(estado == menu){
		DesenhaMenu();
		stopAudio(&audioPlayers[1]);
		stopAudio(&audioPlayers[2]);
		startAudio(&audioPlayers[0]);
	}else if(estado == gameover){
		DesenhaGameover();
		stopAudio(&audioPlayers[0]);
		stopAudio(&audioPlayers[2]);
		startAudio(&audioPlayers[1]);
	}else{
		stopAudio(&audioPlayers[0]);
		stopAudio(&audioPlayers[1]);
		startAudio(&audioPlayers[2]);
		if(estado == pausado){
			DesenhaPausado();
		}
	}
	
	glFlush();
	
}

main(int argc, char** argv){
	
	InicializaMatriz(); 
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);  
	glutInitWindowPosition(5,5);     
	glutInitWindowSize(700,700);  
	glutCreateWindow("Racketman");
	InicializarTexturas();
	initializeAudios();
	glutDisplayFunc(DesenhaTela);
	glutSpecialFunc(Teclado2);    
	glutKeyboardFunc(Teclado1); 
	glutMouseFunc(GerenciaMouse);
	glutPassiveMotionFunc(passiveMouseMotion);
	glutTimerFunc(500, DinamicaDoJogo,1); //inicio do loop de dinamica do jogo
	
	glutMainLoop();
	cleanupAudios();
	return 0;
}

