#include <iostream>
#include <cstring>
#include "ordena.h" 
#include "ordena.cpp"
#include "memlog.h"  
#include "msgassert.h" 

int main(int argc, char ** argv)  
// Descricao: programa principal para execucao da ordenacao dos urls
// Entrada: argc e argv
// Saida: urls ordenadas de acordo com o numero de acessos
{ 
    // verificacao da consistencia dos argumentos   
    erroAssert((argv[1]),                              
        "nome de arquivo com as urls para ordenamento tem que ser definido")  ;
    erroAssert((argv[2]),                               
        "nome de arquivo de saída tem que ser definido");
    erroAssert((argv[3]),                               
        "o numero de entidades por rodada deve ser definido");
    erroAssert((argv[4]),                               
        "o numero de fitas deve ser definido");
    
    // iniciar registro de acesso
    iniciaMemLog(argv[5]);   

    // ativar ou nao o registro de acesso
    if (argv[6])
    {
        if(argv[6][0] == 'l')
        { 
            ativaMemLog();
        } 
    }
    else
    {
        desativaMemLog();
    }

    // arquivo com os dados passado pela linha de comando
    FILE *entrada;  
    entrada = fopen(argv[1],"r");
    erroAssert(entrada != NULL,"Problemas na abertura do arquivo\n");  

    // arquivo de saida
    FILE *out;  
    out = fopen(argv[2],"w" );                  
    erroAssert(out != NULL,"Problemas na abertura do arquivo\n"); 
    
    int numEnt = atoi(argv[3]);
    int numFitas = atoi(argv[4]);
    int numFitasCopia = numFitas;
    
    // variaveis auxiliares
    int i;
    int j;
    std::string aux;
    int numVisitas;
    TipoOrdena fitaRodada [numEnt];
    int endereco = 0;

    // heap e arquivos das rodadas
    TipoOrdena heap[numFitas]; 
    FILE *fitas[numFitas]; 
    defineFaseMemLog(0);  
    
    //escreve as urls ja ordenadas nas fitas
    for(i = 0; i < numFitas; i++) 
    { 

        // le o arquivo de entrada e aloca no TipoOrdena
        for(j = 0; j < numEnt; j++)
        {
            fscanf(entrada,"%s %d", fitaRodada[j].url, &numVisitas); 
            
            // popularidade deve ser positiva
            erroAssert(numVisitas >= 0,"Popularidade do url não pode ser negativa\n");   
 
            fitaRodada[j].SetChave(numVisitas);
            fitaRodada[j].id = endereco;
            ESCREVEMEMLOG((long int)(&(fitaRodada[j])),sizeof(TipoOrdena),fitaRodada[j].id); 
        }
        
        // ordena TipoOrdena de acordo com sua chave
        QuickSort(fitaRodada, numEnt);

        // variaveis para nomear o arquivo da rodada 
        aux = "rodada-";
        aux += std::to_string(i); 
        aux += ".txt"; 
        char *nome = &aux[0];  
        
        // abre o arquivo rodada-n.txt
        FILE *rodadas;  
        rodadas = fopen(nome,"w" );                    
        erroAssert(rodadas != NULL,"Problemas na abertura do arquivo\n");   
        
        // escreve no arquivo rodada-n.txt os TipoOrdena 
        for(j = 0; j < numEnt; j++) 
        {
            fprintf(rodadas, "%s %d",fitaRodada[j].url, fitaRodada[j].GetChave()); 
            LEMEMLOG((long int)(&(fitaRodada[j])),sizeof(TipoOrdena),fitaRodada[j].id); 

            // escreve a quebra de linha no arquivo sem deixar a ultima linha em branco
            // se a ultima linha tiver em branco vai haver erro ao ler o arquivo
            if(j < numEnt -1)
            {
                fprintf(rodadas, "\n"); 
            }
        }
        fclose(rodadas);
    }
    
	defineFaseMemLog(1);
    endereco ++;
    
    // adiciona o primeiro TipoOrdena de cada arquivo rodada-n.txt ao heap
    for(i = 0; i < numFitas; i++)
    { 
        // variaveis para definir o arquivo a ser lido
        aux = "rodada-";
        aux += std::to_string(i);   
        aux += ".txt"; 
        char *nome = &aux[0]; 
        
        // abre o arquivo rodada-n.txt
        fitas[i] = fopen(nome, "r"); 

        // adiciona o primeiro TipoOrdena ao heap
        fscanf(fitas[i], "%s %d",heap[i].url, &numVisitas );    
        heap[i].SetChave(numVisitas);
        
        // adiciona ao TipoOrdena o arquivo de onde ele veio
        heap[i].arquivo = i; 
        heap[i].id = endereco; 
        ESCREVEMEMLOG((long int)(&(heap[i])),sizeof(TipoOrdena), heap[i].id); 
    } 
    
    // ordena o heap de acordo com a Chave do TipoOrdena
    Constroi (heap, numFitas);  
    
    // imprime no arquivo de saida as urls ordenadas
    while(numFitas > 0)   
    {
        // imprime na saida o primeiro TipoOrdena do heap
        fprintf(out, "%s %d",heap[0].url,heap[0].GetChave());   
        LEMEMLOG((long int)(&(heap[0])),sizeof(TipoOrdena),heap[0].id);
        
        // adiciona ao heap um TipoOrdena do arquivo na qual foi tirado um tipoFita 
        // se o arquivo tiver acabado, ele remove um elemento do heap
        if (!feof(fitas[heap[0].arquivo])) 
        {
            fscanf(fitas[heap[0].arquivo], "%s %d",heap[0].url, &numVisitas );  
            heap[0].SetChave(numVisitas);
            ESCREVEMEMLOG((long int)(&(heap[0])),sizeof(TipoOrdena), heap[0].id);

            fprintf(out, "\n");  

            //Refaz (numFitas, heap);  
            Refaz(1, numFitas, heap);
 
        }  
        else
        {
            RetiraMax(heap, &numFitas);
            
            // nao imprime linha a mais
            if(numFitas != 0)
            fprintf(out, "\n");  
        }
    }

    for(i = 0; i < numFitasCopia; i++)
    fclose(fitas[i]);
    
    fclose (out); 
    fclose (entrada); 
    
    // conclui registro de acesso
    return finalizaMemLog();  
}