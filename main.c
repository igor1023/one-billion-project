#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libprg/libprg.h>
#include <time.h>
#include "dado_estacao.h"

#define MAX_LINHA 100

int main() {

    FILE *arq = fopen("measurements_light.txt", "r");
    if (arq == NULL) {
        perror(" Ao abrir arquivo");
        return EXIT_FAILURE;
    }

    thash_t * tabela = thash_cria(gconf_dados_estacao());

    if (tabela == NULL) {
        perror(" Ao criar tabela hash");
        fclose(arq);
        thash_destroi(tabela);
        return EXIT_FAILURE;
    }

    char linha[MAX_LINHA];
    const long int i = time(NULL); //para cronometrar o tempo de operacao do programa

    while (fgets(linha, MAX_LINHA, arq)) {

        linha[strcspn(linha, "\n")] = '\0';  // Remover o '\n' do final da linha

        char * separador = strchr(linha, ';');
        char * nome_estacao = linha;
        const char * temperatura_str = separador + 1; //";NUMERO" => vai apontar somente para "NUMERO"

        //neste momento, tenho a estacao guardada em NOME_ESTACAO e a temperatura em TEMPERATURA
        //agora, tenho que converter a temperatura de STRING para DOUBLE

        const double temperatura_do = strtod(temperatura_str, NULL);

        //sabendo que thash_obtem retorna:
        // 1) o valor, se a chave existe
        // 2) ou NULL, caso contrario
        //Não vou utilizar a funcao thash_existe...

        dados_estacao_t * dados = thash_obtem(tabela, nome_estacao);
        if (dados != NULL) {

            //se já existe, atualizo os dados da estacao
            if (dados->max < temperatura_do)
                dados->max = temperatura_do;
            if (dados->min > temperatura_do)
                dados->min = temperatura_do;

            dados->med += temperatura_do;
            dados->ocorrencia++;

        }

        else {

            //se não existe, adiciona na tabela e seto ocorrencias como 1
            dados = malloc(sizeof(dados_estacao_t));
            if (dados == NULL) {
                perror(" ao alocar memoria");
                fclose(arq);
                thash_destroi(tabela);
                return EXIT_FAILURE;
            }

            dados->min = dados->med = dados->max = temperatura_do;
            dados->ocorrencia = 1;
            thash_adiciona(tabela, nome_estacao, &dados);

        }

    }

    fclose(arq);
    thash_destroi(tabela);

    const long int f = time(NULL); //marca o fim do cronometro
    printf("sai do loop: %ld\n", f - i);

    return EXIT_SUCCESS;
}