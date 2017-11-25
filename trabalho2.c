// compilar com gcc cancela.c -o cancela -lm

#include "smpl.c" //Funções de simulação
#include "stat.c" //Funções estatísticas
#include "rand.c"

int main(void)
{
    //VARIAVEIS QUE PODEM SER ALTERADAS PARA SIMULAÇÃO
    //Todos os tempos são em minutos
    real    tempoChegada=10.0,              //Média de Tempo de chegada de clientes
            DPChegada=1.0,                  //Desvio Padrão da Média de Chegada 
            tempoAtendimento=7.0,           //Média de Tempo de Atendimento 
            DPCaixa=1.0,                    //Desvio Padrão da Média de Chegada 
            tempoCaixa=2.0,                 //Média de Tempo de pagamento no caixa 
            tempoSimulacao=120.0;           //Tempo total de Simulação (2 Horas)
    //Quantidade Facilities
    int     qtdAtendente = 1,               //Quantidade de Garçons
            qtdCaixas = 1,                  //Quantidade de Caixas
            max_jobs=1000;                 //Quantidade máxima de pessoas


    //VARIAVEIS QUE NÃO SÃO ALTERADAS PELO CLIENTE
    int     pessoas=0,                      //Quantidade de Pessoas no total
            pessoa=1,                       //Individuo
            event, 
            caixa,                          //Facility Caixa
            atendente,                      //Facility Garçom
            qtdTotalChegada=0,              //Número de pessoas que chegaram ao estabelecimento
            tamanhoMaximoFilaCaixa=0,       //Tamanho máximo da fila
            tamanhoMaximoFilaAcougue=0,     //Tamanho máximo da fila
            qtdPessoasPagaram=0,            //Quantidade de pessoas que pagaram
            qtdPessoasChegaram=0,           //Quantidade de Pessoas que chegaram no intervalo
            qtdProcessosGerados=0;          //Quatidade de Processos Gerados

    float tx_cheg;

    smpl(0,"Simula Fila Açougue");

    //Facilities
    atendente = facility("atendente",qtdAtendente); 
    caixa = facility("caixa",qtdCaixas); 
    
    schedule(1,0.0,pessoa); 

    while (time()<tempoSimulacao && pessoas<=max_jobs)
    {
        cause(&event, &pessoa);
        switch(event)
        {
            /*
                CHEGOU CLIENTE NO AÇOUGUE
             */
            case 1: 
                schedule(2,0.0,pessoa);
                pessoas++;
                qtdTotalChegada++;
                qtdProcessosGerados++;
                pessoa++;
                schedule(1,expntl(tempoChegada),pessoa);
                break;
            /*
                ATENDENTE ATENDE PESSOA NO AÇOUGUE
             */
            case 2:
                if (request(atendente,pessoa,0) == 0)
                    schedule(3,normal(tempoChegada,DPChegada),pessoa);
                else {
                    if (inq(atendente) > tamanhoMaximoFilaAcougue)
                        tamanhoMaximoFilaAcougue = inq(pessoa);
                }
                break;
            /*
                FILA CAIXA
             */
            case 3:
                    schedule(4,normal(tempoCaixa,DPCaixa),pessoa);
                break;
            /*
                PESSOA PAGA NO CAIXA
             */
            case 4:
                qtdPessoasPagaram++;
                release(atendente,pessoa);
                break;
        }
    }

    printf("\nNumero de pessoas que chegaram: %d",pessoas);
    printf("\nTamanho maximo fila acougue: %d pessoas",tamanhoMaximoFilaAcougue);
    printf("\nTamanho maximo fila caixa: %d pessoas",tamanhoMaximoFilaCaixa);
    printf("\n\nPressione qualquer tecla para gerar o relatório");
    getchar();
    
    report();
}
