#ifndef _SINGLE_WORKER_H
#define _SINGLE_WORKER_H

#include "imageprocessing.h"
#include <time.h>
#include <sys/time.h>

void single_worker(imagem *img, imagem *output_img, int width, int height, int N, long int *stime, long int *utime); /*Do the image processing as an independent process*/

#endif