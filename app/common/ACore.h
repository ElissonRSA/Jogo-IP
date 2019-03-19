#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define FPS 60
#define WIDTH  640
#define HEIGHT 480
#define IP_MAX_SIZE 100
#define LOGIN_MAX_SIZE 13

#define CLIENT_TO_SERVER 0
#define CLIENT_TO_CLIENT 1

#define INICIAL 0           // Envio do nick e do capacete
#define COMECOU 1           // Inicio do jogo
#define WAITING 2
#define ANDAR_CIMA 3
#define ANDAR_BAIXO 4
#define ANDAR_DIREITA 5
#define ANDAR_ESQUERDA 6
#define BOTARTRAPS 7
#define GAME 8
#define ENDGAME 9
#define TEMPO 10
#define CONGELA 11
#define DESCONGELA 12


double startingTime;

ALLEGRO_DISPLAY *main_window;
ALLEGRO_EVENT_QUEUE *eventsQueue;

//========================
//FONT AND BITMAP POINTERS
ALLEGRO_FONT *ubuntu;
ALLEGRO_FONT *start;

ALLEGRO_BITMAP *objects;
ALLEGRO_BITMAP *menuScreen;
//========================
//========================


//===============================
//ESTRUTURAS DO JOGO
// Estrutura para a posição do jogador no mapa
typedef struct{
    int x, y;
}Position;

typedef struct{
    int team;
    Position posiT;
}Traps;
// Estrutura do jogador
typedef struct{
    char name[LOGIN_MAX_SIZE];
    int helmet;                      // Indica qual o tipo do capacete que o player selecionou
    int team;                        // Indica o time do player
    int armadilhas;                  // Quantidade de armadilhas que o player possui
    int congelamentos;               // Quantidade de congelamentos que o player pode realizar
    int id;                          // A id do player ao se conectar ao server
    int comBandeira;                 // Se o player está ou não com a bandeira do time adversário
    int estaCongelado;               // Se o player foi ou não congelado por um player adversário
    int congelou;                    // Se o player congelou alguém ou não           
    Position position;               // A posição x e y do player no mapa
    int ready;                       // Verifica se o jogador apertou pra começar ou não
    Position posicaoPrint;
}Player;

// Estrutura para o protocolo de mensagem enviadas entre server/client durante o jogo
typedef struct{
    char tipo;
    char acao;
    Player todosJogadores[6];
    int id_acao;
    int qntJogadores;
    Traps traps[20];
    int tp;
    char ganhou;
}PROTOCOLO_TESTE;

typedef struct{
    int tipo; 
    int xAnterior, yAnterior;
    char itemAnterior;
    char winner;
    Player todosJogadores[6];
    //Player jogadorSofreuAcao;
    int qntJogadores;
}PROTOCOLO_JOGO;

typedef struct{
    char tipo;
    Player jogador;
}PROTOCOLO_ENVIO_CLIENT;

// Estrutura para o protocolo da mensagem  inicial do server para o client
//para poder atualizar o player do client após a inicialização dele no server
typedef struct{
    int tipo;
    Player jogador;
}PROTOCOLO_INICIAL;     
//===============================
//===============================

//MAIN ALLEGRO FUNCTIONS
bool coreInit();
bool windowInit(int W, int H, char title[]);
bool inputInit();
void allegroEnd();

//FPS CONTROL FUNCTIONS
void startTimer();
double getTimer();
void FPSLimit();

//RESOURCE LOADING FUNCTIONS
bool loadGraphics();
bool fontInit();

//INPUT READING FUNCTION
void readInput(ALLEGRO_EVENT event, char str[], int limit);


#endif