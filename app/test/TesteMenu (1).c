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
//#include "/home/CIN/ahac/Downloads/Joguinho/lib/server.h"
#include "/home/CIN/ahac/Downloads/Joguinho/lib/client.h"
#include "/home/CIN/ahac/Downloads/Joguinho/ACore.h"

//gcc TesteMenu.c -lm -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

/* ------------------------- Novas variáveis ------------------------- */

ALLEGRO_AUDIO_STREAM *musica_fundo = NULL;
ALLEGRO_SAMPLE *escolha = NULL;
ALLEGRO_SAMPLE *som_escolha = NULL;
ALLEGRO_BITMAP *botao1 = NULL;
ALLEGRO_BITMAP *botao1_pressionado = NULL;
ALLEGRO_BITMAP *botao2 = NULL;
ALLEGRO_BITMAP *botao2_pressionado = NULL;
ALLEGRO_BITMAP *setinha_dir = NULL;
ALLEGRO_BITMAP *setinha_esq = NULL;

/* ------------------------- Antigas variáveis ------------------------- */

ALLEGRO_BITMAP *mapa = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *logo = NULL;
ALLEGRO_KEYBOARD *teclado = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_DISPLAY *janela = NULL;

void carrega_arquivos(){
    //aqui serao colocados todos os arquivos a serem carregados no jogo
    botao1 = al_load_bitmap("/home/CIN/ahac/Downloads/Joguinho/Icons/grey_button00.png");
    botao1_pressionado = al_load_bitmap("/home/CIN/ahac/Downloads/Joguinho/Icons/grey_button01.png");
    botao2 = al_load_bitmap("/home/CIN/ahac/Downloads/Joguinho/Icons/grey_button03.png");
    botao2_pressionado = al_load_bitmap("/home/CIN/ahac/Downloads/Joguinho/Icons/grey_button04.png");
    logo = al_load_bitmap("/home/CIN/ahac/Downloads/Joguinho/Icons/index.png");
    mapa = al_load_bitmap("/home/CIN/ahac/Downloads/Joguinho/Characters/mapa.jpeg");
    setinha_dir = al_load_bitmap("/home/CIN/ahac/Downloads/Joguinho/Icons/yellow_sliderRight.png");
    setinha_esq = al_load_bitmap("/home/CIN/ahac/Downloads/Joguinho/Icons/yellow_sliderLeft.png");
    if(!botao1 || !botao1_pressionado || !botao2 || !botao2_pressionado || !logo || !mapa){
        //checar se foram abertos, adicionar o nome do arquivo depois de um ou (||!).
        printf("Nao foi possivel carregar um dos botoes.");
        al_destroy_audio_stream(musica_fundo);
    }

    musica_fundo = al_load_audio_stream("/home/CIN/ahac/Downloads/Joguinho/Musics/Musica_fundo.ogg",4,1024);
    escolha = al_load_sample("/home/CIN/ahac/Downloads/Joguinho/Musics/switch3.ogg");

    if(!musica_fundo || !escolha){
        //aqui sao apenas os arquivos sonoros.
        puts("Musica nao foi carregada.");
    }
    al_attach_audio_stream_to_mixer(musica_fundo, al_get_default_mixer());
    al_set_audio_stream_playmode(musica_fundo,ALLEGRO_PLAYMODE_LOOP);
}

void menu(){
    //aqui puxa a tela inicial.
    al_clear_to_color(al_map_rgb(255,255,255));
    al_draw_bitmap(logo,0,0,0);
    al_flip_display();
    al_rest(5.0);
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
    fonte = al_load_font("/home/CIN/ahac/Downloads/Joguinho/Font/kenvector_future.ttf", 20, 0);
    al_draw_text(fonte,al_map_rgb(5,5,5),645,290,ALLEGRO_ALIGN_CENTER,"Iniciar o jogo");
    al_draw_text(fonte,al_map_rgb(5,5,5),645,370,ALLEGRO_ALIGN_CENTER,"Tutorial");
    al_draw_text(fonte,al_map_rgb(5,5,5),645,450,ALLEGRO_ALIGN_CENTER,"Creditos");

    al_flip_display();
    
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    
    int sair = 0,opcao = 1;


    while(!sair){
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
                            sair = 1;
                            break;

                        case ALLEGRO_KEY_ENTER:
                        //enter pra selecionar a opcao
                            if(opcao == 1){
                                //aqui puxa toda a tela de comecar o game, IP, capacete, etc.
                            }
                            else if(opcao == 2){
                                //aqui puxa a tela de tutorial
                            }
                            else if(opcao == 3){
                                // aqui puxa a tela de creditos
                            }

                        default:
                            break;
                    }
                }
                if(opcao){
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
                    }
                    al_flip_display();
                }
            }
        }
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
    if(!al_init_acodec_addon()){
        printf("Falha ao iniciar Codec de Audio.");
        return 0;
    }
    if(!al_init_font_addon()){
        printf("Nao foi possivel iniciar o addon da fonte");
        return 0;
    }
    if(!al_init_ttf_addon()){
        printf("Nao foi possivel iniciar o addon ttf");
        return 0;
    }
    
    if(!al_install_keyboard()){
        printf("Falha ao instalar o teclado.");
        return 0;
    }

    if(!al_init_image_addon()){
        printf("Nao foi possivel iniciar o addon de imagem.");
        return 0;
    }
    if(!al_reserve_samples(20)){
        printf("Nao foi possivel reservar os audios.");
        return 0;
    }


    
    janela = al_create_display(1280,720);
    
    if(!janela){
        puts("Falha ao iniciar a janela");
        al_destroy_audio_stream(musica_fundo);
        return 0;
    }
    
    al_set_window_title(janela,"Barra Bandeira");
    
    fila_eventos = al_create_event_queue();
    
    if(!fila_eventos){
        puts("Erro ao criar fila de eventos.");
        al_destroy_display(janela);
        return 0;
    }
    
    carrega_arquivos();
    menu();
    return 1;
}
int main(){
    iniciar();
    al_flip_display();
    int sair = 0,tecla = 0;

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
                            tecla = 1;
                            break;
                        //Baixo
                        case ALLEGRO_KEY_DOWN:
                            tecla = 2;
                            break;
                        //Esquerda
                        case ALLEGRO_KEY_LEFT:
                            tecla = 3;
                            break;
                        //Direita.
                        case ALLEGRO_KEY_RIGHT:
                            tecla = 4;
                            break;
                        //esc. sair=1 faz com que o programa saia do loop principal
                        case ALLEGRO_KEY_J:
                            tecla = 5;
                            break;
                        case ALLEGRO_KEY_ESCAPE:
                            sair = 1;
                    }
                }
                if(tecla){
                    switch(tecla){
                        case 1:
                            al_draw_bitmap(mapa,0,0,0);
                            break;
                        case 2:
                            al_draw_bitmap(mapa,0,0,0);
                            break;
                        case 3:
                            al_draw_bitmap(mapa,0,0,0);
                            break;
                        case 4:
                            al_draw_bitmap(mapa,0,0,0);
                            break;
                    }
                    tecla = 0;
                    al_flip_display();
                }
            }
        }

    al_destroy_display(janela);
    al_destroy_audio_stream(musica_fundo);
    al_destroy_event_queue(fila_eventos);
    return 0;
}