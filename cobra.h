#ifndef COBRA_H
#define COBRA_H


typedef struct Coordenada {
    int x, y;
}Coordenada;


typedef struct Nodo{
    Coordenada pos;
    float angulo;
    struct Nodo *proximo;
} Nodo;

typedef struct curva{
    int ordem;
    float angulo;
}Curva;


// Funcao para aumentar a cobra
void crescer_cabeça(Nodo *cabeca);

#endif