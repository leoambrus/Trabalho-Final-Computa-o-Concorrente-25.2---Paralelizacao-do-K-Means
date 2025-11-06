#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

//Como esse é o cpodigo inicial a única coisa que foi mudada aqui foi a inserção de time.h e a medição do tempo de execução


#define DIM 3
int main(void) {
    int i, j, k, n, c;
    double dmin, dx;
    double *x, *mean, *sum;
    int *cluster, *count, color;
    int flips;

    //  Variáveis de Tomada de Tempo 
    clock_t inicio, fim;
    double tempo_total; 

    //  Inicia o Cronômetro Principal 
    inicio = clock(); 

    //  1. FASE DE SETUP (Leitura + Alocação) 
    scanf("%d", &k);
    scanf("%d", &n);

    x = (double *)malloc(sizeof(double)*DIM*n);
    mean = (double *)malloc(sizeof(double)*DIM*k);
    sum= (double *)malloc(sizeof(double)*DIM*k);
    cluster = (int *)malloc(sizeof(int)*n);
    count = (int *)malloc(sizeof(int)*k);

    for (i = 0; i<n; i++) 
        cluster[i] = 0;
    for (i = 0; i<k; i++)
        scanf("%lf %lf %lf", mean+i*DIM, mean+i*DIM+1, mean+i*DIM+2);
    for (i = 0; i<n; i++)
        scanf("%lf %lf %lf", x+i*DIM, x+i*DIM+1, x+i*DIM+2);
    

    //  2. FASE DE EXECUÇÃO (Algoritmo K-Means) 
    flips = n;
    while (flips>0) {
        flips = 0;
        for (j = 0; j < k; j++) {
            count[j] = 0; 
            for (i = 0; i < DIM; i++) 
                sum[j*DIM+i] = 0.0;
        }
        for (i = 0; i < n; i++) {
            dmin = -1; color = cluster[i];
            for (c = 0; c < k; c++) {
                dx = 0.0;
                for (j = 0; j < DIM; j++) 
                    dx +=  (x[i*DIM+j] - mean[c*DIM+j])*(x[i*DIM+j] - mean[c*DIM+j]);
                if (dx < dmin || dmin == -1) {
                    color = c;
                    dmin = dx;
                }
            }
            if (cluster[i] != color) {
                flips++;
                cluster[i] = color;
            }
        }

        for (i = 0; i < n; i++) {
            count[cluster[i]]++;
            for (j = 0; j < DIM; j++) 
                sum[cluster[i]*DIM+j] += x[i*DIM+j];
        }
        for (i = 0; i < k; i++) {
            for (j = 0; j < DIM; j++) {
                if (count[i] > 0) {
                    mean[i*DIM+j] = sum[i*DIM+j]/count[i];
                }
            }
        }
    } 


    //  3. FASE DE ESCRITA (Resultados) 
    for (i = 0; i < k; i++) {
        for (j = 0; j < DIM; j++)
            printf("%5.2f ", mean[i*DIM+j]); // Isso vai para o output.txt
        printf("\n");
    }

    //  4. CÁLCULO E IMPRESSÃO DO TEMPO 
    
    fim = clock(); 
    
    tempo_total = (double)(fim - inicio) / CLOCKS_PER_SEC;


    fprintf(stderr, "Tempo de CPU total (Leitura + Execucao + Escrita): %f segundos\n", tempo_total);


    
    #ifdef DEBUG
    for (i = 0; i < n; i++) {
        for (j = 0; j < DIM; j++)
            printf("%5.2f ", x[i*DIM+j]);
        printf("%d\n", cluster[i]);
    }
    #endif
    
    // Libera a memória
    free(x);
    free(mean);
    free(sum);
    free(cluster);
    free(count);

    return(0);
}