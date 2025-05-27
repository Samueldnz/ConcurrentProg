## Objetivo

Implementar uma variação do padrão produtor/consumidor.

## Descrição

Considere o padrão produtor/consumidor com a seguinte alteração: em uma inserção, os produtores preenchem completamente o buffer (para isso, o buffer precisa estar vazio). A lógica dos consumidores permanece a mesma. Implemente as funções de `insere` e `retira` no buffer, seguindo a lógica descrita acima.

## Roteiro

1. Comece projetando a solução do problema (funções `insere` e `retira`);
2. Implemente o programa em C, criando as threads e gerando log de execução;
3. Teste o programa deixando-o executar por longo tempo e avaliando a corretude do log de saída.

## ✅ Variação aplicada:

- Os **produtores só podem inserir dados no buffer quando ele estiver completamente vazio**.
- Quando possível, um produtor **preenche o buffer por completo**.
- Os **consumidores retiram um item por vez**, como no padrão clássico.

---

## ⚙️ Como compilar e executar

### Requisitos:

- Compilador `gcc`
- Suporte a threads (`-pthread`)

### Comandos:

OBS: o programa roda em looping, use CTRL+C para interromper

```bash
gcc -o produtor_consumidor produtor_consumidor.c -pthread -Wall
./produtor_consumidor


