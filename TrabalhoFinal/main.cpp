// Trabalho final Classificação e Pesquisa de Dados
//
// Integrantes:
//      Ana Caroline Rodrigues
//      Leandra Rodrigues Machado
//
// Roteiro do trabalho
// 1. Estruturas de dados de pesquisa:
//      1.1. Hash para armazenar dados de jogadores 
//      1.2. Estrutura para busca de strings de nomes 
//      1.3. Estrutura para guardar reviews de usuários 
//      1.4. Estrutura para guardar tags 
// 2. Pesquisas
//      2.1. Prefixos de nomes de jogadores - player <prefix> -- short name
//      2.2. Jogadores revisados por usuarios - user <userID>
//      2.3. Top jogadores de determinada posicao - top <N> <position>
//      2.4. Jogadores contendo x tags - tags <list of tags>
//

#include "utils.hpp"
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <iomanip>

using namespace std;

int main(){
    int M = 37879;
    int N = 276989; 

    // Hash tables
    HashTable<Player>   players(M);
    HashTable<User>     users(N);

    // Tries
    Trie playerNames;
    Trie playerTags;

    // Pointers
    Player* playerPtr = nullptr;
    User* userPtr = nullptr;

    // Instances
    Player  oPlayer;
    User    oUser;

    // Input
    string input, query_type, query_args;
    bool quit = false;

    // Vectors
    vector<int> player_id_list;
    vector<Player> player_list;
    vector<Rating> rating_list;
    vector<string> tag_list;

    auto start = chrono::high_resolution_clock::now();

    buildHash(players, users, PLAYERS_DIR, RATING_DIR);
    buildPlayerTrie(players, playerNames);
    buildTagsTrie(TAGS_DIR, playerTags);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    cout << "Processo finalizado em "<< duration.count() << " segundos.\n" << endl;

    cout << "Menu: \n1. <prefixo>\tPrefixos de nomes de jogadores\n2. <user>\tJogadores revisados por usuarios\n3. <top>\tTop jogadores de determinada posicao\n4. <tags>\tJogadores contendo x tags\n5. Sair\n"<< endl;

    // Menu
    while(!quit){
        cout << "Digite a pesquisa desejada: ";
        getline(cin, input);
        istringstream iss(input);
        iss >> query_type;
        query_type = toLowerCase(query_type);

        // Pesq 1: Player <sofifa_id>
        if(query_type == "prefixo"){
            iss >> query_args;
            query_args = toLowerCase(query_args);

            playerNames.startsWith(query_args, player_id_list);
            for(auto j : player_id_list){
                hashSearch(players, j, playerPtr);
                oPlayer = *playerPtr;
                player_list.push_back(oPlayer);
            }

            // Sorts by global rating
            mergeSort(player_list, 0, (player_list.size()-1));
            
            cout << endl;
            for(auto k : player_list){    
                cout    << setw(ID_FIELD_WIDTH)     << k.id << " " 
                        << setw(SHORT_FIELD_WIDTH)  << k.short_name << " " 
                        << setw(LONG_FIELD_WIDTH)   << k.long_name << " " 
                        << setw(POS_FIELD_WIDTH)    << k.player_positions << " "
                        << setw(RATING_FIELD_WIDTH) << fixed << setprecision(6) << k.rating
                        << setw(COUNT_FIELD_WIDTH)  << k.total_ratings 
                        << endl;
            }
        }

        // Pesq 2: User <user_id>
        else if(query_type == "user"){
            iss >> query_args;
            query_args = toLowerCase(query_args);
            int key = stoi(query_args);

            hashSearch(users, key, userPtr);
            oUser = *userPtr;
            rating_list = oUser.user_ratings;

            for(const auto r : rating_list){
                hashSearch(players, r.id, playerPtr);
                oPlayer = *playerPtr;
                player_list.push_back(oPlayer);
            }

            // Ordenação secundária: global rating
            mergeSort(player_list, 0, (player_list.size()-1));

            // Atualiza rating_list de acordo com os ids de player_list.
            updateVectorById(player_list, rating_list);

            // Ordenação primária: user rating
            mergeSort(rating_list, 0, (rating_list.size()-1));

            // Atualiza player_list de acordo com os ids de rating_list.
            updateVectorById(rating_list, player_list);

            cout << endl;
            for(int i = 0; i < 20; i++){
                cout    << setw(ID_FIELD_WIDTH)     << rating_list[i].id << " " 
                        << setw(SHORT_FIELD_WIDTH)  << player_list[i].short_name << " "
                        << setw(LONG_FIELD_WIDTH)   << player_list[i].long_name << " "
                        << setw(RATING_FIELD_WIDTH) << fixed << setprecision(6) << player_list[i].rating << " "
                        << setw(COUNT_FIELD_WIDTH)  << player_list[i].total_ratings << " "
                        << setw(RATING_FIELD_WIDTH) << fixed << setprecision(1) << rating_list[i].rating << " " 
                        << endl;
            }
        }

        // Pesq 3: Top <N> <position>
        else if(query_type == "top"){
            int N;
            string position;

            // Atribui o segundo e terceiro membros da stream à var N e posição, respectivamente.
            iss >> N >> position;

            // Como as posições estão armazenadas em letras maiusculas, 
            // normaliza o input do usuário para letras maiusculas.
            position = toUpperCase(position);

            for(int i = 0; i < players.table.size(); i++){
                for(const auto player : players.table[i]){
                    // Corre pelo hash de jogadores, adicionando ao vector apenas aqueles que
                    // contêm a substring da posição desejada e que possuem 1000 ou mais avaliações.
                    if(player.total_ratings >= 1000 && (containsSubstring(player.player_positions, position))){
                        oPlayer = player;
                        player_list.push_back(oPlayer);
                    }
                }
            }

            mergeSort(player_list, 0, (player_list.size()-1));
            
            cout << endl;
            for(int i = 0; i < N; i++){
                cout    << setw(ID_FIELD_WIDTH) << player_list[i].id << " "
                        << setw(SHORT_FIELD_WIDTH) << player_list[i].short_name << " "
                        << setw(LONG_FIELD_WIDTH) << player_list[i].long_name << " "
                        << setw(POS_FIELD_WIDTH) << player_list[i].player_positions << " "
                        << setw(NATION_FIELD_WIDTH) << player_list[i].nationality << " "
                        << setw(CLUB_FIELD_WIDTH) << player_list[i].club_name << " "
                        << setw(LEAGUE_FIELD_WIDTH) << player_list[i].league_name << " "
                        << setw(RATING_FIELD_WIDTH) << fixed << setprecision(6) << player_list[i].rating << " "
                        << setw(COUNT_FIELD_WIDTH) << player_list[i].total_ratings << " "
                        << endl;
            }
        }

        // Pesq 4: tags <list of tags>
        else if(query_type ==  "tags"){
            // Realiza o parsing do restante da linha escrita pelo usuário.
            // tags são retornadas no vector tag_list.
            tag_list = parseTags(iss);
            vector<vector<int>> tag_results;

            for(int i = 0; i < tag_list.size(); i++){
                // Busca tag na trie, adiciona os ids (int) relacionados à tag 
                // ao vector player_id_list e posteriormente adiciona ao vector tag_results.
                playerTags.search(tag_list[i], player_id_list);
                tag_results.push_back(player_id_list);
                player_id_list.clear();
            }
            
            // Cria o vetor intersection e realiza a interseção de todos os vectors em tag_results.
            vector<int> intersection = intersectMultipleVectors(tag_results);

            // Para cada id na interseção, busca o player correspondente no Hash 'players'.
            for(const auto i : intersection){
                hashSearch(players, i, playerPtr);
                oPlayer = *playerPtr;
                player_list.push_back(oPlayer);
            }

            // Ordena a player_list com base na nota global.
            mergeSort(player_list, 0, (player_list.size()-1));

            cout << endl;
            for(int i = 0; i < player_list.size(); i++){
                cout    << setw(ID_FIELD_WIDTH) << player_list[i].id << " "
                        << setw(SHORT_FIELD_WIDTH) << player_list[i].short_name << " "
                        << setw(LONG_FIELD_WIDTH) << player_list[i].long_name << " "
                        << setw(POS_FIELD_WIDTH) << player_list[i].player_positions << " "
                        << setw(NATION_FIELD_WIDTH) << player_list[i].nationality << " "
                        << setw(CLUB_FIELD_WIDTH) << player_list[i].club_name << " "
                        << setw(LEAGUE_FIELD_WIDTH) << player_list[i].league_name << " "
                        << setw(RATING_FIELD_WIDTH) << fixed << setprecision(6) << player_list[i].rating << " "
                        << setw(COUNT_FIELD_WIDTH) << player_list[i].total_ratings << " "
                        << endl;
            }
        }

        // Sair
        else if(query_type == "5" || query_type == "sair") quit = true;

        // Default: comando desconhecido
        else{
            cout << "Comando desconhecido.";
        }
        cout << endl;

        // Clear buffers
        player_id_list.clear();
        player_list.clear();
        rating_list.clear();
        tag_list.clear();
    }

    return 0;
}