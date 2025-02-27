/*
 > Alunos: Igor da Silva, Gustavo Briance Mengue Mena e Tiago Cargnin Morandi.
 > Turma: 129002 - Programação II
 > Tema: Desafio de 1 bilhão de linhas.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libprg/libprg.h>
#include <time.h>
#include "dado_estacao.h"

#define MAX_LINHA 61
#define MAX_ESTACAO 51

//função para converter string para double
double string_2_double(const char * temp_str) {

    double resultado = 0.0;
    int i = 0;
    int sinal = 1;

    // > Vamos verificar se o numero é negativo.
    if (temp_str[i] == '-') {
        sinal = -1;
        i++; // Avança para o proximo caractere
    }

    // > Vamos verificar a parte inteira do numero
    while (temp_str[i] >= '0' && temp_str[i] <= '9') {
        // > Em str[i] - '0': suponha str[i] = '8'
        // > Se fizermos a subtração '8' - '0', isto é igual a 56 - 48 = 8

        resultado = (resultado * 10) + (temp_str[i] - '0');
        // > Vamos multiplicar o resultado por 10 para deslocar o número à esquerda.
        // > Pensamos no numero 123, como se o 1 fosse o bit mais significativo (neste caso, a centena).
        i++;
    }

    // > Vamos verificar a parte fracionaria
    if (temp_str[i] == '.') {
        i++; // > Avança de caractere (ignora o '.')
        double fator = 0.1; // > Fator multiplicativo para tratar casas decimais
        while (temp_str[i] >= '0' && temp_str[i] <= '9') {

            resultado += (temp_str[i] - '0') * fator;
            // > Como estamos analisando a parte fracionaria, temos que deslocar
            // os numeros para a direita. Por exemplo: 0.12:
            // - para o '1': converteríamos o '1' para decimal (1) e multiplicamos por fator (0.1),
            // depois o fator passa a ser 0.01.
            // - para o '2': converteríamos o '2' para decimal (2) e multiplicamos por fator (0.01),
            // sempre somamos este cálulo no resultado.
            fator /= 10.0;
            i++;
        }
    }
    return sinal * resultado;
}

int main() {

    FILE *arq = fopen("measurements.txt", "r");
    if (arq == NULL) {
        perror(" Ao abrir arquivo");
        return EXIT_FAILURE;
    }

    thash_t * tabela = thash_cria(gconf_dados_estacao());

    if (tabela == NULL) {
        perror(" Ao criar tabela hash");
        fclose(arq);
        return EXIT_FAILURE;
    }

    char * linha = malloc(sizeof(char) * MAX_LINHA);

    // > Vamos declarar as variaveis abaixo fora do while
    // para economizar tempo
    char * estacao = malloc(sizeof(char) * MAX_ESTACAO);

    // > Alocar memoria para linha e estacao apresentou mais rapidez
    // do que utilizá-las como strings (EX: char linha[MAX];)+
    if (estacao == NULL || linha == NULL) {
        perror(" Ao alocar string");
        fclose(arq);
        thash_destroi(tabela);
        return EXIT_FAILURE;
    }

    unsigned int len;
    char * separador, * temperatura_str;
    double temperatura_do;

    while (fgets(linha, MAX_LINHA, arq)) {

        separador = strchr(linha, ';');
        // > Calcula-se o tamanho da string que guardará a estacao
        len = separador - linha;

        strncpy(estacao, linha, len);
        estacao[len] = '\0';

        temperatura_str = separador + 1; //";NUMERO\0" => vai apontar somente para "NUMERO\0"
        temperatura_str[strlen(temperatura_str) - 1] = 0;
        // > Neste momento, tenho a estacao guardada em ESTACAO e a temperatura em TEMPERATURA_STR.
        // > Agora, tenho que converter a temperatura de STRING para DOUBLE

        temperatura_do = string_2_double(temperatura_str);

        // > Sabendo que thash_obtem retorna:
        // 1) o valor, se a chave existe
        // 2) ou NULL, caso contrario
        // não vamos utilizar a função thash_existe()...
        // Utilizaremos a função thash_obtem(): se for NULL, não existe...

        dados_estacao_t * ptr_dados = thash_obtem(tabela, estacao);
        if (ptr_dados != NULL) {
            // > Se já existe, atualizo os dados da estacao.
            if (ptr_dados->max < temperatura_do)
                ptr_dados->max = temperatura_do;
            else if (ptr_dados->min > temperatura_do)
                ptr_dados->min = temperatura_do;

            ptr_dados->med += temperatura_do;
            ptr_dados->ocorrencia++;
        }

        else {
            // > Se não existe, adicionamos na tabela e setamos ocorrencias como 1.
            dados_estacao_t novos_dados = {temperatura_do, temperatura_do, temperatura_do, 1};

            // > A declaração acima é o mesmo que as atribuiçoes abaixo
            // novos_dados.min = novos_dados.med = novos_dados.max = temperatura_do;
            // novos_dados.ocorrencia = 1;

            thash_adiciona(tabela, estacao, &novos_dados);
        }
    }

    free(linha);
    free(estacao);
    fclose(arq);

    puts("Tecle ENTER para imprimir as estações");
    getchar();

    // > Agora vamos imprimir
    thash_inicia_iteracao(tabela);
    par_t * par;
    while ((par = thash_proximo(tabela)) != NULL) {

        const dados_estacao_t * dados = (dados_estacao_t *) par->valor;
        printf("%s = %.2f / %.2f / %.2f\n", par->chave, dados->min, dados->med / dados-> ocorrencia, dados->max);

    }

    thash_destroi(tabela);

    return EXIT_SUCCESS;
}