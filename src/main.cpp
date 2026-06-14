//Trabalho de estrutura de Dados

#include <iostream>
#include "codificador.h"
#include "decodificador.h"

int main() {
    codificador("../texts/input.txt","../texts/texto.hfm","../texts/arvhuf.txt");
    decodificador("../texts/decod.txt","../texts/texto.hfm","../texts/arvhuf.txt");
    return 0;
}