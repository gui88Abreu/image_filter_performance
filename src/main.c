#include <string.h>
#include "imageprocessing.h"


int main(int argc, char *argv[]){
  imagem img;
  unsigned int tmp;
  char output[50] = "filtered_images/";
  char input[50] = "images/";
  float alpha = 0.998;

  strcat(input, argv[1]);
  strcat(output, argv[1]);
  img = abrir_imagem(input);
  for (unsigned int i=0; i<(img.width); i++) {
    for (unsigned int j=0; j<(img.height); j++) {
      /* Ganho no canal R */
      tmp = img.r[j*img.width + i] * 2;
      if (tmp > 255) tmp=255;
      img.r[j*img.width + i] = tmp;

      /* Reducao no canal B */
      img.b[j*img.width + i] /= 2;

      /* Blur exponencial no canal G */
      if (i!=0) {
        img.g[j*img.width + i] = (1-alpha)*img.g[j*img.width + i] +(alpha)* img.g[j*img.width + i -1];
      }

    }
  }

  salvar_imagem(output, &img);
  liberar_imagem(&img);
  return 0;
}
