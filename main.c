#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>
#include <time.h>

#define REAL_MAX 2.0
#define REAL_MIN -2.0
#define IMAG_MAX 1.5
#define IMAG_MIN -1.5

typedef struct{
    long largura;
    long altura;
    long max_iter;
    long *matriz_iter;
} args_pthread;

void *processo_pthread1(void *arg){
    args_pthread *a = (args_pthread *)arg;
    long largura = a->largura;
    long altura = a->altura;
    long max_iter = a->max_iter;
    long *matriz_iter = a->matriz_iter;

    pthread_t id = pthread_self();

    printf("Thread %lu Altura %ld; largura %ld; Maxima Iterações %ld\n", (unsigned long)id, altura, largura, max_iter);

    return NULL;
}

void print_pthread1(long *matriz, long altura, long largura, long max_iter){
    FILE *file = fopen("mandelbrot_dsob_pthreads1.pgm", "w");
    if (file == NULL){
        printf("Erro ao abrir o Arquivo");
        return;
    }

    for (long i = 0; i < altura; i++) {
        for (long y = 0; y < largura; y++) {
            long inten = (matriz[i * largura + y] * 255) / max_iter;
            fprintf(file, "%ld ", inten);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return;
}

void processo_pthread2(float basico){
    FILE *file = fopen("mandelbrot_dsob_pthreads2.pgm", "w");
    if (file == NULL){
        printf("Erro ao abrir o Arquivo");
        return;
    }

    fprintf(file, "%f", basico);
    return;
}

void processo_openmp(long largura,long altura,long max_iter, long threads, long *matriz_iter){
    FILE *file = fopen("mandelbrot_dsob_openmp.pgm", "w");
    if (file == NULL){
        printf("Erro ao abrir o Arquivo");
        return;
    }

    #pragma omp parallel for num_threads(threads)
    for (long i=0; i<altura; i++){
        for(long y=0; y<largura; y++){
            float real = REAL_MIN + ((float)y / (float)largura) * (REAL_MAX - REAL_MIN);
            float imaginario = IMAG_MIN + ((float)i / (float)altura) * (IMAG_MAX - IMAG_MIN);

            float z_real = 0.0, z_imag = 0.0;
            long iter_atual = 0;

            while (z_real * z_real + z_imag * z_imag <= 4.0 && iter_atual < max_iter) {
                float z_real_temp = z_real * z_real - z_imag* z_imag + real;
                z_imag = 2.0 * z_real * z_imag + imaginario;
                z_real = z_real_temp;
                iter_atual++;
            }

            matriz_iter[i * largura + y] = iter_atual;
        }
    }
    for (long i = 0; i < altura; i++) {
        for (long y = 0; y < largura; y++) {
            long inten = (matriz_iter[i * largura + y] * 255) / max_iter;
            fprintf(file, "%ld ", inten);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    return;
}

void processo_serial(long largura,long altura,long max_iter, long *matriz_iter){
    FILE *file = fopen("mandelbrot_dsob_serial.pgm", "w");
    if (file == NULL){
        printf("Erro ao abrir o Arquivo");
        return;
    }

    for(long i=0; i<altura; i++){
        for(long y=0; y<largura; y++){
            float real = REAL_MIN + ((float)y / (float)largura) * (REAL_MAX - REAL_MIN);
            float imaginario = IMAG_MIN + ((float)i / (float)altura) * (IMAG_MAX - IMAG_MIN);

            float z_real = 0.0, z_imag = 0.0;
            long iter_atual = 0;

            while (z_real * z_real + z_imag * z_imag <= 4.0 && iter_atual < max_iter) {
                float z_real_temp = z_real * z_real - z_imag* z_imag + real;
                z_imag = 2.0 * z_real * z_imag + imaginario;
                z_real = z_real_temp;
                iter_atual++;
            }

            matriz_iter[i * largura + y] = iter_atual;

        }
    }
    for (long i = 0; i < altura; i++) {
        for (long y = 0; y < largura; y++) {
            long inten = (matriz_iter[i * largura + y] * 255) / max_iter;
            fprintf(file, "%ld ", inten);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    return;
}

void anotar_timer(char nome[16]){
    FILE *tFile = fopen("timer.txt", "w");
    if (tFile == NULL){
        printf("Erro ao abrir o Arquivo");
        return;
    }

    fclose(tFile);
    return;
}

int main(int argc, char **argv){
    if (argc < 5){
        printf("Menos argumentos do que requisitado");
        return 1;
    }
    args_pthread a1;

    char *receber;
    float largura = strtof(argv[1],&receber);
    float altura = strtof(argv[2],&receber);
    long max_iter = strtol(argv[3],&receber, 10);
    long threads = strtol(argv[4],&receber, 10);
    long *matriz_iter = malloc(largura*altura*sizeof(long));

    a1.largura = largura;
    a1.altura = altura;
    a1.max_iter = max_iter;
    a1.matriz_iter = matriz_iter;

    pthread_t thread[threads];

    processo_serial(largura, altura, max_iter, matriz_iter);
    processo_openmp(largura, altura, max_iter, threads, matriz_iter);

    for(int i=0; i<threads; i++){
        pthread_create(&thread[i], NULL, processo_pthread1, &a1);
    }
    for(int i=0; i<threads; i++){
        pthread_join(thread[i], NULL);
    }
    free(matriz_iter);
    return 0;
}