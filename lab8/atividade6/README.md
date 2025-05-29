## Objetivo

Projetar e implementar um programa concorrente em C que usa o padrão produtor/consumidor.

## Descrição

Vamos retomar com o problema da primalidade :-)

Implemente um programa concorrente onde **UMA thread PRODUTORA** gera uma sequência de **N números inteiros** e os deposita no **canal de inteiros de tamanho M** (M pequeno) — um de cada vez — que será compartilhado com **C threads CONSUMIDORAS**. Os valores de **N**, **M** e **C** devem ser lidos da entrada do programa.

As threads CONSUMIDORAS retiram os números — um de cada vez — e avaliam sua primalidade, usando a seguinte função:

```c

int ehPrimo(long long int n) {
    int i;
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (i = 3; i < sqrt(n) + 1; i += 2)
        if (n % i == 0) return 0;
    return 1;
}

```

Ao final do programa (depois que os N números foram processados), deverá ser retornado: a quantidade de primos encontrados (para avaliar a corretude do programa) e a thread consumidora vencedora (aquela que encontrou a maior quantidade de primos)

## Compilação

Você pode compilar o programa com o `gcc`:

```bash
gcc -o atividade6 atividade6.c -lpthread -lm
```

* `-lpthread`: necessário para usar a biblioteca de threads POSIX.
* `-lm`: necessário para a função `sqrt` da biblioteca `math.h`.

---

## Execução

```bash
./atividade6 <N> <tamBuffer> <C>
```

### Parâmetros:

* `N`: Quantidade de inteiros a serem produzidos.
* `tamBuffer`: Tamanho do canal de comunicação (buffer).
* `C`: Quantidade de threads consumidoras.

### Exemplo:

```bash
./atividade6 10 3 2
```

---

## Saída Esperada

O programa exibirá:

* Produções e consumos ocorrendo em tempo real;
* Quantidade de primos encontrados por cada thread;
* Thread vencedora (que encontrou mais primos).


## Observações

* O número `-1` é usado como sentinela para indicar que a produção terminou.
* O programa usa `semáforos` para garantir sincronização correta entre threads.



