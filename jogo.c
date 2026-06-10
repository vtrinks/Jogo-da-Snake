
// inclusão das tads
#include "cobra.h"
#include "jogador.h"
#include "bibliotecas.h"
#define PI 3.14

typedef enum{
    PARADA,
    ESQUERDA,
    DIREITA,
    CIMA,
    BAIXO
}Direcao;


int obstaculos[5][2] ={ {150, 150}, {650, 150}, {400, 300}, {150, 450}, {650, 450}};


int main(){
    al_init();
    al_install_keyboard();
    al_init_primitives_addon();
    al_set_new_display_flags(ALLEGRO_OPENGL);
    setlocale(LC_ALL, "portuguese");

    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    short fleg_curva = 0; 
    float velocidade = 12.0;
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / velocidade);

    Nodo *cabeca = (Nodo *)malloc(sizeof(Nodo));
    cabeca->pos.x = 450;
    cabeca->pos.y = 300;
    cabeca->angulo = 0;
    cabeca->proximo = NULL;

    Direcao direcao = PARADA;
    int rodando = 1;
    int desenhar = 0;
        
     srand(time(NULL));
    int comida_x = rand() % 780; 
    int comida_y = rand() % 580;


    ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();
    al_register_event_source(fila, al_get_display_event_source(display));
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_timer_event_source(timer));

    al_start_timer(timer);
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    
    ALLEGRO_BITMAP *background = al_load_bitmap("./fundo.jpg");
    ALLEGRO_BITMAP *img_maca = al_load_bitmap("./maca.png");
    ALLEGRO_BITMAP *img_cabeça = al_load_bitmap("./cabeça.png");
    ALLEGRO_BITMAP *img_corpo = al_load_bitmap("./corpo.png");
    ALLEGRO_BITMAP *img_cauda = al_load_bitmap("./cauda.png");
    ALLEGRO_BITMAP *img_curva = al_load_bitmap("./curva.png");
    ALLEGRO_FONT *fonte = al_create_builtin_font();
    int pontos = 0;

    ler_ranking();

    Curva curvas[100];
    int qtd_curvas = 0;
    int tam_cobra = -1;
    
    while (rodando){
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila, &evento);

       if (evento.type == ALLEGRO_EVENT_TIMER){
            //A Cabeça puxa o Corpo
            if (direcao != PARADA){
                int prev_x = cabeca->pos.x;
                int prev_y = cabeca->pos.y;
                int temp_x, temp_y;

                // Move a Cabeça
                if (direcao == ESQUERDA) cabeca->pos.x -= 20;
                if (direcao == DIREITA)  cabeca->pos.x += 20;
                if (direcao == CIMA)     cabeca->pos.y -= 20;
                if (direcao == BAIXO)    cabeca->pos.y += 20;

                //O corpo da cabeça
                Nodo *aux = cabeca->proximo;
                while (aux != NULL){
                    temp_x = aux->pos.x;
                    temp_y = aux->pos.y;
                    
                    aux->pos.x = prev_x;
                    aux->pos.y = prev_y;
                    
                    prev_x = temp_x;
                    prev_y = temp_y;
                    
                    aux = aux->proximo;
                }
            }
            
            desenhar = 1;

            if (cabeca->pos.x < 0) cabeca->pos.x = 780;
            else if (cabeca->pos.x > 780) cabeca->pos.x = 0;

            if (cabeca->pos.y < 0) cabeca->pos.y = 580;       
            else if (cabeca->pos.y > 580) cabeca->pos.y = 0;
            //fazer a cabeça crescer
            if (cabeca->pos.x < comida_x + 20 && 
                cabeca->pos.x + 20 > comida_x &&
                cabeca->pos.y < comida_y + 20 &&
                cabeca->pos.y + 20 > comida_y)
            {
                //para a maça n nascer nos obstaculos
                int maca_valida = 0;
                while (maca_valida == 0)
                {
                    comida_x = rand() % 780;
                    comida_y = rand() % 580;
                    maca_valida = 1;
                    int i;
                    for (i = 0; i < 5; i++)
                    {
                        int obs_x = obstaculos[i][0];
                        int obs_y = obstaculos[i][1];

                        if (comida_x < obs_x + 30 &&
                            comida_x + 20 > obs_x &&
                            comida_y < obs_y + 30 &&
                            comida_y + 20 > obs_y) {
                         //se a maça cair no obstaculos o laço vai rodar de novo
                         maca_valida = 0;
                         break; 
                        }
                    }
                }
                tam_cobra += 1;

                crescer_cabeça(cabeca);
                pontos += 10; 
                
                if(pontos % 50 == 0)//aumentar a velocidade da cabeça
                velocidade += 3.0;
                al_set_timer_speed(timer, 1.0 / velocidade);
            }
            
            //Obstaculos para melhorar a dinamica do jogo
            int i;
            for ( i = 0; i < 5; i++){
                int obs_x = obstaculos[i][0];
                int obs_y = obstaculos[i][1];

                if (cabeca->pos.x < obs_x + 30 &&
                    cabeca->pos.x + 20 > obs_x &&
                    cabeca->pos.y < obs_y + 30 &&
                    cabeca->pos.y + 20 > obs_y){

                        printf("\n=== GAME OVER ===\n");
                        printf("Sua pontuacao final: %d\n", pontos);
                        printf("Digite seu nome para o ranking: ");

                        char nome_jogador[100];
                        scanf(" %[^\n]", nome_jogador);
                        int k;
                        for ( k = 0; k < nome_jogador[k] != '\0'; k++){
                            if (nome_jogador[k] == ' ') nome_jogador[k] = '_';
                        }
                    salvar_ordenar_ranking(nome_jogador, pontos);
                    
                    rodando = 0;
                }
            }

        }else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            rodando = 0;
        }else if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
            //movimento da cabeça
            switch (evento.keyboard.keycode){
            case ALLEGRO_KEY_LEFT:
                if (direcao != DIREITA){
                    direcao = ESQUERDA;
                    cabeca->angulo = (PI/2);
                    if(fleg_curva){
                 curvas[qtd_curvas].ordem = 0;
                 qtd_curvas += 1;
                }
                }
                break;
            case ALLEGRO_KEY_RIGHT:
                if (direcao != ESQUERDA){
                    direcao = DIREITA;
                    cabeca->angulo = -(PI/2);
                     if(fleg_curva){
                     curvas[qtd_curvas].ordem = 0;
                     qtd_curvas += 1;
                    }
                }
                break;
            case ALLEGRO_KEY_UP:
                if (direcao != BAIXO){
                    direcao = CIMA;
                    cabeca->angulo = PI;
                     if(fleg_curva){
                     curvas[qtd_curvas].ordem = 0;
                     qtd_curvas += 1;
                    }
                }
                break;
            case ALLEGRO_KEY_DOWN:
                if (direcao != CIMA){
                    direcao = BAIXO;
                    cabeca->angulo = 0;
                     if(fleg_curva){
                     curvas[qtd_curvas].ordem = 0;
                     qtd_curvas += 1;
                    }
                }
                break;
            case ALLEGRO_KEY_ESCAPE:
                rodando = 0;
                break;
            }
        }

        if (desenhar && al_is_event_queue_empty(fila)){
            al_clear_to_color(al_map_rgb(0, 0, 0));

            if (background){
                al_draw_scaled_bitmap(background,0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 0, 0, 800, 600, 0);
            }
            
            //Placar 
            al_draw_textf(fonte, al_map_rgb(255, 255, 255), 10, 10, 0, "SCORE: %d", pontos);
            int i;
            for (i = 0; i < 5; i++){
                int obs_x = obstaculos[i][0];
                int obs_y = obstaculos[i][1];
                al_draw_filled_rectangle(obs_x, obs_y, obs_x + 30, obs_y + 30, al_map_rgb(100, 100, 100));
            }
            if (img_maca){
                al_draw_scaled_bitmap(img_maca, 0, 0, al_get_bitmap_width(img_maca), al_get_bitmap_height(img_maca), comida_x, comida_y, 20, 20, 0);
            }else{
                al_draw_filled_rectangle(comida_x, comida_y, comida_x + 20, comida_y + 20, al_map_rgb(255, 0, 0));
            }
            //A imagem da cabeça no jogo
             Nodo *aux = cabeca;
             Nodo *frente = NULL;

            int ordem_atual = 0;
            while (aux != NULL){
                float angulo_atual = 0;

                if (aux == cabeca)
                {
                    angulo_atual = cabeca->angulo;
                }else if (frente != NULL){
                     if (frente->pos.x < aux->pos.x) angulo_atual = (PI / 2);//Esquerda
                     else if (frente->pos.x > aux->pos.x) angulo_atual = -(PI /2);//Direita
                     else if (frente->pos.y < aux->pos.y) angulo_atual = PI;//cima
                     else if (frente->pos.y > aux->pos.y) angulo_atual =0;//Baixo
                    }
                //a imagem da cabeça rotacionando
                if (aux == cabeca && img_cabeça){
                    al_draw_scaled_rotated_bitmap(img_cabeça, al_get_bitmap_width(img_cabeça)/2, al_get_bitmap_height(img_cabeça)/2, aux->pos.x + 10, aux->pos.y + 10, 0.5, 0.5, angulo_atual, 0);
                }else if (aux->proximo == NULL && aux != cabeca && img_cauda){
                    al_draw_scaled_rotated_bitmap(img_cauda, al_get_bitmap_width(img_cauda)/2, al_get_bitmap_height(img_cauda)/2, aux->pos.x + 10, aux->pos.y + 10, 0.5, 0.5, angulo_atual + PI, 0);
                }else if(img_corpo && aux != cabeca && aux-> proximo != NULL ){
                    Nodo *tras = aux->proximo;
                    if(frente->pos.x == tras->pos.x || frente->pos.y == tras->pos.y){
                     al_draw_scaled_rotated_bitmap(img_corpo, al_get_bitmap_width(img_corpo)/2, al_get_bitmap_height(img_corpo)/2, aux->pos.x + 10, aux->pos.y + 10, 0.5, 0.5, angulo_atual, 0);
                    }else if (img_curva){
                        if (abs(frente->pos.x - tras->pos.x) > 40 || abs(frente->pos.y - tras->pos.y) > 40) {
                            al_draw_scaled_rotated_bitmap(img_corpo, al_get_bitmap_width(img_corpo)/2, al_get_bitmap_height(img_corpo)/2, aux->pos.x + 10, aux->pos.y + 10, 0.5, 0.5, angulo_atual, 0);
                        }else{
                           float angulo_curva = 0; 
                        
                        // curva para cima e direita
                        if ((frente->pos.y < aux->pos.y && tras->pos.x > aux->pos.x) || (tras->pos.y < aux->pos.y && frente->pos.x > aux->pos.x)) angulo_curva = 0;
                        // curva para direita e baixo
                        else if ((frente->pos.x > aux->pos.x && tras->pos.y > aux->pos.y) || (tras->pos.x > aux->pos.x && frente->pos.y > aux->pos.y)) angulo_curva = PI / 2;
                        // curva para baixo e esquerda
                        else if ((frente->pos.y > aux->pos.y && tras->pos.x < aux->pos.x) || (tras->pos.y > aux->pos.y && frente->pos.x < aux->pos.x)) angulo_curva = PI;
                        // curva para esquerda e cima
                        else if ((frente->pos.x < aux->pos.x && tras->pos.y < aux->pos.y) || (tras->pos.x < aux->pos.x && frente->pos.y < aux->pos.y)) angulo_curva = -PI / 2;

                     al_draw_scaled_rotated_bitmap(img_curva, al_get_bitmap_width(img_curva)/2, al_get_bitmap_height(img_curva)/2, aux->pos.x + 10, aux->pos.y + 10, 0.5, 0.5, angulo_curva + PI, 0);
                    }
                }
                }else{
                   al_draw_filled_rectangle(aux->pos.x, aux->pos.y, aux->pos.x + 20, aux->pos.y + 20, al_map_rgb(0, 255, 0));
                }
                frente = aux;
                aux = aux->proximo;
            }
            al_flip_display();
            desenhar = 0;
        }
    }
        //limpeza
        if (background) al_destroy_bitmap(background);
        if (img_maca) al_destroy_bitmap(img_maca);
        if (img_cabeça) al_destroy_bitmap(img_cabeça);
        if (img_corpo) al_destroy_bitmap(img_corpo);
        if (img_cauda) al_destroy_bitmap(img_cauda);
        al_destroy_font(fonte);

        Nodo *atual = cabeca;
        Nodo *prox;
        while(atual != NULL){
            prox = atual->proximo;
            free(atual);
            atual = prox;
        }

        al_destroy_timer(timer);
        al_destroy_display(display);
        al_destroy_event_queue(fila);
    return 0;
}