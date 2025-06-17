/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 11 */
/* Codigo: Exemplo de uso de futures */
/* Editado: Samuel Sampaio Diniz - 122076523 */
/* -------------------------------------------------------------------*/

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import java.util.ArrayList;
import java.util.List;



// --- Worker 1: Soma de 1 até N ---
class WorkerSoma implements Callable<Long> {
  private final int n;

  public WorkerSoma(int n) {
      this.n = n;
  }

  @Override
  public Long call() {
      long soma = 0;
      for (int i = 1; i <= n; i++) soma += i;
      return soma;
  }
}


// --- Worker 2: Verifica se um número é primo ---
class WorkerPrimo implements Callable<Integer> {
  private final int numero;

  public WorkerPrimo(int numero) {
      this.numero = numero;
  }

  // Verifica se o número é primo
  @Override
  public Integer call() {
      if (numero <= 1) return 0;
      if (numero == 2) return 1;
      if (numero % 2 == 0) return 0;

      for (int i = 3; i <= Math.sqrt(numero); i += 2) {
          if (numero % i == 0) return 0;
      }

      return 1;
  }
}

//classe do método main
public class FutureHello  {
  private static final int N = 20;
  private static final int NTHREADS = 10;

  public static void main(String[] args) {
    //cria um pool de threads (NTHREADS)
    ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);
    //cria uma lista para armazenar referencias de chamadas assincronas
    List<Future<Long>> list = new ArrayList<Future<Long>>();
    List<Future<Integer>> listPrimo = new ArrayList<Future<Integer>>();

    for (int i = 0; i < N; i++) {

      Callable<Long> workerSoma = new WorkerSoma(i);
      Future<Long> submit = executor.submit(workerSoma);
      list.add(submit);


      Callable<Integer> workerPrimo = new WorkerPrimo(i);
      Future<Integer> submitPrimo = executor.submit(workerPrimo);
      listPrimo.add(submitPrimo);
    }

    System.out.println("Tam da lista de números: " + list.size());
    System.out.println("Tam da lista de primos: " + listPrimo.size());
    //pode fazer outras tarefas...

    //recupera os resultados e faz o somatório final
    long sum = 0;
    for (Future<Long> future : list) {
      try {
        sum += future.get(); //bloqueia se a computação nao tiver terminado
      } catch (InterruptedException e) {
        e.printStackTrace();
      } catch (ExecutionException e) {
        e.printStackTrace();
      }
    }
    System.out.println("Soma total dos números de 1 a " + N + ":" + sum);

    // Recupera os resultados (0 ou 1) e soma para contar quantos são primos
    Integer totalPrimos = 0;
    for (Future<Integer> future : listPrimo) {
        try {
            totalPrimos += future.get(); // bloqueia se necessário
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        }
    }
    System.out.println("Total de primos entre 1 e " + N + ": " + totalPrimos);

    executor.shutdown();
  }
}
