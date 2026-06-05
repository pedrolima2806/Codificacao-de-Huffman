//Codificador de Huffman

#include <iostream>
#include <fstream>
#include <map>
#include "codificador.h"

#include <vector>

struct node {
    char character;
    int frequency;
    struct node *left;
    struct node *right;
};


int main() {
    //------------------------------------------------------------------------------
    //Montagem do charFreqPair que é o vetor de pares de caracteres e sua frequência
    //------------------------------------------------------------------------------
    std::ifstream archive("../texts/input.txt");

    if (!archive.is_open()) {
        throw std::runtime_error("Error opening input file");
    }

    char character;
    std::map<char,int> characterFrequency;

    while (archive.get(character)) {
        characterFrequency[character]++;
        //std::cout << character;
    }

    std::vector<std::pair<char,int>> charFreqPairs;

    //adiciona ao vetor as unidades cfPair retirados de map
    for (auto const &cfPair : characterFrequency) {
        charFreqPairs.emplace_back(cfPair);
    }
    archive.close();

    //---------------------------------------------------------
    //BubbleSort para o vetor de pares ordenando por frequência
    //---------------------------------------------------------
    int quantPairs = static_cast<int>(charFreqPairs.size());

    for (int i = 0; i < quantPairs - 1; i++) {
        bool swaped;
        swaped = false;
        for (int j = 0; j < quantPairs - 1 - i; j++) {
            if (charFreqPairs[j].second > charFreqPairs[j+1].second) {
                auto temp = charFreqPairs[j];
                charFreqPairs[j] = charFreqPairs[j+1];
                charFreqPairs[j+1] = temp;
                swaped = true;
            }
        }
        if (!swaped) {
            break;
        }
    }
    //imprime os pares (para conferência)
    for (const auto& par : charFreqPairs) {
        std::cout << par.first << " " << par.second << std::endl;
    }


    //-------------------------
    //Monta a árvore de Huffman
    //-------------------------
    struct node *root = nullptr;





    return 0;
}