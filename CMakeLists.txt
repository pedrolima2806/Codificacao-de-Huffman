cmake_minimum_required(VERSION 4.2)
project(Huffman)

set(CMAKE_CXX_STANDARD 20)

add_executable(Huffman src/main.cpp
        src/codificador.cpp
        src/decodificador.cpp
)
