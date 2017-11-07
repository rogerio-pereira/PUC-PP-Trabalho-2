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

int main(void)
{
	//tempo de serviço em minutos (Tempo médio entre os 2 analistas)
	real ts=74.0;
	//tempo de chegada em minutos
	real ta=28.0;
	//tempo de simulacao em minutos
	real te=1440.0;
	int chamados=0, filaChamados;
	int chamado=1, event;
	int tam_max_fila=0, num_job_serv=0;
	int max_jobs=552;

	smpl(0,"Simula Fila");
	// Facility = quantidade de recursos para atender uma fila. O 1 indica a quantidade de facilities existentes para uma unica fila
	filaChamados = facility("filaChamados",2);

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
				chamado++;
				//expntl() = usada para eventos em que o objetivo � medir o tempo de chega na fila (ta)
				schedule(1,expntl(ta),chamado);
				break;

			//chamado vai para algum analista
			case 2:

				/*
				 request = pedido de reserva de um recurso, retorna 0 se tiver algum servidor disponivel para atender ao pedido,
				 neste caso algum analista. Parametros: recurso a ser utilizado,quem chegou para utiliza-lo, prioridade de quem chegou.
				*/

				if(request(filaChamados,chamado,0) == 0){

						schedule(3,normal(ts,5.0),chamado);

				}
				else{

					//inq = tamanho atual da fila
					if (inq(filaChamados) > tam_max_fila)
						tam_max_fila = inq(filaChamados);
				}
				break;

			//Analista finaliza o chamado
			case 3:

				/* Request e release tem que ser em eventos diferentes e seguidos, ap�s um request vem um release
				   release libera o chamado da fila, libera a facilite para um novo request
				*/
				release(filaChamados,chamado);
				num_job_serv++;
				break;
		}
	}
	/* Resultados Encontrados na simulação */
	printf("\n<----------- Inicio do Relatorio de Simulacao de Chamados -----------> \n");
	printf("\n\t Total de chamados que chegaram -> %d chamados",chamados);
	printf("\n\t Total de chamados concluidos -> %d chamados",num_job_serv);
	printf("\n\t Tamanho maximo da fila -> %d chamados",tam_max_fila);
	printf("\n\t Tamanho medio de fila -> %.2f chamados", Lq(filaChamados));
	printf("\n\t Tempo de simulação -> %.2f ut",time());
	printf("\n\n<----------- Fim do Relatorio de Simulacao de Chamados --------------> \n\n");
  getchar();

	// Escreve na tela o resultado da simula��o.
	report();

}
