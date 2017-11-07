#include <stdio.h>


typedef struct {

	//tempo de serviço em minutos
	real ts;
	int id;
	int chamadoAtual;
	int ch_feitos;
	int max_fila;

}analista;

struct no
{
	analista an;
	struct no *prox;
};

typedef struct
{
	struct no *inicio;
	struct no *fim;
}fila;

void create(fila *q)
{
	q->inicio=NULL;
	q->fim=NULL;
}

int insert (fila *q, analista a)
{
	 struct no *aux;
	 aux = (struct no*)malloc(sizeof(struct no));
	 if (aux==NULL)
	 return 1;

	 aux->an=a;
	 aux->prox=NULL;
	 if (q->inicio==NULL)
	 q->inicio=aux;
	 if (q->fim!=NULL)
	 q->fim->prox=aux;

	 q->fim=aux;
	 return 0;
}


int removee(fila *q, analista *a)
{
	struct no *aux;
	if (q->inicio==NULL)
	 return 1;
	aux=q->inicio;
	q->inicio=aux->prox;
	if (q->inicio == NULL)
	q->fim = NULL;
	*a=aux->an;
	free(aux);
	return 0;
}

int isEmpty(fila *q){

	if(q->inicio==NULL)
		return 0;

	return 1;

}


int imprime(fila q)
{

    struct no *aux;
    aux = q.inicio;

    if (aux != NULL)
    {
        while (aux != NULL)
        {
					printf("\n\n##\nFILA\n%d - %d", aux->an.id, aux->an.chamadoAtual);
					aux = aux->prox;
				}
		}
}
