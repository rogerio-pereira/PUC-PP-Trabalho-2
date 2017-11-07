// compilar com gcc cancela.c -o cancela -lm

#include "smpl.c" //Funções de simulação
#include "stat.c" //Funções estatísticas
#include "rand.c"

int main(void)
{   //tempo de chegada - tempo de serviço - tempo de simulacao (tempo de observação para avaliação)
	real ta=3.0, ts=10.0, te=3600; int carros=0;
	int carro=1, event, cancela;
	int num_total_cheg=0, tam_max_fila=0, num_job_serv=0;
	int num_job_gerados=0, max_jobs=600000;

	float tx_cheg;

	smpl(0,"Simula Fila");
/*
Se for uma fila para 5 caixas
caixa = facility("caixa",5);

Se for 5 filas, cada uma para 1 caixa
caixa1 = facility("caixa1",1);
caixa2 = facility("caixa2",1);
caixa3 = facility("caixa3",1);
caixa4 = facility("caixa4",1);
caixa5 = facility("caixa5",1);
*/
	cancela = facility("cancela",2); // O 1 INDICA a quantidade de facilities existentes para uma unica fila

/*
 Parametro 1 - Evento (definidos no switch case)
 Parametro 2 - Tempo inicial (para o primeiro item, o tempo deve ser zero)
 Parametro 3 - Token (Id do Carro)
 
*/
	schedule(1,0.0,carro); //o schedule adiciona na fila e o cause retira da fila
// o carro vai para o evento 1 na posicao 0.0


/*
 Duas condições, a primeira que ocorrer cancela o loop

 a função time vai incremetando conforme o programa roda
*/
	while (time()<te && carros<=1000)
	{
		cause(&event, &carro);
		switch(event)
		{
			case 1: //chegada
				schedule(2,0.0,carro);
                                carros++;
				num_total_cheg++;
				num_job_gerados++;
				carro++;
				schedule(1,expntl(ta),carro);
				break;
			case 2: //Requisita servidor
				if (request(cancela,carro,0) == 0)
					schedule(3,normal(ts,1.0),carro);
				else
				{
/*
INQ é o numero de itens na fila
*/
					if (inq(cancela) > tam_max_fila)
						tam_max_fila = inq(cancela);
				}
				break;
			case 3: //release cancela
				num_job_serv++;
/*
Libera o item da fila
*/
				release(cancela,carro);
				break;
		}
	}
        //clrscr();
	printf("\n carros que chegaram %d",carros);
	printf("\n tamanho maximo fila  %d",tam_max_fila);
        getchar();
	report();


	      //	printf("\nTempo simulado %.2f numero clientes %d", time(), num_job_getdados);
}
