#ifndef  escalona_h
#define lista_h
#include <iostream>
#include <cstring>
#include "memlog.h"

//define o tamanho maximo de um char* de url ou host
#define MAXTAM 200 

// o tipo No guarda os dados de um elemento da Lista
typedef struct no{
    char url[MAXTAM];
    int profund;
    int urlID; 
    struct no *proximo;
}No; 

typedef struct{
    No *inicio; 
    No *fim;
    int tam; 
}Lista;

void criar_lista(Lista *lista);

// insere ordenado de acordo com a profundidade do url
void inserir_no_inicio(Lista *lista, char* nome, int barra, int id);
void inserir_ordenado(Lista *lista, char* nome, int barra, int id); 
void inserir_no_fim(Lista *lista, char* nome, int barra, int id);

// somente remove do final
void remover(Lista *lista); 

void imprimir(Lista lista, int qtd, FILE * out);

// verifica se há certa url na lista
bool naoDuplicado(Lista *lista, char*nome);

#endif
