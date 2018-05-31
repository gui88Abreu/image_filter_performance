#include "thread.h"

void *worker(void *arg){
  bf buffer = (bf)arg;
  int i = 0, j = 0, cont = 0;
  int width = buffer->input->width, height = buffer->input->height;

  while(1){
    /*Memory critical area: Find a task not accomplished*/
    pthread_mutex_lock(&key);
    while(i < height){
      while(j < width && buffer->pixel[i*width + j] == 1)
        j+=1;
      if (buffer->pixel[i*width + j] == 0)
        break;
      i+=1;
    }
    
    /* There is no more task to be accomplished*/
    if (i >= height && j >= width){
      pthread_mutex_unlock(&key);
      break;
    }

    /* Mark task as accomplished*/
    buffer->pixel[i*width + j] = 1;
    
    /*Here ends the Memory critical area*/
    pthread_mutex_unlock(&key);

    /* Accomplish task*/
    apply_blur(buffer->input, buffer->N_blur, buffer->Area, i, j, buffer->output);
    
    /* j must be assigned with 0 if it has transpassed the bound*/
    if (j == width)
      j = 0;
  }

  return NULL;
}