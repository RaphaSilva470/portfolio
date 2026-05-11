#ifndef fila_h
#define fila_h 
#include "lista.h"

// o tipo Chave guarda os dados do elemento da fila
typedef struct chaveFila
{          
    char host[MAXTAM];   
    Lista lista; 
    int hostID;
    struct chaveFila *seguinte;  
} Chave;   

typedef struct
{         
    Chave *inicio;  
    Chave *fim;  
    int tamanho; 
} Fila;

void inicializacao (Fila * sequencia);

//somente insere no incio e remove do final, obedecendo first-in-first-out 
int inserir (Fila * sequencia, Chave * atual, char *host, char* nome, int barra, int id);       
int remover (Fila * sequencia);    

void exibir (Fila *sequencia, FILE* out);  

// verifica se existe certo host na fila
bool nexiste(Fila * sequencia, char *dado,char* nome, int barra, int id);
#endif