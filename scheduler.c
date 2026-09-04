#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAMANHO_NOME 32
#define MAX_TAREFAS 64

typedef struct {
    char nome[TAMANHO_NOME];
    int periodo; //p
    int prazo_relativo;
    int rajada;
    int indice_ordem;   //usado como criterio de desempate

}Tarefa;

//cada tarefa tem uma instancia ativa por vez
typedef struct {
    int chegou;
    int rajada_restante;
    int prazo_absoluto;
    int instante_chegada;
}EstadoTarefa;

//estatistica acumulada por tarefa ao londo da simulaçao
typedef struct {
    int prazo_perdidos;
    int execucoes_completas;
    int mortas;
}EstatisticaTarefa;