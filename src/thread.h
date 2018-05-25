#ifndef _THREAD_H
#define _THREAD_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "imageprocessing.h"

typedef struct{
  imagem *input;
  imagem *output;
  char *pixel;
  int N_blur;
}Buffer, *bf;

pthread_mutex_t key; /*chave utilizada para travar as threads quando for necessario*/

void *worker(void *);/*Thread function that actually executes blur filter*/

#endif