#ifndef lista_cpp
#define lista_cpp   
#include "fila.h"  
#include "msgassert.h" 
 
void criar_lista(Lista *lista)
// Descricao: inicializa lista 
// Entrada: lista  
// Saida: lista com valores NULL
{
    lista->inicio = NULL; 
    lista->fim = NULL;
    lista->tam = 0;
}

void inserir_no_inicio(Lista *lista, char* nome, int barra, int id)
// Descricao: insere um No no inicio da lista
// Entrada: lista, nome, barra, id
// Saida: lista 
{
    // cria um novo No
    No *novo = (No *) malloc(sizeof(No));

    if(novo)
    {
        // inicializa os valores do No
        strcpy (novo->url, nome);
        novo->urlID = id;
        LEMEMLOG((long int)(&(nome)),sizeof(char*),novo->urlID);  
        ESCREVEMEMLOG((long int)(&(novo->url)),sizeof(char *),novo->urlID); 
        novo->profund = barra;
        
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
        // se houver erro ao alocar o No
        erroAssert(novo,"Erro ao alocar memoria!\n");
}

void inserir_ordenado(Lista *lista, char* nome, int barra, int id)
// Descricao: insere um No ordenado na lista segundo a profundidade do url
// Entrada: lista, nome, barra, id
// Saida: lista 
{
    // confere se url ja esta na lista
    if (naoDuplicado(lista, nome) == true)
    { 
        No *aux;   
                
        // se a lista tiver vazia insere no inicio
        if(lista->inicio == NULL)
            inserir_no_inicio(lista, nome, barra,id);
    
        // se a profundidade do url a ser inserido é menor do que a profundidade 
        // do url no inicio da lista, insere no inicio
        else if(barra < lista->inicio->profund )      
            inserir_no_inicio(lista, nome,barra, id);  
        
        else
        { 
            aux = lista->inicio;
                
            // encontra a posição que a url deve ser inserida
            while(aux->proximo != lista->inicio && barra + 1  > aux->proximo->profund)          
                aux = aux->proximo;
                    
            // se a posição achada for no final, insere no fim
            if(aux->proximo == lista->inicio)
                inserir_no_fim(lista, nome,barra,id);
            else
            {
                // cria um novo No
                No *novo = (No *) malloc(sizeof(No));
            
                if(novo)
                {
                    // inicializa os valores do No
                    novo->urlID = id;
                    strcpy (novo->url, nome); 
                    novo->profund = barra; 
                    LEMEMLOG((long int)(&(nome)),sizeof(char*),novo->urlID);  
                    ESCREVEMEMLOG((long int)(&(novo->url)),sizeof(char *),novo->urlID);  
                    
                    // insere na ordem 
                    novo->proximo = aux->proximo;
                    aux->proximo = novo;
                    
                    // atualiza o tamanho da lista
                    lista->tam++;
                }
                else
                    // se houver erro ao alocar o No
                    erroAssert(novo,"Erro ao alocar memoria!\n");
            }
        }  
    }   
}  
 

void inserir_no_fim(Lista *lista, char* nome, int barra, int id)
// Descricao: insere um No no fim da lista 
// Entrada: lista, nome, barra, id
// Saida: lista 
{
    // cria um novo No
    No *novo = (No *) malloc(sizeof(No));
    
    if(novo)
    {
        // inicializa os valores do No
        strcpy (novo->url, nome); 
        novo->profund = barra;
        novo->urlID = id;
        LEMEMLOG((long int)(&(nome)),sizeof(char*),novo->urlID);  
        ESCREVEMEMLOG((long int)(&(novo->url)),sizeof(char *),novo->urlID); 
        
        // se a lista esiver vazia, insere e atualiza o inicio
        if(lista->inicio == NULL)
        {
            lista->inicio = novo;
            lista->fim = novo;
            lista->fim->proximo = lista->inicio;
        }
        // se a lista nao estiver vazia, insere no fim e atualiza o fim
        else
        {
            lista->fim->proximo = novo;
            lista->fim = novo;
            novo->proximo = lista->inicio;
        }
        // atualiza o tamanho da lista
        lista->tam++;
    }
    else
        // se houver erro ao alocar o No
        erroAssert(novo,"Erro ao alocar memoria!\n");
}

void remover(Lista *lista)  
// Descricao: remove o primeiro No da lista (FIFO)
// Entrada: lista
// Saida: lista 
{ 
    // ponteiro para o No a ser removido
    No *remover = NULL;

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
            remover = lista->inicio;
            lista->inicio = remover->proximo;
            lista->fim->proximo = lista->inicio;
            lista->tam--;
            free(remover);
        }
    }
 
}   

void imprimir(Lista lista, int qtd, FILE* out)
// Descricao: escreve as urls da lista no arquivo
// Entrada: lista, qtd, out
// Saida: hosts da lista 
{
    // No para iterar a lista e número para parar de imprimir
    No *no = lista.inicio;
    int parada = 0;

    // imprime a quantidade passada por parâmetro
    if(no){
        do
        {
            fprintf(out,"http//%s\n",no->url);
            ESCREVEMEMLOG((long int)(&(no->url)),sizeof(char *),no->urlID);    
            parada++;
            // para quando já imprimiu a quantidade necessária
            if(parada == qtd)  
            {
                break; 
            }
            // atualiza o No a ser impresso
            no = no->proximo; 
        }while(no != lista.inicio);
    }
}

bool naoDuplicado(Lista *lista, char*nome) 
// Descricao: verifica se já existe certa url na lista
// Entrada: lista, nome
// Saida: false para já existe, true para nao existe
{
    // No para iterar a lista
    No *aux; 
    aux = lista->inicio;   

    // para facilitar a comparação, transforma a url de char* para string 
    std::string temp1 = nome;

    int listaTam = lista->tam ;
    while(listaTam > 0)     
        {
            // para facilitar a comparação, transforma a url de char* para string 
            std::string temp2 = aux->url ; 
            
            // se for duplicado retorna false
            if( temp1 == temp2) 
            {  
                return false;  
            }
            aux = aux->proximo;     
            listaTam--;     
        }
    // se nao for duplicado retorna true
    return true;
}      

#endif