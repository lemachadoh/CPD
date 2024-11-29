#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

#define DATA_DIR "players.csv"
#define SEARCH_DIR "consultas.csv"

struct Player {
    int id;
    string name;
    vector<string> position;
};

struct Result {
    int id;
    string name;
    int queries;
};

unsigned long long polynomialHash(int number, int mod, int base = 31) {
    unsigned long long hashValue = 0;
    unsigned long long basePower = 1;

    while (number > 0) {
        int digit = number % 10;
        hashValue = (hashValue + digit * basePower) % mod;
        basePower = (basePower * base) % mod;
        number /= 10;
    }

    return hashValue;
}

Player readPlayer(const string &line) {
    stringstream ss(line);
    string item;
    Player player;

    getline(ss, item, ',');
    player.id = stoi(item);

    getline(ss, item, ',');
    player.name = item;

    getline(ss, item, ',');
    stringstream posStream(item);
    string pos;
    while (getline(posStream, pos, ',')) {
        player.position.push_back(pos);
    }

    return player;
}

vector<Player> readPlayersFromCSV(const string &filename) {
    ifstream file(filename);
    vector<Player> players;
    string line;

    getline(file, line);

    while (getline(file, line)) {
        players.push_back(readPlayer(line));
    }

    return players;
}

vector<int> readSearchFromCSV(const string &filename) {
    ifstream file(filename);
    vector<int> searches;
    string line;

    while (getline(file, line)) {
        searches.push_back(stoi(line));
    }

    return searches;
}

void recordStatistics(
    const vector<double> &times, const vector<double> &occupancies,
    const vector<int> &maxLists, const vector<double> &avgLists,
    const string &filename
) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo " << filename << endl;
        return;
    }

    file << fixed << setprecision(3);
    for (const auto &t : times) file << t << ",";
    file.seekp(-1, ios_base::cur);
    file << "\n";

    for (const auto &o : occupancies) file << o << ",";
    file.seekp(-1, ios_base::cur);
    file << "\n";

    for (const auto &m : maxLists) file << m << ",";
    file.seekp(-1, ios_base::cur);
    file << "\n";

    for (const auto &a : avgLists) file << a << ",";
    file.seekp(-1, ios_base::cur);
    file << "\n";

    file.close();
}

void buildAndSearchHashTables(const vector<int>& M, const vector<Player>& players, const vector<int>& searches) {
    vector<double> buildTimes, occupancies, searchTimes;
    vector<int> maxLists;
    vector<double> avgLists;

    ofstream searchStats("estatisticas_consultas.txt");
    if (!searchStats.is_open()) {
        cerr << "Erro ao abrir arquivo estatisticas_consultas.txt." << endl;
        return;
    }

    // Linha de tempo total de consultas
    vector<double> allSearchTimes;

    // Tabela para armazenar resultados individuais de consultas
    vector<vector<int>> searchComparisons(searches.size(), vector<int>(M.size()));
    vector<string> searchNames(searches.size(), "NAO ENCONTRADO");

    for (size_t modIdx = 0; modIdx < M.size(); ++modIdx) {
        int mod = M[modIdx];
        vector<vector<Player>> hashTable(mod);
        int nonEmpty = 0, maxListSize = 0, totalEntries = 0;

        // Construção da tabela hash
        auto startBuild = chrono::high_resolution_clock::now();
        for (const auto& player : players) {
            int hashIndex = polynomialHash(player.id, mod);
            hashTable[hashIndex].push_back(player);
        }
        auto endBuild = chrono::high_resolution_clock::now();
        double buildTime = chrono::duration<double, milli>(endBuild - startBuild).count();

        for (const auto& list : hashTable) {
            if (!list.empty()) {
                nonEmpty++;
                maxListSize = max(maxListSize, (int)list.size());
                totalEntries += list.size();
            }
        }

        buildTimes.push_back(buildTime);
        occupancies.push_back((double)nonEmpty / mod);
        maxLists.push_back(maxListSize);
        avgLists.push_back(nonEmpty ? (double)totalEntries / nonEmpty : 0);

        // Consulta na tabela hash
        auto startSearch = chrono::high_resolution_clock::now();
       for (size_t searchIdx = 0; searchIdx < searches.size(); ++searchIdx) {
    int id = searches[searchIdx];             // ID da chave sendo buscada.
    int hashIndex = polynomialHash(id, mod);  // Calcula o índice na tabela hash.
    int comparisons = 0;                      // Contador de comparações.
    string name = "NAO ENCONTRADO";           // Nome padrão caso não seja encontrado.

    // Verifica a lista encadeada na posição calculada.
    if (!hashTable[hashIndex].empty()) {
        for (const auto& player : hashTable[hashIndex]) {
            comparisons++;                    // Incrementa o número de testes.
            if (player.id == id) {            // Se encontrar o jogador:
                name = player.name;           // Atualiza o nome com o nome encontrado.
                break;                        // Encerra a busca nessa lista.
            }
        }
    } else {
        comparisons++;                        // Conta como uma comparação mesmo se a lista estiver vazia.
    }

    // Atualiza resultados
    searchComparisons[searchIdx][modIdx] = comparisons;
    if (modIdx == 0) { // Salva o nome apenas na primeira iteração.
        searchNames[searchIdx] = name;
    }
}

        auto endSearch = chrono::high_resolution_clock::now();
        double searchTime = chrono::duration<double, milli>(endSearch - startSearch).count();
        allSearchTimes.push_back(searchTime);
    }

    // Gravar estatísticas gerais de consultas
    searchStats << "a3793,a6637,a9473,a12323,a15149\n";
    for (size_t i = 0; i < allSearchTimes.size(); ++i) {
        searchStats << allSearchTimes[i];
        if (i < allSearchTimes.size() - 1) searchStats << ",";
    }
    searchStats << "\n";

    // Gravar detalhes individuais de consultas
    for (size_t searchIdx = 0; searchIdx < searches.size(); ++searchIdx) {
        searchStats << searches[searchIdx] << "," << searchNames[searchIdx];
        for (size_t modIdx = 0; modIdx < M.size(); ++modIdx) {
            searchStats << "," << searchComparisons[searchIdx][modIdx];
        }
        searchStats << "\n";
    }

    searchStats.close();

    // Gerar arquivo de estatísticas de construção
    recordStatistics(buildTimes, occupancies, maxLists, avgLists, "estatisticas_construcao.txt");
}

int main() {
    vector<int> M = {3793, 6637, 9473, 12323, 15149};
    vector<Player> players = readPlayersFromCSV(DATA_DIR);
    vector<int> searches = readSearchFromCSV(SEARCH_DIR);

    buildAndSearchHashTables(M, players, searches);

    return 0;
}
