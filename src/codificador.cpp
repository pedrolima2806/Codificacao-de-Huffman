//Codificador de Huffman

#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>
#include "codificador.h"
#include <vector>
#include <string>

struct Node {
    char character;
    int frequency;
    std::string label;
    struct Node *left;
    struct Node *right;
    bool isLeaf;

    Node(char c, int f) {
        character = c;
        frequency = f;
        label = "C" + std::to_string(static_cast<unsigned char>(character));
        left = nullptr;
        right = nullptr;
        isLeaf = true;
    }

    Node (int f, Node *l, Node *r, int id) {
        frequency = f;
        character = '\0';
        label = "N" + std::to_string(id);
        left = l;
        right = r;
        isLeaf = false;
    }
};

void Bbsort (std::vector<std::pair<char,int>>& charFreqPairs);
void BbsortNodes (std::vector<Node*>& nodes);
void generateCodes(Node* root, std::string code, std::map<char, std::string>& huffmanCodes);
void preOrder(Node* root, std::ofstream& out);
void simOrder(Node* root, std::ofstream& out);
void writeNode(std::ofstream& file, Node* node);
void deleteTree(Node* root);


int codificador() {
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
        //std::cout << character; //imprime o texto de input.txt
    }

    std::vector<std::pair<char,int>> charFreqPairs;
    //adiciona ao vetor as unidades cfPair retirados de map
    for (auto const &cfPair : characterFrequency) {
        charFreqPairs.emplace_back(cfPair);
    }
    archive.close();

    Bbsort(charFreqPairs);

    //-------------------------
    //Monta a árvore de Huffman
    //-------------------------
    std::vector<Node*> nodes;
    for (const auto& par : charFreqPairs) {
        Node* newNode = new Node(par.first, par.second);
        nodes.push_back(newNode);
    }

    //input vazio:
    if (nodes.empty()) {
        std::ofstream outTree("../texts/arvhuf.txt");
        std::ofstream encoded("../texts/texto.hfm");

        if (!outTree.is_open() || !encoded.is_open()) {
            throw std::runtime_error("Error creating output files.");
        }

        outTree << "PRE\n";
        outTree << "IN\n";

        return 0;
    }

    int internalId = 1;
    while (nodes.size() > 1) {
        BbsortNodes(nodes);

        Node* left = nodes[0];
        Node* right = nodes[1];

        int sumFrequency = left->frequency + right->frequency;

        Node* parent = new Node(sumFrequency, left, right, internalId);
        internalId++;

        nodes.erase(nodes.begin());
        nodes.erase(nodes.begin());

        nodes.push_back(parent);
    }
    Node* root = nodes[0];

    //---------------------------------
    //Gera os códigos de cada caractere
    //---------------------------------
    std::map<char, std::string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    //-----------------------------------
    //Gera o arquivo da árvore de Huffman
    //-----------------------------------
    std::ofstream out("../texts/arvhuf.txt");
    if (!out.is_open()) {
        throw std::runtime_error("Error opening output file.");
    }
    out << "PRE\n";
    preOrder(root, out);

    out << "IN\n";
    simOrder(root, out);

    out.close();

    //------------------------------------
    //Gera o arquivo em binário comprimido
    //------------------------------------
    std::ifstream inputAgain("../texts/input.txt");
    if (!inputAgain.is_open()) {
        throw std::runtime_error("Error opening input file again.");
    }

    std::ofstream encoded("../texts/texto.hfm");
    if (!encoded.is_open()) {
        throw std::runtime_error("Error opening texto.hfm.");
    }

    char c;
    while (inputAgain.get(c)) {
        encoded << huffmanCodes[c];
    }

    inputAgain.close();
    encoded.close();

    deleteTree(root);
    return 0;
}

void Bbsort(std::vector<std::pair<char, int> >& charFreqPairs) {
    const int quantPairs = static_cast<int>(charFreqPairs.size());

    for (int i = 0; i < quantPairs - 1; i++) {
        bool swaped = false;
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
}

void BbsortNodes (std::vector<Node*>& nodes) {
     const int quantNodes = static_cast<int>(nodes.size());

    for (int i = 0; i < quantNodes - 1; i++) {
        bool swaped = false;

        for (int j = 0; j < quantNodes - 1 - i; j++) {
            if (nodes[j]->frequency > nodes[j + 1]->frequency) {
                Node* temp = nodes[j];
                nodes[j] = nodes[j + 1];
                nodes[j + 1] = temp;
                swaped = true;
            }
        }

        if (!swaped) {
            break;
        }
    }
}


void generateCodes(Node* root, std::string code, std::map<char, std::string>& huffmanCodes) {
    if (root == nullptr) {
    return;
    }

    if (root->isLeaf) {
        if (code.empty()) {
            huffmanCodes[root->character] = "0";
        } else {
            huffmanCodes[root->character] = code;
        }
        return;
    }

    generateCodes(root->left, code + "0", huffmanCodes);
    generateCodes(root->right, code + "1", huffmanCodes);
}

void preOrder(Node* root, std::ofstream& out) {
    if (root==nullptr) {
        return;
    }
    writeNode(out, root);

    preOrder(root->left, out);
    preOrder(root->right, out);
}

void simOrder(Node* root, std::ofstream& out) {
    if (root == nullptr) {
        return;
    }
    simOrder(root->left, out);
    writeNode(out, root);
    simOrder(root->right, out);

}

void writeNode(std::ofstream& file, Node* node) {
    file << node->label << "\n";
}

void deleteTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);

    delete root;
}