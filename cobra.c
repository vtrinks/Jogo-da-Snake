#include "bibliotecas.h"
#include "cobra.h"


// Funcao para aumentar a cobra
void crescer_cabeça(Nodo *cabeca){
    Nodo *novo = (Nodo *)malloc(sizeof(Nodo));
    
    novo->pos.x = -100; 
    novo->pos.y = -100;
    novo->proximo = NULL;

    //Achar a ponta da cauda
    Nodo *aux = cabeca;
    while (aux->proximo != NULL){
     aux = aux->proximo;
    }

    aux->proximo = novo; 
}