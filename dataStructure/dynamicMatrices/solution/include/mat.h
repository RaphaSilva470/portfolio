//--------------------------------------------------------------------
// Arquivo	: mat.h
// Conteudo	: definicoes do TAD MAT 
// Autor	: Wagner Meira Jr. (meira@dcc.ufmg.br)
// Historico	: 2021-10-18 - arquivo criado
//		: 2021-10-21 - estrutura de diretorios
//---------------------------------------------------------------------

#ifndef MATH
#define MATH

#include "memlog.h"

typedef struct mat{
	double **m;
	int tamx, tamy;
} mat_tipo;

void criaMatriz(mat_tipo * mat, char* m);
void inicializaMatrizNula(mat_tipo * mat);
void escreveMatriz(mat_tipo * mat, char* m);
void somaMatrizes(mat_tipo * a, mat_tipo * b, mat_tipo * c);
void multiplicaMatrizes(mat_tipo * a, mat_tipo * b, mat_tipo * c);
void transpoeMatriz(mat_tipo *a, mat_tipo *b);
void destroiMatriz(mat_tipo *a);
#endif
  