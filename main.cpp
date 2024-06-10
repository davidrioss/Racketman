#include "jogo.h"

#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

using namespace std;

enum Tela{
	MENU,
	GAMEOVER,
	PAUSE,
	JOGO
};

Tela tela = MENU;
Jogo jogo;
int mouseX;
int mouseY;

void resetaJogo(){
	jogo = Jogo();
	jogo.carregaFase(0);
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

	auto carregaLista = [](string s, int t, int n){
		int p = s.size() - 1;
		while(p != -1 && s[p] != '_')
			p--;
		if(p == -1){
			printf("%s faltando '_'\n", s.c_str());
			exit(0);
		}
		
		for(int i = 0; i < n; i++){
			s[p] = '1' + i;
			loadTexture(s.c_str(), t + i);
		}
	};

    loadTexture("imagens/0.png", 0);
    loadTexture("imagens/1.png", 1);
    loadTexture("imagens/2.png", 2);
    loadTexture("imagens/3.png", 3);
    loadTexture("imagens/4.png", 4);
    loadTexture("imagens/5.png", 5);
    loadTexture("imagens/6.png", 6);
    loadTexture("imagens/7.png", 7);
    loadTexture("imagens/8.png", 8);
    loadTexture("imagens/9.png", 9);
    loadTexture("imagens/menu.jpg", T_MENU);
    loadTexture("imagens/gameover.jpg", T_GAMEOVER);
    loadTexture("imagens/grama.jpg", T_GRAMA);
    loadTexture("imagens/pedra.jpg", T_PEDRA);
    loadTexture("imagens/parede.png", T_PAREDE);
	carregaLista("imagens/jogador_.png", T_JOGADOR, 3);
	carregaLista("imagens/mosquito0_.png", T_MOSQUITO  , 2);
	carregaLista("imagens/mosquito1_.png", T_MOSQUITO+2, 2);
	carregaLista("imagens/mosquito2_.png", T_MOSQUITO+4, 2);
    loadTexture("imagens/raquete.png", T_RAQUETE);
    loadTexture("imagens/coracao.png", T_CORACAO);
    loadTexture("imagens/raio.png", T_RAIO);
    loadTexture("imagens/bateria.png", T_BATERIA);
    loadTexture("imagens/pause.png", T_PAUSE);
    loadTexture("imagens/placar.png", T_PLACAR);
    loadTexture("imagens/mosquitinho.png", T_MOSQUITINHO);
	loadTexture("imagens/tenis.png", T_TENIS);
	loadTexture("imagens/raquetebonus.png", T_RAQUETEBONUS);
	loadTexture("imagens/porta.png", T_PORTA);
	carregaLista("imagens/jogadorMorrendo_.png", T_JOGADORMORRENDO, 3);
	carregaLista("imagens/mosquitoMorrendo_.png", T_MOSQUITOMORRENDO, 4);
}

void Teclado(unsigned char key, int x, int y)
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

	if(key == 'n'){
		jogo.carregaFase();
	}

	if(key == 'r'){
		jogo.carregaFase(jogo.fase);
	}

	if(key == '1'){
		if(jogo.player.numRaquetes < RAQUETESMAX)
			jogo.player.numRaquetes++;
	}

	if(key == '2'){
		if(jogo.player.potenciaDaRaquete < POTENCIAMAX)
			jogo.player.potenciaDaRaquete++;
	}

	if(key == '3'){
		if(!jogo.player.movendo() && jogo.player.velocidade < VELOCIDADEMAX - 1)
			jogo.player.velocidade++;
	}

	if(key == '4'){
		if(jogo.player.vidas < 5)
			jogo.player.vidas++;
		if(tela == GAMEOVER){
			tela = JOGO;
			jogo.setas.clear();
		}
	}

	//botão enter para começar
	if(key == 13){
		if(tela == MENU || tela == GAMEOVER){
			resetaJogo();
		}
	}
}

void TecladoEspecial(int key, int x, int y){
	if (key == GLUT_KEY_HOME){
		resetaJogo();
		return;
	}

	if(tela == JOGO){
		jogo.tecladoEspecial(key, x, y);
		return;
	}
}

void TecladoEspecialSolto(int key, int x, int y){
	if(tela == JOGO){
		jogo.tecladoEspecialSolto(key, x, y);
		return;
	}
}

bool MouseEmNovo(float x, float y){
	x /= largura;
	y /= altura;
	return 0.33 <= x && x <= 0.67 && 0.73 <= y && y <= 0.81;
}

bool MouseEmSair(float x, float y){
	x /= largura;
	y /= altura;
	return 0.42 <= x && x <= 0.57 && 0.82 <= y && y <= 0.90;
}

void GerenciaMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON){
        if (state == GLUT_UP && (tela == MENU || tela == GAMEOVER)) {     
	        if (MouseEmNovo(x, y))
				resetaJogo();
	        if (MouseEmSair(x, y))
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
	auto tempoInicio = std::chrono::system_clock::now();
	if(tela == JOGO){
		jogo.atualiza();
		if(jogo.player.vidas == 0 && jogo.player.framesAnimacao == 0){
			tela = GAMEOVER;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluOrtho2D(0.0f, 15.0f, 0.0f, 15.0f);
		}
	}

	glutPostRedisplay();
	auto time = chrono::duration_cast<chrono::microseconds>(std::chrono::system_clock::now() - tempoInicio).count();
	auto espera = 1000.0 / FPS - time / 1000.0;
	if(espera <= 0){
		printf("Atrasado %.3lfms, tempo total %.3lfms\n", -espera, time / 1000.0);
		espera = 0.0;
	}
	//chamada recursiva para gerar dinamica do jogo  
	glutTimerFunc(espera, DinamicaDoJogo, frame + 1); 
}

void DesenhaMosquitinho(){
	if (MouseEmNovo(mouseX, mouseY))
		desenhaTextura(T_MOSQUITINHO, 9.90f, 2.85f, 11.10f, 4.05f);
    if (MouseEmSair(mouseX, mouseY))
		desenhaTextura(T_MOSQUITINHO, 8.55f, 1.50f, 9.75f, 2.7f);
}

void DesenhaMenu(){
	stopAudio(&audioPlayers[1]);
	stopAudio(&audioPlayers[2]);
	startAudio(&audioPlayers[0]);

	glClear(GL_COLOR_BUFFER_BIT);
	desenhaTextura(T_MENU, 0.0f, 0.0f, 15.0f, 15.0f);
    DesenhaMosquitinho();
}

void DesenhaGameover(){
	stopAudio(&audioPlayers[0]);
	stopAudio(&audioPlayers[2]);
	startAudio(&audioPlayers[1]);

	glClear(GL_COLOR_BUFFER_BIT);
	desenhaTextura(T_GAMEOVER, 0.0f, 0.0f, 15.0f, 15.0f);
    DesenhaMosquitinho();
    jogo.desenhaPontuacao(true, 11.7f, 13.5f);
}

void DesenhaPausado(){
	jogo.desenha();
	if(jogo.mapa.linhas > jogo.mapa.colunas){
		float x = jogo.mapa.colunas / 3.0f;
		float y = (jogo.mapa.linhas - jogo.mapa.colunas) / 2.0f + x;
		desenhaTextura(T_PAUSE, x, y, 2 * x, x + y);
	}else{
		float y = jogo.mapa.linhas / 3.0f;
		float x = (jogo.mapa.colunas - jogo.mapa.linhas) / 2.0f + y;
		desenhaTextura(T_PAUSE, x, y, x + y, 2 * y);
	}
}

void DesenhaTela(){
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
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
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(tela == JOGO){
		jogo.ajustaEscala();
	}else{
		gluOrtho2D(0.0f, 15.0f, 0.0f, 15.0f);
	}
}

main(int argc, char** argv){
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);  
	glutInitWindowPosition(5,5);     
	glutInitWindowSize(700,700);  
	glutCreateWindow("Racketman");
	glutReshapeFunc(AlteraTamanhoJanela);
	InicializarTexturas();
	initializeAudios();
	glutDisplayFunc(DesenhaTela);
	glutSpecialFunc(TecladoEspecial);
	glutSpecialUpFunc(TecladoEspecialSolto);
	glutKeyboardFunc(Teclado); 
	glutMouseFunc(GerenciaMouse);
	glutPassiveMotionFunc(passiveMouseMotion);
	glutTimerFunc(500, DinamicaDoJogo, 1); //inicio do loop de dinamica do jogo
	
	glutMainLoop();
	cleanupAudios();
	return 0;
}