# Codificação de Huffman em C++

Este repositório apresenta uma implementação do algoritmo de **Codificação de Huffman**, utilizado para compressão de dados sem perda. O projeto tem como objetivo demonstrar, de forma prática, como é possível reduzir o tamanho de arquivos de texto por meio da atribuição de códigos binários menores aos caracteres mais frequentes.

## Sobre o algoritmo

A Codificação de Huffman é um método clássico de compressão baseado na frequência dos símbolos de uma mensagem. A ideia central é simples:

- caracteres que aparecem com maior frequência recebem códigos menores;
- caracteres menos frequentes recebem códigos maiores;
- nenhum código gerado é prefixo de outro, garantindo decodificação única.

Para isso, o algoritmo constrói uma **árvore binária de Huffman**, na qual cada folha representa um caractere do arquivo original.

## Funcionalidades

- Leitura de arquivos de texto;
- Cálculo da frequência de cada caractere;
- Construção da árvore de Huffman;
- Geração da tabela de códigos binários;
- Codificação do conteúdo original;
- Decodificação da mensagem comprimida;
- Manipulação de arquivos em C++ usando `fstream`.

## Tecnologias utilizadas

- C++;
- Biblioteca padrão STL;
- Filas de prioridade;
- Árvores binárias;
- Manipulação de arquivos.

## Estrutura do projeto

```txt
.
├── src/
│   ├── decodificador.cpp
│   ├── codificador.cpp
│   └── main.cpp
├── texts/
|   ├── arvhuf.txt
|   ├── decod.txt
|   ├── input.txt
|   └── texto.hfm
└── README.md
