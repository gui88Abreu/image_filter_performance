#include "thread.h"

void *worker(void *arg){
  bf buffer = (bf)arg;
  int i = 0, j = 0, cont = 0;
  int width = buffer->input->width, height = buffer->input->height;

  while(1){
    /* Primeira area critica de memoria: Encontra uma tarefa ainda nao realizada*/
    pthread_mutex_lock(&key);
    while(i < height){
      while(j < width && buffer->pixel[i*width + j] == 1)
        j+=1;
      if (buffer->pixel[i*width + j] == 0)
        break;
      i+=1;
    }
    
    /* encerra thread caso nao haja mais tarefas a ser realizadas*/
    if (i >= height && j >= width){
      pthread_mutex_unlock(&key);
      break;
    }
    
    /* marca tarefa como realizada*/
    buffer->pixel[i*width + j] = 1;
    pthread_mutex_unlock(&key);

    /* realiza tarefa*/
    mean_value(buffer->input, buffer->N_blur, i, j, buffer->output);
    if (j == width)
      j = 0;
  }

  return NULL;
}