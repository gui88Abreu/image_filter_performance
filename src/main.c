#include <string.h>
#include <stdlib.h>
#include "imageprocessing.h"
#include "thread.h"

#define N 10
#define N_THREADS 4

void initialize_img(imagem *, unsigned int, unsigned int);
void threading_method(imagem *, imagem *);

int main(int argc, char *argv[]){
  imagem img, output_img;
  char output[50] = "filtered_images/";
  char input[50] = "images/";

  if (argc < 3){
    printf("-----------------------------\n");
    printf("PROGRAM NEEDS MORE ARGUMENTS \n");
    printf("THE THIRD ARGUMENT NEEDS TO  \n");
    printf("BE A NUMBER:                 \n");
    printf("0 FOR THREADING METHOD       \n"); 
    printf("1 FOR PROCESS METHOD         \n");
    printf("-----------------------------\n");
    exit(EXIT_SUCCESS);
  }

  strcat(input, argv[1]);
  strcat(output, argv[1]);

  img = abrir_imagem(input);
  initialize_img(&output_img, img.width, img.height);

  if (strcmp(argv[2], "0") == 0)
    threading_method(&img, &output_img);

  salvar_imagem(output, &output_img);
  liberar_imagem(&img);
  liberar_imagem(&output_img);
  return 0;
}

void threading_method(imagem *img, imagem *output_img){
  Buffer buffer[N_THREADS];
  pthread_t thread[N_THREADS];
  char *tasks;

  tasks = (char *)calloc(img->width*img->height, sizeof(char));
  for (int i = 0; i < N_THREADS; i++) {
    buffer[i].input = img;
    buffer[i].output = output_img;
    buffer[i].N_blur = N;
    buffer[i].pixel = tasks;
    pthread_create(&(thread[i]), NULL, worker, &(buffer[i]));
  }

  /* Espera pelo fim das N_THREADS threads que estao sendo executadas*/
  for(int i = 0; i < N_THREADS; i++){
    pthread_join(thread[i], NULL);
  }

  return;
}

void initialize_img(imagem *img, unsigned int width, unsigned int height){
  img->width = width;
  img->height = height;
  img->r = (float*)malloc(sizeof(float) * width * height);
  img->g = (float*)malloc(sizeof(float) * width * height);
  img->b = (float*)malloc(sizeof(float) * width * height);
}