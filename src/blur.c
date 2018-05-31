#include <stdio.h>
#include "imageprocessing.h"

void apply_blur(imagem *img, int N, float Area,unsigned int i, unsigned int j, imagem *output){
  int Yi = i-N, Yf = i+N;
  int Xi = j-N, Xf = j+N;
  float r, g , b;

  /*Define Blur Area*/
  if (Xi < 0) Xi = 0;
  if (Yi < 0) Yi = 0;
  if (Xf > (int)img->width) Xf = img->width-1;
  if (Yf > (int)img->height) Yf = img->height-1;

  /*Add pixel values on Blur Area*/
  r = 0.0, g = 0.0, b = 0.0;
  for (int y = Yi; y <= Yf; y++){
    for (int x = Xi; x <= Xf; x++){
      r += img->r[y*img->width + x];
      g += img->g[y*img->width + x];
      b += img->b[y*img->width + x];
    }
  }

  /*Store new values on output image*/
  output->r[i*output->width + j] = r/Area;
  output->g[i*output->width + j] = g/Area;
  output->b[i*output->width + j] = b/Area;
  return; 
}