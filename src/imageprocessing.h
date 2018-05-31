#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <FreeImage.h>

typedef struct {
  unsigned int width, height;
  float *r, *g, *b;
} imagem;

imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo, imagem *I);
void liberar_imagem(imagem *i);
void apply_blur(imagem *img, int N, float Area,unsigned int i, unsigned int j, imagem *output);/*Applies blur filter on pixel specified by i and j of area N*/

#endif
