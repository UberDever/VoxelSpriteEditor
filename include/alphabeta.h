//
// Created by uberdever on 27.07.2020.
//
// Alphabeta is core of engine, that assembles all tools together.
// Contains init and clean functions
#ifndef ALPHABETA_ALPHABETA_H
#define ALPHABETA_ALPHABETA_H

#include "alpha.h"
#include "delta.h"
#include "gamma.h"
#include "pi.h"
#include "omega.h"

typedef struct gamma_info_t {
    uint w, h;
    bool is_fullscreen;
    uint win_flags, ren_flags;
} gamma_info_t;

void alphabeta_init(gamma_info_t scr_info);
void alphabeta_clean();

#endif //ALPHABETA_ALPHABETA_H
