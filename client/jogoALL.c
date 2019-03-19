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

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE + 100)
#define LOGIN_MAX_SIZE 13
#define HIST_MAX_SIZE 200

#define MAX_LOG_SIZE 17

//gcc Jogo.c -lm -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

ALLEGRO_FONT *fonte = NULL;
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_AUDIO_STREAM *musica_fundo = NULL;
ALLEGRO_BITMAP *personagem = NULL;
ALLEGRO_BITMAP *personagemm = NULL;
ALLEGRO_BITMAP *mapa = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *logo = NULL;
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
ALLEGRO_BITMAP *capacetesAzul1 = NULL;
ALLEGRO_BITMAP *capacetesAzul2 = NULL;
ALLEGRO_BITMAP *capacetesVerm1 = NULL;
ALLEGRO_BITMAP *capacetesVerm2 = NULL;
ALLEGRO_BITMAP *capacetes = NULL;
ALLEGRO_BITMAP *trap = NULL;
ALLEGRO_BITMAP *shuriken = NULL;
ALLEGRO_BITMAP *congelado = NULL;

PROTOCOLO_INICIAL sendPlayer, rcvPlayer;
Player jogador;
Player jogadores[6];
char loginP[14], ip[30];
int sair = 0;
bool login = true;
bool ipAd = true;
int current_x = 0, current_y = 0;
int checkType = 1;

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
void tutorial();
void creditos();
void lobby();


int main(){
    int ret;

    iniciar();
    startScreen();
    ret = menu();
    if(ret ==1){
        lobby();
        while(sair){
            runGame();
        }
        endGame();
    }
    else if(ret == 2){
        tutorial();
    }
    else if(ret == 3){
        creditos();
    }
    al_flip_display();

    return 0;
}


void error_msg(char *text){
	ALLEGRO_DISPLAY *error = NULL;
    error = al_create_display(400, 300);
    al_set_window_position(error,860,600);
}

int menu(){
    int ret;

    //aqui puxa a tela inicial.
    al_clear_to_color(al_map_rgb(255,255,255));
    // al_draw_bitmap(logo,0,0,0);
    al_flip_display();
    // al_rest(5.0);
    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_bitmap(mapa,0,0,0);
    al_flip_display();
    al_draw_bitmap(logo,0,0,0);
    al_flip_display();
    al_draw_bitmap(botao1,545,240+40,0);
    al_draw_bitmap(botao1,545,240+120,0);
    al_draw_bitmap(botao1,545,320+120,0);
    al_draw_bitmap(setinha_dir,510,285,0);
    al_draw_bitmap(setinha_esq,730,285,0);
    fonte = al_load_font("./app/Resources/Fontes/kenvector_future.ttf", 20, 0);
    al_draw_text(fonte,al_map_rgb(5,5,5),645,290,ALLEGRO_ALIGN_CENTER,"Iniciar o jogo");
    al_draw_text(fonte,al_map_rgb(5,5,5),645,370,ALLEGRO_ALIGN_CENTER,"Tutorial");
    al_draw_text(fonte,al_map_rgb(5,5,5),645,450,ALLEGRO_ALIGN_CENTER,"Creditos");

    al_flip_display();
    
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    
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
                        sairMenu = 1;
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
                            ret=2;
                            sairMenu=1;
                        }
                        else if(opcao == 3){
                            ret=3;
                            sairMenu=1;
                        }

                    default:
                        break;
                }
            }
            if(opcao && !server){
                fonte = al_load_font("./app/Resources/Fontes/kenvector_future.ttf", 20, 0);
                switch(opcao){
                    //caso a opcao seja 1, printar a seta em cima do "iniciar o jogo"
                    case 1:
                        al_draw_bitmap(mapa,0,0,0);
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
                        al_draw_bitmap(mapa,0,0,0);
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
                        al_draw_bitmap(mapa,0,0,0);
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
    botao1 = al_load_bitmap("./app/Resources/Icons/grey_button00.png");
    botao1_pressionado = al_load_bitmap("./app/Resources/Icons/grey_button01.png");
    botao2 = al_load_bitmap("./app/Resources/Icons/grey_button03.png");
    botao2_pressionado = al_load_bitmap("./app/Resources/Icons/grey_button04.png");
    logo = al_load_bitmap("./app/Resources/Icons/index.png");
    mapa = al_load_bitmap("./app/Resources/Characters/mapa.jpeg");
    setinha_dir = al_load_bitmap("./app/Resources/Icons/yellow_sliderRight.png");
    setinha_esq = al_load_bitmap("./app/Resources/Icons/yellow_sliderLeft.png");
    personagem = al_load_bitmap("./app/Resources/Characters/Personagem(1).png");
    personagemm = al_load_bitmap("./app/Resources/Characters/Personagem_C3R.png");
    capacetesVerm1 = al_load_bitmap("./app/Resources/capacetes/capacete_vermelho_1.png");
    capacetesVerm2 = al_load_bitmap("./app/Resources/capacetes/capacete_vermelho_2.png");
    capacetesAzul1 = al_load_bitmap("./app/Resources/capacetes/capacete_azul_1.png");
    capacetesAzul2 = al_load_bitmap("./app/Resources/capacetes/capacete_azul_2.png");
    capacetes = al_load_bitmap("./app/Resources/capacetes/chapeusfinalizados.png");
    fonte = al_load_font("./app/Resources/Fontes/OldLondon.ttf", 24, 0);
    trap = al_load_bitmap("./app/Resources/Icons/ice_trap.png");
    shuriken = al_load_bitmap("./app/Resources/Icons/shuriken.png");
    congelado = al_load_bitmap("./app/Resources/Icons/congelado.png");

    musica_fundo = al_load_audio_stream("./app/Resources/Musics/Musica_fundo.ogg",4,1024);
    escolha = al_load_sample("./app/Resources/Musics/switch3.ogg");

    if(!botao1 || !botao1_pressionado || !botao2 || !botao2_pressionado || !logo || !mapa){
        //checar se foram abertos, adicionar o nome do arquivo depois de um ou (||!).
        printf("Nao foi possivel carregar um dos botoes.");
        al_destroy_audio_stream(musica_fundo);
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
    if (!al_install_mouse())
    {
        printf("Falha ao inicializar o mouse.\n");
        al_destroy_display(janela);
        return 0;
    }
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        printf("Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(janela);
        return 0;
    }
    al_clear_to_color(al_map_rgb(255,255,255));
   
    carrega_arquivos();
    

    if(!personagem){
        printf("Nao foi possivel carregar o personagem.");
        al_destroy_audio_stream(musica_fundo);
        return 0;
    }
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(evento_ip, al_get_keyboard_event_source());
    al_register_event_source(evento_ip, al_get_mouse_event_source());
    return 1;
}


void startScreen(){
    al_draw_bitmap(logo,0,0,0);
    fonte = al_load_font("./app/Resources/Fontes/Minecraft.ttf", 48, 0);
    al_draw_text(fonte, al_map_rgb(0, 0, 0), 640, 600, ALLEGRO_ALIGN_CENTRE, "PRESS START!");
    al_flip_display();
    while(!sair){
            //enquanto esc nao for pressionado
        while(!al_event_queue_is_empty(fila_eventos)){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos,&evento);
            
            if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
                //verifica qual tecla foi pressionada
                switch(evento.keyboard.keycode){
                    //seta para cima
                    case ALLEGRO_KEY_SPACE:
                        sair = 1;
                        break;
                }
            }
        }
    }
}

// void menuScreen(){
// }

void runGame(){
    int tecla, ret, i;
    PROTOCOLO_JOGO msg;
    PROTOCOLO_JOGO estado_jogo;

    al_destroy_audio_stream(musica_fundo);

    al_draw_bitmap(mapa,0,0,0);
    al_draw_bitmap(personagem,jogador.position.x,current_y,0);
    // al_flip_display();

    // recvMsgFromServer((PROTOCOLO_JOGO *) &estado_jogo, WAIT_FOR_IT);
    // jogador = estado_jogo.todosJogadores[jogador.id];
    
    
    while(!al_event_queue_is_empty(fila_eventos)){
        recvMsgFromServer((PROTOCOLO_JOGO *) &estado_jogo, WAIT_FOR_IT);
        jogador = estado_jogo.todosJogadores[jogador.id];
        ALLEGRO_EVENT evento;
        ALLEGRO_EVENT evento2;
        for(i=0;i<estado_jogo.qntJogadores;i++){
            if(i!=jogador.id){
                al_draw_bitmap(personagem, estado_jogo.todosJogadores[i].position.x, estado_jogo.todosJogadores[i].position.y, ALLEGRO_FLIP_HORIZONTAL);
            }
        }
        // al_draw_bitmap(personagem)
        al_flip_display();
        al_wait_for_event(fila_eventos,&evento);
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
            if(!jogador.estacongelado){

                //verifica qual tecla foi pressionada
                switch(evento.keyboard.keycode){
                    
                    //seta para cima
                    case ALLEGRO_KEY_W:
                        msg.tipo= ANDAR_CIMA;
                        msg.todosJogadores[0] = jogador;
                    // al_wait_for_event(fila_eventos,&evento2);
                        ret = sendMsgToServer((PROTOCOLO_JOGO *) &msg, sizeof(PROTOCOLO_JOGO));
                        if(ret != SERVER_DISCONNECTED){
                            // al_draw_bitmap(mapa,0,0,0);
                            jogador.position.y-=4;
                            al_draw_bitmap(personagem,jogador.position.x,jogador.position.y,ALLEGRO_FLIP_HORIZONTAL);
                        }
                        break;
                    //Baixo
                    case ALLEGRO_KEY_S:
                        msg.tipo= ANDAR_BAIXO;
                        msg.todosJogadores[0] = jogador;
                        //al_wait_for_event(fila_eventos,&evento2);
                        ret = sendMsgToServer((PROTOCOLO_JOGO *) &msg, sizeof(PROTOCOLO_JOGO));
                        if(ret != SERVER_DISCONNECTED){
                            // al_draw_bitmap(mapa,0,0,0);
                            jogador.position.y+=4;
                            al_draw_bitmap(personagem,jogador.position.x,jogador.position.y,ALLEGRO_FLIP_HORIZONTAL);
                        }
                        break;
                    //Esquerda
                    case ALLEGRO_KEY_A:
                        msg.tipo= ANDAR_ESQUERDA;
                        msg.todosJogadores[0] = jogador;
                        //al_wait_for_event(fila_eventos,&evento2);
                        ret = sendMsgToServer((PROTOCOLO_JOGO *) &msg, sizeof(PROTOCOLO_JOGO));
                        if(ret != SERVER_DISCONNECTED){
                            // al_draw_bitmap(mapa,0,0,0);
                            jogador.position.x-=4;
                            al_draw_bitmap(personagem,jogador.position.x,jogador.position.y,ALLEGRO_FLIP_HORIZONTAL);
                        }
                        break;
                    //Direita.
                    case ALLEGRO_KEY_D:
                        msg.tipo= ANDAR_DIREITA;
                        msg.todosJogadores[0] = jogador;
                        //al_wait_for_event(fila_eventos,&evento2);
                        ret = sendMsgToServer((PROTOCOLO_JOGO *) &msg, sizeof(PROTOCOLO_JOGO));
                        if(ret != SERVER_DISCONNECTED){
                            // al_draw_bitmap(mapa,0,0,0);
                            jogador.position.x+=4;
                            al_draw_bitmap(personagem,jogador.position.x,jogador.position.y,ALLEGRO_FLIP_HORIZONTAL);
                        }
                        break;
                    //bota trap    
                    case ALLEGRO_KEY_SPACE:
                        msg.tipo= BOTARTRAPS;
                        msg.todosJogadores[0] = jogador;
                        //al_wait_for_event(fila_eventos,&evento2);
                        ret = sendMsgToServer((PROTOCOLO_JOGO *) &msg, sizeof(PROTOCOLO_JOGO));

                        if(ret != SERVER_DISCONNECTED && jogador.armadilhas > 0){
                            al_draw_bitmap(trap,jogador.position.x,jogador.position.y,ALLEGRO_FLIP_HORIZONTAL);
                            al_draw_bitmap(personagem,jogador.position.x,jogador.position.y,ALLEGRO_FLIP_HORIZONTAL);
                        }
                        
                        break;

                    case ALLEGRO_KEY_LSHIFT:
                        msg.tipo = CONGELA;
                        msg.todosJogadores[0] = jogador;
                        ret = sendMsgToServer((PROTOCOLO_JOGO *) &msg, sizeof(PROTOCOLO_JOGO));

                        if(ret != SERVER_DISCONNECTED && jogador.congelamentos > 0){
                            //al_draw_bitmap(mapa,0,0,0);
                            al_draw_bitmap(shuriken,jogador.position.x,jogador.position.y,ALLEGRO_FLIP_HORIZONTAL);
                            al_draw_bitmap(personagem,jogador.position.x,jogador.position.y,ALLEGRO_FLIP_HORIZONTAL);
                        }

                        break;
                    //esc. sair=1 faz com que o programa saia do loop principal
                    case ALLEGRO_KEY_ESCAPE:
                        msg.tipo=ENDGAME;
                        ret = sendMsgToServer((PROTOCOLO_JOGO *) &msg, sizeof(PROTOCOLO_JOGO));
                        if(ret != SERVER_DISCONNECTED){
                            sair = 0;
                        }

                }
                
            }
            else{
                // al_draw_bitmap(mapa,0,0,0);
                al_draw_bitmap(congelado,jogador.position.x,jogador.position.y,ALLEGRO_FLIP_HORIZONTAL);
                
                for(i=0;estado_jogo.qntJogadores;i++){
                    if(estado_jogo.todosJogadores[i].congelou && estado_jogo.todosJogadores[i].team==jogador.team){
                        al_draw_bitmap(mapa,0,0,0);
                        al_draw_bitmap(congelado,jogador.position.x,jogador.position.y,ALLEGRO_FLIP_HORIZONTAL);
                    }
                }
            }
            //al_draw_bitmap(mapa,0,0,0);
            // al_flip_display();
            

        }
    }
}

void endGame(){
    al_destroy_display(janela);
    al_destroy_audio_stream(musica_fundo);
    al_destroy_event_queue(fila_eventos);
}

void readInputIP(ALLEGRO_EVENT event, char str[], int limit){
    // puts("entrou");
    if (event.type == ALLEGRO_EVENT_KEY_CHAR)
    {
        if (strlen(str) <= limit)
        {
            char temp[] = {event.keyboard.unichar, '\0'};
            if (event.keyboard.unichar == ' ')
            {
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= '!' &&
                     event.keyboard.unichar <= '?')
            {
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= 'A' &&
                     event.keyboard.unichar <= 'Z')
            {
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= 'a' &&
                     event.keyboard.unichar <= 'z')
            {
                strcat(str, temp);
            }
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0)
        {
            str[strlen(str) - 1] = '\0';
        }
         al_clear_to_color(al_map_rgb(255,255,255));
        //al_draw_bitmap(logo,0,0,0);
        fonte = al_load_font("./app/Resources/Fontes/OldLondon.ttf", 48, 0);
        al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "IP adress: ");
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2, ALLEGRO_ALIGN_CENTRE, "%s", ip);
        al_flip_display();
    }
}

void readInput2(ALLEGRO_EVENT event, char str[], int limit){
    if (event.type == ALLEGRO_EVENT_KEY_CHAR)
    {
        if (strlen(str) <= limit)
        {
            char temp[] = {event.keyboard.unichar, '\0'};
            if (event.keyboard.unichar == ' ')
            {
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= '!' &&
                     event.keyboard.unichar <= '?')
            {
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= 'A' &&
                     event.keyboard.unichar <= 'Z')
            {
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= 'a' &&
                     event.keyboard.unichar <= 'z')
            {
                strcat(str, temp);
            }
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0)
        {
            str[strlen(str) - 1] = '\0';
        }
         al_clear_to_color(al_map_rgb(255,255,255));
        //al_draw_bitmap(logo,0,0,0);
        fonte = al_load_font("./app/Resources/Fontes/OldLondon.ttf", 48, 0);
        al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "Login: ");
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2, ALLEGRO_ALIGN_CENTRE, "%s", loginP);
        al_flip_display();
    }
}

void readLogin(){
    al_clear_to_color(al_map_rgb(255,255,255));
    //al_draw_bitmap(logo,0,0,0);
    fonte = al_load_font("./app/Resources/Fontes/OldLondon.ttf", 48, 0);
    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "Login: ");
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
            
            

            if (loginEvent.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(loginEvent.keyboard.keycode)
                {
                    case ALLEGRO_KEY_ENTER:
                        if(strlen(loginP) > 1){
                            strcpy(jogador.name, loginP);
                            puts(jogador.name);
                            login = false;
                        }else{
                            al_clear_to_color(al_map_rgb(255,255,255));
                            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "Login: ");
                            al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2, ALLEGRO_ALIGN_CENTRE, "%s", loginP);
                            al_draw_text(fonte, al_map_rgb(255,0,0), LARGURA_TELA/2, ALTURA_TELA/4, ALLEGRO_ALIGN_CENTRE, "Digite um nick de no minimo 2 caracteres!");
                            al_flip_display();
                            al_rest(2.0);
                        }
                        break;
                    case ALLEGRO_KEY_PAD_ENTER:
                        if(strlen(loginP) > 1){
                            strcpy(jogador.name, loginP);
                            puts(jogador.name);
                            login = false;
                        }else{
                            al_clear_to_color(al_map_rgb(255,255,255));
                            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "Login: ");
                            al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2, ALLEGRO_ALIGN_CENTRE, "%s", loginP);
                            al_draw_text(fonte, al_map_rgb(255,0,0), LARGURA_TELA/2, ALTURA_TELA/4, ALLEGRO_ALIGN_CENTRE, "Digite um nick de no minimo 2 caracteres!");
                            al_flip_display();
                            al_rest(2.0);
                        }
                        break;
                }
            }

            if(loginEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                login = false;
            }
        }

    }
    
}

void readHelmet(){
    int capaCheck=1;
    int foi=1, capaEscolhido;
    

    al_clear_to_color(al_map_rgb(255,255,255));
    //al_draw_bitmap(logo,0,0,0);
    fonte = al_load_font("./app/Resources/Fontes/OldLondon.ttf", 32, 0);
    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/6, ALLEGRO_ALIGN_CENTRE, "Escolha seu capacete");
    al_draw_bitmap(capacetes,LARGURA_TELA/2, ALTURA_TELA/2,0);
    al_flip_display();

    while(capaCheck){
        while(!al_is_event_queue_empty(fila_eventos)){
            al_clear_to_color(al_map_rgb(255,255,255));
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/6, ALLEGRO_ALIGN_CENTRE, "Escolha seu capacete");           
            al_draw_bitmap(capacetes, LARGURA_TELA/2, ALTURA_TELA/2,0);
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
                    
                    case ALLEGRO_KEY_3:
                        tecla = 3;
                        foi=0;
                        break;
                    
                    case ALLEGRO_KEY_4:
                        tecla = 4;
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
                    case ALLEGRO_KEY_PAD_3:
                        tecla=3;
                        foi=0;
                        break;
                    case ALLEGRO_KEY_PAD_4:
                        tecla=4;
                        foi=0;
                        break;
                }
            }
            if(!foi){
                al_clear_to_color(al_map_rgb(255, 255, 255));
                al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/6, ALLEGRO_ALIGN_CENTRE, "Tem certeza?");            
                al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/5, ALLEGRO_ALIGN_CENTRE, "1-Sim     2-Nao");            
                al_draw_bitmap(capacetes, LARGURA_TELA/2, ALTURA_TELA/2,0);
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
                            capaCheck=0;
                            break;
                    
                    }
                }
            }        
            capaEscolhido=tecla;    
        }
    }
    jogador.helmet=capaEscolhido;
}

int readIP(){
    enum conn_ret_t serverConnection;
    al_clear_to_color(al_map_rgb(255,255,255));
    //al_draw_bitmap(logo,0,0,0);
    fonte = al_load_font("./app/Resources/Fontes/OldLondon.ttf", 48, 0);
    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "IP adress: ");
    al_flip_display();

    while(ipAd){
        
        while(!al_is_event_queue_empty(evento_ip) && ipAd)
        {
            // puts("entrou");
            ALLEGRO_EVENT ipEvent;
            al_wait_for_event(fila_eventos, &ipEvent);
            readInputIP(ipEvent, ip, LOGIN_MAX_SIZE);
            
            

            if (ipEvent.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(ipEvent.keyboard.keycode)
                {
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
            al_clear_to_color(al_map_rgb(255,255,255));
            al_draw_text(fonte, al_map_rgb(0, 100 , 200), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "Servidor conectado!");
            al_flip_display();
            // al_rest(1.0);
            while(checkType){
                recvMsgFromServer((PROTOCOLO_INICIAL *) &rcvPlayer, WAIT_FOR_IT);
                if(rcvPlayer.tipo==INICIAL){
                    checkType=0;
                    jogador = rcvPlayer.jogador;
                } 
            }
            if (ret == SERVER_DISCONNECTED){
                exit(1);
            }
            return 1;
        }
        else if(serverConnection == SERVER_DOWN){
            al_clear_to_color(al_map_rgb(255,255,255));
            al_draw_text(fonte, al_map_rgb(255,0,0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "Servidor fora do ar, tente novamente");
            al_flip_display();
            al_rest(2.0);
            ipAd=true;
            strcpy(ip, "");
        }
        else if(serverConnection == SERVER_FULL){
            al_clear_to_color(al_map_rgb(255,255,255));
            al_draw_text(fonte, al_map_rgb(255,0,0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "Servidor cheio!");
            al_flip_display();
            al_rest(2.0); 
            ipAd=true;
            strcpy(ip, "");       
        }
        else if(serverConnection == SERVER_CLOSED){
            al_clear_to_color(al_map_rgb(255,255,255));
            al_draw_text(fonte, al_map_rgb(255,0,0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "Servidor fechado!");
            al_flip_display();
            al_rest(2.0);
            ipAd=true;
            strcpy(ip, "");
        }
        else if(serverConnection == SERVER_TIMEOUT){
            al_clear_to_color(al_map_rgb(255,255,255));
            al_draw_text(fonte, al_map_rgb(255,0,0), LARGURA_TELA/2, ALTURA_TELA/3, ALLEGRO_ALIGN_LEFT, "Tempo excedido! Tente novamente");
            al_flip_display();
            al_rest(2.0);
            ipAd=true;
            strcpy(ip, "");
        }  
        al_flip_display(); 
    }
    
    return 0;
}

void lobby(){
    int ret;

    //aqui puxa a tela inicial.
    al_clear_to_color(al_map_rgb(255,255,255));
    // al_draw_bitmap(logo,0,0,0);
    al_flip_display();
    // al_rest(5.0);
    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_bitmap(mapa,0,0,0);
    al_flip_display();
    al_draw_bitmap(logo,0,0,0);
    al_flip_display();
    al_draw_bitmap(botao1,545,240+40,0);
    al_draw_bitmap(botao1,545,240+120,0);
    al_draw_bitmap(botao1,545,320+120,0);
    al_draw_bitmap(setinha_dir,510,285,0);
    al_draw_bitmap(setinha_esq,730,285,0);
    fonte = al_load_font("./app/Resources/Fontes/kenvector_future.ttf", 20, 0);
    al_draw_text(fonte,al_map_rgb(5,5,5),645,290,ALLEGRO_ALIGN_CENTER,"Iniciar o jogo");
    al_draw_text(fonte,al_map_rgb(5,5,5),645,370,ALLEGRO_ALIGN_CENTER,"Tutorial");
    al_draw_text(fonte,al_map_rgb(5,5,5),645,450,ALLEGRO_ALIGN_CENTER,"Creditos");

    al_flip_display();
    
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    
    int sairLobby = 1,opcao = 1, server=0;


    while(sairLobby){
       // puts("oi");
        //enquanto esc nao for pressionado
        while(!al_event_queue_is_empty(fila_eventos) && sairLobby){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos,&evento);
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                //verifica qual tecla foi pressionada
                switch(evento.keyboard.keycode){
                    
                    case ALLEGRO_KEY_ESCAPE:
                    //esc = sair do loop.
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
                        sairLobby = 0;
                        jogador.ready=1;
                    break;
                    
                    default:
                        break;
                }
            }

            if(jogador.ready){
                //puts("aqui");
                sendPlayer.tipo=COMECOU;
                sendPlayer.jogador=jogador;
                ret = sendMsgToServer((PROTOCOLO_INICIAL *) &sendPlayer, sizeof(PROTOCOLO_INICIAL));
                if(ret != SERVER_DISCONNECTED){
                    sairLobby = 0;
                    sair=1;
                }
            }
            
        }
    }
}

void tutorial(){

}

void creditos(){

}

