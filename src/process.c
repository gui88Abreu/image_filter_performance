#include "process.h"

void processing_worker(imagem *img, imagem *output_img, char *tasks, sem_t *sem, int width, int height, int N, float Area){
  int i = 0, j = 0;
  
  while(1){
    /*Memory Critical Area: Find a task not accomplished*/
    sem_wait(sem);
    while(i < height){
      while(j < width && tasks[i*width + j] == 1)
        j+=1;
      if (tasks[i*width + j] == 0)
        break;
      i+=1;
    }
    
    /* There is no more task to be accomplished*/
    if (i >= height && j >= width){
      sem_post(sem);
      break;
    }

    /* Mark task as accomplished*/
    tasks[i*width + j] = 1;
    
    /*Here ends the Memory Critical Area*/
    sem_post(sem);

    /* Accomplish task*/
    apply_blur(img, N, Area, i, j, output_img);
    if (j == width)
      j = 0;
  }

  return;
}