#ifndef ordena_cpp
#define ordena_cpp 
#include "memlog.h" 
 
void Particao(int Esq, int Dir, int *i, int *j, TipoOrdena *A)
// Descricao: ordena parte do vetor *A em ordem decrescente usando quicksort
// Entrada: Esq, Dir, *i, *j, *A
// Saida: *A 
{
    TipoOrdena x, w;
    *i = Esq; *j = Dir;
    
    // obtem o pivo x
    x = A[(*i + *j)/2];
    LEMEMLOG((long int)(&(A[(*i + *j)/2])),sizeof(TipoOrdena),A[(*i + *j)/2].id); 
    ESCREVEMEMLOG((long int)(&(x)),sizeof(TipoOrdena), A[(*i + *j)/2].id);
      
    // percorre o vetor ate achar um maior e um menor e troca eles de posicao
    do
    { 
        while (x.GetChave() < A[*i].GetChave()) (*i)++; 
        while (x.GetChave() > A[*j].GetChave()) (*j)--; 
        if (*i <= *j) 
        {     
            
                if(A[*i].GetChave() < A[*j].GetChave())
                {
                    w = A[*i]; A[*i] = A[*j]; A[*j] = w; 
                    LEMEMLOG((long int)(&(A[*i])),sizeof(TipoOrdena),A[*i].id); 
                    ESCREVEMEMLOG((long int)(&(w)),sizeof(TipoOrdena), A[*i].id);
    
                    LEMEMLOG((long int)(&(A[*j])),sizeof(TipoOrdena),A[*j].id); 
                    ESCREVEMEMLOG((long int)(&(A[*i])),sizeof(TipoOrdena), A[*i].id);
                    
                    LEMEMLOG((long int)(&(w)),sizeof(TipoOrdena),A[*j].id); 
                    ESCREVEMEMLOG((long int)(&(A[*j])),sizeof(TipoOrdena), A[*j].id);
                    
                    (*i)++; (*j)--;  
                }
                // se houver um empate emtra popularidade, usa ordem alfabetica
                else if(A[*i].GetChave() == A[*j].GetChave())
                {  
                    if(strcmp (A[*i].url, A[*j].url) > 0)     
                    { 
                        w = A[*i]; A[*i] = A[*j]; A[*j] = w;
                        LEMEMLOG((long int)(&(A[*i])),sizeof(TipoOrdena),A[*i].id); 
                        ESCREVEMEMLOG((long int)(&(w)),sizeof(TipoOrdena), A[*i].id);
    
                        LEMEMLOG((long int)(&(A[*j])),sizeof(TipoOrdena),A[*j].id); 
                        ESCREVEMEMLOG((long int)(&(A[*i])),sizeof(TipoOrdena), A[*i].id);
                        
                        LEMEMLOG((long int)(&(w)),sizeof(TipoOrdena),A[*j].id); 
                        ESCREVEMEMLOG((long int)(&(A[*j])),sizeof(TipoOrdena), A[*j].id);
                        
                        (*i)++; (*j)--;    
                    }
                    else   
                    {
                        (*i)++; (*j)--;     
                    }
            }
        }  
    } while (*i <= *j); /* ate que os indices se cruzem */
}

void Ordena(int Esq, int Dir, TipoOrdena *A)
// Descricao: chama a funcao particao ate o vetor A estar todo ordenado
// Entrada: Esq, Dir, *i, *j, *A
// Saida: *A 
{ 
    int i;
    int j;
    Particao(Esq, Dir, &i, &j, A);
    if (Esq < j) Ordena(Esq, j, A);
    if (i < Dir) Ordena(i, Dir, A);
}

void QuickSort(TipoOrdena *A, int n)
// Descricao: ordena o vetor *A
// Entrada: *A, n
// Saida: *A 
{
    Ordena(0, n-1, A);
}

void Refaz(int Esq, int Dir, TipoOrdena *A)
// Descricao: chama a funcao ordena
// Entrada: *A, n
// Saida: *A 
{
    int i, j;
    TipoOrdena x;
    i = Esq; 
    j = i * 2;
    x = A[i - 1];
    LEMEMLOG((long int)(&(A[i - 1])),sizeof(TipoOrdena),A[i - 1].id); 
    ESCREVEMEMLOG((long int)(&(x)),sizeof(TipoOrdena),A[i - 1].id );
    
    // enquanto j esta dentro dos limites do vetor
    while (j <= Dir)
    {
        // se no vetor tem um filho da direita
        if (j < Dir)
        {
            if (A[j - 1].GetChave() <= A[j].GetChave()) 
            {
                // se a chave de um filho for maior que do outro, escolhe o maior
                if (A[j - 1].GetChave() < A[j].GetChave()) 
                    j++;
                    
                // se as chaves dos filhos forem iguais, escolhe em ordem alfabetica
                else if(A[j - 1].GetChave() ==  A[j].GetChave()) 
                {
                    if(strcmp (A[j - 1].url, A[j].url) > 0)          
                    j ++;    
                }
            }

        }  
         
        // se o pai for maior que os filhos, termina o loop
        if (x.GetChave() > A[j - 1].GetChave()) break;  
        
        // se a chave do pai for igual a do filho, ordena em ordem alfabetica
        else if (x.GetChave() == A[j - 1].GetChave())    
        { 
            if(strcmp (x.url, A[j - 1].url) > 0) 
            {  
                A[i - 1] = A[j - 1];
                LEMEMLOG((long int)(&(A[j - 1])),sizeof(TipoOrdena),A[j - 1].id); 
                ESCREVEMEMLOG((long int)(&(A[i - 1])),sizeof(TipoOrdena), A[i - 1].id);
    
                i = j; 
                j = i *2; 
            }
            else 
            break;
        }
        // se a chave do pai for menor, troca de posicao
        else if(x.GetChave() < A[j - 1].GetChave())
        { 
            A[i - 1] = A[j - 1];
            LEMEMLOG((long int)(&(A[j - 1])),sizeof(TipoOrdena),A[j - 1].id); 
            ESCREVEMEMLOG((long int)(&(A[i - 1])),sizeof(TipoOrdena), A[i - 1].id);
    
            i = j; 
            j = i *2;
        }
    }   
    A[i - 1] = x; 
    LEMEMLOG((long int)(&(x)),sizeof(TipoOrdena), A[i - 1].id);
    ESCREVEMEMLOG((long int)(&(A[i - 1])),sizeof(TipoOrdena),A[i - 1].id); 
}

void RetiraMax(TipoOrdena *A, int *n) 
// Descricao: retira o maior elemento do heap e reordena
// Entrada: *A, n
// Saida: *A 
{
    if (*n < 1)
    printf("Erro: heap vazio\n");
    else 
    {
        // diminui o heap em uma unidade
        (*n)--;  
        
        // coloca o ultimo elemento do heap na primeira posicao
        A[0] = A[*n];
        LEMEMLOG((long int)(&(A[*n])),sizeof(TipoOrdena),A[*n].id); 
        ESCREVEMEMLOG((long int)(&(A[0])),sizeof(TipoOrdena), A[0].id);
   
        
        // ordena
        Refaz(1, *n, A); 
    }
}

void Constroi(TipoOrdena *A, int n)   
// Descricao: chama a funcao refaz para a primeira metade do heap
// Entrada: *A, n
// Saida: *A 
{ 
    int Esq;
    
    Esq = n / 2 + 1;
    while (Esq > 1)   
    {
        Esq--;
        Refaz(Esq, n, A);  

    }
}

#endif