//
// Created by Igor on 24/02/25.
//

#include"dado_estacao.h"

dados_estacao_t * dados_clone(dados_estacao_t * original) {
    g_alloc(novo, dados_estacao_t);
    *novo = * original;
    return novo;
}

gconf_t gconf_dados_estacao() {
    gconf_t conf = {.f_comp=NULL, .f_clone = (Fclone)dados_clone, .f_destroi=(Fdestroi)free, .f_size=NULL, .f_hash=NULL};

    return conf;
}