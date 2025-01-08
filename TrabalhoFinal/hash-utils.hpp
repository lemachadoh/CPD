#ifndef HASH_UTILS_HPP
#define HASH_UTILS_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <typename T>
struct tabelaHash{
    vector<vector<T>> tabela;

    tabelaHash(size_t tableSize){
        tabela.resize(tableSize);
    }
};

// Retorna o indice para a inserção do item na tabela hash.
unsigned long long polynomialHash(int numero, int mod, int base = 31){
    unsigned long long indiceHash = 0;
    unsigned long long basePower = 1;

    while (numero > 0) {
        int digit = numero % 10;
        indiceHash = (indiceHash + digit * basePower) % mod;
        basePower = (basePower * base) % mod;
        numero /= 10;
    }

    return indiceHash;
}

// Insere o item na tabela.
template <typename T>
void insercaoHash(tabelaHash<T> &tabelaHash, T oItem){
    int indiceHash = polynomialHash(oItem.id, tabelaHash.tabela.size());
    tabelaHash.tabela[indiceHash].push_back(oItem);
}

// Função de busca. Receba uma tabelaHash, uma chave (int) e um ponteiro para o item.
// O item é retornado por referência.
template <typename T>
void pesquisaHash(tabelaHash<T> &tabelaHash, int key, T* &oItem){
    bool achou = false;

    int indiceHash = polynomialHash(key, tabelaHash.tabela.size());

    if(!tabelaHash.tabela[indiceHash].empty()){
        for(auto &item : tabelaHash.tabela[indiceHash]){
            if(item.id == key){
                oItem = &item;      // pointer to target
                achou = true;
                break;
            }
        }
        if(!achou) oItem = nullptr; // Retorna nullptr quando não encontrado.
    }
    else oItem = nullptr;          // Retorna null pointer quando vazio.
}

// Função de avaliação dos stats da hash tabela. Apenas para debug. 
template <typename T>
void hashStats(tabelaHash<T> tabelaHash, bool verbose = false){
    int vazio = 0;
    int maxListSize = 0;
    int entries = 0;
    int avgListSize = 0;

    for(int i = 0; i < tabelaHash.tabela.size(); i++){

        if(verbose != false){
            cout << i << "\t";
        }

        if(!tabelaHash.tabela[i].empty()){

            if(tabelaHash.tabela[i].size() > maxListSize){
                maxListSize = tabelaHash.tabela[i].size();
            }

            for(const auto &item : tabelaHash.tabela[i]){
                entries++;
                if(verbose != false){
                    cout << item.id << "\t";
                }
            }
        }else vazio++;

        if(verbose != false) cout << endl;
    }

    avgListSize = entries/(tabelaHash.tabela.size() - vazio);
    
    cout << "Total Entries: \t" << entries << endl;
    cout << "Max List Size: \t" << maxListSize << endl;
    cout << "Avg List Size: \t" << avgListSize << endl;
    cout << "Empty Indexes: \t" << vazio << endl;
    cout << "------------------------" << endl;
}

#endif // HASH_UTILS_HPP
