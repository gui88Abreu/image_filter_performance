#ifndef _PROCESS_H
#define _PROCESS_H

#include <unistd.h>
#include <semaphore.h>

#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h> /*waitpid*/
#include <sys/mman.h> /* memory map*/

#include "imageprocessing.h"

void processing_worker(imagem *img, imagem *output_img, char *tasks, sem_t *sem, int width, int height, int N, float Area); /*Do the image processing as an independent process*/

#endif