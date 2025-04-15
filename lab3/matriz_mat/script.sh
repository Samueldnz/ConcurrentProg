#!/bin/bash

# Defina as dimensões das matrizes
dimensoes=(500 1000 2000 3000 5000 10000 20000)

# Defina as threads e repetições
threads=(1 2 3 4)
repeticoes=(1 2 3 4 5 6 7 8 9 10)

# Geração das matrizes
for dim in "${dimensoes[@]}"
do
    ./geraMatriz "$dim" "$dim" "mat$dim"
    echo "Matriz mat$dim gerada com sucesso!"
done

# Geração dos vetores
for dim in "${dimensoes[@]}"
do
    ./geraMatriz "$dim" "1" "vet$dim"
    echo "Vetor vet$dim gerado com sucesso!"
done

# Execução do programa a.out
for dim in "${dimensoes[@]}"
do
    for t in "${threads[@]}"
    do
        for r in "${repeticoes[@]}"
        do
            echo "Executando: dimensão=$dim, threads=$t, repetição=$r"
            ./a.out "mat$dim" "vet$dim" "$t" >> "saida.csv"
        done
        echo "" >> "saida.csv"  # Adiciona uma quebra de linha entre execuções (opcional)
    done
done

echo "Execuções finalizadas!"
