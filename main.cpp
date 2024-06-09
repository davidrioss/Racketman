#include "jogo.h"

#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define NUM_AUDIOS 3

using namespace std;

enum Tela{
	MENU,
	GAMEOVER,
	PAUSE,
	JOGO
};

Tela tela = MENU;
Jogo jogo = Jogo();
int mouseX;
int mouseY;

void resetaJogo(){
	jogo = Jogo();
	tela = JOGO;
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
    loadTexture("menu.jpg", T_MENU);
    loadTexture("gameover.jpg", T_GAMEOVER);
    loadTexture("grama.jpg", T_GRAMA);
    loadTexture("pedra.jpg", T_PEDRA);
    loadTexture("parede.png", T_PAREDE);
    loadTexture("jogador.png", T_JOGADOR);
    loadTexture("mosquito.png", T_MOSQUITO);
    loadTexture("raquete.png", T_RAQUETE);
    loadTexture("coracao.png", T_CORACAO);
    loadTexture("raio.png", T_RAIO);
    loadTexture("bateria.png", T_BATERIA);
    loadTexture("pause.png", T_PAUSE);
    loadTexture("placar.png", T_PLACAR);
    loadTexture("mosquitinho.png", T_MOSQUITINHO);
	loadTexture("tenis.png", T_TENIS);
	loadTexture("raquetebonus.png", T_RAQUETEBONUS);
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
}

void GerenciaMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON){
        if (state == GLUT_UP && (tela == MENU || tela == GAMEOVER)) {     
	        if (x>=238 && x<=466 && y>=510 && y<=565){
				resetaJogo();
			}
	        if (x>=300 && x<=400 && y>=575 && y<=630)
	         	exit(0);      
	        }
	}
}

void passiveMouseMotion(int x, int y) {
    mouseX=x;
    mouseY=y;
	  
    glutPostRedisplay(); 
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

void DesenhaMosquitinho(){
	if (mouseX>=238 && mouseX<=466 && mouseY>=510 && mouseY<=565){
		desenhaTextura(T_MOSQUITINHO, 9.90f, 2.85f, 11.10f, 4.05f);
	}
    if (mouseX>=300 && mouseX<=400 && mouseY>=575 && mouseY<=630){
		desenhaTextura(T_MOSQUITINHO, 8.55f, 1.50f, 9.75f, 2.7f);
	}
}

void DesenhaMenu(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0.0f, 15.0f, 0.0f, 15.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	desenhaTextura(T_MENU, 0.0f, 0.0f, 15.0f, 15.0f);
    DesenhaMosquitinho();
}

void DesenhaGameover(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0.0f, 15.0f, 0.0f, 15.0f);
	glClear(GL_COLOR_BUFFER_BIT);
    DesenhaMosquitinho();
	desenhaTextura(T_GAMEOVER, 0.0f, 0.0f, 15.0f, 15.0f);
    jogo.desenhaPontuacao(false);
}

void DesenhaPausado(){
	jogo.desenha();
	desenhaTextura(T_PAUSE, 5.0f, 5.0f, 10.0f, 10.0f);
}

void DesenhaTela(){
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	switch (tela)
	{
	case MENU:
		DesenhaMenu();
		stopAudio(&audioPlayers[1]);
		stopAudio(&audioPlayers[2]);
		startAudio(&audioPlayers[0]);
		break;
	case GAMEOVER:
		DesenhaGameover();
		stopAudio(&audioPlayers[0]);
		stopAudio(&audioPlayers[2]);
		startAudio(&audioPlayers[1]);
		break;
	case PAUSE:
		stopAudio(&audioPlayers[0]);
		stopAudio(&audioPlayers[1]);
		startAudio(&audioPlayers[2]);
		DesenhaPausado();
		break;
	case JOGO:
		stopAudio(&audioPlayers[0]);
		stopAudio(&audioPlayers[1]);
		startAudio(&audioPlayers[2]);
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
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);  
	glutInitWindowPosition(5,5);     
	glutInitWindowSize(700,700);  
	glutCreateWindow("Racketman");
	glutReshapeFunc(AlteraTamanhoJanela);
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