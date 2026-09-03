#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
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

typedef struct{
    long inicio;
    long fim;
    long largura;
    long max_iter;
    long *matriz_iter;
    char *buffer;
} args_print;

void *processo_pthread1(void *arg){
    args_pthread *a = (args_pthread *)arg;
    if (a == NULL){
        return NULL;
    }
    long largura = a->largura;
    long altura = a->altura;
    long max_iter = a->max_iter;
    long *matriz_iter = a->matriz_iter;

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

long *processo_pthread2(long largura,long altura,long max_iter, long *matriz_iter){
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
    return matriz_iter;
}

void *ajuste_thread2(void *arg){
    args_print *a = (args_print *)arg;
    if(a == NULL){
        return NULL;
    }

    size_t capacidade = (size_t)(a->fim - a->inicio) * a->largura * 4 + (size_t)(a->fim - a->inicio) + 1;
    char *buffer = malloc(capacidade);
    size_t pos = 0;

    for (long i = a->inicio; i < a->fim; i++){
        for (long y = 0; y < a->largura; y++){
            long inten = (a->matriz_iter[i * a->largura + y] * 255) / a->max_iter;
            pos += sprintf(buffer + pos, "%ld ", inten);
        }
        pos += sprintf(buffer + pos, "\n");
    }

    a->buffer = buffer;
    return NULL;
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

int verificar_float(char *str, float *saida){
    char *receber;
    errno = 0;
    *saida = strtof(str,&receber);

    if(receber == str){
        return 0;
    }
    while(*receber !='\0'){
        if(!isspace((unsigned char)*receber)){
            return 0;
        }
        receber++;
    }
    if(errno == ERANGE){
        return 0;
    }
    return 1;
}

int verificar_long(char *str, long *saida){
    char *receber;
    errno = 0;
    *saida = strtof(str,&receber);

    if(receber == str){
        return 0;
    }
    while(*receber !='\0'){
        if(!isspace((unsigned char)*receber)){
            return 0;
        }
        receber++;
    }
    if(errno == ERANGE){
        return 0;
    }
    return 1;
}

int main(int argc, char **argv){
    if (argc < 5){
        printf("Menos argumentos do que requisitado");
        return 1;
    }
    args_pthread a1;

    float largura, altura;
    long max_iter, threads;

    if(!verificar_float(argv[1],&largura)){
        printf("Argumento da Largura invalido\n");
        return 1;
    }
    if(!verificar_float(argv[2],&altura)){
        printf("Argumento da Altura invalido\n");
        return 1;
    }
    if(!verificar_long(argv[3],&max_iter)){
        printf("Argumento das Iterações Maximas invalido\n");
        return 1;
    }
    if(!verificar_long(argv[4],&threads)){
        printf("Argumento de Threads invalido\n");
        return 1;
    }
    long *matriz_iter = malloc(largura*altura*sizeof(long));
    if(matriz_iter == NULL){
        printf("Erro na alocação de memoria");
        return 1;
    }

    a1.largura = largura;
    a1.altura = altura;
    a1.max_iter = max_iter;
    a1.matriz_iter = matriz_iter;

    pthread_t thread[threads];
    pthread_t thread2[threads];

    processo_serial(largura, altura, max_iter, matriz_iter);
    processo_openmp(largura, altura, max_iter, threads, matriz_iter);

    for(int i=0; i<threads; i++){
        pthread_create(&thread[i], NULL, processo_pthread1, &a1);
    }
    for(int i=0; i<threads; i++){
        pthread_join(thread[i], NULL);
    }

    matriz_iter = processo_pthread2(largura, altura, max_iter, matriz_iter);

    args_print a2[threads];
    long linhas_por_thread = altura / threads;

    for (int i = 0; i < threads; i++){
        a2[i].inicio = i * linhas_por_thread;
        a2[i].fim = (i == threads - 1) ? altura : (i + 1) * linhas_por_thread;
        a2[i].largura = largura;
        a2[i].max_iter = max_iter;
        a2[i].matriz_iter = matriz_iter;
        pthread_create(&thread2[i], NULL, ajuste_thread2, &a2[i]);
    }
    for (int i = 0; i < threads; i++){
        pthread_join(thread2[i], NULL);
    }

    FILE *file = fopen("mandelbrot_dsob_pthreads2.pgm", "w");
    if (file == NULL){
        printf("Erro ao abrir o Arquivo");
        return 1;
    }
    for (int i = 0; i < threads; i++){
        fwrite(a2[i].buffer, sizeof(char), strlen(a2[i].buffer), file);
        free(a2[i].buffer);
    }
    fclose(file);

    free(matriz_iter);
    return 0;
}