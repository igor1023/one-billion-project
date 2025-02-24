//
// Created by Igor on 24/02/25.
//

#ifndef DADO_ESTACAO_H
#define DADO_ESTACAO_H
#include<libprg/libprg.h>

typedef struct {

    double min, med, max;
    int ocorrencia;

} dados_estacao_t;

dados_estacao_t * dados_clone(dados_estacao_t * original);
gconf_t gconf_dados_estacao();

#endif //DADO_ESTACAO_H
