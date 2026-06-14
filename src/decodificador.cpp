//Decodificador de Huffman

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "decodificador.h"

struct Node {
    char character;
    std::string label;
    Node *left;
    Node *right;
    bool isLeaf;

    Node(std::string lbl) {
        label = lbl;
        left = nullptr;
        right = nullptr;

        if (!label.empty() && label[0] == 'C') {
            int asciiCode = std::stoi(label.substr(1));
            character = static_cast<char>(asciiCode);
            isLeaf = true;
        } else {
            character = '\0';
            isLeaf = false;
        }
    }
};

void readTreeFile(const std::string& filename, std::vector<std::string>& preOrder, std::vector<std::string>& simOrder);
Node* rebuildTree(const std::vector<std::string>& preOrder, int& preIndex, int simStart, int simEnd, const std::unordered_map<std::string, int>& simPosition);
std::string readEncodedText(const std::string& filename);
void decodeText(Node* root, const std::string& encodedText, const std::string& outputFile);
void deleteDecodTree(Node* root);

void decodificador(const std::string& outputFile, const std::string& codedFile,const std::string& treeFile) {
    try {
        std::vector<std::string> preOrder;
        std::vector<std::string> simOrder;

        readTreeFile(treeFile, preOrder, simOrder);

        std::string encodedText = readEncodedText(codedFile);

        if (preOrder.empty() && simOrder.empty()) {
            std::ofstream output(outputFile);

            if (!output.is_open()) {
                throw std::runtime_error("Error creating decod.txt.");
            }

            return;
        }

        if (preOrder.size() != simOrder.size()) {
            throw std::runtime_error("Invalid tree file: PRE and IN have different sizes.");
        }

        std::unordered_map<std::string, int> simPosition;

        for (int i = 0; i < static_cast<int>(simOrder.size()); i++) {
            simPosition[simOrder[i]] = i;
        }

        int preIndex = 0;

        Node* root = rebuildTree(
            preOrder,
            preIndex,
            0,
            static_cast<int>(simOrder.size()) - 1,
            simPosition
        );

        decodeText(root, encodedText, outputFile);

        deleteDecodTree(root);

        std::cout << "Decodificacao concluida. Arquivo decod.txt gerado.\n";

    } catch (const std::exception& error) {
        std::cerr << "Erro: " << error.what() << '\n';
        return;
    }
}

void readTreeFile(const std::string& filename, std::vector<std::string>& preOrder, std::vector<std::string>& simOrder) {
    std::ifstream file(filename);

    std::string line;
    bool readingPre = false;
    bool readingIn = false;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        if (line == "PRE") {
            readingPre = true;
            readingIn = false;
            continue;
        }

        if (line == "IN") {
            readingPre = false;
            readingIn = true;
            continue;
        }
        if (readingPre) {
            preOrder.push_back(line);
        } else if (readingIn) {
            simOrder.push_back(line);
        }
    }

    file.close();
}

Node* rebuildTree(const std::vector<std::string>& preOrder, int& preIndex, int simStart, int simEnd, const std::unordered_map<std::string, int>& simPosition) {
    if (simStart > simEnd) {
        return nullptr;
    }
    if (preIndex >= static_cast<int>(preOrder.size())) {
        throw std::runtime_error("Invalid tree file: preorder ended unexpectedly");
    }

    std::string currentLabel = preOrder[preIndex];
    preIndex++;

    Node* root = new Node(currentLabel);
    auto it = simPosition.find(currentLabel);

    if (it == simPosition.end()) {
        throw std::runtime_error("Invalid tree file: label not found in order");
    }

    int rootPosition = it->second;

    if (rootPosition < simStart || rootPosition > simEnd) {
        throw std::runtime_error("Invalid tree file: inconsistent traversals.");
    }

    root->left = rebuildTree(preOrder, preIndex, simStart, rootPosition - 1, simPosition);
    root->right = rebuildTree(preOrder, preIndex, rootPosition + 1, simEnd, simPosition);

    if (root->left != nullptr || root->right != nullptr) {
        root->isLeaf = false;
    }

    return root;
}

std::string readEncodedText(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file");
    }
    std::string encodedText;
    char bit;

    while (file.get(bit)) {
        if (bit =='0' || bit =='1') {
            encodedText += bit;
        }
    }

    file.close();
    return encodedText;
}

void decodeText(Node* root, const std::string& encodedText, const std::string& outputFile) {
    std::ofstream output(outputFile);

    if (!output.is_open()) {
        throw std::runtime_error("Unable to open file");
    }

    if (root == nullptr) {
        return;
    }
    if (root->isLeaf) {
        for (char bit : encodedText) {
            if (bit != '0' && bit != '1') {
                throw std::runtime_error("Invalid character.");
            }
            output << root->character;
        }
        output.close();
        return;
    }

    Node* current = root;
    for (char bit : encodedText) {
        if (bit == '0') {
            current = current->left;
        } else if (bit == '1') {
            current = current->right;
        } else {
            throw std::runtime_error("Invalid character in texto.hfm.");
        }

        if (current == nullptr) {
            throw std::runtime_error("texto.hfm does not match arvhuf.txt.");
        }

        if (current->isLeaf) {
            output << current->character;
            current = root;
        }
    }

    if (current != root) {
        throw std::runtime_error("texto.hfm ended in the middle of Huffman code.");
    }
    output.close();
}

void deleteDecodTree(Node* root) {
    if (root == nullptr) {
        return;
    }
    deleteDecodTree(root->left);
    deleteDecodTree(root->right);
    delete root;
}