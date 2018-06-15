/*Apply blur filter without parallelism*/

#include "single_worker.h"

void single_worker(imagem *img, imagem *output_img, int width, int height, int N, long int *stime, long int *utime){
  struct timeval rt0, rt1, drt;/*They will get real time*/

  /*Get time before the beginning of the processing*/
  gettimeofday(&rt0, NULL);
  
  float Area = 2*N + 1;
  Area *= Area;
  for (int i = 0; i < height; i++)
    for (int j = 0; j< width; j++)
      apply_blur(img, N, Area, i, j, output_img);

  /*Get the time after the end of the whole processing*/
  gettimeofday(&rt1, NULL);

  timersub(&rt1, &rt0, &drt);

  *stime = drt.tv_sec,*utime = drt.tv_usec;
  return;
}