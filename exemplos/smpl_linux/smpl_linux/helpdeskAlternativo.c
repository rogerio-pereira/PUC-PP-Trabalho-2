/*##############################################################################
#	Tema: Simula��o de Fila de Chamados de HelpDesk                        #
#	Alunos: Leonardo B. Neves | Ant�nio J. M. Vianna		       #
#									       #
#	Dados: | Analista | T.Chegada | T.Servi�o | T.Simula��o | Max Jobs     #
#		    1	      28min	  49min       1440min        52        #
#		    2	      28min       99min       1440min        52        #
#									       #
#	Compilar: gcc arquivo.c -o arquivo -lm 				       #
##############################################################################*/

#include <stdio.h>
#include "smpl.c"
#include "stat.c"
#include "rand.c"
#include "fila.h"

typedef struct {

int tempo_chegada;
int tempo_saida;

}chamado;

analista analista1, analista2;

int escalonaChamado(int chamado){
	if(status(analista1.id)==0){
		analista1.chamadoAtual = chamado;
		return analista1.id;
	}
	else if(status(analista1.id)==1 && status(analista2.id)==1){

		if(inq(analista1.id) < inq(analista2.id)){
			analista1.chamadoAtual = chamado;
			return analista1.id;
		}
		else{
			analista2.chamadoAtual = chamado;
			return analista2.id;
		}

	}
	else if(status(analista2.id)==0){
		analista2.chamadoAtual = chamado;
		return analista2.id;
	}

}

int main(void)
{
	//tempo de servi�o em minutos
	real ts=0.0;
	//tempo de chegada em minutos
	real ta=28.0;
	//tempo de simulacao em minutos
	real te=1440.0;

	fila filaChamadosRelease;
	analista analistaRelease;
	int analistaCorrente;
	analista1.ts = 49.00;
	analista2.ts = 99.00;
	create(&filaChamadosRelease);

	int chamados=0;
	// int chamado, substituir por uma struct com tempo_chegada e tempo_saida, para saber quanto tempo o chamado ficou na fila
	int chamado=1, event;
	int num_total_cheg=0, tam_max_fila=0, num_job_serv=0;
	int num_job_gerados=0, max_jobs=52;

	float tx_cheg;

	smpl(0,"Simula Fila");
	// Facility = quantidade de recursos para atender uma fila. O 1 indica a quantidade de facilities existentes para uma unica fila
	analista1.id = facility("analista1",1);
	analista2.id = facility("analista2",1);
	analista1.ch_feitos = 0;
	analista2.ch_feitos = 0;

	/*
	  schedule = Escalonamento ou agendamento de eventos. Parametros: evento,tempo_de_permanencia_no_evento,chamado.
	  Segundo parametro � 0.0 pois a chegada na fila n�o conta tempo.
	*/
	schedule(1,0.0,chamado);

	while (time()<te && chamados<=max_jobs)
	{
		//Recupera o proximo evento da lista
		cause(&event, &chamado);
		switch(event)
		{
			//chegada do chamado
			case 1:

				schedule(2,0.0,chamado);
        chamados++;
				num_total_cheg++;
				num_job_gerados++;
				chamado++;
				//expntl() = usada para eventos em que o objetivo � medir o tempo de chega na fila (ta)
				schedule(1,expntl(ta),chamado);
				break;

			//Analista assume o chamado
			case 2:

				/*
				 request = pedido de reserva de um recurso, retorna 0 se tiver algum servidor disponivel para atender ao pedido,
				 neste caso algum analista. Parametros: recurso a ser utilizado,quem chegou para utiliza-lo, prioridade de quem chegou.
				*/
				analistaCorrente = escalonaChamado(chamado);
				if (request(analistaCorrente,chamado,0) == 0){
					if(analistaCorrente == analista1.id){
						insert(&filaChamadosRelease,analista1);
						// normal() = usada para eventos que tem tempo de servi�o. 5.0 varia��o para + ou -
						schedule(3,normal(analista1.ts,5.0),analista1.chamadoAtual);
					}
					else{
						insert(&filaChamadosRelease,analista2);
						// normal() = usada para eventos que tem tempo de servi�o. 5.0 varia��o para + ou -
						schedule(3,normal(analista2.ts,5.0),analista2.chamadoAtual);
					}
				}
				else{
					//inq = tamanho atual da fila
					if(analistaCorrente == analista1.id){
						if (inq(analistaCorrente) > analista1.max_fila){

								analista1.max_fila = inq(analistaCorrente);
						}
				  }
					else{
						if (inq(analistaCorrente) > analista2.max_fila)

								analista2.max_fila = inq(analistaCorrente);
					}
				}
				break;

			//Analista finaliza o chamado
			case 3:
				num_job_serv++;
				// Request e release tem que ser em eventos diferentes e seguidos, ap�s um request vem um release
				// release libera o chamado da fila, libera a facilite para um novo request
				if(isEmpty(&filaChamadosRelease)==1){
					removee(&filaChamadosRelease,&analistaRelease);
				  release(analistaRelease.id,analistaRelease.chamadoAtual);
					if(analistaRelease.id == analista1.id)
						analista1.ch_feitos++;
					else
						analista2.ch_feitos++;
			  }

				break;
		}
	}

	printf("\n<----------- Inicio do Relatorio de Simulacao de Chamados -----------> \n");
	printf("\n\t Total de chamados que chegaram -> %d chamados",chamados);
	printf("\n\t Tamanho maximo da fila analista 1-> %d chamados",analista1.max_fila);
	printf("\n\t Tamanho maximo da fila analista 2-> %d chamados",analista2.max_fila);
	printf("\n\t Tamanho medio de fila do analista 1: %.2f chamados", Lq(analista1.id));
	printf("\n\t Tamanho medio de fila do analista 2: %.2f chamados", Lq(analista2.id));
	printf("\n\t Quantidade de chamados feitos analista 1: %d chamados", analista1.ch_feitos);
	printf("\n\t Quantidade de chamados feitos analista 2: %d chamados", analista2.ch_feitos);
	printf("\n\t Utilizacao do analista 1: %.2f%c", U(analista1.id)*100,37);
	printf("\n\t Utilizacao do analista 2: %.2f%c", U(analista2.id)*100,37);
	printf("\n\n<----------- Fim do Relatorio de Simulacao de Chamados --------------> \n\n");
  getchar();
  // Escreve na tela o resultado da simula��o.
	report();
	//printf("\nTempo simulado %.2f numero clientes %d", time(), num_job_getdados);
}
