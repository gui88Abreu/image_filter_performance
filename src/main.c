#include <string.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>

#include "imageprocessing.h"
#include "thread.h"
#include "process.h"

#define N 10
#define N_THREADS 4
#define N_PROCESS 4

void initialize_img(imagem *, unsigned int, unsigned int);/*Set up output image*/
void threading_method(imagem *, imagem *, long int*, long int*);/*Execute Threading Method*/
void process_method(imagem *, char *, long int*, long int*); /*Execute Process Method*/

int main(int argc, char *argv[]){
  imagem img, output_img, *img2;
  char output[50] = "filtered_images/";
  char input[50] = "images/";

  int width, height;
  long int stime, utime;

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

  /*Adjust the image destiny*/
  strcat(input, argv[1]);
  strcat(output, argv[1]);

  /*Open image and initialize output image*/
  img = abrir_imagem(input);
  initialize_img(&output_img, img.width, img.height);

  width = img.width, height = img.height; /*Image Dimension*/
  
  /*Execute Blur filter with Threads*/
  if (strcmp(argv[2], "0") == 0){
    threading_method(&img, &output_img, &stime, &utime);
    salvar_imagem(output, &output_img);
  }
  /*Execute Blur filter with Process*/
  else if (strcmp(argv[2], "1") == 0){
    process_method(&img, output, &stime, &utime);
  }
  else{
    printf("The third argument is not a valid option\n");
    exit(EXIT_SUCCESS);
  }

  /*Free allocated memory*/
  liberar_imagem(&img);
  liberar_imagem(&output_img);

  printf("%ix%i,%ld.%ld\n", width, height, stime, utime);  
  return 0;
}

void threading_method(imagem *img, imagem *output_img, long int *stime, long int *utime){
  Buffer buffer;
  pthread_t thread[N_THREADS];
  char *tasks;
  float Area;

  struct timeval rt0, rt1, drt;/*They will get real time*/

  /*Get time before the beginning of the processing*/
  gettimeofday(&rt0, NULL);

  /*Task matrix*/
  tasks = (char *)calloc(img->width*img->height, sizeof(char));
  
  /*Calculate area where will be applied blur filter*/
  Area = 2*N + 1;
  Area *= Area;

  /*Put data on the buffer*/
  buffer.input = img;
  buffer.output = output_img;
  buffer.N_blur = N;
  buffer.Area = Area;
  buffer.pixel = tasks;


  /*Boot up Thread Workers*/
  for (int i = 0; i < N_THREADS; i++) {
    pthread_create(&(thread[i]), NULL, worker, &(buffer));
  }

  /* Wait for threads execution*/
  for(int i = 0; i < N_THREADS; i++){
    pthread_join(thread[i], NULL);
  }

  free(tasks);

  /*Get the time after the end of the whole processing*/
  gettimeofday(&rt1, NULL);

  timersub(&rt1, &rt0, &drt);

  *stime = drt.tv_sec,*utime = drt.tv_usec;

  return;
}

void process_method(imagem *img, char *output_file, long int *stime, long int *utime){
  int segment_sem,shared;
  sem_t *sem;
  pid_t pid[N_PROCESS];
  float Area;

  struct timeval rt0, rt1, drt;/*They will get real time*/

  /*Get time before the beginning of the processing*/
  gettimeofday(&rt0, NULL);

  /*Calculate area where will be applied blur filter*/
  Area = 2*N + 1;
  Area *= Area;

  /*Define flags of protection and visibility memory*/
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  /*Create Shared Memory Area*/
  char *tasks = (char *)mmap(NULL, img->width*img->height*sizeof(int), protection, visibility, 0, 0);
  imagem *output_img = (imagem *)mmap(NULL, sizeof(imagem), protection, visibility, 0, 0);
  output_img->width = img->width, output_img->height = img->height;
  
  /*Create Shared Memory Area for output image*/
  output_img->r = (float*)mmap(NULL, sizeof(float)*img->width*img->height, protection, visibility, 0, 0);
  output_img->g = (float*)mmap(NULL, sizeof(float)*img->width*img->height, protection, visibility, 0, 0);
  output_img->b = (float*)mmap(NULL, sizeof(float)*img->width*img->height, protection, visibility, 0, 0);

  /*Reset tasks*/
  for(unsigned int i = 0; i < img->height; i++){
    for(unsigned int j = 0; j < img->width; j++){
      tasks[i*img->width + j] = 0;
    }
  }
  
  /*Create semaphore to control Memory Critical Area*/
  segment_sem = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | SHM_W | SHM_R);
  sem = (sem_t *)shmat(segment_sem, NULL, 0);
  sem_init(sem, 1, 1); // initialize semaphore
  
  /*Create N_PROCESS processes*/
  for( int k = 0; k < N_PROCESS; k++){
    pid[k] = fork();

    if(pid[k] == 0){
      /*Call Function that make the processing*/
      processing_worker(img, output_img, tasks, sem, img->width, img->height, N, Area);
      /*Terminate This Process*/
      exit(EXIT_SUCCESS);
    }
  }
  
  // processo pai
  for(int k = 0; k < N_PROCESS; k++){// espera processos acabarem
    waitpid(pid[k], NULL, 0);
    sem_destroy(sem);
  }
  shmdt(sem);

  /*Get the time after the end of the whole processing*/
  gettimeofday(&rt1, NULL);

  timersub(&rt1, &rt0, &drt);

  *stime = drt.tv_sec,*utime = drt.tv_usec;

  salvar_imagem(output_file, output_img);
  return;
}

void initialize_img(imagem *img, unsigned int width, unsigned int height){
  img->width = width;
  img->height = height;
  img->r = (float*)malloc(sizeof(float) * width * height);
  img->g = (float*)malloc(sizeof(float) * width * height);
  img->b = (float*)malloc(sizeof(float) * width * height);
}
