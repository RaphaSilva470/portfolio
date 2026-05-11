    #include <dirent.h>      
#include <algorithm>
#include <cstring> 
#include <math.h>  
#include <getopt.h>
#include "documento.h"
#include "indice.h" 
#include "msgassert.h"
#include "memlog.h"

// variaveis globais para opcoes 
char lognome[100];
char i [100];
char o [100];
char co [100];
char s[100];
int regmem;

int hashing (std::string temp)
// Descricao: hashing de uma palava usando peso
// Entrada: temp
// Saida: soma
{
    int soma = 0;
    int size = temp.length();
    
    // faz o hasing com peso nas posicoes
    for(int k = 0;k < size; k++)
    {
        int ascii = (int) temp[k];  
        soma = soma + ((ascii - 96) * (k + 1));     
    } 
    return soma;
}
void uso()
// Descricao: imprime as opcoes de uso
// Entrada: nao tem
// Saida: impressao das opcoes de linha de comando
{
  fprintf(stderr,"buscar\n");
  fprintf(stderr,"\t-i \t(consulta) \n");
  fprintf(stderr,"\t-o \t(ranking - saida) \n");
  fprintf(stderr,"\t-c \t(corpus)\n");
  fprintf(stderr,"\t-s <arq>\t(stopwords)\n");
}

void parse_args(int argc,char ** argv)
// Descricao: le as opcoes da linha de comando 
// Entrada: argc e argv
// Saida: i, o, co, s, lognome
{
     // variaveis externas do getopt
     extern char * optarg;

     // variavel auxiliar
     int c;

     // inicializacao variaveis globais para opcoes
    regmem = 0;
    lognome[0] = 0;
    i [0] = 0;
    o [0] = 0;
    co [0] = 0;
    s [0] = 0;
    
     // getopt - letra indica a opcao, : junto a letra indica parametro
     while ((c = getopt(argc, argv, "i:o:c:s:p:")) != EOF)
       switch(c) {
         case 'i':
	          strcpy(i,optarg);
                  break;
         case 'o':
	          strcpy(o,optarg);
                  break;
         case 'c':
	          strcpy(co,optarg);
                  break;
         case 's': 
	          strcpy(s,optarg);
		  break;
        case 'p': 
	          strcpy(lognome, optarg);
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
       erroAssert(strlen(lognome)>0,
         "buscar - nome de arquivo de registro de acesso tem que ser definido");
       erroAssert(strlen(i)>0,
         "buscar - nome de arquivo da consulta tem que ser definido");
       erroAssert(strlen(o)>0,
         "buscar - nome de arquivo do resultado tem que ser definido");
       erroAssert(strlen(co)>0,
         "buscar - nome de arquivo do corpus tem que ser definido"); 
       erroAssert(strlen(s)>0,
         "matop - nome de arquivo do stopwords tem que ser definido");
}

int main(int argc, char ** argv)    
// Descricao: programa principal para maquina de busca 
// Entrada: argc e argv
// Saida: arquivo com os 10 melhores resultados da busca
{
  // avaliar linha de comando
    parse_args(argc, argv);
    
    // abre arquivo da consulta
    FILE * consulta;
    consulta = fopen(i,"r"); 
    erroAssert(consulta != NULL,"Problemas na abertura do arquivo\n"); 
    
    // abre arquivo de saida, que serao escritos resultados
    FILE * ranking;
    ranking = fopen(o,"w");
    erroAssert(ranking != NULL,"Problemas na abertura do arquivo\n"); 
 
    // abre arquivo com stopwords
    FILE * stop;
    stop = fopen(s,"r");
    erroAssert(stop != NULL,"Problemas na abertura do arquivo\n"); 

    // iniciar registro de acesso
    iniciaMemLog(lognome);

    // ativar ou nao o registro de acesso
    if (regmem)
    { 
        ativaMemLog();
    }
    else
    {
        desativaMemLog();
    }
    
    // le as stopwords e coloca em um array
    char stopwords[127][20];
    for(int i =0; i < 127; i++)
    {
        fscanf(stop,"%s", stopwords[i]); 
    } 

    // cria o indice invertido e inicializa
    Indice ind[10001];   
    for(int i = 0; i < 10001; i++)
    {
        inicializacao(&ind[i]);
    }
    int tamanho_corpus = 0;
    std::string aux;
    char texto[1000]; 
    double wd[20000] = {0};
    char * text;  

    // abre o diretorio do corpus
    DIR *dir;
    struct dirent *lsdir;
    dir = opendir(co);  
    
    // abre um arquivo para escrever as palavras encontradas em um arquivo
    FILE * temp; 

    // percorre todo o diretorio
    while ( ( lsdir = readdir(dir) ) != NULL )
    {
        // abre um arquivo do corpus
        aux = co;
	    aux += "/"; 
        aux += lsdir->d_name;
        FILE* atual;
        char *nome = &aux[0]; 
        atual = fopen(nome, "r");  
        
        // copia conteudo do arquivo aberto
        fgets(texto, sizeof(texto), atual);  

        // tira pontuacoes, numeros e coloca em lowercase
        std::string manip;
        manip = texto; 
        //manip.erase(std::remove_if(manip.begin(), manip.end(), ispunct), manip.end());
        int size = manip.length();
        for(int i = 0; i < size; i++)
        {
            manip[i] = tolower(manip[i]); 
            if(int(manip[i]) < 97 || manip[i] >122)
            {
                manip[i] = ' '; 
            }
        }
        text = &manip[0]; 
        
        // abre e escreve o texto manipulado em um arquivo temporario
        temp = fopen("vocabulario", "w"); 
        fputs(text, temp);
        fclose(temp); 

        // abre o arquivo temporario
        temp = fopen("vocabulario", "r");
        
        // cria a lista de vocabulario 
        Vocabulario lista;
        criar_lista (&lista);
         
        //insere cada palavra na lista de vocabulario
         while(!feof(temp))
        {
            char palavra[50];
            fscanf(temp, "%s", palavra);
            inserir_ordenado(&lista, palavra);
        }
        
        // variaveis para percorrer a lista
        Palavra *aux; 
        aux = lista.inicio;   
        int listaTam = lista.tam;
        int a = 0;
        
        // insere os dados no indice invertido
        while(a < listaTam)     
        {
            // variaveis para fazer o hashing
            std::string temp = aux->word;  
            
            // faz o hasing com peso nas posicoes
            int soma = hashing(temp);

            // verifica se a palavra do vocabulario esta nas stopwords
            for(int i = 0; i < 127; i++)
            {
                if(aux->word == stopwords[i])
                {
                    aux = aux->proximo;      
                    a++;
                    tamanho_corpus ++;
                    continue;
                } 
            }

            if(soma >=0)
            {
                // insere no indice invertido
                if(soma <= 10000)
                    inserir(&ind[soma], ind->fim, atoi(lsdir->d_name), aux->qtd, aux->word);
                if(soma > 10000)
                    inserir(&ind[10001], ind->fim, atoi(lsdir->d_name), aux->qtd, aux->word);
            }
            tamanho_corpus++; 
            a++;   
            aux = aux->proximo;  
        }
        // calcula wd
        wd[atoi(lsdir->d_name)] = sqrt(lista.tam);
        remover(&lista);  
        fclose(temp);
        fclose(atual);  
    }  
    

    
    // variaveis para realizar a consulta
    char consul[50];
    double wq = 0;
    double idf = 0;
    int osoma;
    double resultado [20000] = {0};
    int qtd = 0;

    // calcula para cada termo da consulta
    while(!feof(consulta))  
    {
        double wqt;
        bool continua = false;
        fscanf(consulta, "%s ", consul);

        std::string temp = consul;
        int size = temp.length();
        //verifica se a consulta e valida
        for(int i = 0; i < size; i++)
        {
            erroAssert(int(temp[i]) >= 97,"Consulta invalida\n"); 
        }
        
        // transforma o termo em hashing usando peso
        osoma = hashing(temp);  
        if (osoma > 10000)
        osoma = 10001;  
        
        // variaveis para percorrer a lista no indice invertido
        Chave *atual; 
        atual = ind[osoma].inicio;
        
        // verifica se o termo esta no indice
        for (int i = 0; i < ind[osoma].tamanho; i++)
        {
            if(temp == atual->palavra)
            {
                continua = true;
                break;
            }
            else
            {
                atual = atual->seguinte;
                continua = false;
            }
        }
        int docs [ind[osoma].tamanho][2];
        atual = ind[osoma].inicio; 

        // olha quais documentos possuem o termo
        int quant = 0;
        if(continua == true)
        {
            for (int i = 0; i < ind[osoma].tamanho; i++)
            {
                if(temp == atual->palavra)
                {
            
                    docs[i][0] = atual->nome_doc;
                    docs[i][1] = atual->qtd;
                    quant++;
                    atual = atual->seguinte;  
                }
                else
                {
                    docs[i][0] = -1;
                    docs[i][1] = -1;                
                    atual = atual->seguinte;
                }
            }
            // calcula idf, wqt e wq
            idf = log(tamanho_corpus/quant);
            wqt = idf; 
            wq = wq + (wqt * wqt);
        
            // passa para resultado o resultado de cada arquivo seguindo o indice 
            for (int i = 0; i < ind[osoma].tamanho; i++)
            {
                if(docs[i][0] != -1)
                {
                    qtd++;
                    int tf = docs[i][1];
                    int wdt = tf * idf;
                    resultado[docs[i][0]] = resultado[docs[i][0]] + (wqt * wdt);
                }
            }
        }
    }
    double imprimir[qtd][2];
    wq = sqrt(wq);
    int j = 0;
    
    // normaliza o resultado e passa o resultado e seu respectivo arquivo para a array imprimir
    for(int i = 0; i < 20000; i++)
    {
        resultado[i] = resultado[i]/(wq * wd[i]);
        if  (resultado[i] > 0) 
        {
            if(j < qtd)
            {
                imprimir[j][0] = i;
                imprimir[j][1] = resultado[i];
                j++;
            }
        }
    }
    int index;
    
    // imprime no arquivo de saida os arquivos com maior resultado
    // em caso de empate sera inserido de acordo com id do documento
    if(qtd >= 10)
    {
        for(int i = 0; i < 10; i++)
        {
            double result = 0;

            for(int j = 0; j < qtd; j++)
            {
                if(imprimir[j][1] > result)
                {
                    index = j;
                    result = imprimir[j][1];
                }
            }
            fprintf(ranking, "%.0f ", imprimir[index][0]);
            imprimir[index][1] = 0;  
        }
    }
    else
    {
        for(int i = 0; i < qtd; i++)
        {
            double result = 0;

            for(int j = 0; j < qtd; j++)
            {
                if(imprimir[j][1] > result)
                {
                    index = j;
                    result = imprimir[j][1];
                }
            }
            fprintf(ranking, "%.0f ", imprimir[index][0]);
            imprimir[index][1] = 0;  
        }
    }
    for(int i = 0; i < 10000; i++)
    {
        remover(&ind[i]); 
    }
    closedir(dir);   
    fclose(consulta);
    fclose(ranking);
    fclose(stop);
    return finalizaMemLog();
} 
    
