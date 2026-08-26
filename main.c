#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

float calculo_mandelbrot(float largura, float altura, long max_iter, long threads){
    printf("%f %f %ld %ld\n", largura, altura, max_iter, threads);
    return 2.0;
}

void processo_pthread(float basico){
    FILE *file = fopen("mandelbrot_dsob_pthreads1.pgm", "w");

    fprintf(file, "%f", basico);
    return;
}

void processo_openmp(float basico){
    FILE *file = fopen("mandelbrot_dsob_openmp.pgm", "w");
    
    fprintf(file, "%f", basico);
    return;
}

void processo_serial(float basico){
    FILE *file = fopen("mandelbrot_dsob_serial.pgm", "w");

    fprintf(file, "%f", basico);
    return;
}

int main(int argc, char **argv){
    if (argc < 5){
        printf("Menos argumentos do que requisitado");
        return 1;
    }

    char *receber;
    float largura = strtof(argv[1],&receber);
    float altura = strtof(argv[2],&receber);
    long max_iter = strtol(argv[3],&receber, 10);
    long threads = strtol(argv[4],&receber, 10);

    float basico = calculo_mandelbrot(largura, altura, max_iter, threads);

    processo_serial(basico);
    processo_openmp(basico);
    processo_pthread(basico);

    return 0;
}