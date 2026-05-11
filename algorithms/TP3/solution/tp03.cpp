#include <iostream>
#include <vector>
#include<stack>
#include <algorithm>
void maximo(int *linha, int *retangulos, int M)
{
    int topo;
    int area = 0;
    int i = 0;
    std::stack<int> result;
    // percorre a linha
    while (i < M)
    {
        // se o valor de linha for maior que a que ta no stack, coloca a linha no stack
        if (result.empty() || linha[result.top()] <= linha[i])
        {
            result.push(i++);
        }
        else
        {
            topo = linha[result.top()];
            result.pop();
            area = topo * i;

            // Para cada retangulo
            if (!result.empty())
            {
                // se o retangulo for menor que o topo da pilha
                // calcula a área do retângulo com o topo da pilha como comprimento mínimo
                area = topo * (i - result.top() - 1);

                // se o retangulo tem menor area que o calculado
                if(retangulos[topo] < i - result.top() - 1)
                {
                    // troca as dimensoes do retangulo
                    retangulos[topo] = i - result.top() - 1;
                }
            }
        }
    }
}
void ordena_empate(std::vector<std::pair<int,std::pair< int, int >>> &mesas)
// ordena segundo o criterio de desempate com bubble sort
{
    // para cada mesa i
    for(auto iter = mesas.begin(); iter != mesas.end(); iter++)
    {
        // olhar as mesas despois da mesa i
        for(auto it = iter; it != mesas.end(); it++)
        {
            // se houver empate
            if(iter->first == it->first )
            {
                // se estiver fora de ordem
                if(iter->second.second < it->second.second )
                {
                    // troca as mesas de posicao
                    int prim = it->first;
                    int sec = it->second.first;
                    int terc = it->second.second;
                    it->first = iter->first;
                    it->second.first = iter->second.first;
                    it->second.second = iter->second.second;
                    iter->first = prim;
                    iter->second.first = sec;
                    iter->second.second = terc;
                }
            }
            // se não houver empate passa para a proxima mesa i
            else
            break;
        }
    }
}
int main()
{
    // dimensao da planta
    int N, M;
    std::cin >> N >> M;
    int planta_01[N][M];
    char planta;

    // pega a entrada da planta e transforma em zeros e uns
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
        {
            std::cin >> planta;
            // se tiver espaco ocupado coloca 0
            if(planta == '#')
            {
                planta_01[i][j] = 0;
            }
            // se estiver espaco livre coloca 1
            if(planta == '.')
            {
                planta_01[i][j] = 1;
            }
        }
    }

    // quantidade de mesas, comprimento e largura
    int tam;
    int comp;
    int larg;

    // vetor para mesas <area, <comprimento, largura>>
    std::vector<std::pair<int,std::pair< int, int >>> mesas;
    std::cin >> tam;

    for(int i = 0; i < tam; i++)
    {
        std::cin >> comp >> larg;
        mesas.push_back(std::make_pair(comp * larg,std::make_pair(comp,larg)));
    }

    int linha[M] = {0};
    int retangulos[N+1] = {0};

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
        {
            // soma uma linha da planta com a proxima e coloca em um vetor
            if(planta_01[i][j] == 0)
            linha[j] = planta_01[i][j];
            else
            linha[j] ++;
        }
        // acha as maiores dimensoes e coloca em retangulos
        maximo(linha, retangulos, M);
    }

    // ordena as mesas em relacao a area
    sort(mesas.rbegin(), mesas.rend());
    // ordena para desempatar
    ordena_empate(mesas);

    // para sair do loop
    bool escrito = 0;

    // para cada mesa olha se ela cabe em um retangulo
    for(auto it = mesas.begin(); it != mesas.end(); it ++)
    {
        for(int j = 0; j < N; j++)
        {
            // se a mesa coube no retangulo escreve a resposta e sai do loop
            if(it->second.first <= retangulos[j] && it->second.second <= j)
            {
                std::cout << it->second.first << " " << it->second.second <<std::endl;
                escrito = 1;
                break;
            }
            // rotaciona a mesa e se couber no retangulo escreve a resposta e sai do loop
            if(it->second.first <= j && it->second.second <= retangulos[j])
            {
                std::cout << it->second.first << " " << it->second.second <<std::endl;
                escrito = 1;
                break;
            }

        }
        if(escrito == 1)
        {
            break;
        }
    }
    return 0;
}

