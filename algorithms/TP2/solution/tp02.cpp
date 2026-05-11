#include<iostream>
#include <queue>
#include <climits>

int dijkstra_modificado(std::vector<std::vector<std::pair<int, int> > >& mapa,std::pair<int, int> consulta)
// retorna o maior peso que pode ser levado da cidade inicio para destino
{
    // Vetor para o maior peso suportado
    std::vector<int> maior_peso(mapa.size(), INT_MIN);

    // Priority queue para saber qual o maior peso suportado em todo o caminho
    std::priority_queue<std::pair<int, int> >min_atual;

    // Inicializa com o vertice de inicio e o maior peso possível, que é 0
    min_atual.push(std::make_pair(0, consulta.first));

    maior_peso[consulta.first] = INT_MAX;

    while (min_atual.empty() == false)
    {
        int atual = min_atual.top().second;
        min_atual.pop();

        for (auto it = mapa[atual].begin(); it != mapa[atual].end(); it++)
        {
           // acha qual o maior peso suportado ate o vertice atual
            int menor = 0;
            int peso = 0;

            if(maior_peso[atual] > it->first)
                menor = it->first;
            else
                menor = maior_peso[atual];

            if(maior_peso[it->second] > menor )
                peso = maior_peso[it->second];
            else
                peso = menor;

            if (peso > maior_peso[it->second])
            // relaxa a aresta
            {
                // atualiza o maior peso possivel
                maior_peso[it->second] = peso;

                // adiciona a aresta na priority queue
                min_atual.push(std::make_pair(peso, it->second));
            }
        }
    }
    return maior_peso[consulta.second];
}

int main()
{
    //numero de cidades, numero de rodovias, numero de consultas
    int N, M, Q;
    std::cin >> N >> M >> Q;

    // grafo do mapa
    std::vector<std::vector<std::pair<int, int> > > mapa;
    mapa.assign(N + 1, std::vector<std::pair<int, int> >());

    // cidade inicio, cidade destino, peso suportado
    int u, v, w;

    for(int i = 0; i < M; i++)
    // popula o grafo
    // os vertices apontam de u para v
    {
        std::cin >> u >> v >> w;
        mapa[u].push_back(std::make_pair(w, v));
    }

    // cidade de inicio, cidade de destino
    int u_inicio;
    int v_destino;



    for(int i = 0; i < Q; i++)
    {
        // pega a consulta da entrada
        std::cin >> u_inicio >> v_destino;
        std::pair<int, int> consulta;
        consulta.first = u_inicio;
        consulta.second = v_destino;
        // faz o dikstra modificado, acha o maior peso e imprime na tela
        int saida = dijkstra_modificado(mapa, consulta);
        std::cout << saida << std::endl;
    }
    return 0;
}