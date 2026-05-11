#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

struct Cache
{
    std::vector<u_int16_t> index;
    std::vector<bool> valido;
    std::vector<u_int32_t> endereco;
    std::vector<u_int> lru;
};

int main(int argc, char **argv)
{
    Cache cache;

    int tam_cache = std::stoi(argv[1]);
    int tam_linha = std::stoi(argv[2]);
    int assoc = std::stoi(argv[3]);
    const int linhas = tam_cache / tam_linha;

    cache.index.resize(linhas);
    cache.valido.resize(linhas);
    cache.endereco.resize(linhas);
    cache.lru.resize(linhas);

    int offset = std::log2(tam_linha);

    int index = 0;

    for (int i = 0; i < linhas; i++)
    {
        cache.index[i] = i;
    }

    FILE * arquivo;
    arquivo = fopen(argv[4],"r");

    u_int32_t block_addr;
    u_int32_t n_assoc = linhas / assoc;
    int bits_id = std::ceil(std::log2f(n_assoc));

    u_int timer = 0;
    u_int32_t endereco;
    while (!feof(arquivo))
    {
        fscanf(arquivo, "%X", &endereco);
        timer++;

        int end_bloco = (endereco >> offset);

        int index = end_bloco % n_assoc;
        index = index * assoc;

        int tag = (end_bloco >> bits_id);
        bool hit = false;
        bool insere = false;
        for (int i = 0; i < assoc; i++)
        {

            if (cache.valido[index + i] == 1 && cache.endereco[index + i] == tag)
            {
                cache.lru[index + i] = timer;
                hit = true;
                break;
            }
        }

        if (!hit)
        {
            for (int i = 0; i < assoc; i++)
            {

                if (cache.valido[index + i] == 0)
                {
                    cache.endereco[index + i] = tag;
                    cache.lru[index + i] = timer;
                    cache.valido[index + i] = true;
                    insere = true;
                    break;
                }
            }
            if (insere == false)
            {

                std::vector<std::pair<u_int, int>> LRU;

                for (int i = 0; i < assoc; i++)
                {
                    LRU.push_back(std::pair<u_int, int>(
                        cache.lru[index + i],
                        i));
                }

                std::pair<u_int, int> last_block = *(std::min_element(LRU.begin(), LRU.end()));

                cache.endereco[index + last_block.second] = tag;
                cache.lru[index + last_block.second] = timer;
            }
        }

        std::cout << "================" << std::endl;
        std::cout << "IDX V ** ADDR **" << std::endl;
        for (int i = 0; i < cache.endereco.size(); i++)
        {
            std::cout << std::dec << std::setfill('0') << std::setw(3) << cache.index[i] << " "<< cache.valido[i];
            if (cache.valido[i])
            {
                std::cout << " 0x" << std::hex << std::setfill('0') << std::setw(8) << std::uppercase << cache.endereco[i] << std::endl;
            }
            else
                std::cout << std::endl;
        }
    }
    return 0;
}
