#include <vector>
#include <algorithm>
#include<iostream>

#define MAX 1000
void ordenar(std::vector<std::pair <int, int>>& vetor)
// ordena os casos de empate do vetor de acordo com id do visitante
{
    for(int i = 0; i < vetor.size(); i++)
    {
        int swap = 0;
        for(auto it = vetor.begin(); it != vetor.end(); it++)
        {
            // se houver empate na nota
            if(it->first == (it+1)->first)
            {
                // se não estiver ordenado segundo id
                if(it->second > (it+1)->second)
                {
                    //troca o segundo elemento do vetor
                    int troca = it->second;
                    int troca2 = (it+1)->second;
                    it->second = troca2;
                    (it+1)->second = troca;
                    //troca o primeiro elemento do vetor
                    troca = it->first;
                    troca2 = (it+1)->first;
                    it->first = troca2;
                    (it+1)->first = troca;
                    swap ++;
                }
            }
        }
        // se o vetor foi percorrido mas nao houve trocas
        if (swap == 0)
        {
            break;
        }
    }
}
void BFS(char mapa[MAX][MAX], std::vector<std::pair <int, int>>& vis, int qtd, int larg, int comp)
// retorna todas as distancias entre uma bicicleta e os visistantes, ja ordenado
{
    //faz uma copia do mapa, para fazer marcaçao de ja visitado
    char copia[comp][larg];
    for(int i = 0; i < comp; i++)
    {
        for(int j = 0; j < larg; j++)
        {
            copia[i][j] = mapa[i][j];
        }
    }

    int dist = 0;
    std::vector<std::pair <int, int>>viz;
    std::vector<std::pair <int, int>>distancia;

    // enquanto não foi descoberto a distancia para todos os visistantes
    while(distancia.size() < qtd )
    {
        dist ++;
        // percorre o vetor com indices para visitar
        for (auto it = vis.begin(); it != vis.end(); it++)
        {
            // flag que o indice foi visitado
            copia[it->first][it->second] = 'n';

            // olha os indices visitaveis em volta
            if(copia[it->first + 1][it->second] != 'n' && copia[it->first + 1][it->second] != '-' && it->first + 1 < comp)
            {
                // coloca na lista para ser visitado
                viz.push_back( std::make_pair(it->first + 1, it->second));
                // se for um visitante
                if(int (copia[it->first + 1][it->second]) >= 97 && int (copia[it->first + 1][it->second] <= 106 && it->first + 1 < comp))
                {
                    // coloca no vetor a distancia e o visitante
                    distancia.push_back( std::make_pair(dist, copia[it->first + 1][it->second] - 49));
                }
                // flag que o indice foi visitado
                copia[it->first + 1][it->second] = 'n';
            }

            if(copia[it->first][it->second + 1] != 'n' && copia[it->first][it->second + 1] != '-' && it->second + 1 < larg)
            {
                // coloca na lista para ser visitado
                viz.push_back( std::make_pair(it->first, it->second + 1));
                // se for um visitante
                if(int (copia[it->first][it->second + 1]) >= 97 && int (copia[it->first][it->second + 1] <= 106 && it->second + 1 < larg))
                {
                    // coloca no vetor a distancia e o visitante
                    distancia.push_back( std::make_pair(dist, copia[it->first][it->second + 1] - 49));
                }
                // flag que o indice foi visitado
                copia[it->first][it->second + 1] = 'n';
            }

            if(copia[it->first][it->second - 1] != 'n' && copia[it->first][it->second - 1] != '-' && it->second - 1 >= 0)
            {
                // coloca na lista para ser visitado
                viz.push_back( std::make_pair(it->first, it->second - 1));
                // se for um visitante
                if(int (copia[it->first][it->second - 1]) >= 97 && int (copia[it->first][it->second - 1] <= 106 && it->second - 1 >= 0))
                {
                    // coloca no vetor a distancia e o visitante
                    distancia.push_back( std::make_pair(dist, copia[it->first][it->second - 1] - 49));
                }
                // flag que o indice foi visitado
                copia[it->first][it->second - 1] = 'n';
            }

            if(copia[it->first - 1][it->second] != 'n' && copia[it->first - 1][it->second] != '-' && it->first -1 >= 0)
            {
                // coloca na lista para ser visitado
                viz.push_back( std::make_pair(it->first - 1, it->second));
                // se for um visitante
                if(int (copia[it->first - 1][it->second]) >= 97 && int (copia[it->first - 1][it->second] <= 106 && it->first -1 >= 0))
                {
                    // coloca no vetor a distancia e o visitante
                    distancia.push_back( std::make_pair(dist, copia[it->first - 1][it->second] - 49));
                }
                // flag que o indice foi visitado
                copia[it->first - 1][it->second] = 'n';
            }
        }
        // atualiza a lista para visitar
        vis.erase(vis.begin(), vis.end());
        vis = viz;
        viz.erase(viz.begin(), viz.end());
    }
    vis.erase(vis.begin(), vis.end());
    vis = distancia;
    // ordena a lista com as distancias
    sort(vis.begin(), vis.end());
}

bool prefere(int atual, int proposta, std::vector<std::pair <int, int>>& bicPref)
// retorna se a bicicleta prefere o atual par do ue o par proposto
{
    // precorre a lista de preferrencias da bicicleta
    for(auto it = bicPref.begin(); it != bicPref.end(); it++)
    {
        // se preferir a atual retorna 0, se preferir a proposta retorna 1
        if(it->second - 48 == atual)
            return 0;
        if(it->second - 48 == proposta)
            return 1;
    }
    return 0;
}

int main(int argc, char ** argv)
// imprime na tela a melhor combinacao para todos visitantes
{
    // abre o arquivo de entrada
    FILE * entrada;
    entrada = fopen(argv[1],"r");

    // variaveis para iterar loops
    int i;
    int j;

    // variaveis para receber entrada
    int comp;
    int larg;
    int qtd;
    char mapa[MAX][MAX];

    // recebe  entrada
    fscanf(entrada, "%d", &qtd);
    fscanf(entrada, "%d %d", &comp, &larg);
    int bis [qtd][2];
    for(i = 0; i < comp; i++)
    {
        for(j = 0; j < larg; j++)
        {
            fscanf(entrada, " %c ", &mapa[i][j]);
            // se e uma bicicleta no mapa, guarda ele em bis
            if (int (mapa[i][j]) >= 48 && int (mapa[i][j] <= 57))
            {
                int a = int (mapa[i][j]);
                bis[a - 48][0] = i;
                bis[a - 48][1] = j;
            }
        }
    }


    int temp;
    // vetor de preferencia dos visitantes
    std::vector< std::pair <int,int> > pref[qtd];
    // recebe as preferencias dos visitantes e guarda em pref
    for(i = 0; i < qtd; i++)
    {
        for(j = 0; j < qtd; j++)
        {
            fscanf(entrada, "%d", &temp);
            pref[i].push_back( std::make_pair(temp, j));
        }
        // ordena as preferencias sem empates
        std::sort(pref[i].rbegin(), pref[i].rend());
    }

    // ordena os empates
    for(i = 0; i < qtd; i++)
    {
        ordenar(pref[i]);
    }

    // vetor de preferencia das bicicletas
    std::vector< std::pair <int,int> > bicPref[qtd];

    // BFS para descobrir as preferencias de toda bicicleta
    for(i = 0; i < qtd; i++)
    {
        bicPref[i].push_back(std::make_pair(bis[i][0], bis[i][1]));
        BFS(mapa, bicPref[i], qtd, larg, comp);
    }

    // vetor dos pareamentos
    std::vector< std::pair <int,int> > par;

    // vetor dos visitantes sem par
    std::vector <int> semPar;
    for(i = 0; i < qtd; i++)
    {
        semPar.push_back(i);
    }

    // enquanto houver visitante sem bicicleta
    while(semPar.size() != 0)
    {
        // pega o primero visitante sem par e sua bicicleta de preferencia
        int indice = semPar.at(0);
        int b = pref[indice].begin()->second;
        bool pareado = 0;
        int atual;
        bool preferencia = 0;
        // olha se essa bicicleta ja tem par
        for (auto it = par.begin(); it != par.end(); it++)
        {
            // se a biscicleta ja tiver par
            if(it->second == b)
            {
                pareado = 1;
                atual = it->first;
                // olha se a bicicleta prefere o seu par atual ou o par proposto
                preferencia = prefere(atual, indice, bicPref[b]);
                break;
            }
        }
        // se a bicicleta nao tiver par
        if(pareado == 0)
        {
            // atualiza o vetor de pares
            par.push_back(std::make_pair(indice, b));
            // apaga a bicicleta da preferencia dos visitantes
            pref[indice].erase(pref[indice].begin());
            // apaga o visitante do vetor de semPar
            semPar.erase(semPar.begin());
        }

        // se a bicicleta preferir o visitante proposto
        else if(preferencia == 1)
        {
            // procura o par com essa bicicleta
            for (auto it = par.begin(); it != par.end(); it++)
            {
                if(it->second == b)
                {
                    // troca o visitante da bicicleta
                    it->first = indice;
                    // apaga a bicicleta da preferencia dos visitantes
                    pref[indice].erase(pref[indice].begin());
                    // apaga o visitante escolhido do vetor de semPar
                    semPar.erase(semPar.begin());
                    // coloca o visitante rejeitado no vetor semPar
                    semPar.push_back(atual);
                    // ordena o vetor de semPar
                    std::sort(semPar.begin(), semPar.end());
                    break;
                }
            }
        }

        // a bicicleta rejeita o visitante
        else
        {
            // apaga a bicicleta da preferencia dos visitantes
            pref[indice].erase(pref[indice].begin());
        }
    }

    // ordena segundo o id dos vistantes
    std::sort(par.begin(), par.end());
    // imprime na tela os pares
    for(auto it = par.begin(); it < par.end(); it++)
    {
        // transforma o id do visitante de numero para letra e imprime;
        std::cout << char(it->first + 97) << " " << it->second << std::endl;
    }

    fclose(entrada);
    return 0;
}