#ifndef documento_cpp
#define documento_cpp
#include "documento.h"
#include "msgassert.h"
#include <cstdlib>
#include <string>
#include <cstring>

void criar_lista(Vocabulario *lista)
// Descricao: inicializa lista 
// Entrada: lista  
// Saida: lista com valores NULL
{
    lista->inicio = NULL; 
    lista->fim = NULL;
    lista->tam = 0;
}
void inserir_no_inicio(Vocabulario *lista, char* nome)
// Descricao: insere um Vocabulario no inicio da lista
// Entrada: lista, nome
// Saida: lista 
{
    // cria um novo Vocabulario
    Palavra *novo = (Palavra *) malloc(sizeof(Palavra));

    if(novo)
    {
        // inicializa os valores do Vocabulario
        strcpy (novo->word, nome);
        novo->qtd = 1;
        //LEMEMLOG((long int)(&(nome)),sizeof(char*),novo->urlID);  
        //ESCREVEMEMLOG((long int)(&(novo->url)),sizeof(char *),novo->urlID); 

        // se a lista esta vazia, insere no fim
        if(lista->fim == NULL)
            lista->fim = novo;
        
        // insere no inicio
        novo->proximo = lista->inicio;
        lista->inicio = novo;
        lista->fim->proximo = lista->inicio;
        
        // atualiza o tamanho da lista
        lista->tam++;
    }
    else
        // se houver erro ao alocar o Vocabulario
        erroAssert(novo,"Erro ao alocar memoria!\n");
}
void inserir_ordenado(Vocabulario *lista, char* nome)
// Descricao: insere um Vocabulario na lista sem duplicar 
// Entrada: lista, nome
// Saida: lista 
{
    // confere se url ja esta na lista
    if (naoDuplicado(lista, nome) == true)
    { 
        Palavra *aux;   
                
        // se a lista tiver vazia insere no inicio
        if(lista->inicio == NULL)
            inserir_no_inicio(lista, nome);
    
        else
        { 
            aux = lista->inicio;
                // cria um novo Vocabulario
                Palavra *novo = (Palavra *) malloc(sizeof(Palavra));
            
                if(novo)
                {
                    // inicializa os valores do Vocabulario
                    strcpy (novo->word, nome); 
                    novo->qtd = 1;
                    //LEMEMLOG((long int)(&(nome)),sizeof(char*),novo->urlID);  
                    //ESCREVEMEMLOG((long int)(&(novo->url)),sizeof(char *),novo->urlID);  
                    
                    // insere 
                    novo->proximo = aux->proximo;
                    aux->proximo = novo;
                    
                    // atualiza o tamanho da lista
                    lista->tam++;
                }
                else
                    // se houver erro ao alocar o Vocabulario
                    erroAssert(novo,"Erro ao alocar memoria!\n");
        }
          
    }   
} 
bool naoDuplicado(Vocabulario *lista, char*nome) 
// Descricao: verifica se já existe certo Vocabulario na lista
// Entrada: lista, nome
// Saida: false para já existe, true para nao existe
{
    // Vocabulario para iterar a lista
    Palavra *aux; 
    aux = lista->inicio;   

    // para facilitar a comparação, transforma a url de char* para string 
    std::string temp1 = nome;

    int listaTam = lista->tam ;
    while(listaTam > 0)     
        {
            // para facilitar a comparação, transforma a url de char* para string 
            std::string temp2 = aux->word ; 
            
            // se for duplicado retorna false
            if( temp1 == temp2) 
            {  
                aux->qtd ++;  
                return false;  
            }
            aux = aux->proximo;     
            listaTam--;     
        }
    // se nao for duplicado retorna true
    return true;
}       
void remover(Vocabulario *lista)  
// Descricao: remove todo Vocabulario
// Entrada: lista
// Saida: lista 
{ 
    // ponteiro para a Palavra a ser removida
    Palavra *remover = NULL;

    if(lista->inicio)
    {
        // se houver apenas um elemento na lista, atualiza os valores com NULL
        if(lista->inicio == lista->fim)
        {
            remover = lista->inicio;
            lista->inicio = NULL;
            lista->fim = NULL;
            lista->tam--;
            free(remover); 

        }
        // se houver muitos elementos na lista, atualiza inicio da lista
        else
        {
            while(lista->tam != 0)
            {
                remover = lista->inicio;
                lista->inicio = remover->proximo;
                lista->fim->proximo = lista->inicio;
                lista->tam--;
                free(remover);
            }  
        }
    }
 
}  
#endif 