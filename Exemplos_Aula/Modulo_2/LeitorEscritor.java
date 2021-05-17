
//Monitor que implementa a logica do padrao leitores/escritores
//Não possui nenhuma lógica da aplicação, apenas do padrão
//dessa forma o padrão pode ser reutilizado por outras aplicações

//Java permite mistura de métodos sincronizados e não sincronizados, isso
//fere o conceito de monitor, por isso dizemos que java não o implementa.
//Porém oferece condições parecidas.
class LE{
	private int leit, escr;


	LE(){
		this.leit = 0; //leitores ativos (0 ou mais)
		this.escr = 0; //escritores ativos (0 ou 1)
	}

	public synchronized void entraLeitor(int id){ //importante a não implementação da lógica de leitura
		try{				      //pois a instancia da classe é usada no lock
						      //então durante a leitura o ideal é q a thread tenha saído
                                                     //para permitir a entrada de outra leitora
			while(this.escr > 0){
				System.out.println("le.leitorBloqueado("+id+")");
				wait();
			}
			this.leit++;
			System.out.println("le.leitorLendo("+id+")");
		}catch(InterruptedException e){}
	}

	public synchronized void  SaiLeitor(int d){
		this.leit--;
		if(this.leit == 0) this.notify(); //libera escritor(caso exista escritor bloqueado)
		System.out.println("le.leitorSaindo("+id+")");
	}

	public synchronized void entraEscritor(int id){
		try{
			while(this.leit > 0 || this.escr > 0){
				System.out.println("le.escritorBloequeado("+id+")");
				wait();
			}
			this.escr++;
			System.out.println("le.escritorEscrevendo("+id+")");
		}catch(InterruptedException e){
		}
	
	}

	public synchronized void saiEscritor(int id){
		this.escr--;
		notifyAll();
		System.out.println("le.escritorSaindo("+id+")");
	}

}



class Leitor extends Thread{
	int id;
	int delay;
	LE monitor;

	Leitor(int id, int delayTime, LE m){
		this.id=id;
		this.delay=delayTime;
		this.monitor=m;
	} 

	public void run(){
		double j=77777777.7, i;
		try{
			for(;;){
				this.monitor.entraLeitor(this.id);
				for(i=0;i<1000000;i++){j=j/2;}//loop bobo para simbolizar o tempo de leitura
				this.monitor.saiLeitor(this.id);
				sleep(this.delay);
			}
		}catch(InterruptedException e){return;}
	}
}


class Escritor extends Thread{
	int id;
	int delay;
	LE monitor;

	Escritor(int id; int delayTime; LE m){
		this.id=id;
		this.delay=delayTime;
		this.monitor = m;
	}

	public void run(){
		double j=77777777777.7, i;
		try{
			for(;;){
				this.monitor.entraEscritor(this.id);
				for(i=0;i <1000000;i++){j=j/2;};
				this.monitor.saiEscritor(this.id);
				sleep(this.delay);
			}
		}catch(InterruptedException e){return;}
}



class LeitorEscritor{
	static final int L = 4; //qunatidade de leitores
	static final int E = 3; //quantidade de escritores;

	public static void main(String[] args){
		int i;
		LE monitor = new LE(); //criando apenas um monitor
		Leitor[] l = new Leitor[L]; //criando array para vários leitores
		Escritor[] e = new Escritor[E]; //criando array para vários escritores

		System.out.println("import verificaLE");
		System.out.println("le = verificaLE.LE()");

		for(i=0;i<L;i++){
			l[i] = new Leitor(i+1, (i+1)*500, monitor);//colocando vários leitores no array
			l[i].start(); //iniciando leitores
		}

		for(i=0;i<E;i++){
			e[i] = new Escritor(i+1, (i+1)*500, monitor);//colocando vários escritores no array
			e[i].start(); //iniciando escritores
		}
	}


}
