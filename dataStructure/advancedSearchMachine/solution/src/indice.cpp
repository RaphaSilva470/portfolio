#ifndef indice_cpp
#define indice_cpp
#include "msgassert.h"
#include "indice.h" 
#include <cstring>

void inicializacao (Indice * sequencia)
// Descricao: inicializa sequencia 
// Entrada: sequencia
// Saida: sequencia com valores NULL
{         
    sequencia->inicio = NULL;           
    sequencia->fim = NULL;          
    sequencia->tamanho = 0; 
}

int inserir (Indice * sequencia, Chave * atual,int nome_doc, int qtd, char* palavra)
// Descricao: insere palavra na sequencia
// Entrada: sequencia, atual, nome_doc, qtd, palavra
// Saida: sequencia 
{         
    // cria uma nova Chave
    Chave *novo_elemento = (Chave *) malloc (sizeof (Chave));   
    erroAssert(novo_elemento,"Erro ao alocar memoria!\n");   

    // inicializa os elementos da Chave
    novo_elemento->nome_doc = nome_doc;
    novo_elemento->qtd = qtd; 
    strcpy (novo_elemento->palavra, palavra); 

    //LEMEMLOG((long int)(&(nome)),sizeof(char*),novo_elemento->hostID);  
    //ESCREVEMEMLOG((long int)(&(novo_elemento->host)),sizeof(int),novo_elemento->hostID); 
    
    // se a fila está vazia, insere no inicio
    if(atual == NULL)
    {           
        if(sequencia->tamanho == 0)             
        sequencia->fim = novo_elemento;           
        novo_elemento->seguinte = sequencia->inicio;           
        sequencia-> inicio = novo_elemento;  
    }
    
    // se a fila não está vazia, insere no proximo elemento que estiver vazio
    else 
    {            
        if(atual->seguinte == NULL)  
        {
            sequencia->fim = novo_elemento;           
            novo_elemento->seguinte = atual->seguinte;           
            atual-> seguinte = novo_elemento;  
        }
}  
    //atualiza o tamanho da fila
    sequencia->tamanho++;         
    return 0;       
}

int remover (Indice * sequencia)
// Descricao: remove todos elementos do Indice
// Entrada: sequencia
// Saida: sequencia 
{         
    // Chave a ser removida
    Chave *remov_elemento;  
    
    // se a fila estiver vazia, retorna -1;
    if (sequencia->tamanho == 0)           
        return -1;    
        
    // limpa toda o Indice
    while(sequencia->tamanho != 0)
    {
        // colocar o primeiro elemento para ser removido(FIFO) e atualiza inicio 
        remov_elemento = sequencia->inicio;         
        sequencia-> inicio = sequencia->inicio->seguinte;
    
        //remove o elemento e atualiza o tamanho da sequencia
        free (remov_elemento); 
        sequencia->tamanho--;             
    }
    return 0;       
}
#endif