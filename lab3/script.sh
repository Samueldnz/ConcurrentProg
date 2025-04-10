#!/bin/bash

# Defina as dimensões das matrizes
dimensoes=(500 1000 2000 3000 5000 10000 20000)

# Defina as threads e repetições
threads=(1 2 3 4)
repeticoes=(1 2 3 4 5 6 7 8 9 10)

# Geração das matrizes
for dim in "${dimensoes[@]}"
do
    # Gera a matriz com a dimensão <dim> x <dim>
    ./geraMatriz "$dim" "$dim" "mat$dim"
    echo "Matriz mat$dim gerada com sucesso!"
done

# Execução do programa a.out
for dim in "${dimensoes[@]}"
do
    for i in "${threads[@]}"
    do
        for j in "${repeticoes[@]}"
        do
            # Executa o a.out com o arquivo de matriz, vetor e número de threads
            ./a.out "mat$dim" "vet$dim" "$dim" "$i"
        done
        echo ""
    done
done
