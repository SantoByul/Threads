#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

void calculo_mandelbrot(float largura, float altura, long max_iter, long threads){
    printf("%f %f %ld %ld\n", largura, altura, max_iter, threads);
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

    calculo_mandelbrot(largura, altura, max_iter, threads);
    return 0;
}