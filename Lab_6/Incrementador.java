//import java.utils.Arrays;

class Incrementador{
        private int[] vetor;
	private int quantidadeThreads;

        public Incrementador(int quantidadeThreads, int[] vetor){
                this.vetor = vetor;
		this.quantidadeThreads = quantidadeThreads;
        }

        public synchronized void incrementar(int tid){
                for(int i = tid; i< vetor.length; i += this.quantidadeThreads){
			vetor[i] += 1;
		}
        }

        public synchronized void getVetor() {
		boolean sucesso = true;

		for(int numero : vetor){
			if(numero != 2) sucesso=false;
			System.out.print(numero + " ");
		}

//		if(Arrays.equals(vetorTeste,vetor)) System.out.println("Resultado correto");
		if(sucesso) System.out.println("\nResultado correto");
		else System.out.println("\nResultado errado");
        }

}

class T extends Thread {
	private int id;
	Incrementador incrementador;
 
	public T(int tid, Incrementador incrementador) { 
		this.id = tid; 
		this.incrementador = incrementador;
	}

	public void run() {
		System.out.println("Thread " + this.id + " iniciou!");
		incrementador.incrementar(id);
		System.out.println("Thread " + this.id + " terminou!"); 
	}
}



class MainClass{
        static int tamanhoVetor=9;
        static int quantidadeThreads = tamanhoVetor;
        public static void main (String[] args){
                
                if(args.length < 1) {
                  System.out.println("Digite MainClass <quantidade threads>");
                }
                
                if(Integer.parseInt(args[0]) < 9) quantidadeThreads = Integer.parseInt(args[0]);
                Thread[] threads = new Thread[quantidadeThreads];
                
                int[] vetor = new int[tamanhoVetor];
                for (int i=0; i<tamanhoVetor; i++) {
                  vetor[i] = 1;
                }

                Incrementador incrementador=  new Incrementador(quantidadeThreads, vetor);

                for (int i=0; i<threads.length; i++) {
                  threads[i] = new T(i, incrementador);
                }
                
                for (int i=0; i<threads.length; i++) {
                   threads[i].start();
                }

                for (int i=0; i<threads.length; i++) {
                   try { threads[i].join(); } 
                   catch (InterruptedException e) { return; }
                }

                System.out.println("Terminou, resultado final:");
		incrementador.getVetor(); 
        }
}


