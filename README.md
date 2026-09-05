# Escalonamento de Tarefas Críticas de Voo — rate-monotonic vs EDF

Aluna: Vitória Gabrielle Janeiro Duran (login: vgjda2)
Disciplina: Infraestrutura de Software — Implementação 3

## Sistema operacional utilizado
Ubuntu 24.04 LTS via WSL2 (Windows Subsystem for Linux)

## Arquivos
- `scheduler.c` — arquivo único com toda a implementação:
  - leitura e validação do arquivo de entrada (`analisar_arquivo_entrada`);
  - motor de simulação tick-a-tick com suporte a rate-monotonic e EDF (`simular`, `tem_prioridade_maior`);
  - geração do arquivo de saída no formato exigido (`escrever_saida`, `para_maiusculas`).
- `Makefile` — regras de compilação (`make` gera o executável `scheduler`; `make clean` remove binários e arquivos `.out`).

## Como compilar
```
make
```
Compila com `gcc -Wall -Wextra -std=c11 -O2`, sem gerar nenhum aviso, e produz o executável `scheduler`.

## Como executar
```
./scheduler rate <arquivo_de_entrada>
./scheduler edf <arquivo_de_entrada>
```
Gera, respectivamente, `rate_vgjda2.out` ou `edf_vgjda2.out` no diretório atual. Nada é impresso em stdout durante a execução normal.

## Como limpar
```
make clean
```

## Como testar
1. **Caminho feliz**: crie o arquivo de exemplo do próprio enunciado e rode os dois algoritmos:
   ```
   printf "100\nATT 20 12 8\nNAV 50 30 15\n" > voo.txt
   ./scheduler rate voo.txt
   ./scheduler edf voo.txt
   ```
   Compare `rate_vgjda2.out` com o exemplo `EXECUTION BY RATE` do enunciado — devem ser idênticos.

2. **Casos de erro** (todos devem retornar código de saída ≠ 0, escrever mensagem em `stderr` e **não** gerar `.out`):
   - número incorreto de argumentos: `./scheduler rate`
   - algoritmo inválido: `./scheduler foo voo.txt`
   - arquivo inexistente: `./scheduler rate nao_existe.txt`
   - arquivo malformado (campo faltando ou valor não numérico/não positivo)
   - tarefa violando `C <= D <= P`

3. **Trace de depuração (opcional)**: compile com a flag extra `-DDEBUG_TRACE` para imprimir em `stderr`, instante a instante, qual tarefa executou e as estatísticas finais de cada tarefa — não interfere na saída padrão exigida pelo enunciado:
   ```
   gcc -Wall -Wextra -std=c11 -O2 -DDEBUG_TRACE -o scheduler scheduler.c
   ```

## Observações de implementação
- O login usado para nomear os arquivos de saída está definido como macro (`LOGIN "vgjda2"`) no topo de `scheduler.c`.
- A checagem de perda de deadline é feita **antes** da chegada de uma nova instância, no mesmo instante `t`, para tratar corretamente o caso em que `D == P`.
- `concluiu_em` e `perdeu_em` são vetores de bitmask (um bit por tarefa) alocados com `calloc`, usados para decidir se cada bloco de execução termina com motivo `F` (terminou), `L` (perdeu o prazo) ou `H` (foi interrompido / cortado pelo fim da simulação).
 
