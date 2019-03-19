#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>
#include "client.h"
#include "ACore.h"

#define LARGURA_TELA 1280
#define ALTURA_TELA 720
#define LARGURA_TELA_JOGO 934
#define ALTURA_TELA_JOGO 934

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE + 100)
#define LOGIN_MAX_SIZE 13
#define HIST_MAX_SIZE 200
#define fps 60
#define MAX_LOG_SIZE 17


ALLEGRO_TIMER *timer = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_FONT *fonte_grande = NULL;
ALLEGRO_FONT *fonte_titulo = NULL;
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_AUDIO_STREAM *musica_fundo = NULL;
ALLEGRO_AUDIO_STREAM *musicaVitoria = NULL;
ALLEGRO_AUDIO_STREAM *musicaDerrota = NULL;
ALLEGRO_BITMAP *mapa = NULL;
ALLEGRO_BITMAP *frontmapa = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_BITMAP *fundo_pergaminho = NULL;
ALLEGRO_BITMAP *telaError = NULL;
ALLEGRO_KEYBOARD *teclado = NULL;
ALLEGRO_EVENT_QUEUE *eventsQueue2=NULL;
ALLEGRO_EVENT_QUEUE *evento_ip=NULL;
ALLEGRO_SAMPLE *escolha = NULL;
ALLEGRO_SAMPLE *som_escolha = NULL;
ALLEGRO_BITMAP *botao1 = NULL;
ALLEGRO_BITMAP *botao1_pressionado = NULL;
ALLEGRO_BITMAP *botao2 = NULL;
ALLEGRO_BITMAP *botao2_pressionado = NULL;
ALLEGRO_BITMAP *setinha_dir = NULL;
ALLEGRO_BITMAP *setinha_esq = NULL;
ALLEGRO_BITMAP *capaceteAzul_1_menu = NULL;
ALLEGRO_BITMAP *capaceteVerm_1_menu = NULL;
ALLEGRO_BITMAP *capaceteAzul_2_menu = NULL;
ALLEGRO_BITMAP *capaceteVerm_2_menu = NULL;
ALLEGRO_BITMAP *capaceteAzul_1 = NULL;
ALLEGRO_BITMAP *capaceteVerm_1 = NULL;
ALLEGRO_BITMAP *capaceteAzul_2 = NULL;
ALLEGRO_BITMAP *capaceteVerm_2 = NULL;
ALLEGRO_BITMAP *capaceteAzul_1C = NULL;
ALLEGRO_BITMAP *capaceteVerm_1C = NULL;
ALLEGRO_BITMAP *capaceteAzul_2C = NULL;
ALLEGRO_BITMAP *capaceteVerm_2C = NULL;
ALLEGRO_BITMAP *vitoriaVermelho = NULL;
ALLEGRO_BITMAP *vitoriaAzul = NULL;
ALLEGRO_BITMAP *capacetes = NULL;
ALLEGRO_BITMAP *trap = NULL;
ALLEGRO_BITMAP *ice = NULL;
ALLEGRO_BITMAP *shuriken = NULL;
ALLEGRO_BITMAP *congelado = NULL;
ALLEGRO_BITMAP *pronto = NULL;
ALLEGRO_BITMAP *nao_pronto = NULL;
ALLEGRO_BITMAP *idPlayer = NULL;
ALLEGRO_BITMAP *logo = NULL;

ALLEGRO_EVENT evento;
PROTOCOLO_INICIAL sendPlayer, rcvPlayer;
PROTOCOLO_JOGO estado_jogo;
Traps armadilhas[20];
int tempocongelado=20;
int auxtempo=0;
int tp = 0;
Player jogador;
Player jogadores[6], jogadoresServer[6];
char loginP[14], ip[30], recebeuTrap='n';
int sair = 0, loopLobby = 1;
bool login = true;
bool ipAd = true;
int current_x = 0, current_y = 0;
int checkType = 1;
int idCLient, j;
int tecla, ret, i, n, capaEscolhido;
char flag='s', congelou='n';
bool redraw=true;
PROTOCOLO_JOGO msg;
PROTOCOLO_TESTE teste_recebe;
int sairLobby = 1,opcao = 1, server=0, jogadorReady = 0, qntJogadores = 0, jogoInicio = 1, x_tela = 0,y_tela = 0, aux;


void error_msg(char *text);
int menu();
void carrega_arquivos();
int iniciar();
void startScreen();
void runGame();
void endGame();
void readInputIP(ALLEGRO_EVENT event, char str[], int limit);
void readInput2(ALLEGRO_EVENT event, char str[], int limit);
void readLogin();
void readHelmet();
int readIP();
void lobby();
void printTimer();
void printStatus(Player jogador);
void timeAzulGanhou(int id);
void timeVermGanhou(int id);

int main(){
    int ret, oi = 0;

    iniciar();
    startScreen();
    //printf("%lf", sizeof(PROTOCOLO_JOGO));
    ret = menu();
    if(ret ==1){
        while(loopLobby)
            lobby();
        al_destroy_display(janela);
        janela = al_create_display(912+22,912+22);
        al_destroy_audio_stream(musica_fundo);
        // al_clear_to_color(al_map_rgb(0,255,0));
        while(sair){
            //puts("gamee");
            runGame();
        }
        endGame();
    }
    else if(ret == 2){
    }            
    else if(ret == 3){
    }
    al_flip_display();

    return 0;
}

// ================================================================================== //
// ===================================== FUNÇÕES ==================================== //

void error_msg(char *text){
	ALLEGRO_DISPLAY *error = NULL;
    error = al_create_display(400, 300);
    al_set_window_position(error,860,600);
}

int menu(){
    int ret = 0;
    al_draw_bitmap(fundo,0,0,0);
    al_draw_bitmap(botao1,545,240+40,0);
    al_draw_bitmap(botao1,545,240+120,0);
    al_draw_bitmap(botao1,545,320+120,0);
    al_draw_bitmap(setinha_dir,510,285,0);
    al_draw_bitmap(setinha_esq,730,285,0);
    fonte_grande = al_load_font("./app/Resources/Fontes/kenvector_future_thin.ttf", 40, 0);
    al_draw_text(fonte,al_map_rgb(5,5,5),645,290,ALLEGRO_ALIGN_CENTER,"Iniciar o jogo");
    al_draw_text(fonte,al_map_rgb(5,5,5),645,370,ALLEGRO_ALIGN_CENTER,"Tutorial");
    al_draw_text(fonte,al_map_rgb(5,5,5),645,450,ALLEGRO_ALIGN_CENTER,"Creditos");
    al_flip_display();
    // al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    int sairMenu = 0,opcao = 1, server=0;
    while(!sairMenu){
        //enquanto esc nao for pressionado
        while(!al_event_queue_is_empty(fila_eventos)){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos,&evento);
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                //verifica qual tecla foi pressionada
                switch(evento.keyboard.keycode){
                    //seta para cima
                    case ALLEGRO_KEY_UP:
                        if(opcao > 1){
                            opcao -= 1;
                        }
                        break;
                    //Baixo
                    case ALLEGRO_KEY_DOWN:
                        if(opcao < 3 ){
                            opcao += 1;
                            }
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                    //esc = sair do loop.
                        endGame();
                        break;

                    case ALLEGRO_KEY_ENTER:
                    //enter pra selecionar a opcao
                        if(opcao == 1){
                            readLogin();
                            readHelmet();
                            server = readIP();

                            ret=1;
                            sairMenu=1;
                        }
                        else if(opcao == 2){
                            al_draw_bitmap(fundo,0,0,0);
                            al_draw_text(fonte_titulo,al_map_rgb(255,204,0),LARGURA_TELA/2,ALTURA_TELA/3-45,ALLEGRO_ALIGN_CENTER,"Controles:");
                            al_draw_text(fonte_grande,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+10,ALLEGRO_ALIGN_CENTER,"Movimentacao:");
                            al_draw_text(fonte,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+60,ALLEGRO_ALIGN_CENTER,"W - Mover para cima");
                            al_draw_text(fonte,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+90,ALLEGRO_ALIGN_CENTER,"S - Mover para baixo");
                            al_draw_text(fonte,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+120,ALLEGRO_ALIGN_CENTER,"A -Mover para esquerda");
                            al_draw_text(fonte,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+150,ALLEGRO_ALIGN_CENTER,"D - Mover para direita");
                            al_draw_text(fonte_grande,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+190,ALLEGRO_ALIGN_CENTER,"Taticos:");
                            al_draw_text(fonte,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+240,ALLEGRO_ALIGN_CENTER,"SPACE - Solta a trap");
                            al_draw_text(fonte,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+270,ALLEGRO_ALIGN_CENTER,"J - Congela o adversario ao redor");
                            al_flip_display();
                            while(!sairMenu){
                                //enquanto esc nao for pressionado
                                if(!al_event_queue_is_empty(fila_eventos)){
                                    ALLEGRO_EVENT evento;
                                    al_wait_for_event(fila_eventos,&evento);
                                    if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                                        //verifica qual tecla foi pressionada
                                        switch(evento.keyboard.keycode){
                                            case ALLEGRO_KEY_ESCAPE:
                                            //esc = sair do loop.
                                                menu();
                                                break;

                                            case ALLEGRO_KEY_ENTER:
                                            //enter pra selecionar a opcao
                                            menu();
                                            break;

                                            default:
                                                break;
                                        }
                                    }
                                    else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                                        endGame();
                                    }
                                }
                            }
                            ret=1;
                            sairMenu=1;
                        }
                        else if(opcao == 3){
                            al_draw_bitmap(fundo,0,0,0);
                            al_draw_text(fonte_titulo,al_map_rgb(255,204,0),LARGURA_TELA/2,ALTURA_TELA/3-45,ALLEGRO_ALIGN_CENTER,"Equipe: Devassa");
                            al_draw_text(fonte_grande,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+10,ALLEGRO_ALIGN_CENTER,"BACK-END:");
                            al_draw_text(fonte,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+60,ALLEGRO_ALIGN_CENTER,"Elisson Rodrigo");
                            al_draw_text(fonte,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+90,ALLEGRO_ALIGN_CENTER,"Natalia Soares");
                            al_draw_text(fonte,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+120,ALLEGRO_ALIGN_CENTER,"Matheus Viana");
                            al_draw_text(fonte,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+150,ALLEGRO_ALIGN_CENTER,"Lucas Grisi");
                            al_draw_text(fonte_grande,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+175,ALLEGRO_ALIGN_CENTER,"FRONT-END:");
                            al_draw_text(fonte,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+230,ALLEGRO_ALIGN_CENTER,"Pedro Sarmento");
                            al_draw_text(fonte,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+260,ALLEGRO_ALIGN_CENTER,"Arthur Henrique");
                            al_draw_text(fonte_grande,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+290,ALLEGRO_ALIGN_CENTER,"FEATURING:");
                            al_draw_text(fonte,al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/3+340,ALLEGRO_ALIGN_CENTER,"Gabriel de Melo");
                            al_flip_display();
                            int sairMenu = 0,opcao = 1, server=0;
                            while(!sairMenu){
                                //enquanto esc nao for pressionado
                                while(!al_event_queue_is_empty(fila_eventos)){
                                    ALLEGRO_EVENT evento;
                                    al_wait_for_event(fila_eventos,&evento);
                                    if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                                        //verifica qual tecla foi pressionada
                                        switch(evento.keyboard.keycode){
                                            case ALLEGRO_KEY_ESCAPE:
                                            //esc = sair do loop.
                                                menu();
                                                break;

                                            case ALLEGRO_KEY_ENTER:
                                            //enter pra selecionar a opcao
                                                menu();
                                            default:
                                                break;
                                        }
                                    }
                                    else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                                        endGame();
                                    }
                                }
                            }
                            ret=1;
                            sairMenu=1;
                        }

                    default:
                        break;
                }
            }
            else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                endGame();
            }
            if(opcao && !server){
                fonte = al_load_font("./app/Resources/Fontes/kenvector_future.ttf", 20, 0);
                switch(opcao){
                    case 1:
                    //caso a opcao seja 1, printar a seta em cima do "iniciar o jogo"
                        al_draw_bitmap(fundo_pergaminho,0,0,0);
                        al_draw_bitmap(botao1_pressionado,545,280,0);
                        al_draw_bitmap(botao1,545,360,0);
                        al_draw_bitmap(botao1,545,440,0);
                        al_draw_bitmap(setinha_esq,730,285,0);
                        al_draw_bitmap(setinha_dir,510,285,0);
                        al_draw_text(fonte,al_map_rgb(5,5,5),645,290,ALLEGRO_ALIGN_CENTER,"Iniciar o jogo");
                        al_draw_text(fonte,al_map_rgb(5,5,5),645,370,ALLEGRO_ALIGN_CENTER,"Tutorial");
                        al_draw_text(fonte,al_map_rgb(5,5,5),645,450,ALLEGRO_ALIGN_CENTER,"Creditos");
                        al_play_sample(escolha,1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                        break;
                    case 2:
                    //caso a opcao seja 2, printar a seta em cima de tutorial
                        al_draw_bitmap(fundo_pergaminho,0,0,0);
                        al_draw_bitmap(botao1,545,240+40,0);
                        al_draw_bitmap(botao1_pressionado,545,240+120,0);
                        al_draw_bitmap(botao1,545,320+120,0);
                        al_draw_bitmap(setinha_esq,730,368,0);
                        al_draw_bitmap(setinha_dir,510,368,0);
                        al_draw_text(fonte,al_map_rgb(5,5,5),645,290,ALLEGRO_ALIGN_CENTER,"Iniciar o jogo");
                        al_draw_text(fonte,al_map_rgb(5,5,5),645,370,ALLEGRO_ALIGN_CENTER,"Tutorial");
                        al_draw_text(fonte,al_map_rgb(5,5,5),645,450,ALLEGRO_ALIGN_CENTER,"Creditos");
                        al_play_sample(escolha,1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                        break;
                    case 3:
                    //caso a opcao seja 3, printar a seta em cima de creditos
                        al_draw_bitmap(fundo_pergaminho,0,0,0);
                        al_draw_bitmap(botao1,545,280,0);
                        al_draw_bitmap(botao1,545,360,0);
                        al_draw_bitmap(botao1_pressionado,545,320+120,0);
                        al_draw_bitmap(setinha_esq,730,450,0);
                        al_draw_bitmap(setinha_dir,510,450,0);
                        al_draw_text(fonte,al_map_rgb(5,5,5),645,290,ALLEGRO_ALIGN_CENTER,"Iniciar o jogo");
                        al_draw_text(fonte,al_map_rgb(5,5,5),645,370,ALLEGRO_ALIGN_CENTER,"Tutorial");
                        al_draw_text(fonte,al_map_rgb(5,5,5),645,450,ALLEGRO_ALIGN_CENTER,"Creditos");
                        al_play_sample(escolha,1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                        break;
                }
                al_flip_display();
            }
        }
    }
    return ret;
}

void carrega_arquivos(){
    //aqui serao colocados todos os arquivos a serem carregados no jogo
    logo = al_load_bitmap("./app/Resources/Characters/logo2.png");
    idPlayer = al_load_bitmap("./app/Resources/Icons/idplay.png");
    vitoriaAzul = al_load_bitmap("./app/Resources/Characters/vitoria_blue.png");
    vitoriaVermelho = al_load_bitmap("./app/Resources/Characters/vitoria_vermelho.png");
    botao1 = al_load_bitmap("./app/Resources/Icons/grey_button03.png");
    botao1_pressionado = al_load_bitmap("./app/Resources/Icons/grey_button04.png");
    fundo = al_load_bitmap("./app/Resources/Icons/Plano_de_fundo.jpg");
    fundo_pergaminho = al_load_bitmap("./app/Resources/Icons/Plano_de_fundo_perg.jpg");
    telaError = al_load_bitmap("./app/Resources/Icons/tela_erro.jpg");
    mapa = al_load_bitmap("./app/Resources/Characters/mapa.jpg");
    frontmapa = al_load_bitmap("./app/Resources/Characters/arvore.png");
    setinha_dir = al_load_bitmap("./app/Resources/Icons/yellow_sliderRight.png");
    setinha_esq = al_load_bitmap("./app/Resources/Icons/yellow_sliderLeft.png");
    fonte = al_load_font("./app/Resources/Fontes/OldLondon.ttf", 24, 0);
    fonte_titulo = al_load_font("./app/Resources/Fontes/kenvector_future_thin.ttf", 44, 0);
    trap = al_load_bitmap("./app/Resources/Characters/trap.png");
    ice = al_load_bitmap("./app/Resources/Etc/congelares.png");
    nao_pronto = al_load_bitmap("./app/Resources/Icons/red_cross.png");
    pronto = al_load_bitmap("./app/Resources/Icons/green_checkmark.png");
    shuriken = al_load_bitmap("./app/Resources/Icons/shuriken.png");
    congelado = al_load_bitmap("./app/Resources/Icons/congelado.png");
    capaceteAzul_1_menu = al_load_bitmap("./app/Resources/capacetes/capacete_azul_1.png");
    capaceteVerm_1_menu = al_load_bitmap("./app/Resources/capacetes/capacete_vermelho_1.png");
    capaceteAzul_2_menu = al_load_bitmap("./app/Resources/capacetes/capacete_azul_2.png");
    capaceteVerm_2_menu = al_load_bitmap("./app/Resources/capacetes/capacete_vermelho_2.png");
    capaceteAzul_1 = al_load_bitmap("./app/Resources/Characters/C1A.png");
    capaceteVerm_1 = al_load_bitmap("./app/Resources/Characters/C1V.png");
    capaceteAzul_2 = al_load_bitmap("./app/Resources/Characters/C2A.png");
    capaceteVerm_2 = al_load_bitmap("./app/Resources/Characters/C2V.png");
    capaceteAzul_1C = al_load_bitmap("./app/Resources/Characters/capacete_azul_1C.png");
    capaceteVerm_1C = al_load_bitmap("./app/Resources/Characters/capacetebugado.png");
    capaceteAzul_2C = al_load_bitmap("./app/Resources/Characters/capacete_azul_2C.png");
    capaceteVerm_2C = al_load_bitmap("./app/Resources/Characters/capacete_vermelho_2C.png");
    if(!botao1 || !botao1_pressionado || !fundo || !mapa || !capaceteVerm_2_menu || !capaceteAzul_2_menu){
        //checar se foram abertos, adicionar o nome do arquivo depois de um ou (||!).
        printf("Nao foi possivel carregar um dos botoes.");
        al_destroy_audio_stream(musica_fundo);
    }
    musicaDerrota = al_load_audio_stream("./app/Resources/Musics/musicaDerrota.ogg",4,1024);
    musicaVitoria = al_load_audio_stream("./app/Resources/Musics/musicaVitoria.ogg",4,1024);
    musica_fundo = al_load_audio_stream("./app/Resources/Musics/Musica_fundo.ogg",4,1024);
    escolha = al_load_sample("./app/Resources/Musics/switch3.ogg");
    if(!musicaDerrota || !escolha){
        //aqui sao apenas os arquivos sonoros.
        puts("Musica nao foi carregada.");
    }
    if(!musicaVitoria || !escolha){
        //aqui sao apenas os arquivos sonoros.
        puts("Musica nao foi carregada.");
    }
    if(!musica_fundo || !escolha){
        //aqui sao apenas os arquivos sonoros.
        puts("Musica nao foi carregada.");
    }
    al_attach_audio_stream_to_mixer(musica_fundo, al_get_default_mixer());
    al_set_audio_stream_playmode(musica_fundo,ALLEGRO_PLAYMODE_LOOP);
}

int iniciar(){
    if(!al_init()){
        printf("Tela nao foi iniciada.");
        return 0;
    }
    if(!al_install_audio()){
        printf("Falha ao iniciar o audio.");
        return 0;
    }
    al_init_font_addon();
    if(!al_init_ttf_addon()){
        printf("Falha ao iniciar o ttf addon.");
        return 0;
    }
    if(!al_init_acodec_addon()){
        printf("Falha ao iniciar Codec de Audio.");
        return 0;
    }
    fonte = al_load_font("arial.ttf", 48, 0);
    if(!al_install_keyboard()){
        printf("Falha ao instalar o teclado.");
        return 0;
    }
    if(!al_init_image_addon()){
        printf("Nao foi possivel iniciar o addon de imagem.");
        return 0;
    }
    if(!al_reserve_samples(5)){
        printf("Nao foi possivel reservar os audios.");
        return 0;
    }
    janela = al_create_display(1280,720);
    if(!janela){
        error_msg("Falha ao iniciar a janela");
        al_destroy_audio_stream(musica_fundo);
        return 0;
    }
    al_set_window_title(janela,"Barra Bandeira");
    fila_eventos = al_create_event_queue();
    eventsQueue2 = al_create_event_queue();
    evento_ip = al_create_event_queue();
     if(!evento_ip){
        error_msg("Erro ao criar fila de eventos.");
        al_destroy_display(janela);
        return 0;
    }
    if(!fila_eventos){
        error_msg("Erro ao criar fila de eventos.");
        al_destroy_display(janela);
        return 0;
    }
    if(!eventsQueue2){
        error_msg("Erro ao criar fila de eventos.");
        al_destroy_display(janela);
        return 0;
    }
    timer = al_create_timer(1.0/fps);
    if(!timer){
        error_msg("Erro ao criar timer.");
        al_destroy_display(janela);
        return 0;
    }
    // if (!al_install_mouse())
    // {
    //     printf("Falha ao inicializar o mouse.\n");
    //     al_destroy_display(janela);
    //     return 0;
    // }
    // if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    // {
    //     printf("Falha ao atribuir ponteiro do mouse.\n");
    //     al_destroy_display(janela);
    //     return 0;
    // }
    al_clear_to_color(al_map_rgb(255,255,255));
    carrega_arquivos();
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(evento_ip, al_get_keyboard_event_source());
    return 1;
}

void startScreen(){
    // printf("%ld", sizeof(PROTOCOLO_JOGO));
    al_draw_bitmap(logo,0,0,0);
    fonte = al_load_font("./app/Resources/Fontes/Minecraft.ttf", 48, 0);
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 640, 600, ALLEGRO_ALIGN_CENTRE, "PRESS ENTER TO START!");
    al_flip_display();
    while(!sair){
            //enquanto esc nao for pressionado
        while(!al_event_queue_is_empty(fila_eventos)){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos,&evento);

            if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
                //verifica qual tecla foi pressionada
                switch(evento.keyboard.keycode){
                    case ALLEGRO_KEY_PAD_ENTER:
                    case ALLEGRO_KEY_ENTER:
                        sair = 1;
                        break;
                }
            }
            else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                endGame();
            }
        }
    }
}

void runGame(){
    al_start_timer(timer);
    if(redraw && al_event_queue_is_empty(fila_eventos)){
        redraw = false;
        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_bitmap(mapa,0+19,0+19,0);
        for(j = 0; j < teste_recebe.tp; j++){
            if(armadilhas[j].team == jogador.team){
                al_draw_bitmap(trap,armadilhas[j].posiT.y*19,armadilhas[j].posiT.x*19,0);
            }            
        }
    
        for(i = 0; i < qntJogadores; i++){
            if(jogadores[i].id >= 0){
                if(jogadores[i].team == 1){
                    if(jogadores[i].helmet == 1){
                        if(jogadores[i].estaCongelado){
                            al_draw_bitmap(capaceteAzul_1C,jogadores[i].position.x,jogadores[i].position.y,0);
                        }
                        else {
                            al_draw_bitmap(capaceteAzul_1,jogadores[i].position.x,jogadores[i].position.y,0);
                        }
                    }
                    else{
                        if(jogadores[i].estaCongelado){
                            al_draw_bitmap(capaceteAzul_2C,jogadores[i].position.x,jogadores[i].position.y,0);
                        }
                        else {
                            al_draw_bitmap(capaceteAzul_2,jogadores[i].position.x,jogadores[i].position.y,0);
                        }
                    }
                }
                else{
                    if(jogadores[i].helmet == 1){
                        if(jogadores[i].estaCongelado){
                            al_draw_bitmap(capaceteVerm_1C,jogadores[i].position.x,jogadores[i].position.y,0);
                        }
                        else {
                            al_draw_bitmap(capaceteVerm_1,jogadores[i].position.x,jogadores[i].position.y,0);
                        }
                    }
                    else{
                        if(jogadores[i].estaCongelado){
                            al_draw_bitmap(capaceteVerm_2C,jogadores[i].position.x,jogadores[i].position.y,0);
                        }
                        else{
                            al_draw_bitmap(capaceteVerm_2,jogadores[i].position.x,jogadores[i].position.y,0);
                        }
                    }
                }
            }
            if(jogadores[i].id == idCLient){
                al_draw_bitmap(idPlayer,msg.todosJogadores[i].posicaoPrint.x+4,msg.todosJogadores[i].posicaoPrint.y-10,0);
            }
        }
        printStatus(jogadoresServer[idCLient]);
        al_draw_bitmap(frontmapa,19,19,0);
        if(teste_recebe.ganhou=='s'){
            for(i=0;i<qntJogadores;i++){
                if(jogadores[i].comBandeira){
                    if(jogadores[i].team==1){
                        puts("azul ganhou");
                        timeAzulGanhou(i);
                    } 
                    else{
                        puts("vermelho ganhou");
                        timeVermGanhou(i);
                    } 
                }
            }
        }
        if(teste_recebe.todosJogadores[idCLient].estaCongelado == 1){
            printTimer();
        }
        else{
            auxtempo = 0;
            tempocongelado = 5;
        }
        al_flip_display();
    }
    msg.tipo = GAME;
    while(!al_event_queue_is_empty(fila_eventos)){
        al_wait_for_event(fila_eventos,&evento);
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
            if(!jogador.estaCongelado){
                //verifica qual tecla foi pressionada
                switch(evento.keyboard.keycode){
                    case ALLEGRO_KEY_UP:
                    case ALLEGRO_KEY_W:
          
                        msg.tipo = ANDAR_CIMA;
                        checkType = 0;
                        for(i = 0; i < qntJogadores; i++){
                            msg.todosJogadores[i] = jogadoresServer[i]; 
                        }
                        ret = sendMsgToServer((PROTOCOLO_JOGO *) &msg, sizeof(PROTOCOLO_JOGO));
                        if(ret != SERVER_DISCONNECTED){
                            checkType = 0;
                        }
                        else{
                            puts("server disconnected");
                            endGame();
                        }
                        break;

                    case ALLEGRO_KEY_DOWN:
                    case ALLEGRO_KEY_S:
                        msg.tipo= ANDAR_BAIXO;
                        checkType = 0;
                        for(i = 0; i < qntJogadores; i++){
                            msg.todosJogadores[i] = jogadoresServer[i]; 
                        }
                        ret = sendMsgToServer((PROTOCOLO_JOGO *) &msg, sizeof(PROTOCOLO_JOGO));
                        if(ret != SERVER_DISCONNECTED){
                            checkType = 0;
                        }
                        else{
                            endGame();
                        }
                        break;

                    case ALLEGRO_KEY_LEFT:
                    case ALLEGRO_KEY_A:
                        msg.tipo = ANDAR_ESQUERDA;
                        checkType = 0;
                        for(i = 0; i < qntJogadores; i++){
                            msg.todosJogadores[i] = jogadoresServer[i]; 
                        }
                        ret = sendMsgToServer((PROTOCOLO_JOGO *) &msg, sizeof(PROTOCOLO_JOGO));
                        if(ret != SERVER_DISCONNECTED){
                            checkType = 0;
                        }
                        else{
                            endGame();
                        }
                        break;

                    case ALLEGRO_KEY_RIGHT:
                    case ALLEGRO_KEY_D:
                        msg.tipo= ANDAR_DIREITA;
                        checkType = 0;
                        for(i = 0; i < qntJogadores; i++){
                            msg.todosJogadores[i] = jogadoresServer[i]; 
                        }
                        ret = sendMsgToServer((PROTOCOLO_JOGO *) &msg, sizeof(PROTOCOLO_JOGO));
                        if(ret != SERVER_DISCONNECTED){
                            checkType = 0;
                        }
                        else{
                            endGame();
                        }
                        break;

                    case ALLEGRO_KEY_SPACE:
                        msg.tipo= BOTARTRAPS;
                
                        for(i = 0; i < qntJogadores; i++){
                            msg.todosJogadores[i] = jogadoresServer[i]; 
                        }
                      
                        ret = sendMsgToServer((PROTOCOLO_JOGO *) &msg, sizeof(PROTOCOLO_JOGO));
                        if(ret != SERVER_DISCONNECTED){
                            checkType = 0;
                        }
                        else{
                            endGame();
                        }                        
                        break;

                    case ALLEGRO_KEY_J:
                        msg.tipo = CONGELA;
                        for(i = 0; i < qntJogadores; i++){
                            msg.todosJogadores[i] = jogadoresServer[i]; 
                        }
                        ret = sendMsgToServer((PROTOCOLO_JOGO *) &msg, sizeof(PROTOCOLO_JOGO));
                        if(ret != SERVER_DISCONNECTED){
                            checkType = 0;
                        }
                        else{
                            endGame();
                        }
                        break;

                    case ALLEGRO_KEY_ESCAPE:
                        msg.tipo = ENDGAME;
                        ret = sendMsgToServer((PROTOCOLO_JOGO *) &msg, sizeof(PROTOCOLO_JOGO));
                        if(ret != SERVER_DISCONNECTED){
                            sair = 0;
                        }
                        break;
                    default:                      
                        break;
                }                
            }            
        }
        else if(evento.type == ALLEGRO_EVENT_TIMER){
            redraw = true;
            if(jogadores[idCLient].estaCongelado == 1){
                auxtempo++;
                if(auxtempo==fps){
                    auxtempo=0;
                    tempocongelado--;
                }
                if(tempocongelado <= 0){
                    congelou='n';
                    jogadores[idCLient].estaCongelado = 0;
                    jogadoresServer[idCLient].estaCongelado = 0;
                    msg.tipo = DESCONGELA;
                        for(i = 0; i < qntJogadores; i++){
                            msg.todosJogadores[i] = jogadoresServer[i]; 
                        }
                        ret = sendMsgToServer((PROTOCOLO_JOGO *) &msg, sizeof(PROTOCOLO_JOGO));
                        if(ret != SERVER_DISCONNECTED){
                            checkType = 0;
                        }
                        else{
                            endGame();
                    }
                }
            }
        }
        
        n = recvMsgFromServer((PROTOCOLO_TESTE *) &teste_recebe, DONT_WAIT);
        if(n!=NO_MESSAGE){
            

            if(teste_recebe.tipo == 'G'){                   
                if(teste_recebe.acao == 'c'){
                    for(i = 0; i < qntJogadores; i++){
                        jogadoresServer[i] = teste_recebe.todosJogadores[i];
                        jogadores[i] = teste_recebe.todosJogadores[i];
                        if(i == teste_recebe.id_acao && congelou == 'n'){
                            jogadores[i].position.x = (teste_recebe.todosJogadores[i].posicaoPrint.x);
                            jogadores[i].position.y = (teste_recebe.todosJogadores[i].posicaoPrint.y - 19);
                            jogadoresServer[i].position.x -= 1;
                            if(teste_recebe.todosJogadores[i].estaCongelado) {
                                congelou='s';
                            }
                        } 
                        else{
                            jogadores[i].position.x = (teste_recebe.todosJogadores[i].posicaoPrint.x);
                            jogadores[i].position.y = (teste_recebe.todosJogadores[i].posicaoPrint.y);
                            congelou = 'n';
                        }
                        jogadoresServer[i].posicaoPrint = jogadores[i].position;
                        msg.todosJogadores[i] = jogadoresServer[i];   
                    }
                    msg.tipo = -1;
                }
                else if(teste_recebe.acao == 'b'){
                    for(i = 0; i < qntJogadores; i++){
                        jogadoresServer[i] = teste_recebe.todosJogadores[i];
                        jogadores[i] = teste_recebe.todosJogadores[i];
                        if(i == teste_recebe.id_acao && congelou=='n'){
                            jogadores[i].position.x = (teste_recebe.todosJogadores[i].posicaoPrint.x);
                            jogadores[i].position.y = (teste_recebe.todosJogadores[i].posicaoPrint.y + 19);
                            jogadoresServer[i].position.x += 1;
                            if(teste_recebe.todosJogadores[i].estaCongelado) congelou='s';
                        }
                        else{
                            jogadores[i].position.x = (teste_recebe.todosJogadores[i].posicaoPrint.x);
                            jogadores[i].position.y = (teste_recebe.todosJogadores[i].posicaoPrint.y);
                            congelou = 'n';
                        }
                        jogadoresServer[i].posicaoPrint = jogadores[i].position;
                        msg.todosJogadores[i] = jogadoresServer[i];
                        
                    }
                    msg.tipo = -1;
                }
                else if(teste_recebe.acao == 'e'){
                    for(i = 0; i < qntJogadores; i++){
                        jogadoresServer[i] = teste_recebe.todosJogadores[i];
                        jogadores[i] = teste_recebe.todosJogadores[i];
                        if(i == teste_recebe.id_acao && congelou=='n'){
                            jogadores[i].position.x = (teste_recebe.todosJogadores[i].posicaoPrint.x - 19);
                            jogadores[i].position.y = (teste_recebe.todosJogadores[i].posicaoPrint.y);
                            jogadoresServer[i].position.y -= 1;
                            if(teste_recebe.todosJogadores[i].estaCongelado) congelou='s';
                        } 
                        else{
                            jogadores[i].position.x = (teste_recebe.todosJogadores[i].posicaoPrint.x);
                            jogadores[i].position.y = (teste_recebe.todosJogadores[i].posicaoPrint.y);
                            congelou = 'n';
                        }
                        
                        jogadoresServer[i].posicaoPrint = jogadores[i].position;
                        msg.todosJogadores[i] = jogadoresServer[i];          
                    }
                    msg.tipo = -1;
                }
                else if(teste_recebe.acao == 'd'){
                    for(i = 0; i < qntJogadores; i++){
                        jogadoresServer[i] = teste_recebe.todosJogadores[i];
                        jogadores[i] = teste_recebe.todosJogadores[i];
                        if(i == teste_recebe.id_acao && congelou=='n'){
                            jogadores[i].position.x = (teste_recebe.todosJogadores[i].posicaoPrint.x + 19);
                            jogadores[i].position.y = (teste_recebe.todosJogadores[i].posicaoPrint.y);
                            jogadoresServer[i].position.y += 1;
                            if(teste_recebe.todosJogadores[i].estaCongelado) congelou='s';
                        }
                         
                        else{
                            jogadores[i].position.x = (teste_recebe.todosJogadores[i].posicaoPrint.x);
                            jogadores[i].position.y = (teste_recebe.todosJogadores[i].posicaoPrint.y);
                            congelou = 'n';
                        }
                    
                        jogadoresServer[i].posicaoPrint = jogadores[i].position;
                        msg.todosJogadores[i] = jogadoresServer[i];
                        
                    }
                    msg.tipo = -1;
                }
                else if(teste_recebe.acao == 'n'){
                     for(i = 0; i < qntJogadores; i++){
                        jogadoresServer[i] = teste_recebe.todosJogadores[i];
                        jogadores[i] = teste_recebe.todosJogadores[i];
                        jogadores[i].position.x = (teste_recebe.todosJogadores[i].posicaoPrint.x);
                        jogadores[i].position.y = (teste_recebe.todosJogadores[i].posicaoPrint.y);
                        jogadoresServer[i].posicaoPrint = jogadores[i].position;
                        msg.todosJogadores[i] = jogadoresServer[i];
                     }
                     congelou = 'n';
                     msg.tipo = -1;
                }
                else if(teste_recebe.acao == 't'){
                    for(i=0;i<=tp;i++){
                        armadilhas[i].posiT.x = teste_recebe.traps[i].posiT.x;
                        armadilhas[i].posiT.y = teste_recebe.traps[i].posiT.y;
                        armadilhas[i].team = teste_recebe.traps[i].team;
                    }
                    tp++;
                    for(i = 0; i < qntJogadores; i++){
                        jogadoresServer[i] = teste_recebe.todosJogadores[i];
                        jogadores[i] = teste_recebe.todosJogadores[i]; 
                        if(i == teste_recebe.id_acao){
                            jogadores[i].position.x = (teste_recebe.todosJogadores[i].posicaoPrint.x);
                            jogadores[i].position.y = (teste_recebe.todosJogadores[i].posicaoPrint.y);
                            jogadores[i].armadilhas -= 1;
                        } 
                        else{
                            jogadores[i].position.x = (teste_recebe.todosJogadores[i].posicaoPrint.x);
                            jogadores[i].position.y = (teste_recebe.todosJogadores[i].posicaoPrint.y);
                        }
                        jogadoresServer[i].posicaoPrint = jogadores[i].position;
                        jogadoresServer[i].armadilhas = jogadores[i].armadilhas;
                        msg.todosJogadores[i] = jogadoresServer[i];

                    }
                    msg.tipo = -1;
                }

                if(jogador.id == teste_recebe.id_acao){
                    ret = sendMsgToServer((PROTOCOLO_JOGO *) &msg, sizeof(PROTOCOLO_JOGO));

                    if(ret != SERVER_DISCONNECTED){
                        checkType = 0;
                    }
                    else{
                        endGame();
                    }
                }
            }
        }
    }
}

void printTimer(){
    int min;
    int seg;
    min = tempocongelado/60;
    seg = tempocongelado%60;
    if(seg<10){
        al_draw_textf(fonte,al_map_rgb(0,0,0),ALTURA_TELA_JOGO/2 + 10,LARGURA_TELA_JOGO/2,ALLEGRO_ALIGN_CENTRE,"%i:0%i",min,seg);
    }
    else{
        al_draw_textf(fonte,al_map_rgb(0,0,0),ALTURA_TELA_JOGO/2 + 10,LARGURA_TELA_JOGO/2,ALLEGRO_ALIGN_CENTRE,"%i:%i",min,seg);
    }
}

void printStatus(Player jogador){
    al_draw_bitmap(trap,38,39,0);
    al_draw_textf(fonte,al_map_rgb(0,0,0),80,39,ALLEGRO_ALIGN_CENTRE,"%d",jogador.armadilhas);
    al_draw_bitmap(ice,38,62,0);
    al_draw_textf(fonte,al_map_rgb(0,0,0),80,62,ALLEGRO_ALIGN_CENTRE,"%d",jogador.congelamentos);
}

void timeAzulGanhou(int c){
    if(msg.todosJogadores[idCLient].team == 1){
        al_attach_audio_stream_to_mixer(musicaVitoria, al_get_default_mixer());
        al_set_audio_stream_playmode(musicaVitoria,ALLEGRO_PLAYMODE_LOOP);
    }else{
        al_attach_audio_stream_to_mixer(musicaDerrota, al_get_default_mixer());
        al_set_audio_stream_playmode(musicaDerrota,ALLEGRO_PLAYMODE_LOOP);
    }
    al_draw_bitmap(vitoriaAzul,912/4,912/4,0);
    // al_draw_textf(fonte_grande, al_map_rgb(0, 0, 0), 912/2, 912/2, ALLEGRO_ALIGN_CENTRE, "Time Azul Ganhou! Carry: %s", jogadores[c].name);
    al_flip_display();
    al_rest(10.0);

    al_destroy_display(janela);
    al_destroy_audio_stream(musicaVitoria);
    al_destroy_audio_stream(musicaDerrota);
    al_destroy_event_queue(fila_eventos);
}

void timeVermGanhou(int c){
    if(msg.todosJogadores[idCLient].team == 2){
        al_attach_audio_stream_to_mixer(musicaVitoria, al_get_default_mixer());
        al_set_audio_stream_playmode(musicaVitoria,ALLEGRO_PLAYMODE_LOOP);
    }else{
        al_attach_audio_stream_to_mixer(musicaDerrota, al_get_default_mixer());
        al_set_audio_stream_playmode(musicaDerrota,ALLEGRO_PLAYMODE_LOOP);
    }
    al_draw_bitmap(vitoriaVermelho, 912/5, 912/5, 0);
    // al_draw_textf(fonte_grande, al_map_rgb(0, 0, 0), 912/2, 912/2, ALLEGRO_ALIGN_CENTRE, "Time Vermelho Ganhou! Carry: %s", jogadores[c].name);
    al_flip_display();
    al_rest(10.0);

    al_destroy_display(janela);
    al_destroy_audio_stream(musicaVitoria);
    al_destroy_audio_stream(musicaDerrota);
    al_destroy_event_queue(fila_eventos);
}

void endGame(){
    al_destroy_display(janela);
    janela = al_create_display(1280, 720);
    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_bitmap(telaError,0,0,0);
    al_flip_display();
    al_rest(2.0);

    al_destroy_display(janela);
    al_destroy_audio_stream(musicaVitoria);
    al_destroy_audio_stream(musicaDerrota);
    al_destroy_event_queue(fila_eventos);
}

void readInputIP(ALLEGRO_EVENT event, char str[], int limit){
    // puts("entrou");
    if (event.type == ALLEGRO_EVENT_KEY_CHAR){
        if (strlen(str) <= limit){
            char temp[] = {event.keyboard.unichar, '\0'};
            if (event.keyboard.unichar == ' '){
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= '!' && event.keyboard.unichar <= '?'){
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= 'A' && event.keyboard.unichar <= 'Z'){
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= 'a' && event.keyboard.unichar <= 'z'){
                strcat(str, temp);
            }
        }
        if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0){
            str[strlen(str) - 1] = '\0';
        }
        al_draw_bitmap(fundo_pergaminho,0,0,0);
        // al_draw_bitmap(espaco_branco,ALTURA_TELA/2,LARGURA_TELA/4+42,0);
        //al_draw_bitmap(fundo,0,0,0);
        fonte = al_load_font("./app/Resources/Fontes/kenvector_future.ttf", 48, 0);
        al_draw_text(fonte_grande, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "IP adress: ");
        al_draw_textf(fonte_grande, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2, ALLEGRO_ALIGN_CENTRE, "%s", ip);
        al_flip_display();
    }
}

void readInput2(ALLEGRO_EVENT event, char str[], int limit){
    if (event.type == ALLEGRO_EVENT_KEY_CHAR){
        if (strlen(str) <= limit){
            char temp[] = {event.keyboard.unichar, '\0'};
            if (event.keyboard.unichar == ' '){
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= '!' && event.keyboard.unichar <= '?'){
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= 'A' && event.keyboard.unichar <= 'Z'){
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= 'a' && event.keyboard.unichar <= 'z'){
                strcat(str, temp);
            }
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0)
        {
            str[strlen(str) - 1] = '\0';
        }
        al_draw_bitmap(fundo_pergaminho,0,0,0);
        // al_draw_bitmap(espaco_branco,ALTURA_TELA/2,LARGURA_TELA/4+42,0);
        fonte = al_load_font("./app/Resources/Fontes/kenvector_future.ttf", 48, 0);
        al_draw_text(fonte_grande, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "Login: ");
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2, ALLEGRO_ALIGN_CENTRE, "%s", loginP);
        al_flip_display();
    }
}

void readLogin(){
    // al_draw_bitmap(fundo,0,0,0);
    fonte = al_load_font("./app/Resources/Fontes/kenvector_future.ttf", 48, 0);
    //al_draw_bitmap(espaco_branco,ALTURA_TELA/2,LARGURA_TELA/4+42,0);
    al_draw_text(fonte_grande, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "Login: ");
    al_flip_display();
    ALLEGRO_EVENT_QUEUE *eventsQueue3;
    eventsQueue3 = al_create_event_queue();
    if(!eventsQueue3){
        puts("Erro ao criar fila de eventos.");
        al_destroy_display(janela);
    }

    while(login){
        while(!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT loginEvent;
            al_wait_for_event(fila_eventos, &loginEvent);
            readInput2(loginEvent, loginP, LOGIN_MAX_SIZE);
            if (loginEvent.type == ALLEGRO_EVENT_KEY_DOWN){
                switch(loginEvent.keyboard.keycode){
                    case ALLEGRO_KEY_PAD_ENTER:
                    case ALLEGRO_KEY_ENTER:
                        if(strlen(loginP) > 1){
                            strcpy(jogador.name, loginP);
                            puts(jogador.name);
                            login = false;
                        }else{
                            al_clear_to_color(al_map_rgb(255,255,255));
                           // al_draw_bitmap(espaco_branco,ALTURA_TELA/2,LARGURA_TELA/4+42,0);
                            al_draw_text(fonte_grande, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "Login: ");
                            al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2, ALLEGRO_ALIGN_CENTRE, "%s", loginP);
                            al_draw_text(fonte, al_map_rgb(255,0,0), LARGURA_TELA/2, ALTURA_TELA/4, ALLEGRO_ALIGN_CENTRE, "Digite um nick de no minimo 2 caracteres!");
                            al_flip_display();
                            al_rest(2.0);
                        }
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        menu();
                        break;
                }
            }
            else if(loginEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                endGame();
            }
        }
    }
}

void readHelmet(){
    int capaCheck=1;
    int foi=1;
    al_draw_bitmap(fundo_pergaminho,0,0,0);
    //al_draw_bitmap(fundo,0,0,0);
    fonte = al_load_font("./app/Resources/Fontes/kenvector_future.ttf", 32, 0);
    al_flip_display();
    while(capaCheck){
        while(!al_is_event_queue_empty(fila_eventos)){
            al_draw_bitmap(fundo,0,0,0);
            al_draw_text(fonte_grande, al_map_rgb(255, 255, 255), LARGURA_TELA/2, ALTURA_TELA/2, ALLEGRO_ALIGN_CENTRE, "Escolha seu capacete");
            al_draw_bitmap(capaceteAzul_1_menu, 100,100,0);
            al_draw_bitmap(capaceteVerm_1_menu, 980,100,0);
            al_draw_bitmap(capaceteAzul_2_menu, 100,340,0);
            al_draw_bitmap(capaceteVerm_2_menu, 980,340,0);
            al_flip_display();
            ALLEGRO_EVENT evento;
            int tecla;
            al_wait_for_event(fila_eventos,&evento);
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                //verifica qual tecla foi pressionada
                switch(evento.keyboard.keycode){
                    case ALLEGRO_KEY_1:
                        tecla = 1;
                        foi=0;
                        break;
                    case ALLEGRO_KEY_2:
                        tecla = 2;
                        foi=0;
                        break;
                    //esc. sair=1 faz com que o programa saia do loop principal
                    case ALLEGRO_KEY_PAD_1:
                        tecla=1;
                        foi=0;
                        break;
                    case ALLEGRO_KEY_PAD_2:
                        tecla=2;
                        foi=0;
                        break;
                }
            }
            if(!foi){
                al_draw_bitmap(fundo,0,0,0);
                al_draw_text(fonte_grande, al_map_rgb(255, 255, 255), LARGURA_TELA/2, ALTURA_TELA/2, ALLEGRO_ALIGN_CENTRE, "Tem certeza?");
                al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA/2, ALTURA_TELA/2+50, ALLEGRO_ALIGN_CENTRE, "1 - Sim     2 - Nao");

                if (tecla == 1) {
                    al_draw_bitmap(capaceteAzul_1_menu, 100,200,0);
                    al_draw_bitmap(capaceteVerm_1_menu, 980,200,0);
                }
                else{
                    al_draw_bitmap(capaceteAzul_2_menu,100,200,0);
                    al_draw_bitmap(capaceteVerm_2_menu,980,200,0);
                }
                al_flip_display();
                ALLEGRO_EVENT evento1;
                al_wait_for_event(fila_eventos,&evento1);
                if (evento1.type == ALLEGRO_EVENT_KEY_DOWN){
                    //verifica qual tecla foi pressionada
                    switch(evento1.keyboard.keycode){
                        case ALLEGRO_KEY_1:
                            foi=1;
                            capaCheck=0;
                            break;
                        case ALLEGRO_KEY_2:
                            foi=1;
                            break;
                        case ALLEGRO_KEY_PAD_1:
                            foi=1;
                            capaCheck=0;
                            break;
                        case ALLEGRO_KEY_PAD_2:
                            foi=1;
                            break;
                    }
                }
            }
            capaEscolhido=tecla;
            jogador.helmet=capaEscolhido;
        }
    }
}

int readIP(){
    enum conn_ret_t serverConnection;
    al_draw_bitmap(fundo_pergaminho,0,0,0);
    // al_draw_bitmap(espaco_branco,ALTURA_TELA/2,LARGURA_TELA/4+42,0);
    fonte = al_load_font("./app/Resources/Fontes/kenvector_future.ttf", 48, 0);
    al_draw_text(fonte_grande, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "IP adress: ");
    al_flip_display();
    while(ipAd){
        while(!al_is_event_queue_empty(evento_ip) && ipAd){
            ALLEGRO_EVENT ipEvent;
            al_wait_for_event(fila_eventos, &ipEvent);
            readInputIP(ipEvent, ip, LOGIN_MAX_SIZE);
            if (ipEvent.type == ALLEGRO_EVENT_KEY_DOWN){
                switch(ipEvent.keyboard.keycode){
                    case ALLEGRO_KEY_ENTER:
                        puts(ip);
                        ipAd = false;
                        // puts(ipAd);
                        break;
                    case ALLEGRO_KEY_PAD_ENTER:
                        puts(ip);
                        ipAd = false;
                        break;
                }
            }
            if(ipEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                ipAd = false;
            }
        }
        serverConnection = connectToServer(ip);
        if(serverConnection == SERVER_UP){
            sendPlayer.jogador = jogador;
            sendPlayer.tipo = INICIAL;
            int ret = sendMsgToServer((PROTOCOLO_INICIAL *) &sendPlayer, sizeof(PROTOCOLO_INICIAL));
            recvMsgFromServer((PROTOCOLO_INICIAL *) &rcvPlayer, WAIT_FOR_IT);
            if(rcvPlayer.tipo==INICIAL){
                // puts("funfou");
                checkType = 0;
                jogador = rcvPlayer.jogador;
                idCLient = jogador.id;

            }
            if (ret == SERVER_DISCONNECTED){
                exit(1);
            }
            return 1;
        }
        else if(serverConnection == SERVER_DOWN){
            // al_draw_bitmap(fundo,0,0,0);
            // al_draw_text(fonte, al_map_rgb(255,0,0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "Servidor fora do ar, tente novamente");
            // al_flip_display();
            endGame();
            //al_rest(2.0);
            ipAd=true;
            strcpy(ip, "");
        }
        else if(serverConnection == SERVER_FULL){
            al_draw_bitmap(fundo,0,0,0);
            al_draw_text(fonte, al_map_rgb(255,0,0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "Servidor cheio!");
            al_flip_display();
            al_rest(2.0);
            ipAd=true;
            strcpy(ip, "");
        }
        else if(serverConnection == SERVER_CLOSED){
            al_draw_bitmap(fundo,0,0,0);
            al_draw_text(fonte, al_map_rgb(255,0,0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "Servidor fechado!");
            al_flip_display();
            al_rest(2.0);
            ipAd=true;
            strcpy(ip, "");
        }
        else if(serverConnection == SERVER_TIMEOUT){
            al_draw_bitmap(fundo,0,0,0);
            al_draw_text(fonte, al_map_rgb(255,0,0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_LEFT, "Tempo excedido! Tente novamente");
            al_flip_display();
            al_rest(2.0);
            ipAd=true;
            strcpy(ip, "");
        }
        al_flip_display();
    }

    /* --------------------------- Selecionar o capacete do personagem ---------------------------
    if(jogador.team == 1){
        if(capaEscolhido == 1){
            jogador.helmet = capaceteAzul_1;
        }
        else if(capaEscolhido == 2){
            jogador.helmet = capaceteAzul_2;
        }
    }
    else if(jogador.team == 2){
        if(capaEscolhido == 1){
            jogador.helmet = capaceteVerm_1;
        }
        else if(capaEscolhido == 2){
            jogador.helmet = capaceteVerm_2;
        }
    }
      --------------------------- Fim da funcao ---------------------------*/
    return 0;
}

void lobby(){
    int ret;
    PROTOCOLO_JOGO estado_lobby;
    //aqui puxa a tela inicial.
    al_clear_to_color(al_map_rgb(255,255,255));
    al_flip_display();
    al_clear_to_color(al_map_rgb(0,0,0));
    fonte = al_load_font("./app/Resources/Fontes/kenvector_future.ttf", 20, 0);
    al_flip_display();
    al_draw_bitmap(fundo,0,0,0);
    al_draw_text(fonte,al_map_rgb(255,255,255),640,290,ALLEGRO_ALIGN_CENTRE,"O Jogo esta pronto para ser iniciado");
    al_draw_text(fonte,al_map_rgb(255,255,255),640,360,ALLEGRO_ALIGN_CENTRE,"Pressione enter quando estiver preparado.");
    al_draw_text(fonte,al_map_rgb(255,255,255),640,420,ALLEGRO_ALIGN_CENTRE,"Pronto");
    al_draw_bitmap(nao_pronto,630,465,0);
    al_flip_display();
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    sairLobby = 1;
    while(sairLobby){
        sendPlayer.tipo = -1;
        while(!al_event_queue_is_empty(fila_eventos) && !jogador.ready){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos,&evento);
            if(!jogadorReady){
                if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                    switch(evento.keyboard.keycode){
                        case ALLEGRO_KEY_ESCAPE:
                            puts("esc");
                            sairLobby = 0;
                            sendPlayer.jogador=jogador;
                            sendPlayer.tipo=ENDGAME;
                            ret = sendMsgToServer((PROTOCOLO_INICIAL *) &sendPlayer, sizeof(PROTOCOLO_INICIAL));
                            if(ret == SERVER_DISCONNECTED){
                                al_clear_to_color(al_map_rgb(255,255,255));
                                al_draw_text(fonte,al_map_rgb(5,5,5),645,450,ALLEGRO_ALIGN_CENTER,"SERVER DOWN :'(");
                                al_flip_display();
                                al_rest(1.0);
                            }
                            endGame();
                            exit(1);
                            break;

                        case ALLEGRO_KEY_ENTER:
                            al_clear_to_color(al_map_rgb(0,0,0));
                            al_flip_display();
                            al_draw_bitmap(fundo,0,0,0);
                            al_draw_text(fonte,al_map_rgb(255,255,255),640,290,ALLEGRO_ALIGN_CENTRE,"O Jogo esta pronto para ser iniciado");
                            al_draw_text(fonte,al_map_rgb(255,255,255),640,360,ALLEGRO_ALIGN_CENTRE,"Pressione enter quando estiver preparado.");
                            al_draw_text(fonte,al_map_rgb(255,255,255),640,420,ALLEGRO_ALIGN_CENTRE,"Pronto");
                            al_draw_bitmap(pronto,630,465,0);
                            al_flip_display();
                            jogadorReady = 1;
                            sairLobby = 0;
                        break;

                        default:
                            break;
                    }
                }
            }
            else{
                if(!jogador.ready){
                    sendPlayer.tipo = COMECOU;
                    sendPlayer.jogador = jogador;
                    ret = sendMsgToServer((PROTOCOLO_INICIAL *) &sendPlayer, sizeof(PROTOCOLO_INICIAL));
                    if(ret != SERVER_DISCONNECTED){
                        int retLobby = recvMsgFromServer((PROTOCOLO_TESTE*) &teste_recebe, WAIT_FOR_IT);
                        if(retLobby > 0){
                            qntJogadores = teste_recebe.qntJogadores;
                            for(i = 0; i < qntJogadores; i++){
                                jogadores[i] = teste_recebe.todosJogadores[i];
                                jogadores[i].position.x = teste_recebe.todosJogadores[i].posicaoPrint.x;
                                jogadores[i].position.y = teste_recebe.todosJogadores[i].posicaoPrint.y;

                                jogadoresServer[i] = teste_recebe.todosJogadores[i];
                            }
                            jogador = teste_recebe.todosJogadores[idCLient];
                            if(teste_recebe.tipo == 'l'){
                                sairLobby = 0;
                                sair=1;
                                loopLobby = 0;
                            }
                        }
                    }
                    else{
                        endGame();
                    }
                }
            }
        }
        if(jogador.ready == 1){
            if(idCLient == 0){
                sairLobby = 0;
                sair=1;
                loopLobby = 0;
            }
            else{
                int retLobby = recvMsgFromServer((PROTOCOLO_TESTE*) &teste_recebe, WAIT_FOR_IT);
                qntJogadores = teste_recebe.qntJogadores;
                if(retLobby > 0){
                    
                    qntJogadores = teste_recebe.qntJogadores;
                    for(i = 0; i < qntJogadores; i++){
                        jogadores[i] = teste_recebe.todosJogadores[i];
                        jogadores[i].position.x *= 19;
                        jogadores[i].position.y *= 19;
                        jogadoresServer[i] = teste_recebe.todosJogadores[i];
                    }
                    jogador = teste_recebe.todosJogadores[idCLient];
                    if(teste_recebe.tipo == 'l'){
                        sairLobby = 0;
                        sair=1;
                        loopLobby = 0;
                    }
                }
            }
        }
    }
}
