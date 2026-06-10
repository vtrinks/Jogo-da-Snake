#include "jogador.h"

Jogador ranking[MAX_JOGADORES];

void ler_ranking(){
    FILE *file = fopen("ranking.txt", "r");
    if (file != NULL){
    // se deu certo    
    int i;
        for ( i = 0; i < MAX_JOGADORES; i++){
            if (fscanf(file, "%s %d", ranking[i].nome, &ranking[i].pontuacao) != 2){
             strcpy(ranking[i].nome, "vazio");
             ranking[i].pontuacao = 0;
            }
        }
        fclose(file);

    }else{
        // se deu errado
        int i;
        for ( i = 0; i < MAX_JOGADORES; i++){
         strcpy(ranking[i].nome, "vazio");
         ranking[i].pontuacao = 0;
        }
        
    }
}

int busca_jogador(char *nome){
    int i;
    for ( i = 0; i < MAX_JOGADORES; i++){
        if (strcmp(ranking[i].nome, nome) == 0){
         return i;
        }
    }
    return -1;
}

void salvar_ordenar_ranking(char *nome, int pontos_atuais){
    int pos = busca_jogador(nome);
    
    if (pos != -1){
        if (pontos_atuais > ranking[pos].pontuacao){
            ranking[pos].pontuacao = pontos_atuais;
        }
    }else{
        if (pontos_atuais > ranking[MAX_JOGADORES -1 ].pontuacao){
            strcpy(ranking[MAX_JOGADORES -1].nome, nome);
            ranking[MAX_JOGADORES - 1].pontuacao = pontos_atuais;
        }
    }

    int i, j;
    for ( i = 0; i < MAX_JOGADORES -1; i++){
        for ( j = 0; j < MAX_JOGADORES -1 -i; j++){
            if (ranking[j].pontuacao < ranking[j +1].pontuacao){
             Jogador temp = ranking[j];
             ranking[j] = ranking[j + 1];
             ranking[j + 1] = temp;
            }
        }
    }

    FILE *file = fopen("ranking.txt", "w");
    if (file != NULL){
        int i;
        for ( i = 0; i < MAX_JOGADORES; i++){
            fprintf(file," %s %d\n", ranking[i].nome, ranking[i].pontuacao);
        }
        fclose(file);
    }
}
