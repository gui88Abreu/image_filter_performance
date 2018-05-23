#include <string.h>
#include <stdlib.h>
#include "imageprocessing.h"

#define N 10

void initialize_img(imagem *, unsigned int, unsigned int);

int main(int argc, char *argv[]){
  imagem img, output_img;
  char output[50] = "filtered_images/";
  char input[50] = "images/";

  strcat(input, argv[1]);
  strcat(output, argv[1]);

  img = abrir_imagem(input);
  initialize_img(&output_img, img.width, img.height);
  for (unsigned int i=0; i < img.height; i++) {
    for (unsigned int j=0; j < img.width; j++) {
      mean_value(img, N, i, j, &output_img);
    }
  }

  salvar_imagem(output, &output_img);
  liberar_imagem(&img);
  liberar_imagem(&output_img);
  return 0;
}

void initialize_img(imagem *img, unsigned int width, unsigned int height){
  img->width = width;
  img->height = height;
  img->r = (float*)malloc(sizeof(float) * width * height);
  img->g = (float*)malloc(sizeof(float) * width * height);
  img->b = (float*)malloc(sizeof(float) * width * height);
}