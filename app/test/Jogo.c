#include <stdio.h>
#include <string.h>
#include "client.h"
#include "ACore.h"
#include <ctype.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE + 100)
#define LOGIN_MAX_SIZE 13
#define HIST_MAX_SIZE 200

#define MAX_LOG_SIZE 17

#define LARGURA_TELA 640
#define ALTURA_TELA 480



//gcc Jogo.c -lm -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

ALLEGRO_FONT *fonte = NULL;
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_AUDIO_STREAM *musica_fundo = NULL;
ALLEGRO_BITMAP *personagem = NULL;
ALLEGRO_BITMAP *capacetes = NULL;
ALLEGRO_BITMAP *personagemm = NULL;
ALLEGRO_BITMAP *mapa = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *logo = NULL;
ALLEGRO_KEYBOARD *teclado = NULL;

DADOS_LOBBY msg_chat;
int TAM_PROTOCOLO = (int)sizeof(PROTOCOLO_JOGO);
Player jogador;

void error_msg(char *text){
	ALLEGRO_DISPLAY *error = NULL;
    error = al_create_display(400, 300);

    al_set_window_position(error,860,600);
}

void runChat(int *state) {
    char str_buffer[BUFFER_SIZE], type_buffer[MSG_MAX_SIZE] = {0};
    char msg_history[HIST_MAX_SIZE][MSG_MAX_SIZE] = {{0}};
    int type_pointer = 0;
    DADOS_LOBBY msg;
    DADOS_LOBBY msg_server;
    al_clear_to_color(al_map_rgb(255,255,255));
    
    al_flip_display();   
    while (1) {
        // Lê uma tecla digitada
        char ch = getch();
        if (ch == '\n') {
            puts("if1");
            type_buffer[++type_pointer] = '\0';
            strcpy(msg.mensagem, type_buffer);
            int ret = sendMsgToServer((DADOS_LOBBY *)&msg, sizeof(DADOS_LOBBY));
            if (ret == SERVER_DISCONNECTED) {
                puts("saiuu");
                return;
            }
            type_pointer = 0;
            type_buffer[type_pointer] = '\0';
        }
        
        else if (ch == 127 || ch == 8) {
            puts("if2");
            if (type_pointer > 0) {
                --type_pointer;
                type_buffer[type_pointer] = '\0';
            }
        } 
        
        else if (ch != NO_KEY_PRESSED && type_pointer + 1 < MSG_MAX_SIZE) {
            puts("if3");
            type_buffer[type_pointer] = ch;
            type_buffer[++type_pointer] = '\0';
        }

        // Lê uma mensagem do servidor
        int ret = recvMsgFromServer(&msg_server, DONT_WAIT);
        if (ret == SERVER_DISCONNECTED) {
            puts("saiuuu");
            return;
        } 
        else if (ret != NO_MESSAGE && msg_server.tipo == CHAT && msg_server.funcao==CLIENT_TO_CLIENT) {
            puts("nomsg");
            int i;
            for (i = 0; i < HIST_MAX_SIZE - 1; ++i) {
                strcpy(msg_history[i], msg_history[i + 1]);
            }
            strcpy(msg_history[HIST_MAX_SIZE - 1], msg_server.mensagem);
        }
        else if(ret != NO_MESSAGE && msg_server.funcao == COMECOU){
            *state = COMECOU;
            puts("joguinho");
            break; 
        }

        // Printa novo estado no chat
        //system("clear");
        int i;
        al_clear_to_color(al_map_rgb(255,255,255));
        
        al_flip_display();
        for (i = 0; i < HIST_MAX_SIZE; ++i) {
            //puts("");
            al_draw_textf(fonte, al_map_rgb(0, 0, 0), 10 , 10 + (i+1)*50, ALLEGRO_ALIGN_LEFT, "%s", msg_server.mensagem);
        //printf("%s\n", msg_history[i]);
        }
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), 10, ALTURA_TELA + 10, ALLEGRO_ALIGN_LEFT, "Your message: %s\n", msg_history[i]);
        al_flip_display();
        //printf("\nYour message: %s\n", msg.mensagem);
    }
    puts("fora");
}

void lerIP(char * ipAdress){
    char ch;
    int type_pointer = 0;

    al_clear_to_color(al_map_rgb(255,255,255));
    al_draw_text(fonte, al_map_rgb(0, 0, 0), 10, 10, ALLEGRO_ALIGN_LEFT, "IP Adress: ");
    al_flip_display();

    while(1){
        ch = getch();
        //type_pointer++;
        ipAdress = (char *) realloc(ipAdress, (type_pointer + 1)*(sizeof(char)));
        if(ipAdress != NULL){
            if(ch == '\n') {
                ipAdress[++type_pointer] = '\0';
                break;
            } 
            else if (ch == 127 || ch == 8) {
                if (type_pointer > 0) {
                    --type_pointer;
                    ipAdress[type_pointer] = '\0';
                }
            } 
            else if (ch != NO_KEY_PRESSED && type_pointer + 1 < MSG_MAX_SIZE) {
                ipAdress[type_pointer] = ch;
                ipAdress[++type_pointer] = '\0';
                al_clear_to_color(al_map_rgb(255,255,255));
                al_draw_text(fonte, al_map_rgb(0, 0, 0), 10, 10, ALLEGRO_ALIGN_LEFT, "IP Adress: ");
                al_draw_textf(fonte, al_map_rgb(100, 60, 90), 10, 60, ALLEGRO_ALIGN_LEFT, "%s", ipAdress);
                al_flip_display();
                
            }
            //ch = getch();
        }
      
    }
}

void defineNick(){
    puts("dfnick");
    char type_buffer[LOGIN_MAX_SIZE+1] = {0};
    int type_pointer = 0;
    char *ipAdress = NULL;
    enum conn_ret_t serverConnection;

    DADOS_LOBBY msg;
    msg.tipo = NICK;
    msg.funcao = CLIENT_TO_SERVER;

   // janela = al_create_display(1280,720);
   // fonte = al_load_font("./app/Resources/Fontes/OldLondon.ttf", 48, 0);
    //al_clear_to_color(al_map_rgb(255,255,255));
    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTRE, "Nick:");
    al_flip_display();
    //al_rest(10.0);
    while (1) {
        // LER UMA TECLA DIGITADA
        char ch = getch();
        if (ch == '\n') {
            puts("IF1");
            type_buffer[++type_pointer] = '\0';
            strcpy(msg.mensagem, type_buffer);
            type_pointer = 0;
            type_buffer[type_pointer] = '\0';
            puts(type_buffer);
            break;
        } 
        else if (ch == 127 || ch == 8) {
            puts("IF2");
            if (type_pointer > 0) {
                puts("IF2.1");
                --type_pointer;
                type_buffer[type_pointer] = '\0';
            }
        } 
        else if (ch != NO_KEY_PRESSED && type_pointer + 1 < MSG_MAX_SIZE) {
            puts("IF3");
            type_buffer[type_pointer] = ch;
            type_buffer[++type_pointer] = '\0';
            puts(type_buffer);
            
            al_clear_to_color(al_map_rgb(255,255,255));
            //fonte = al_load_font("./app/Resources/Fontes/OldLondon.ttf", 48, 0);
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTRE, "Nick:");
            //fonte = al_load_font("./app/Resources/Fontes/OldLondon.ttf", 30, 0);
            al_draw_textf(fonte, al_map_rgb(100, 60, 90), (LARGURA_TELA / 2) + 80, ALTURA_TELA / 2 + 30, ALLEGRO_ALIGN_CENTRE, "%s", type_buffer);
            al_flip_display();
                       
        }   
    }
    puts("saiu");

    lerIP(ipAdress);

    al_clear_to_color(al_map_rgb(255,255,255));
    al_flip_display();

    serverConnection = connectToServer(ipAdress);

    if(serverConnection == SERVER_UP){
        int ret = sendMsgToServer((DADOS_LOBBY *) &msg, sizeof(DADOS_LOBBY));
        al_draw_text(fonte, al_map_rgb(0, 100 , 200), 10, 10, ALLEGRO_ALIGN_LEFT, "Servidor conectado!");
        if (ret == SERVER_DISCONNECTED) {
            return;
        }
    }
    else if(serverConnection == SERVER_DOWN){
        al_draw_text(fonte, al_map_rgb(255, 0, 0), 10, 10, ALLEGRO_ALIGN_LEFT, "Nao foi possivel encontrar o servidor");
    }
    else if(serverConnection == SERVER_FULL){
        al_draw_text(fonte, al_map_rgb(255, 0, 0), 10, 10, ALLEGRO_ALIGN_LEFT, "O servidor esta cheio");
    }
    else if(serverConnection == SERVER_CLOSED){
        al_draw_text(fonte, al_map_rgb(255, 0, 0), 10, 10, ALLEGRO_ALIGN_LEFT, "o servidor está fechado para conexões");
    }
    else if(serverConnection == SERVER_TIMEOUT){
        al_draw_text(fonte, al_map_rgb(255, 0, 0), 10, 10, ALLEGRO_ALIGN_LEFT, "o servidor demorou para dar uma resposta sobre o status da conexão");
    }  
    al_flip_display(); 
}

void selectHelmet(){
    int foi=1;
    puts("select");
    capacetes = al_load_bitmap("./app/Resources/capacetes/chapeusfinalizados.png");
    puts("deu bom?");

    int capacete;

    DADOS_LOBBY msg;
    msg.tipo = CAPACETE;
    msg.funcao = CLIENT_TO_SERVER;

    al_clear_to_color(al_map_rgb(255,255,255));
    al_flip_display();
    al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, "Selecione o Capacete");
    al_draw_bitmap(capacetes, 0,0,0);
    al_flip_display();
    puts("deu bom");
    if(!al_event_queue_is_empty(fila_eventos)){
        puts("entrou");
        while(foi){
            printf("%d", foi);
            al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, "Selecione o Capacete");
            al_draw_bitmap(capacetes, 0,0,0);
            al_flip_display();
            ALLEGRO_EVENT evento;
            int tecla;

            al_wait_for_event(fila_eventos,&evento);
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                //verifica qual tecla foi pressionada
                switch(evento.keyboard.keycode){
                    //seta para cima
                    case ALLEGRO_KEY_1:
                        tecla = 1;
                        foi=0;
                        break;
                    //Baixo
                    case ALLEGRO_KEY_2:
                        tecla = 2;
                        foi=0;
                        break;
                    //Esquerda
                    case ALLEGRO_KEY_3:
                        tecla = 3;
                        foi=0;
                        break;
                    //Direita.
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
            msg.msg = tecla;  
            printf("%d", foi);
            
        }

        int ret = sendMsgToServer((DADOS_LOBBY *)&msg, sizeof(DADOS_LOBBY));
        puts("enviou");
        if (ret == SERVER_DISCONNECTED) {
            return;
        }
    }
    //al_destroy_font(fonte);
    al_destroy_bitmap(capacetes);
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
    fonte = al_load_font("./app/Resources/Fontes/OldLondon.ttf", 48, 0);
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
  
    musica_fundo = al_load_audio_stream("./app/Resources/Musics/Musica_fundo.ogg",4,1024);
    if(!musica_fundo){
        error_msg("Musica nao foi carregada.");
        return 0;
    }
    al_attach_audio_stream_to_mixer(musica_fundo, al_get_default_mixer());
    al_set_audio_stream_playmode(musica_fundo,ALLEGRO_PLAYMODE_LOOP);
    janela = al_create_display(640,480);
    if(!janela){
        error_msg("Falha ao iniciar a janela");
        al_destroy_audio_stream(musica_fundo);
        return 0;
    }
    al_set_window_title(janela,"Barra Bandeira");
    fila_eventos = al_create_event_queue();
    if(!fila_eventos){
        error_msg("Erro ao criar fila de eventos.");
        al_destroy_display(janela);
        return 0;
    }
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_clear_to_color(al_map_rgb(255,255,255));
    personagem = al_load_bitmap("./app/Resources/Characters/Personagem(1).png");
    personagemm = al_load_bitmap("./app/Resources/Characters/Personagem_C3R.png");
    logo = al_load_bitmap("./app/Resources/Characters/index.png");
    mapa = al_load_bitmap("./app/Resources/Characters/mapa.jpeg");

    if(!personagem){
        printf("Nao foi possivel carregar o personagem.");
        al_destroy_audio_stream(musica_fundo);
        return 0;
    }
    
    //puts("dmankdjhak");
    return 1;
}

void comeca(){
    al_draw_bitmap(logo,0,0,0);
    al_flip_display();
    al_destroy_bitmap(logo);
    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_bitmap(mapa,0,0,0);
    al_flip_display();
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
}

int lobby(){
    int state = 0;
    DADOS_LOBBY msg[1];
    PROTOCOLO_INICIAL msg_inicial;

    puts("lobby");
    defineNick();
    puts("lobby2");
    selectHelmet();
    puts("lobby3");
    int msgOK = recvMsgFromServer((PROTOCOLO_INICIAL *)&msg_inicial, WAIT_FOR_IT);
    if(msgOK == SERVER_DISCONNECTED){
        al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, "Servidor Desconectado!");
    }
    else if(msg_inicial.tipo == GAME){
        jogador = msg_inicial.jogador;
    }
    int msg = recvMsgFromServer(&state)
    //while(1);
    //runChat(&state);

    return state;
}

int main(){
    int state;
    PROTOCOLO_JOGO jogada, jogada_server;
    
    iniciar();
    if (fonte == NULL) {
        puts("flkaslfkahld");
        return 0;
    }
    state = lobby();
    
    if(state == COMECOU){
        puts("comecou");
        comeca();
        al_flip_display();
        int sair = 0, msgOK = 0;
        int current_x = 0,current_y = 0;
        al_draw_bitmap(personagem,current_x,current_y,0);
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
                        case ALLEGRO_KEY_UP:
                            jogada.teclado = 'W';
                            jogada.tipo = ANDAR;
                            while(msgOK != TAM_PROTOCOLO){
                                msgOK =  sendMsgToServer((PROTOCOLO_JOGO *) &jogada, TAM_PROTOCOLO);
                                if(msgOK == SERVER_DISCONNECTED){
                                    al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, "Servidor Desconectado!");
                                    break;
                                }
                            }
                            msgOK = recvMsgFromServer((PROTOCOLO_JOGO *) &jogada_server, WAIT_FOR_IT);
                            if(msgOK == SERVER_DISCONNECTED)
                                al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, "Servidor Desconectado!");
                            else if(jogada_server.tipo == ANDAR){
                                jogador = jogada_server.jogadorAtual;
                                al_draw_bitmap(mapa,0,0,0);
                                //current_y-=4;
                                al_draw_bitmap(personagem,jogador.position.x,jogador.position.y,0);
                            }
                            break;
                        //Baixo
                        case ALLEGRO_KEY_DOWN:
                            jogada.teclado = 'S';
                            jogada.tipo = ANDAR;
                            while(msgOK != TAM_PROTOCOLO){
                                msgOK =  sendMsgToServer((PROTOCOLO_JOGO *) &jogada, TAM_PROTOCOLO);
                                if(msgOK == SERVER_DISCONNECTED){
                                    al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, "Servidor Desconectado!");
                                    break;
                                }
                            }
                            msgOK = recvMsgFromServer((PROTOCOLO_JOGO *) &jogada_server, WAIT_FOR_IT);
                            if(msgOK == SERVER_DISCONNECTED)
                                al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, "Servidor Desconectado!");
                            else if(jogada_server.tipo == ANDAR){
                                jogador = jogada_server.jogadorAtual;
                                al_draw_bitmap(mapa,0,0,0);
                                //current_y-=4;
                                al_draw_bitmap(personagem,jogador.position.x,jogador.position.y,0);
                            }
                            break;
                        //Esquerda
                        case ALLEGRO_KEY_LEFT:
                            jogada.teclado = 'A';
                            jogada.tipo = ANDAR;
                            while(msgOK != TAM_PROTOCOLO){
                                msgOK =  sendMsgToServer((PROTOCOLO_JOGO *) &jogada, TAM_PROTOCOLO);
                                if(msgOK == SERVER_DISCONNECTED){
                                    al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, "Servidor Desconectado!");
                                    break;
                                }
                            }
                            msgOK = recvMsgFromServer((PROTOCOLO_JOGO *) &jogada_server, WAIT_FOR_IT);
                            if(msgOK == SERVER_DISCONNECTED)
                                al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, "Servidor Desconectado!");
                            else if(jogada_server.tipo == ANDAR){
                                jogador = jogada_server.jogadorAtual;
                                al_draw_bitmap(mapa,0,0,0);
                                //current_y-=4;
                                al_draw_bitmap(personagem,jogador.position.x,jogador.position.y,0);
                            }
                            break;
                        //Direita.
                        case ALLEGRO_KEY_RIGHT:
                            jogada.teclado = 'D';
                            jogada.tipo = ANDAR;
                            while(msgOK != TAM_PROTOCOLO){
                                msgOK =  sendMsgToServer((PROTOCOLO_JOGO *) &jogada, TAM_PROTOCOLO);
                                if(msgOK == SERVER_DISCONNECTED){
                                    al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, "Servidor Desconectado!");
                                    break;
                                }
                            }
                            msgOK = recvMsgFromServer((PROTOCOLO_JOGO *) &jogada_server, WAIT_FOR_IT);
                            if(msgOK == SERVER_DISCONNECTED)
                                al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, "Servidor Desconectado!");
                            else if(jogada_server.tipo == ANDAR){
                                jogador = jogada_server.jogadorAtual;
                                al_draw_bitmap(mapa,0,0,0);
                                //current_y-=4;
                                al_draw_bitmap(personagem,jogador.position.x,jogador.position.y,0);
                            }
                            break;
                        // J = botar armadilha
                        case ALLEGRO_KEY_J:
                            jogada.teclado = 'J';
                            jogada.tipo = BOTARTRAPS;
                            jogada.xAnterior = jogador.position.x;
                            jogada.yAnterior = jogador.position.y;
                            jogada.jogadorAtual = jogador;
                            while(msgOK != TAM_PROTOCOLO){
                                msgOK =  sendMsgToServer((PROTOCOLO_JOGO *) &jogada, TAM_PROTOCOLO);
                                if(msgOK == SERVER_DISCONNECTED){
                                    al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, "Servidor Desconectado!");
                                    break;
                                }
                            }
                            msgOK = recvMsgFromServer((PROTOCOLO_JOGO *) &jogada_server, WAIT_FOR_IT);
                            if(msgOK == SERVER_DISCONNECTED)
                                al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, "Servidor Desconectado!");
                            else if(jogada_server.tipo == ANDAR){
                                al_draw_bitmap(mapa,0,0,0);
                                //current_y-=4;
                                //al_draw_bitmap(trap,jogador.position.x,jogador.position.y,0);
                                al_draw_bitmap(personagem,jogador.position.x,jogador.position.y,0);
                            }
                            break;
                        //esc. sair=1 faz com que o programa saia do loop principal
                        case ALLEGRO_KEY_ESCAPE:
                            sair = 1;
                    }
                    msgOK = recvMsgFromServer((PROTOCOLO_JOGO *) &jogada_server, WAIT_FOR_IT);
                            if(msgOK == SERVER_DISCONNECTED)
                                al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, "Servidor Desconectado!");
                            else{
                                if(jogada_server.tipo == ANDAR || jogada_server.tipo == GAME)
                                    jogador = jogada_server.jogadorAtual; 
                                state = jogada_server.tipo;
                            }
                            
                }  

            }
        }
    }
    else if(state == ENDGAME){
        al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, jogada_server.mensagem);
    }
    al_destroy_display(janela);
    al_destroy_audio_stream(musica_fundo);
    al_destroy_event_queue(fila_eventos);
    return 0;
}