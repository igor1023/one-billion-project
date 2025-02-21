#include <stdio.h>
// para usar a libprg, inclua librpg/libprg.h
#include <libprg/libprg.h>
#include<stdlib.h>

typedef struct {

  char * estacao;
  double min;
  double med;
  double max;
  int ocorrencia;

} Dados_temp_t;

#define MAX_ESTACAO 100

//Retorna o indice que contem a estacao ou -1 se não existe
int pos_vetor_dados_temp(Dados_temp_t ** dados, char * estacao) {

  int i = 0;
  while (dados[i] != NULL) {

    if (! strcmp(dados[i]->estacao, estacao))
      return i;

    i++;
  }

  return -1;
}

int main() {
  // comece aqui seu programa

  FILE * arq = fopen("measurements_light.txt", "r");

  if (arq == NULL) {
    perror(" ao abrir arquivo\n");
    return 1;
  }

  thash_t * tabela = thash_cria(gconf_string());

  if (tabela == NULL) {
    perror(" ao criar tabela\n");
    return 2;
  }

  Dados_temp_t ** dados = malloc(MAX_ESTACAO * sizeof(Dados_temp_t *));
  if (dados == NULL) {
    perror(" no sistema\n");
    return 3;
  }

  for (int j = 0; j < MAX_ESTACAO; j++) {

    dados[j] = malloc(sizeof(Dados_temp_t));
    if (dados[j] == NULL)
      return 4;

    dados[j]->min = 1000.0;
    dados[j]->med = 0.0;
    dados[j]->max = -1000.0;
    dados[j]->ocorrencia = 0;

  }
  int i = 0;
  char estacao[50];
  double temperatura;

  while (fscanf(arq, "%49[^;]%lf", estacao, &temperatura) != EOF) {

    if (thash_existe(tabela, estacao)) {

      const int pos = pos_vetor_dados_temp(dados, estacao);
      if (temperatura < dados[pos]->min)
        dados[pos]->min = temperatura;

      if (temperatura > dados[pos]->max)
        dados[pos]->max = temperatura;

      dados[pos]->med += temperatura;
      dados[pos]->ocorrencia++;

      //converter os valores de temperatura DOUBLE para STRING
      char temp_min[10], temp_max[10], temp_med[10];
      char temp_str[15]; //será o valor a ser atualizado na tabela

      sprintf(temp_min, "%.1f;", dados[pos]->min);
      sprintf(temp_med, "%.1f;", dados[pos]->med / dados[pos]->ocorrencia);
      sprintf(temp_max, "%.1f", dados[pos]->max);

      strcpy(temp_str, temp_min);
      strcat(temp_str, temp_med);
      strcat(temp_str, temp_max);

      thash_adiciona(tabela, estacao, temp_str);
    }

    else {

      dados[i]->estacao = strdup(estacao);
      if (temperatura < dados[i]->min)
        dados[i]->min = temperatura;

      if (temperatura > dados[i]->min)
        dados[i]->max = temperatura;

      dados[i]->med += temperatura;
      dados[i]->ocorrencia = 1;
    }

  }

  fclose(arq);

  thash_inicia_iteracao(tabela);
  par_t * par;

  while ((par = thash_proximo(tabela)) != NULL) {

    char * chave = par->chave;
    char * valor = par->valor;

    printf("> %s = %s\n", chave, valor);

  }

  for (int j = 0; j < MAX_ESTACAO; j++) {
    free(dados[j]);
  }
  free(dados);
  thash_destroi(tabela);

  return 0;
}