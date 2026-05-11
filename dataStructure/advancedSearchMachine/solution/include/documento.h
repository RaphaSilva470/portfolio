#ifndef documento_h
#define documento_h 
#include <string>

// o tipo Palavra guarda os dados de um elemento da Vocabulario
typedef struct Palavra{
    char word[200]; 
    int qtd;
    struct Palavra *proximo;
}Palavra; 

typedef struct{
    Palavra *inicio; 
    Palavra *fim; 
    int tam; 
}Vocabulario;

void criar_lista(Vocabulario *lista);
void remover(Vocabulario *lista);  

// somente insere nao duplicado
void inserir_ordenado(Vocabulario *lista, char* nome); 
void inserir_no_inicio(Vocabulario *lista, char* nome);  
bool naoDuplicado(Vocabulario *lista, char*nome);

#endif