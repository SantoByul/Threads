#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>
#include <time.h>

#define REAL_MAX 2.0
#define REAL_MIN -2.0
#define IMAG_MAX 1.5
#define IMAG_MIN -1.5

void processo_pthread(float basico){
    FILE *file = fopen("mandelbrot_dsob_pthreads1.pgm", "w");

    fprintf(file, "%f", basico);
    return;
}

void processo_openmp(long largura,long altura,long max_iter, long threads){
    FILE *file = fopen("mandelbrot_dsob_openmp.pgm", "w");
    long *matriz_iter = malloc(largura*altura*sizeof(long));
    int soma = 0;

    #pragma omp parallel for num_threads(threads) reduction(+:soma)
    for (long i=0; i<10; i++){
        for(long j=0; j<10; j++){
            printf("Teste da Thread %d no 'momento' %ld\n", omp_get_thread_num(), i);
            soma++;
        }
    }
    fprintf(file, "Teste finalizado e resultado é: %d\n", soma);
    fclose(file);
    free(matriz_iter);
    return;
}

void processo_serial(long largura,long altura,long max_iter){
    FILE *file = fopen("mandelbrot_dsob_serial.pgm", "w");
    long *matriz_iter = malloc(largura*altura*sizeof(long));

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
    free(matriz_iter);
    return;
}

void anotar_timer(char nome[16]){
    FILE *tFile = fopen("timer.txt", "w");

    fclose(tFile);
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

    processo_serial(largura, altura, max_iter);
    processo_openmp(largura, altura, max_iter, threads);

    return 0;
}