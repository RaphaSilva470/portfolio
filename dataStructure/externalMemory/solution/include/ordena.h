#ifndef  ordena_h
#define ordena_h
#define URL_MAXTAM 300

// popularidade do url
typedef int Chave;

// guarda os elementos a serem ordenados segundo a chave e as funcoes de ordenamento
class TipoOrdena 
{
    public:
        char url[URL_MAXTAM];
        int arquivo; 
        int id;
        TipoOrdena();
        void SetChave(Chave c);
        Chave GetChave();

    private:
        Chave chaveOrdena;
};

TipoOrdena::TipoOrdena()
// Descricao: inicializa TipoOrdena com items vazios
// Entrada: nenhuma
// Saida: TipoOrdena
{
    chaveOrdena = -1; 
    arquivo = -1;
}

void TipoOrdena::SetChave(Chave c)
// Descricao: define a chave
// Entrada: c
// Saida: nenhuma  
{
    chaveOrdena = c;
}

Chave TipoOrdena::GetChave()
// Descricao: retorna a chave
// Entrada: nenhuma
// Saida: Chave
{
    return chaveOrdena;
}
#endif
