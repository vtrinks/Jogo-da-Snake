#ifndef JOGADOR_H
#define JOGADOR_H

#include "bibliotecas.h"

#define MAX_JOGADORES 3


typedef struct jogador{
    char nome[50];
    int pontuacao;
} Jogador;

extern Jogador ranking [MAX_JOGADORES];

void ler_ranking();
int busca_jogador(char *nome);
void salvar_ordenar_ranking(char *nome, int pontos_atuais);

#endif
