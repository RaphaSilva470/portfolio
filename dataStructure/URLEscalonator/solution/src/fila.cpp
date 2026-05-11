#ifndef fila_cpp
#define fila_cpp
#include "fila.h"
#include "msgassert.h" 

void inicializacao (Fila * sequencia)
// Descricao: inicializa sequencia 
// Entrada: sequencia
// Saida: sequencia com valores NULL
{         
    sequencia->inicio = NULL;           
    sequencia->fim = NULL;          
    sequencia->tamanho = 0;       
}

int inserir (Fila * sequencia, Chave * atual, char *host, char* nome, int barra, int id)
// Descricao: insere host na sequencia
// Entrada: sequencia, atual, host, nome, barra, id
// Saida: sequencia 
{         
    // cria uma nova Chave
    Chave *novo_elemento = (Chave *) malloc (sizeof (Chave)); 
    erroAssert(novo_elemento,"Erro ao alocar memoria!\n");   

    // inicializa os elementos da Chave
    strcpy (novo_elemento->host, host); 
    novo_elemento->hostID = id;
    LEMEMLOG((long int)(&(nome)),sizeof(char*),novo_elemento->hostID);  
    ESCREVEMEMLOG((long int)(&(novo_elemento->host)),sizeof(int),novo_elemento->hostID); 
    
    //cria lista para esse host e já insere o primeiro url
    Lista li;     
    criar_lista(&li);  
    inserir_ordenado(&li, nome, barra, id);  
    novo_elemento->lista = li; 

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

int remover (Fila * sequencia)
// Descricao: remove host na fila
// Entrada: sequencia
// Saida: sequencia 
{         
    // Chave a ser removida
    Chave *remov_elemento;  
    
    // se a fila estiver vazia, retorna -1;
    if (sequencia->tamanho == 0)           
        return -1;    
        
    // colocar o primeiro elemento para ser removido(FIFO) e atualiza inicio 
    remov_elemento = sequencia->inicio;         
    sequencia-> inicio = sequencia->inicio->seguinte;
    
    //remove o elemento e atualiza o tamanho da sequencia
    free (remov_elemento);                           
    sequencia->tamanho--;             
    return 0;       
}

void exibir (Fila *sequencia, FILE* out)
// Descricao: escreve todos os hosts no arquivo
// Entrada: sequencia, out
// Saida: hosts na sequencia
{        
    // Chave para iterar a fila
    Chave *atual;         
    int i;         
    atual = sequencia->inicio;   
    
    // itera a fila toda e imprime os hosts
    for(i = 0 ; i < sequencia->tamanho; ++i)
    { 
        fprintf(out,"%s\n",atual->host);  
        ESCREVEMEMLOG((long int)(&(atual->host)),sizeof(int),atual->hostID);
        
        // atualiza proximo elemento a ser escrito
        atual = atual->seguinte;         
    }       
}

bool nexiste(Fila * sequencia, char *host, char* nome, int barra, int id)
// Descricao: verifica se já tem certo host na fila
// Entrada: sequencia, host, nome, barra, id
// Saida: sequencia
{
    // Chave para iterar a fila
    Chave *atual;         
    int i;    
    
    // transforma em string para facilitar comparação
    std::string temp1 = host;
    
    atual = sequencia->inicio; 
    for(i=0;i<sequencia->tamanho;++i)   
    {   
        // transforma em string para facilitar comparação
        std::string temp2 = atual->host;
        
        // se já houver host na fila, insere a url na lista da fila
        if(temp1 == temp2)     
        {      
            inserir_ordenado(&atual->lista, nome, barra,id);   
            return false; 
        }
        
        // atualiza proximo elemento a ser verificado
        atual = atual->seguinte; 
    }
    return true;
}
#endif