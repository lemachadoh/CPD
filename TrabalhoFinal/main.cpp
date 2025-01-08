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
//      2.1. Prefixos de nomes de jogadores - jogador <prefix> -- short nome
//      2.2. Jogadores revisados por usuarios - usuario <usuarioID>
//      2.3. Top jogadores de determinada posicao - top <N> <posicao>
//      2.4. Jogadores contendo x tags - tags <lista of tags>
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
    tabelaHash<Jogador>   jogadores(M);
    tabelaHash<Usuario>     usuarios(N);

    // Tries
    Trie jogadorNames;
    Trie jogadorTags;

    // Pointers
    Jogador* jogadorPtr = nullptr;
    Usuario* usuarioPtr = nullptr;

    // Instances
    Jogador  oJogador;
    Usuario    oUsuario;

    // Input
    string input, query_type, query_args;
    bool quit = false;

    // Vectors
    vector<int> jogador_id_lista;
    vector<Jogador> jogador_lista;
    vector<Rating> rating_lista;
    vector<string> tag_lista;

    auto start = chrono::high_resolution_clock::now();

    buildHash(jogadores, usuarios, PLAYERS_DIR, RATING_DIR);
    buildJogadorTrie(jogadores, jogadorNames);
    buildTagsTrie(TAGS_DIR, jogadorTags);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    cout << "Processo finalizado em "<< duration.count() << " segundos.\n" << endl;

    cout << "Menu: \n1. <prefixo>\tPrefixos de nomes de jogadores\n2. <usuario>\tJogadores revisados por usuarios\n3. <top>\tTop jogadores de determinada posicao\n4. <tags>\tJogadores contendo x tags\n5. Sair\n"<< endl;

    // Menu
    while(!quit){
        cout << "Digite a pesquisa desejada: ";
        getline(cin, input);
        istringstream iss(input);
        iss >> query_type;
        query_type = toLowerCase(query_type);

        // Pesq 1: Jogador <sofifa_id>
        if(query_type == "prefixo"){
            iss >> query_args;
            query_args = toLowerCase(query_args);

            jogadorNames.startsWith(query_args, jogador_id_lista);
            for(auto j : jogador_id_lista){
                pesquisaHash(jogadores, j, jogadorPtr);
                oJogador = *jogadorPtr;
                jogador_lista.push_back(oJogador);
            }

            // Sorts by global rating
            mergeSort(jogador_lista, 0, (jogador_lista.size()-1));
            
            cout << endl;
            for(auto k : jogador_lista){    
                cout    << setw(ID_FIELD_WIDTH)     << k.id << " " 
                        << setw(SHORT_FIELD_WIDTH)  << k.short_name << " " 
                        << setw(LONG_FIELD_WIDTH)   << k.long_name << " " 
                        << setw(POS_FIELD_WIDTH)    << k.jogador_posicoes << " "
                        << setw(RATING_FIELD_WIDTH) << fixed << setprecision(6) << k.rating
                        << setw(COUNT_FIELD_WIDTH)  << k.total_ratings 
                        << endl;
            }
        }

        // Pesq 2: Usuarios <usuario_id>
        else if(query_type == "usuario"){
            iss >> query_args;
            query_args = toLowerCase(query_args);
            int key = stoi(query_args);

            pesquisaHash(usuarios, key, usuarioPtr);
            oUsuario = *usuarioPtr;
            rating_lista = oUsuario.usuario_ratings;

            for(const auto r : rating_lista){
                pesquisaHash(jogadores, r.id, jogadorPtr);
                oJogador = *jogadorPtr;
                jogador_lista.push_back(oJogador);
            }

            // Ordenação secundária: global rating
            mergeSort(jogador_lista, 0, (jogador_lista.size()-1));

            // Atualiza rating_lista de acordo com os ids de jogador_lista.
            updateVectorById(jogador_lista, rating_lista);

            // Ordenação primária: usuario rating
            mergeSort(rating_lista, 0, (rating_lista.size()-1));

            // Atualiza jogador_lista de acordo com os ids de rating_lista.
            updateVectorById(rating_lista, jogador_lista);

            cout << endl;
            for(int i = 0; i < 20; i++){
                cout    << setw(ID_FIELD_WIDTH)     << rating_lista[i].id << " " 
                        << setw(SHORT_FIELD_WIDTH)  << jogador_lista[i].short_name << " "
                        << setw(LONG_FIELD_WIDTH)   << jogador_lista[i].long_name << " "
                        << setw(RATING_FIELD_WIDTH) << fixed << setprecision(6) << jogador_lista[i].rating << " "
                        << setw(COUNT_FIELD_WIDTH)  << jogador_lista[i].total_ratings << " "
                        << setw(RATING_FIELD_WIDTH) << fixed << setprecision(1) << rating_lista[i].rating << " " 
                        << endl;
            }
        }

        // Pesq 3: Top <N> <posicao>
        else if(query_type == "top"){
            int N;
            string posicao;

            // Atribui o segundo e terceiro membros da stream à var N e posição, respectivamente.
            iss >> N >> posicao;

            // Como as posições estão armazenadas em letras maiusculas, 
            // normaliza o input do usuário para letras maiusculas.
            posicao = toUpperCase(posicao);

            for(int i = 0; i < jogadores.tabela.size(); i++){
                for(const auto jogador : jogadores.tabela[i]){
                    // Corre pelo hash de jogadores, adicionando ao vector apenas aqueles que
                    // contêm a substring da posição desejada e que possuem 1000 ou mais avaliações.
                    if(jogador.total_ratings >= 1000 && (containsSubstring(jogador.jogador_posicoes, posicao))){
                        oJogador = jogador;
                        jogador_lista.push_back(oJogador);
                    }
                }
            }

            mergeSort(jogador_lista, 0, (jogador_lista.size()-1));
            
            cout << endl;
            for(int i = 0; i < N; i++){
                cout    << setw(ID_FIELD_WIDTH) << jogador_lista[i].id << " "
                        << setw(SHORT_FIELD_WIDTH) << jogador_lista[i].short_name << " "
                        << setw(LONG_FIELD_WIDTH) << jogador_lista[i].long_name << " "
                        << setw(POS_FIELD_WIDTH) << jogador_lista[i].jogador_posicoes << " "
                        << setw(NATION_FIELD_WIDTH) << jogador_lista[i].nacionalidade << " "
                        << setw(CLUB_FIELD_WIDTH) << jogador_lista[i].clube_name << " "
                        << setw(LEAGUE_FIELD_WIDTH) << jogador_lista[i].ligas << " "
                        << setw(RATING_FIELD_WIDTH) << fixed << setprecision(6) << jogador_lista[i].rating << " "
                        << setw(COUNT_FIELD_WIDTH) << jogador_lista[i].total_ratings << " "
                        << endl;
            }
        }

        // Pesq 4: tags <lista of tags>
        else if(query_type ==  "tags"){
            // Realiza o parsing do restante da linha escrita pelo usuário.
            // tags são retornadas no vector tag_lista.
            tag_lista = parseTags(iss);
            vector<vector<int>> tag_results;

            for(int i = 0; i < tag_lista.size(); i++){
                // Busca tag na trie, adiciona os ids (int) relacionados à tag 
                // ao vector jogador_id_lista e posteriormente adiciona ao vector tag_results.
                jogadorTags.search(tag_lista[i], jogador_id_lista);
                tag_results.push_back(jogador_id_lista);
                jogador_id_lista.clear();
            }
            
            // Cria o vetor intersection e realiza a interseção de todos os vectors em tag_results.
            vector<int> intersection = intersectMultipleVectors(tag_results);

            // Para cada id na interseção, busca o jogador correspondente no Hash 'jogadores'.
            for(const auto i : intersection){
                pesquisaHash(jogadores, i, jogadorPtr);
                oJogador = *jogadorPtr;
                jogador_lista.push_back(oJogador);
            }

            // Ordena a jogador_lista com base na nota global.
            mergeSort(jogador_lista, 0, (jogador_lista.size()-1));

            cout << endl;
            for(int i = 0; i < jogador_lista.size(); i++){
                cout    << setw(ID_FIELD_WIDTH) << jogador_lista[i].id << " "
                        << setw(SHORT_FIELD_WIDTH) << jogador_lista[i].short_name << " "
                        << setw(LONG_FIELD_WIDTH) << jogador_lista[i].long_name << " "
                        << setw(POS_FIELD_WIDTH) << jogador_lista[i].jogador_posicoes << " "
                        << setw(NATION_FIELD_WIDTH) << jogador_lista[i].nacionalidade << " "
                        << setw(CLUB_FIELD_WIDTH) << jogador_lista[i].clube_name << " "
                        << setw(LEAGUE_FIELD_WIDTH) << jogador_lista[i].ligas << " "
                        << setw(RATING_FIELD_WIDTH) << fixed << setprecision(6) << jogador_lista[i].rating << " "
                        << setw(COUNT_FIELD_WIDTH) << jogador_lista[i].total_ratings << " "
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
        jogador_id_lista.clear();
        jogador_lista.clear();
        rating_lista.clear();
        tag_lista.clear();
    }

    return 0;
}