# Classificador Linear por Geometria Computacional

> Trabalho Prático — Algoritmos II  
> Autora: Raphaela Maria Costa e Silva — 2020006973

---

## Visão Geral

Implementação de um **classificador binário linear** construído inteiramente com técnicas de geometria computacional, sem o uso de bibliotecas de machine learning para o modelo em si. O algoritmo determina se dois conjuntos de pontos 2D são **linearmente separáveis** e, em caso positivo, produz uma **reta separadora ótima** entre eles.

### Pipeline

```
Dados 2D com duas classes
        │
        ▼
┌─────────────────────┐
│  Envoltória Convexa │  ← Gift Wrapping  O(n·m)
│  (uma por classe)   │
└────────┬────────────┘
         │
         ▼
┌─────────────────────┐
│  Detecção de        │  ← Sweep Line     O(n log n)
│  Interseção         │
└────────┬────────────┘
         │
    Sepáráveis? ──── Não ──▶  Classes não separáveis
         │
        Sim
         │
         ▼
┌─────────────────────┐
│  Par de Vértices    │  ← Força-bruta    O(n·m)
│  Mais Próximos      │
└────────┬────────────┘
         │
         ▼
┌─────────────────────┐
│  Reta Mediatriz     │  ← Geometria      O(1)
│  (modelo separador) │
└────────┬────────────┘
         │
         ▼
┌─────────────────────┐
│  Classificação      │  ← Substituição   O(n)
│  de Novos Pontos    │
└─────────────────────┘
```

---

## Estrutura do Repositório

```
.
├── AlgII_tp1_improved.ipynb   # Notebook principal (versão melhorada)
├── README.md
└── data/                      # Coloque aqui os CSVs do KEEL
    ├── iris.csv
    ├── newthyroid.csv
    ├── abalone.csv
    ├── shuttle.csv
    ├── page-blocks.csv
    ├── winequality-red.csv
    ├── winequality-white.csv
    ├── wine.csv
    ├── vowel.csv
    └── thyroid.csv
```

---

## Algoritmos Implementados

### 1. Envoltória Convexa — Gift Wrapping (Jarvis March)
Constrói iterativamente o polígono convexo mínimo que envolve um conjunto de pontos. A cada passo, seleciona o ponto mais anti-horário em relação ao atual até retornar ao ponto inicial.

- **Complexidade:** O(n·m), onde n = pontos totais e m = pontos na envoltória.

### 2. Detecção de Interseção — Sweep Line
Varre os segmentos da envoltória da esquerda para a direita usando uma AVL tree como estrutura de suporte. Verifica se algum segmento de uma envoltória cruza um segmento da outra.

- **Complexidade:** O(n log n).

### 3. Par de Vértices Mais Próximos
Força-bruta entre todos os pares de vértices das duas envoltórias. Retorna o par com menor distância euclidiana.

- **Complexidade:** O(n·m).

### 4. Modelo — Reta Mediatriz
Calcula a reta perpendicular ao segmento que une os dois vértices mais próximos, passando pelo seu ponto médio. Esta reta é o separador linear.

- **Complexidade:** O(1).

### 5. Classificador
Para cada ponto (xᵢ, yᵢ), avalia se yᵢ > m·xᵢ + b (acima) ou yᵢ < m·xᵢ + b (abaixo), atribuindo o rótulo correspondente.

- **Complexidade:** O(n).

---

## Datasets

Os testes foram realizados com datasets do repositório [KEEL](https://sci2s.ugr.es/keel/datasets.php):

| # | Dataset | Classes testadas |
|---|---------|-----------------|
| 1 | Iris | Setosa (0) vs. Versicolor + Virginica (1) |
| 2 | Newthyroid | Classe 2 (1) vs. Classe 3 (0) |
| 3 | Abalone | Anéis ≤ 3 (0) vs. Anéis ≥ 10 (1) |
| 4 | Shuttle | Classe 6 (0) vs. Classe 1 (1) |
| 5 | Page-Blocks | Classe 4 (0) vs. Classe 3 (1) |
| 6 | Wine Quality Red | Qualidade 4 (0) vs. 40 (1) |
| 7 | Wine Quality White | Qualidade 4 (0) vs. 60 (1) |
| 8 | Wine | Classe 3 (0) vs. Classe 1 (1) |
| 9 | Vowel | Classe 2 (0) vs. Classe 7 (1) |
| 10 | Thyroid | Classe 2 (0) vs. Classe 1 (1) |

> Os CSVs devem conter pelo menos as colunas `x`, `y` e `e` (rótulo).

---

## Como Executar

### Google Colab (recomendado)

1. Faça upload do notebook `AlgII_tp1_improved.ipynb` no Colab.
2. Faça upload dos arquivos CSV para `/content/`.
3. Execute as células em ordem.

### Localmente

```bash
# Crie um ambiente virtual
python -m venv .venv
source .venv/bin/activate   # Windows: .venv\Scripts\activate

# Instale as dependências
pip install numpy pandas matplotlib plotly scikit-learn bintrees

# Abra o notebook
jupyter notebook AlgII_tp1_improved.ipynb
```

---

## Dependências

| Pacote | Uso |
|--------|-----|
| `numpy` | Operações numéricas e geração de linspace |
| `pandas` | Manipulação dos datasets |
| `matplotlib` | Visualização de pontos, envoltórias e reta |
| `plotly` | Gráfico interativo de dispersão |
| `scikit-learn` | Métricas de avaliação (recall, precision, F1) |
| `bintrees` | AVL Tree para a varredura linear |

---

## Resultados e Conclusões

- Quando as envoltórias **não se intersectam**, as métricas são geralmente altas, confirmando que o algoritmo é eficaz para dados linearmente separáveis.
- Quando as envoltórias **se intersectam**, o algoritmo identifica corretamente a não-separabilidade e não produz um modelo.
- O desempenho do classificador depende diretamente da separabilidade intrínseca dos dados no plano 2D.

**Limitações:**
- Restrito a dados bidimensionais.
- O par mais próximo é calculado por força-bruta; a versão O(n log n) seria mais eficiente em conjuntos grandes.
- A avaliação usa uma única divisão treino/teste, sem validação cruzada.

---

## Licença

Uso acadêmico — Trabalho Prático de Algoritmos II.
