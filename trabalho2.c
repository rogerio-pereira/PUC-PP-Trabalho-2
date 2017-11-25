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
            tempoSimulacao=3600.0;          //Tempo total de Simulação (1 Hora)
    //Quantidade Facilities
    int     qtdLog = 1000,                  //Quantidade de items a serem salvos no log (numero de facilities)
            max_jobs=1000000;                //Quantidade máxima de acessos


    //VARIAVEIS QUE NÃO SÃO ALTERADAS PARA A SIMULACAO
    int     acessos=0,                      //Quantidade de Acessos no Total
            acesso=1,                       //Acesso individual
            cliques=0,                      //Quantidade de Acessos no Total
            clique=1,                       //Acesso individual
            event, 
            fAcesso,                        //Facility Acesso
            fClique,                        //Facility Cliques
            qtdTotalAcessos=0,              //Número de pessoas que chegaram ao estabelecimento
            qtdTotalCliques=0,              //Número de pessoas que chegaram ao estabelecimento
            tamanhoMaximoFilaAcesso=0,      //Tamanho máximo da fila de acessos
            tamanhoMaximoFilaCliques=0,     //Tamanho máximo da fila de cliques
            qtdProcessosGerados=0;          //Quatidade de Processos Gerados

    float tx_cheg;

    smpl(0,"Simula Acesso a sistema");

    //Facilities
    fAcesso = facility("acesso",1); 
    fClique = facility("cliques",1); 
    
    schedule(1,0.0,acesso); 

    while (time()<tempoSimulacao && acessos<=max_jobs)
    {
        cause(&event, &acesso);
        switch(event)
        {
            /*
                Acesso a algum site
             */
            case 1: 
                schedule(2,0.0,acesso);
                acessos++;
                qtdTotalAcessos++;
                qtdProcessosGerados++;
                acesso++;
                //Grava Log
                if (inq(fAcesso) >= qtdLog)
                    schedule(2,0.0,acesso);

                //Novas Chegadas
                schedule(1,expntl(tempoAcessoSite),acesso);
                schedule(3,expntl(tempoClique),clique);

                break;
            /*
                Grava log Acesso
             */
            case 2:
                if (request(fAcesso,acesso,0) == 0)
                    release(fAcesso,acesso);
                else
                {
                    if (inq(fAcesso) > tamanhoMaximoFilaAcesso)
                        tamanhoMaximoFilaAcesso = inq(fAcesso);
                }
                break;
            /*
                Cliques
             */
            case 3:
                cliques++;
                qtdTotalCliques++;
                clique++;
                //Grava Log
                if (inq(fClique) >= qtdLog)
                    schedule(4,0.0,acesso);
                break;
            /*
                Grava Log Cliques
             */
            case 4:
                if (request(fClique,clique,0) == 0)
                    release(fClique,clique);
                else
                {
                    if (inq(fClique) > tamanhoMaximoFilaCliques)
                        tamanhoMaximoFilaCliques = inq(fClique);
                }
                break;
        }
    }

    printf("\nNumero de acessos que chegaram: %d",acessos);
    printf("\nTamanho maximo fila acesso: %d acessos",tamanhoMaximoFilaAcesso);
    printf("\nTamanho maximo fila cliques: %d cliques",tamanhoMaximoFilaCliques);
    printf("\n\nPressione qualquer tecla para gerar o relatório");
    getchar();
    
    report();
}
