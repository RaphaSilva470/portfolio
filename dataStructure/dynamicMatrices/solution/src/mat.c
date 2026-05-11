//---------------------------------------------------------------------
// Arquivo	: mat.c
// Conteudo	: implementacao do TAD MAT
// Autor	: Wagner Meira Jr. (meira@dcc.ufmg.br)
// Historico	: 2021-10-18 - arquivo criado
//		: 2021-10-21 - estrutura de diretorios
//---------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mat.h"
#include "memlog.h"
#include "msgassert.h"

// Macro que realiza swap sem variavel auxiliar
#define ELEMSWAP(x,y) (x+=y,y=x-y,x-=y) 

void criaMatriz(mat_tipo* mat, char* m)
// Descricao: le arquivo e cria matriz
// Entrada: mat, m
// Saida: mat
{
  FILE *matriz;

  // lendo matriz:
  matriz = fopen(m,"r");
  
  // verifica se o arquivo e valido
  erroAssert(matriz != NULL,"Problemas na abertura do arquivo\n"); 

  // inicializa a matriz
  fscanf(matriz,"%d %d",&mat->tamx,&mat->tamy);
  escreveMemLog((long int)(&(mat->tamx)),sizeof(int));
  escreveMemLog((long int)(&(mat->tamy)),sizeof(int));
  inicializaMatrizNula(mat);

  // transcreve o arquivo para a matriz
  for(int i = 0; i < mat->tamx; i++)
  {
    for(int j = 0; j < mat->tamy; j++)
    { 
      fscanf(matriz,"%lf",& mat->m[i][j]);
      escreveMemLog((long int)(&(mat->m[i][j])),sizeof(double));
    }
  }
  fclose(matriz);

  // verifica se os valores de linha e coluna são validos
  erroAssert(mat->tamx>0,"Dimensao nula");
  erroAssert(mat->tamy>0,"Dimensao nula");
}

void inicializaMatrizNula(mat_tipo * mat)
// Descricao: inicializa mat com valores nulos
// Entrada: mat
// Saida: mat
{
  // inicializa todos os elementos da matriz com 0, por seguranca
  mat->m = (double **) malloc (mat->tamx * sizeof (double*)) ;

  // aloca um vetor com todos os elementos da matriz
  mat->m[0] = (double *) malloc (mat->tamx * mat->tamy * sizeof (double)) ;

  // ajusta os demais ponteiros de linhas (i > 0)
  for (int i=1; i < mat->tamx; i++)
  {
    mat->m[i] = mat->m[0] + i * mat->tamy ;
  }
  
  for (int i=0; i < mat->tamx; i++)
  {
    for (int j=0; j < mat->tamy; j++) 
    {
      mat->m[i][j] = 0;
      escreveMemLog((long int)(&(mat->m[i][j])),sizeof(int));
    }
  }
}

void escreveMatriz(mat_tipo * mat, char* m)
// Descricao: le a matriz e escreve no arquivo
// Entrada: mat
// Saida: m
{
  int i,j;
  FILE* matriz;
  matriz = fopen(m,"w");
  
  // verifica se o arquivo e valido
  erroAssert(matriz != NULL,"Problemas na abertura do arquivo\n"); 


  // escreve o tamanho da matriz
  fprintf(matriz,"%d %d\n",mat->tamx,mat->tamy);
  escreveMemLog((long int)(&(mat->tamx)),sizeof(int));
  escreveMemLog((long int)(&(mat->tamy)),sizeof(int));
  
  // escreve a matriz
  for (i=0; i<mat->tamx; i++){
    for(j=0; j<mat->tamy; j++){
      fprintf(matriz," %.6f",mat->m[i][j]);
      escreveMemLog((long int)(&(mat->m[i][j])),sizeof(double));
    }
      fprintf(matriz,"\n");
  }
  fclose(matriz);
}

void somaMatrizes(mat_tipo *a, mat_tipo *b, mat_tipo *c)
// Descricao: soma as matrizes a e b e armazena o resultado em c
// Entrada: a, b
// Saida: c
{
  int i,j;
  // verifica se as dimensoes das matrizes a e b sao as mesmas
  erroAssert(a->tamx==b->tamx,"Dimensoes incompativeis");
  erroAssert(a->tamy==b->tamy,"Dimensoes incompativeis");

  // inicializa a matriz c garantindo a compatibilidade das dimensoes
  c->tamx = a->tamx;
  c->tamy = a->tamy;
  inicializaMatrizNula(c);

  // faz a soma elemento a elemento
  for (i=0; i<a->tamx; i++){
    for(j=0; j<a->tamy; j++){
      c->m[i][j] = a->m[i][j]+b->m[i][j];
      leMemLog((long int)(&(a->m[i][j])),sizeof(double));
      leMemLog((long int)(&(b->m[i][j])),sizeof(double));
      escreveMemLog((long int)(&(c->m[i][j])),sizeof(double));
    }
  }
}

void multiplicaMatrizes(mat_tipo *a, mat_tipo *b, mat_tipo *c)
// Descricao: multiplica as matrizes a e b e armazena o resultado em c
// Entrada: a,b
// Saida: c
{
  int i,j,k;
  // verifica a compatibilidade das dimensoes
  erroAssert(a->tamy==b->tamx,"Dimensoes incompativeis");

  // inicializa a matriz c garantindo a compatibilidade das dimensoes
  c->tamx = a->tamx;
  c->tamy = b->tamy;
  inicializaMatrizNula(c);

  // realiza a multiplicacao de matrizes
  for (i=0; i<c->tamx;i++){
    for (j=0; j<c->tamy;j++){
      for (k=0; k<a->tamy;k++){
        c->m[i][j] += a->m[i][k]*b->m[k][j];
        leMemLog((long int)(&(a->m[i][k])),sizeof(double));
        leMemLog((long int)(&(b->m[k][j])),sizeof(double));
        escreveMemLog((long int)(&(c->m[i][j])),sizeof(double));
      }
    }
  }
}

void transpoeMatriz(mat_tipo *a, mat_tipo *b)
// Descricao: transpoe a matriz a
// Entrada: a
// Saida: a
{
  int i,j;

  // inicializa a matriz c garantindo a compatibilidade das dimensoes
  b->tamx = a->tamy;
  b->tamy = a->tamx;
  inicializaMatrizNula(b);
  
  // faz a transposicao como se a matriz fosse quadrada
  for (i=0; i<b->tamx; i++){
    for(j=0; j<b->tamy; j++){
      b->m[i][j] = a->m[j][i];
      leMemLog((long int)(&(a->m[j][i])),sizeof(double));
      escreveMemLog((long int)(&(b->m[i][j])),sizeof(double));
    }
  }
  // inverte as dimensoes da matriz transposta
  ELEMSWAP(a->tamx,a->tamy);
}

void destroiMatriz(mat_tipo *a) 
// Descricao: destroi a matriz a, que se torna inacessível
// Entrada: a
// Saida: a
{
  // apenas um aviso se a matriz for destruida mais de uma vez
  avisoAssert(((a->tamx>0)&&(a->tamy>0)),"Matriz já foi destruida");

  // torna as dimensoes invalidas
  a->tamx = a->tamy = -1;
  // libera a memória da matriz
  free (a->m[0]) ;
  free (a->m) ;
}
