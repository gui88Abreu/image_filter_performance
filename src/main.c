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
void threading_method(imagem *, imagem *);/*Execute Threading Method*/
void process_method(imagem *, imagem*); /*Execute Process Method*/

int main(int argc, char *argv[]){
  imagem img, output_img;
  char output[50] = "filtered_images/";
  char input[50] = "images/";

  clock_t t_0, t; /*It will get usr time*/
  struct timeval rt0, rt1, drt;/*They will get real time*/

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

  strcat(input, argv[1]);
  strcat(output, argv[1]);

  img = abrir_imagem(input);
  initialize_img(&output_img, img.width, img.height);

  gettimeofday(&rt0, NULL);
  t_0 = clock();
  
  /*Execute Blur filter with Threads*/
  if (strcmp(argv[2], "0") == 0)
    threading_method(&img, &output_img);
  else if (strcmp(argv[2], "1") == 0)
    process_method(&img, &output_img);
  else{
    printf("That is not an option\n");
    exit(EXIT_SUCCESS);
  }
  t = clock();
  gettimeofday(&rt1, NULL);

  salvar_imagem(output, &output_img);
  liberar_imagem(&img);
  liberar_imagem(&output_img);

  timersub(&rt1, &rt0, &drt);
  printf("%f,%ld.%ld\n", (double)(t - t_0)/CLOCKS_PER_SEC, drt.tv_sec, drt.tv_usec);  
  return 0;
}

void threading_method(imagem *img, imagem *output_img){
  Buffer buffer[N_THREADS];
  pthread_t thread[N_THREADS];
  char *tasks;

  /*Task matrix*/
  tasks = (char *)calloc(img->width*img->height, sizeof(char));
  
  /*Boot up Thread Workers*/
  for (int i = 0; i < N_THREADS; i++) {
    buffer[i].input = img;
    buffer[i].output = output_img;
    buffer[i].N_blur = N;
    buffer[i].pixel = tasks;
    pthread_create(&(thread[i]), NULL, worker, &(buffer[i]));
  }

  /* Wait for threads execution*/
  for(int i = 0; i < N_THREADS; i++){
    pthread_join(thread[i], NULL);
  }

  free(tasks);
  return;
}

void process_method(imagem *img, imagem *output_img){
  // criando variaveis 
  int segment_sem,shared;
  sem_t *sem;
  pid_t pid[N_PROCESS];
  
  // task matrix
  char *tasks;

  /* Defini flags de protecao e visibilidade de memoria */
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  /* Cria area de memoria compartilhada */
  tasks = (char *)mmap(NULL, img->width*img->height*sizeof(int), protection, visibility, 0, 0);

  /*Reset tasks*/
  for(int i = 0; i< img->height; i++){
    for(int j = 0; j< img->width; j++){
      tasks[i*img->width + j] = 0;
    }
  }
  
  segment_sem = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | SHM_W | SHM_R);// cria semaforo compartilhado pelos processos
  sem = (sem_t *)shmat(segment_sem, NULL, 0); // address para variavel
  sem_init(sem, 1, 1); // inicializa semaphore compartilhado em multiprocessos e com valor inicial 1
  
  /*
  shared = shmget(IPC_PRIVATE, sizeof(Buffer), IPC_CREAT | SHM_W | SHM_R);// cria variavel compartilhado pelos processos
  buf = (bf )shmat(shared, NULL, 0); // address para variavel*/

  for( int k = 0; k <N_PROCESS;k++){// criando 4 processos
    pid[k] = fork();
    if(pid[k] == 0){// esta no processo filho
      int i = 0, j = 0, cont =0;
      int width = img->width, height = img->height;
      
      while(1){
      /*Memory critical area: Find a task not accomplished*/
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

        cont += 1;
        /* Mark task as accomplished*/
        tasks[i*width + j] = 1;
        sem_post(sem);

        /* Accomplish task*/
        apply_blur(img, N, i, j, output_img);
        if (j == width)
          j = 0;
      }

      printf("cont = %i\n", cont);
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
  return;
}

void initialize_img(imagem *img, unsigned int width, unsigned int height){
  img->width = width;
  img->height = height;
  img->r = (float*)malloc(sizeof(float) * width * height);
  img->g = (float*)malloc(sizeof(float) * width * height);
  img->b = (float*)malloc(sizeof(float) * width * height);
}