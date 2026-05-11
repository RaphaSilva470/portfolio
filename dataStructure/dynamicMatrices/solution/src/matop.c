//---------------------------------------------------------------------
// Arquivo      : matop.c
// Conteudo     : programa de avaliacao do TAD MAT 
// Autor        : Wagner Meira Jr. (meira@dcc.ufmg.br)
// Historico    : 2021-10-18 - arquivo criado
//              : 2021-10-21 - estrutura de diretorios
//              : 2021-10-25 - opcoes de linha de comando 
//---------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <string.h>
#include "mat.h"
#include "memlog.h" 
#include "msgassert.h" 

// definicoes de operacoes a serem testadas
#define OPSOMAR 1
#define OPMULTIPLICAR 2
#define OPTRANSPOR 3

// variaveis globais para opcoes
static int opescolhida;
char lognome[100];
char m1[70];
char m2[70];
char m3[70];
int regmem;
 
void uso()
// Descricao: imprime as opcoes de uso
// Entrada: nao tem
// Saida: impressao das opcoes de linha de comando
{
  fprintf(stderr,"matop\n");
  fprintf(stderr,"\t-s \t(somar matrizes) \n");
  fprintf(stderr,"\t-m \t(multiplicar matrizes) \n");
  fprintf(stderr,"\t-t \t(transpor matriz)\n");
  fprintf(stderr,"\t-p <arq>\t(arquivo de registro de acesso)\n");
  fprintf(stderr,"\t-1 <arq>\t(matriz 1)\n");
  fprintf(stderr,"\t-2 <arq>\t(matriz 2)\n");
  fprintf(stderr,"\t-o <arq>\t(matriz resultante)\n");
  fprintf(stderr,"\t-l \t(registrar acessos a memoria)\n");
}


void parse_args(int argc,char ** argv)
// Descricao: le as opcoes da linha de comando e inicializa variaveis
// Entrada: argc e argv
// Saida: optescolhida, m1, m2, m3, regmem, lognome
{
     // variaveis externas do getopt
     extern char * optarg;
     extern int optind;

     // variavel auxiliar
     int c;

     // inicializacao variaveis globais para opcoes
     opescolhida = -1;
     regmem = 0;
     lognome[0] = 0;
     m1[0] = 0;
     m2[0] = 0;
     m3[0] = 0; 
     // getopt - letra indica a opcao, : junto a letra indica parametro
     // no caso de escolher mais de uma operacao, vale a ultima
     while ((c = getopt(argc, argv, "smtp:o:1:2:lh")) != EOF)
       switch(c) {
         case 'm':
		  avisoAssert(opescolhida==-1,"Mais de uma operacao escolhida");
	          opescolhida = OPMULTIPLICAR;
                  break;
         case 's':
		  avisoAssert(opescolhida==-1,"Mais de uma operacao escolhida");
	          opescolhida = OPSOMAR;
                  break;
         case 't':
		  avisoAssert(opescolhida==-1,"Mais de uma operacao escolhida");
	          opescolhida = OPTRANSPOR;
                  break;
         case 'p': 
	          strcpy(lognome,optarg);
		  break;
         case '1': 
	          strcpy(m1,optarg);
		  break;
         case '2': 
	          strcpy(m2,optarg);
		  break;
         case 'o':
	          strcpy(m3,optarg);
         break;
         case 'l': 
	          regmem = 1;
		  break;
         case 'h':
         default:
                  uso();
                  exit(1);

       }
       // verificacao da consistencia das opcoes
       erroAssert(opescolhida>0,"matop - necessario escolher operacao");
       erroAssert(strlen(lognome)>0,
         "matop - nome de arquivo de registro de acesso tem que ser definido");
       erroAssert(strlen(m1)>0,
         "matop - nome de arquivo da matriz 1 tem que ser definido");
       erroAssert(strlen(m1)>0,
         "matop - nome de arquivo do resultado tem que ser definido");
}


int main(int argc, char ** argv)
// Descricao: programa principal para execucao de operacoes de matrizes 
// Entrada: argc e argv
// Saida: depende da operacao escolhida
{

  // ate 3 matrizes sao utilizadas, dependendo da operacao
  mat_tipo a, b, c;

  // avaliar linha de comando
  parse_args(argc,argv);

  // iniciar registro de acesso
  iniciaMemLog(lognome);

  // ativar registro de acesso
  ativaMemLog(); 

  // execucao dependente da operacao escolhida
  switch (opescolhida){
    case OPSOMAR:
         // verificacao da consistencia da segunda matriz
         erroAssert(strlen(m2)>0,
            "matop - nome de arquivo da matriz 2 tem que ser definido");
            
         // cria matrizes a e b aleatorias, que sao somadas para a matriz c
	 // matriz c é impressa e todas as matrizes sao destruidas
         criaMatriz(&a,m1 );
         criaMatriz(&b, m2);
         somaMatrizes(&a,&b,&c);
	 escreveMatriz(&c, m3);
         destroiMatriz(&a);
         destroiMatriz(&b);
         destroiMatriz(&c);
	 break;
    case OPMULTIPLICAR:
         // verificacao da consistencia da segunda matriz
         erroAssert(strlen(m2)>0,
            "matop - nome de arquivo da matriz 2 tem que ser definido");
            
         // cria matrizes a e b aleatorias, que sao multiplicadas para matriz c
	 // matriz c é impressa e todas as matrizes sao destruidas
         criaMatriz(&a,m1);
         criaMatriz(&b,m2);
         multiplicaMatrizes(&a,&b,&c);
	 escreveMatriz(&c, m3);
         destroiMatriz(&a);
         destroiMatriz(&b);
         destroiMatriz(&c);
	 break; 
    case OPTRANSPOR:
         // cria matriz a aleatoria, que e transposta, impressa e destruida
         criaMatriz(&a, m1);
	 transpoeMatriz(&a, &b);
	 escreveMatriz(&b, m3);
         destroiMatriz(&a); 
         destroiMatriz(&b); 
	 break;
   default:
         // nao deve ser executado, pois ha um erroAssert em parse_args
         uso();
	 exit(1);
  }

  // conclui registro de acesso
  return finalizaMemLog();
}


    
