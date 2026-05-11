#include "fila.h" 
#include "msgassert.h"

void uso()
// Descricao: imprime as opcoes de uso
// Entrada: nao tem
// Saida: impressao das opcoes de linha de comando
{
    fprintf(stderr,"\tADD_URLS <qtd>             \t(adiciona urls ao escalonador) \n");
    fprintf(stderr,"\tESCALONA_TUDO              \t(escalona todas urls) \n");
    fprintf(stderr,"\tESCALONA <qtd>             \t(escalona uma quantidade de urls)\n");
    fprintf(stderr,"\tESCALONA_HOST <host> <qtd> \t(escalona urls de um host)\n");
    fprintf(stderr,"\tVER_HOST <host>            \t(mostra as urls do host)\n");
    fprintf(stderr,"\tLISTA_HOSTS                \t(mostra hosts)\n");
    fprintf(stderr,"\tLIMPA_HOST <host>          \t(apaga as urls do host)\n");
    fprintf(stderr,"\tLIMPA_TUDO                 \t(apaga todos hosts e urls)\n"); 
}

int main(int argc, char ** argv) 
// Descricao: programa principal para execucao do escalonamento dos hosts e urls
// Entrada: argc e argv
// Saida: depende da operacao escolhida  
{
    // verificacao da consistencia dos argumentos
    erroAssert((argv[1]),                              
        "matop - nome de arquivo com os comandos de escalonamento tem que ser definido");
    erroAssert((argv[2]),                              
        "matop - nome de arquivo de registro de acesso tem que ser definido");
        
    // iniciar registro de acesso
    iniciaMemLog(argv[2]);   

    // ativar ou nao o registro de acesso
    if (argv[3])
    {
        if(argv[3][0] == 'l')
        { 
            ativaMemLog();
        } 
    }
    else
    {
        desativaMemLog();
    }
  
    // numero de linhas a serem lidas do arquivo e nome dos comandos a serem realizados
    // (é preciso um char para receber via scanf e um string para comparar)
    int linhas;
    char temp[20];  
    std::string comando;
     
    // fila que serão alocados os hosts
    Fila *sequencia;   
    if ((sequencia = (Fila *) malloc (sizeof (Fila))) == NULL)              
    return -1;  
    inicializacao (sequencia);          

    // arquivo passado pela linha de comando
    FILE *coletado;  
    coletado = fopen(argv[1],"r");
    erroAssert(coletado != NULL,"Problemas na abertura do arquivo\n");  

    // manipulacao de string para transformar o arquivoDeEntrada.txt em arquivoDeEntrada-out.txt
    char* saida =(char*)malloc(((strlen(argv[1])+4)*sizeof(char)));    
    int a;
    int strTamanho = strlen(argv[1]);
    for(a = 0; a < strTamanho ; a++)   
    {
        saida[a]=argv[1][a];  
        if(saida[a] == '.')
        {  
            saida[a] = '-';   
            saida[a+1] = 'o';
            saida[a+2] = 'u';
            saida[a+3] = 't';
            saida[a+4] = '.';
            saida[a+5] = 't';      
            saida[a+6] = 'x';  
            saida[a+7] = 't';
            break;  
        } 
    }
    FILE *out;  
    out = fopen(saida,"w" );                  
    erroAssert(out != NULL,"Problemas na abertura do arquivo\n");   

    // le e executa comandos do arquivo de entrada até o fim do arquivo
    while(!feof(coletado))
    {
        // le o comando e o numero de linhas do arquivo 
        // para fazer a comparação do comando, o char* é transformado em string
        fscanf(coletado,"%s %d", temp, &linhas);
  
        comando = temp; 

        if (comando == "ADD_URLS")
        {  
            defineFaseMemLog(0); 

            int i;
            int j;
            int k;
            for(i = 0; i < linhas; i++) 
            {
                //coleta o url e o transforma em string para facilitar manipulação
                char no[100];  
                fscanf(coletado,"%s", no);
                LEMEMLOG((long int)(&(coletado)),sizeof(char*),i);  

                std::string host = no;
                
                //verifica se a extensão é válida, se não for, descarta url
                bool extensaoVal;
                if(host[7] != 'w' && host[8] != 'w' && host[9] != 'w'  && host[host.length()-4] == '.')   
                {
                    if(host[host.length()-3] == 'p' && host[host.length()-2 == 'd'] &&  host[host.length()-1 == 'f'] )
                       extensaoVal = false;
                    if(host[host.length()-3] == 'd' && host[host.length()-2 == 'o'] &&  host[host.length()-1 == 'c'] )
                       extensaoVal = false;
                    if(host[host.length()-3] == 'a' && host[host.length()-2 == 'v'] &&  host[host.length()-1 == 'i'] )
                       extensaoVal = false;
                    if(host[host.length()-3] == 'm' && host[host.length()-2 == 'p'] &&  host[host.length()-1 == '3'] )
                       extensaoVal = false;
                    if(host[host.length()-3] == 'g' && host[host.length()-2 == 'i'] &&  host[host.length()-1 == 'f'] )
                       extensaoVal = false;
                    if(host[host.length()-3] == 'j' && host[host.length()-2 == 'p'] &&  host[host.length()-1 == 'g'] )
                       extensaoVal = false;
                }
                else
                extensaoVal = true;
                
                if(extensaoVal == true)
                {
                    //se houver " www.", retira do url
                    if(no[7] == 'w' && no[8] == 'w' && no[9] == 'w')  
                        host.erase(7,4); 
                        
                    //verifica se a extensão http, se não for, descarta url
                    if(host[0] == 'h' && host[1] == 't' && host[2] == 't' && host[3] == 'p')
                    {      
                        //declaracao das variáveis que decidirao a profundidade
                        int barra = 1;
                        host.erase(0,7);
                        int dimensao = host.length();
                        
                        for(j = 0; j < dimensao + 1; j++) 
                        {
                            if(host[j] == '/')
                            {
                                //retira o ultimo "/", se houver
                                if(host[host.size()-1] == '/')         
                                    host = host.substr(0, host.size()-1); 
                                    
                                //atualiza a profundidade do url e retira o fragmento #
                                for(k = 0; k < dimensao   ; k++)
                                {   
                                    if(host[k] == '/')
                                    {             
                                        barra ++;
                                    }
                                    if(host[k] == '#') 
                                    {
                                        host = host.substr(0, host.size() - (host.size()-k )) ;      
                                        break;  
                                    }  
                                }     
                        
                                //separa o host do url
                                char url [host.length() + 1];
                                strcpy(url, host.c_str());  
                                
                                //deixa o host com o formato devido e transforma em char*, para alocação da Fila
                                std::string st = host.substr(0, host.size() - (host.size() -  j) );  
                                char *nome = &st[0];
                                
                                //se nao houver host, insere host na fila e url na lista, se já houver host, insere url na devida lista
                                if(nexiste(sequencia, nome, url, barra,i) == true)
                                    inserir (sequencia, sequencia->fim, nome,  url,barra,i);     
                                break;   
                            }             
                        } 
                    } 
                } 
            }
        }  
        
        else if(comando == "LISTA_HOSTS")
        {
            //imprime no arquivo todos hosts da fila
            exibir (sequencia, out);   
        }
        
        else if(comando == "LIMPA_TUDO")
        {

            // Chave para iterar a fila
            Chave * atual;
            atual = sequencia ->inicio;
            int tamFila = sequencia->tamanho;
            int i;
            int j;
            
            // itera todas as lista e remove todas as urls de todos hosts
            for(i = 0; i < tamFila; i++)
            {
                int tamLista = atual->lista.tam;
                for(j = 0; j < tamLista; j++) 
                    remover(&atual->lista); 
                atual = atual->seguinte; 
            }
            
            //remove todos hosts da fila
            for(i = 0; i < tamFila; i++)
                remover(sequencia);

        } 
        
        else if(comando == "LIMPA_HOST") 
        {  
            int i;
            int j;
            
            // Chave para iterar a fila
            Chave *atual;         
            atual = sequencia->inicio; 
            
            char temp[MAXTAM];        
            fscanf(coletado,"%s ", temp);   
            
            // transforma em string para facilitar comparação
            std::string paraLimpar = temp;
            
            //procura pelo host e remove todos os ulrs, sem remover o host
            for(i = 0; i < sequencia->tamanho; i++)
            {
                if(atual->host == paraLimpar)
                { 
                    for(j = 0; j < atual->lista.tam; j++) 
                        remover(&atual->lista); 
                break; 
                }
                atual = atual->seguinte;    
            }
        }
        
        else if(comando == "VER_HOST")
        {
            // Chave para iterar a fila
            Chave * atual;
            atual = sequencia ->inicio;
            
            char temp[MAXTAM];
            int i;
            fscanf(coletado,"%s ", temp); 
            
            // transforma em string para facilitar comparação
            std::string paraExibir = temp; 
            
            // procura pelo host e imprime no arquivo todas urls dele
            for(i = 0; i < sequencia->tamanho; i++)
            {
                if(atual->host == paraExibir )
                {
                    imprimir(atual->lista, atual->lista.tam,out);      
                    break;   
                }    
                atual = atual->seguinte; 
            }
        }
        else if(comando == "ESCALONA_HOST")
        {
            // Chave para iterar a fila
            Chave * atual;
            atual = sequencia ->inicio;
            char temp[MAXTAM];      
            int quant;
            int i;
            int j;
            fscanf(coletado,"%s %d ", temp,& quant); 
            
            // transforma em string para facilitar comparação
            std::string paraEscal = temp;   
            
            //procura pelo host e escalona quantidade pedida e em seguida remove os urls impressos
            for(i = 0; i < sequencia->tamanho; i++) 
            {  
                if(atual->host == paraEscal)  
                {   
                    imprimir(atual->lista,quant, out);
                    for(j = 0; j < quant; ++j)      
                    {
                        remover(&atual->lista); 
                    }
                    break; 
                }
                atual = atual->seguinte;
            } 
        }
        
        else if(comando == "ESCALONA_TUDO")
        {
            defineFaseMemLog(1); 
            
            // Chave para iterar a fila
            Chave * atual;
            atual = sequencia ->inicio;
            
            int tamFila = sequencia->tamanho;
            int i;
            int j;
            
            //imprime todas as urls de todos os hosts na ordem depth-first e então apaga as urls
            for(i = 0; i < tamFila; i++)
            {
                if(atual->lista.tam != 0)
                    imprimir(atual->lista, atual->lista.tam, out );   
                int tamLista = atual->lista.tam;     
                for(j = 0; j < tamLista; j++) 
                    remover(&atual->lista); 
                atual = atual->seguinte; 
            }   
        } 
        
        else if(comando == "ESCALONA")
        {
            // Chave para iterar a fila
            Chave * atual;
            atual = sequencia ->inicio;
             
            int i; 
            fscanf(coletado,"%d",& linhas);
            
            //imprime a quantidade de urls pedidas na ordem depth-first e então apaga as urls
            while(linhas > 0)
            {
                //imprime e apaga as urls que sobraram para serem escalonadas do host
                if(atual->lista.tam >= linhas)
                {
                    imprimir(atual->lista, linhas, out);
                    for(i = 0; i< atual->lista.tam; i++) 
                        remover(&atual->lista);
                    linhas = linhas - linhas;
                }
                
                //imprime e apaga todas as urls do host
                if(atual->lista.tam < linhas)
                {
                    imprimir(atual->lista, atual->lista.tam, out );
                    linhas = linhas - atual->lista.tam;
                    for(i = 0; i< linhas; i++)
                        remover(&atual->lista);
                    atual = atual->seguinte;   
                }
            } 
        }
        
        // caso apareça alguma opção inválida no arquivo
        else
        {
            uso();
            exit(1);  
        }
    }   
    
    //limpa todos os hosts e urls, caso não tenham sido limpados antes. Isso evita que a haja vazamento de memoria
    Chave * atual;
    atual = sequencia ->inicio;
    int tamFila = sequencia->tamanho;
    int i;
    int j;
    for(i = 0; i < tamFila; i++)
    {
        int tamLista = atual->lista.tam;
 
        for(j = 0; j < tamLista; j++) 
            remover(&atual->lista); 
        atual = atual->seguinte; 
    }   
    for(i = 0; i < tamFila; i++)
        remover(sequencia);
   
    // libera a memoria alocada no main e fecha os arquivos usados                    
    free(sequencia);
    fclose(coletado);
    fclose(out); 
    free(saida); 
    
    // conclui registro de acesso
    return finalizaMemLog();  
}