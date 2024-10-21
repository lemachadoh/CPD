# Quicksort - Projeto de Classificação e Pesquisa de Dados

Este projeto implementa quatro versões do algoritmo de Quicksort, utilizando duas estratégias de escolha de pivô e duas estratégias de particionamento, conforme solicitado na tarefa do curso de **Classificação e Pesquisa de Dados**.

## Integrantes
- Leandra Machado
- Ana Caroline de Jesus

## Estruturas de Particionamento e Pivô

As versões do algoritmo de Quicksort implementam as seguintes combinações:

1. **Métodos de Escolha de Pivô:**
   - Mediana de três elementos (primeiro, meio e último).
   - Pivô escolhido aleatoriamente.

2. **Métodos de Particionamento:**
   - Particionamento de Lomuto.
   - Particionamento de Hoare.

## Arquivos

- `main.cpp`: Arquivo principal contendo a implementação do Quicksort e a lógica de execução.
- `../utils.cpp`: Funções auxiliares para manipulação de dados.
- `entrada-quicksort.txt`: Arquivo de entrada contendo os vetores a serem ordenados.
- `resultados.txt`: Arquivo gerado com os resultados das execuções de Quicksort.

## Compilação e Execução

Para compilar e executar o programa, siga os seguintes passos:

1. Compile o código utilizando o comando:
   ```bash
   g++ main.cpp ../utils.cpp -o quickSort
   ```

2. Execute o programa:
   ```bash
   ./quickSort.exe
   ```
3. Ou utilize:
    ```bash
    g++ main.cpp ../utils.cpp -o quickSort && ./quickSort.exe
    ```

## Formato dos Arquivos

### Arquivo de Entrada (```entrada-quicksort.txt```)
Cada linha do arquivo contém um vetor para ser ordenado. O formato é o seguinte:
- O primeiro número da linha indica o tamanho do vetor.
- Os números subsequentes são os elementos do vetor.

Exemplo:
    ```
    16 16 14 12 1 8 4 9 6 15 13 11 2 7 3 10 5
    ```
### Arquivo de Saída (``resultados.txt``)
Os resultados são gravados neste arquivo no seguinte formato:

```bash
    tamanho,escolha-pivo,particionamento,trocas,recursoes,tempo
    100,aleatorio,lomuto,999,999,9.999
    100,aleatorio,hoare,999,999,9.999
    100,mediana3,lomuto,999,999,9.999
    100,mediana3,hoare,999,999,9.999
```

## Resultados Gerados

Para cada combinação de escolha de pivô e particionamento, o código calcula e armazena:
- O número de trocas realizadas.
- O número de recursões.
- O tempo de execução (em milissegundos).

## Referências

- Este projeto foi baseado nas especificações fornecidas pela disciplina de Classificação e Pesquisa de Dados.