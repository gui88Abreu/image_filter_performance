#include <stdio.h>
#include "imageprocessing.h"

void mean_value(imagem img, int N, unsigned int i, unsigned int j, imagem *output){
  int Yi = i-N, Yf = i+N;
  int Xi = j-N, Xf = j+N;
  float r, g , b;

  if (Xi < 0) Xi = 0;
  if (Yi < 0) Yi = 0;

  if (Xf > (int)img.width) Xf = img.width-1;
  if (Yf > (int)img.height) Yf = img.height-1;

  r = 0.0, g = 0.0, b = 0.0;
  for (int y = Yi; y <= Yf; y++){
    for (int x = Xi; x <= Xf; x++){
      r += img.r[y*img.width + x];
      g += img.g[y*img.width + x];
      b += img.b[y*img.width + x];
    }
  }
  if (N == 0) N = 1;
  output->r[i*output->width + j] = r/(float)(N*N);
  output->g[i*output->width + j] = g/(float)(N*N);
  output->b[i*output->width + j] = b/(float)(N*N);
  return; 
}