#ifndef indice_h
#define indice_h

#include <string> 

// o tipo Chave guarda os dados do elemento do indice
typedef struct chaveFila
{          
    int nome_doc;
    int qtd; 
    char palavra[200];         
    struct chaveFila *seguinte;  
} Chave;    

typedef struct
{         
    Chave *inicio;  
    Chave *fim;  
    int tamanho; 
} Indice;

void inicializacao (Indice * sequencia);
int remover (Indice * sequencia);    

//somente insere no inicio
int inserir (Indice * sequencia, Chave * atual,int nome_doc, int qtd, char* palavra); 
#endif