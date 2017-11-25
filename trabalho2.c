// compilar com gcc cancela.c -o cancela -lm

#include "smpl.c" //Funções de simulação
#include "stat.c" //Funções estatísticas
#include "rand.c"

int main(void)
{
    //VARIAVEIS QUE PODEM SER ALTERADAS PARA SIMULAÇÃO
    //Todos os tempos são em minutos
    real    tempoAcessoSite=0.2,            //Média de Tempo de acesso, para exibir anuncio
            DPAcesso=1.0,                   //Desvio Padrão da Média de Acessos
            tempoClique=2.0,                //Média de Tempo cliques nos anuncios
            DpClique=1.0,                   //Desvio Padrão da Média de Tempo de Cliques 
            //tempoGravarLog=0.0000012,       //Tempo para gravação de log (4 milisegundos)
            tempoGravarLog=4.0,       //Tempo para gravação de log (4 milisegundos)
            DPGravarLog=1.0,                //Desvio Padrão tempo de Log
            tempoSimulacao=3600.0;          //Tempo total de Simulação (1 Hora)
    //Quantidade Facilities
    int     qtdParaGravarLog = 1000,         //Quantidade de items a serem salvos no log (numero de facilities)
            max_jobs=1000000;                //Quantidade máxima de acessos


    //VARIAVEIS QUE NÃO SÃO ALTERADAS PARA A SIMULACAO
    int     acessos=0,                      //Quantidade de Acessos no Total
            acesso=1,                       //Acesso individual
            cliques=0,                      //Quantidade de Acessos no Total
            clique=1,                       //Acesso individual
            event, 
            fAcesso,                        //Facility Acesso
            fClique,                        //Facility Cliques
            qtdFilaLogAcesso=0,             //Número de pessoas que chegaram ao estabelecimento
            qtdFilaLogCliques=0,              //Número de pessoas que chegaram ao estabelecimento
            tamanhoMaximoFilaAcesso=0,      //Tamanho máximo da fila de acessos
            tamanhoMaximoFilaCliques=0,     //Tamanho máximo da fila de cliques
            qtdProcessosGerados=0,          //Quatidade de Processos Gerados
            i,
            j;


    float tx_cheg;

    smpl(0,"Simula Acesso a sistema");

    //Facilities
    fAcesso = facility("acesso",1); 
    fClique = facility("cliques",1); 
    
    schedule(1,0.0,acesso); 

    while (time()<tempoSimulacao && qtdProcessosGerados<=max_jobs)
    {
        cause(&event, &acesso);
        switch(event)
        {
            /*
                Acesso a algum site
             */
            case 1:                 
                qtdFilaLogAcesso++;
                acessos++;
                qtdProcessosGerados++;

                //Grava Log
                if(qtdFilaLogAcesso == qtdParaGravarLog)
                {
                    schedule(2,0.0,acesso);

                    acesso++;
                    
                    //Limpa Fila
                    qtdFilaLogAcesso=0;
                }
                
                //Novas Chegadas
                schedule(1,expntl(tempoAcessoSite),acesso);
                schedule(4,expntl(tempoClique),clique);

                break;
            /*
                Grava log Acesso
             */
            case 2:
                    if (request(fAcesso,acesso,0) == 0)
                    {
                        schedule(3,normal(tempoGravarLog,DPGravarLog),acesso);
                    }
                    else
                    {
                        if (inq(fAcesso) > tamanhoMaximoFilaAcesso)
                            tamanhoMaximoFilaAcesso = inq(fAcesso);
                    }
                break;
            /*
                LIBERA LOG ACESSO
             */
            case 3:
                release(fAcesso,acesso);
                break;
            /*
                Cliques
             */
            /*case 4:
                qtdFilaLogCliques++;
                cliques++;

                //Grava Log
                if(qtdFilaLogCliques == qtdParaGravarLog)
                {
                    schedule(5,0.0,clique);

                    clique++;
                    
                    //Limpa Fila
                    qtdFilaLogCliques=0;
                }
                break;*/
            /*
                Grava Log Cliques
             */
            /*case 5:
                if (request(fClique,clique,0) == 0)
                    {
                        schedule(6,normal(tempoGravarLog,DPGravarLog),clique);
                    }
                    else
                    {
                        if (inq(fClique) > tamanhoMaximoFilaCliques)
                            tamanhoMaximoFilaCliques = inq(fClique);
                    }
                break;*/
            /*
                LIBERA LOG CLIQUE
             */
            /*case 6:
                release(fAcesso,acesso);
                break;*/
        }
    }

    printf("\nQuantidade de acessos: %d acessos",acessos);
    printf("\nQuantidade de cliques: %d cliques",cliques);
    printf("\n\nPressione qualquer tecla para gerar o relatório");
    getchar();
    
    report();
}
