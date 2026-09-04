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

static int eh_int_positivo(const char *s){
    if (s == NULL || *s == '\0'){
        return 0;
    }

    int maior_zero = 0;

    for (const char *p = s; *p != '\0'; p++ ){
        //se encontrar qualquer espaço, sinal rejeita
        if (!isdigit((unsigned char) *p)){
            return 0;
        }

        if (*p > '0'){
            maior_zero = 1;
        }
    }
    return maior_zero;
}

//le o arquivo de entrada, valida e preenche

int analisar_arquivo_entrada (const char *camainho, int *tempo_total, Tarefa tarefas[], int *num_tarefas){
    FILE *fp = fopen(caminho, "r");
    if (fp == NULL){
        fprintf(stderr, "Erro: nao foi possivel abrir o arquivo de entrada '%s'\n", caminho);
        return 1;
    }
    char linha[256];
    int num_linha = 0;
    int tem_tempo_total = 0;
    *num_tarefas = 0;

    while (fgests(linha, sizeof(linha), fp) != NULL){
        num_linha++;

        linha[strcspn(linha, "\r\n")] = '\0';

        //pula linha em branco
        char *aparada = linha;
        while (isspace((unsigned char)) *aparada){
            aparada ++;
        }
        if (*aparada == '\0'){
                continue;
        }

        //Se ainda não lemos o tempo total de simulação, significa que esta na primeira linha válida do arquivo
        if(!tem_tempo_total){
            //olha se é inteiro
            if (!eh_int_positivo(aparada)){
                fprinf(stderr,"Erro: linha %d deveria conter o tempo total de simulacao (inteiro positivo)\n",
                        num_linha);
                fclose(fp);
                return 1;
            }

            *tempo_total = atoi(aparada);
            tem_tempo_total = 1;
            continue;
        }

        //se a qtn de tarefas atingiu o limite do vetor
        if(*num_tarefas >= MAX_TAREFAS){
            fprintf(stderr, "Erro: numero de tarefas excede o limite maximo (%d)\n", MAX_TAREFAS);
            fclose(fp);
            return 1;
        }

        //armazena os 4 campos lidos
        char nome[TAMANHO_NOME];
        char periodo_temp[32], prazo_temp[32], rajada_temp[32];

        int campos = sscanf(aparada, "%31s %31s %31s %31s", nome, periodo_temp, prazo_temp, rajada_temp);

        //se o scanf nao leu exatamente os 4 valores
        if (campos != 4){
            fprinf(stderr, "Erro: linha %d malformada, esperado 'NOME PERIODO DEADLINE BURST'\n",
                    num_linha);
            fclose(fp);
            return 1;
        }

        //verifica se é positivo
        if (!eh_inteiro_positivo(periodo_temp) ||
            !eh_inteiro_positivo(prazo_temp) ||
            !eh_inteiro_positivo(rajada_temp)) {
            fprintf(stderr,
                    "Erro: linha %d contem valor nao numerico ou nao positivo\n",
                    num_linha);
            fclose(fp);
            return 1;
        }

        //converte para inteiro
        int periodo = atoi(periodo_temp);
        int prazo = atoi(prazo_temp);
        int rajada = atoi(rajada_temp);

        //valida a regra do enunciado: BURST - DEADLINE - PERIODO
        if (!(rajada <= prazo && prazo <= periodo)) {
            fprintf(stderr,
                    "Erro: tarefa '%s' (linha %d) viola a restricao C <= D <= P\n",
                    nome, num_linha);
            fclose(fp);
            return 1;
        }

        //ponteiro para a pos de tarefas
        Tarefa *t = tarefas[*num_tarefas];
        strncpy(t->nome, nome, TAMANHO_MAX_NOME - 1);
        t->nome[TAMANHO_MAX_NOME - 1] = '\0';

        //preenche com os valores int convertidos
        t->periodo = periodo;
        t->prazo_relativo = prazo;
        t->rajada = rajada;

        t->indice_ordem = *num_tarefas //armazena a ordem de leitura
        (*num_tarefas)++;
    }

    fclose(fp);
    if (!tem_tempo_total){
        fprintf(stderr, "Erro: arquivo de entrada vazio ou sem tempo total de simulacao\n");
        return 1;
    }

    //se nao encontrou nenhuma tarefa valida apos o tempo total
    if (*num_tarefas == 0){
        fprinf(stderr, "Erro: nenhuma tarefa encontrada no arquivo de entrada\n");
        return 1;
    }

    return 0;

}