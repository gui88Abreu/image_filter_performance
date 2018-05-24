
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
void aply_blur(imagem *img, int N, unsigned int i, unsigned int j, imagem *output);

#endif
