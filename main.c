#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libprg/libprg.h>
#include <time.h>
#include "dado_estacao.h"

#define MAX_LINHA 61
#define MAX_ESTACAO 51

//função para converter string para double (manual)
double string_2_double(const char * temp_str) {

    double resultado = 0.0;
    int i = 0;
    int sinal = 1;

    //vamos verificar se o numero é negativo
    if (temp_str[i] == '-') {
        sinal = -1;
        i++; //avança para o proximo caractere
    }

    //vamos verificar a parte inteira do numero
    while (temp_str[i] >= '0' && temp_str[i] <= '9') {
        //em str[i] - '0': suponha str[i] = '8'
        //se fizermos a subtração '8' - '0', isto é igual a 56 - 48 = 8

        resultado = (resultado * 10) + (temp_str[i] - '0');
        //vamos multiplicar o resultado por 10 para deslocar o número à esquerda
        //pensamos no numero 123, como se o 1 fosse o bit mais significativo (neste caso, a centena)
        i++;
    }

    //vamos verificar a parte fracionaria
    if (temp_str[i] == '.') {
        i++; //avança de caractere (ignora o '.')
        double fator = 0.1;
        while (temp_str[i] >= '0' && temp_str[i] <= '9') {

            resultado += (temp_str[i] - '0') * fator;
            //como estamos analisando a parte fracionaria, temos que deslocar
            //os numeros para a direita. Por exemplo: 0.12
            //para o '1': converteríamos o '1' para decimal (1) e multiplicamos por fator (0.1)
            //depois o factor passa a ser 0.01
            //para o '2': converteríamos o '2' para decimal (2) e multiplicamos por fator (0.01)
            //sempre somamos este cálulo no resultado (na parte fracionaria do resultado)
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

    //vou declarar as variaveis abaixo fora do while
    //para economizar tempo
    char * estacao = malloc(sizeof(char) * MAX_ESTACAO);
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
        len = separador - linha;

        strncpy(estacao, linha, len);
        estacao[len] = '\0';

        temperatura_str = separador + 1; //";NUMERO\0" => vai apontar somente para "NUMERO\0"
        temperatura_str[strlen(temperatura_str) - 1] = 0;
        //neste momento, tenho a estacao guardada em ESTACAO e a temperatura em TEMPERATURA_STR
        //agora, tenho que converter a temperatura de STRING para DOUBLE

        temperatura_do = string_2_double(temperatura_str);

        // sabendo que thash_obtem retorna:
        // 1) o valor, se a chave existe
        // 2) ou NULL, caso contrario
        // Não vou utilizar a funcao thash_existe...
        // Utilizo a funcao OBTEM: se for NULL, nao existe...

        dados_estacao_t * ptr_dados = thash_obtem(tabela, estacao);
        if (ptr_dados != NULL) {
            //se já existe, atualizo os dados da estacao
            if (ptr_dados->max < temperatura_do)
                ptr_dados->max = temperatura_do;
            else if (ptr_dados->min > temperatura_do)
                ptr_dados->min = temperatura_do;

            ptr_dados->med += temperatura_do;
            ptr_dados->ocorrencia++;
        }

        else {
            //se não existe, adiciona na tabela e seto ocorrencias como 1
            dados_estacao_t novos_dados = {temperatura_do, temperatura_do, temperatura_do, 1};

            // a declaração acima é o mesmo que
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

    //agora vou imprimir
    thash_inicia_iteracao(tabela);
    par_t * par;
    while ((par = thash_proximo(tabela)) != NULL) {

        const dados_estacao_t * dados = (dados_estacao_t *) par->valor;
        printf("%s = %.2f / %.2f / %.2f\n", par->chave, dados->min, dados->med / dados-> ocorrencia, dados->max);

    }

    thash_destroi(tabela);

    return EXIT_SUCCESS;
}