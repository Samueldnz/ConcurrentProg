#!/bin/bash

# Valores de N a serem testados
valoresN=(1000 10000 100000 1000000 10000000)

# Número de threads
threads=(1 2 3 4)

# Número de repetições por combinação
repeticoes=(1 2 3 4 5 6 7 8 9 10)

# Compila o código
gcc ehprimo.c -o ehprimo -Wall -lm -pthread

# Loop de execuções
for N in "${valoresN[@]}"; do
    for t in "${threads[@]}"; do
        for r in "${repeticoes[@]}"; do
            echo "Executando: N=$N, threads=$t, repetição=$r"
            ./ehprimo "$N" "$t" >> saida.csv
        done
        echo "" >> saida.csv  # Linha em branco opcional para separar
    done
done

echo "Execuções finalizadas!"
