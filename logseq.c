#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> // Para medir o tempo

#define DIM 3

int main(void) {
    int i, j, k, n, c;
    double dmin, dx;
    double *x, *mean, *sum;
    int *cluster, *count, color;
    int flips;
    int iter = 0; // <-- ADICIONADO: Contador de iteração

    //  Variáveis de Tomada de Tempo 
    clock_t inicio, fim;
    double tempo_total;

    //  MUDANÇA: Redireciona stderr para logseq.txt 
    const char *log_file_path = "logseq.txt"; // Nome do arquivo de log
    if (freopen(log_file_path, "w", stderr) == NULL) {
        perror("Erro ao redirecionar stderr para logseq.txt");
        return 1;
    }
    // A partir daqui, fprintf(stderr, ...) irá para logseq.txt

    //  Inicia o Cronômetro Principal 
    inicio = clock();

    fprintf(stderr, "Iniciando K-Means Sequencial...\n"); // <-- ADICIONADO: Log inicial

    //  1. FASE DE SETUP (Leitura + Alocação) 
    fprintf(stderr, "[Iter %d]: Etapa 1 (Setup) Iniciada...\n", iter); // <-- ADICIONADO: Log
    scanf("%d", &k);
    scanf("%d", &n);
    fprintf(stderr, "[Iter %d]: Lendo K=%d clusters e N=%d pontos.\n", iter, k, n); // <-- ADICIONADO: Log

    x = (double *)malloc(sizeof(double)*DIM*n);
    mean = (double *)malloc(sizeof(double)*DIM*k);
    sum= (double *)malloc(sizeof(double)*DIM*k);
    cluster = (int *)malloc(sizeof(int)*n);
    count = (int *)malloc(sizeof(int)*k);

    for (i = 0; i<n; i++)
        cluster[i] = 0;
    fprintf(stderr, "[Iter %d]: Lendo %d centroides iniciais...\n", iter, k); // <-- ADICIONADO: Log
    for (i = 0; i<k; i++)
        scanf("%lf %lf %lf", mean+i*DIM, mean+i*DIM+1, mean+i*DIM+2);
    fprintf(stderr, "[Iter %d]: Lendo %d pontos de dados...\n", iter, n); // <-- ADICIONADO: Log
    for (i = 0; i<n; i++)
        scanf("%lf %lf %lf", x+i*DIM, x+i*DIM+1, x+i*DIM+2);
    fprintf(stderr, "[Iter %d]: Etapa 1 (Setup) Concluida.\n", iter); // <-- ADICIONADO: Log


    //  2. FASE DE EXECUÇÃO (Algoritmo K-Means) 
    flips = n;
    while (flips>0) {
        fprintf(stderr, "\n[Iter %d]: Iniciando iteracao K-Means...\n", iter); // <-- ADICIONADO: Log

        //  Etapa de Atribuição 
        fprintf(stderr, "[Iter %d]: Etapa de Atribuicao Iniciada...\n", iter); // <-- ADICIONADO: Log
        flips = 0;
        // Zera contadores (parte da atualização, mas feito antes da atribuição)
        for (j = 0; j < k; j++) {
            count[j] = 0;
            for (i = 0; i < DIM; i++)
                sum[j*DIM+i] = 0.0;
        }
        // Atribui pontos
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
        fprintf(stderr, "[Iter %d]: Etapa de Atribuicao Concluida. Flips nesta iteracao: %d\n", iter, flips); // <-- ADICIONADO: Log

        // Se não houve flips, o algoritmo convergiu
        if (flips == 0) {
             fprintf(stderr, "[Iter %d]: CONVERGIU! Saindo do loop.\n", iter); // <-- ADICIONADO: Log
             break;
        }

        //  Etapa de Atualização 
        fprintf(stderr, "[Iter %d]: Etapa de Atualizacao Iniciada...\n", iter); // <-- ADICIONADO: Log
        // Soma
        for (i = 0; i < n; i++) {
            count[cluster[i]]++;
            for (j = 0; j < DIM; j++)
                sum[cluster[i]*DIM+j] += x[i*DIM+j];
        }
        // Média
        for (i = 0; i < k; i++) {
            for (j = 0; j < DIM; j++) {
                if (count[i] > 0) {
                    mean[i*DIM+j] = sum[i*DIM+j]/count[i];
                }
            }
        }
        fprintf(stderr, "[Iter %d]: Etapa de Atualizacao Concluida.\n", iter); // <-- ADICIONADO: Log

        iter++; // Incrementa o contador de iteração
    } // Fim do loop 'while'


    //  3. FASE DE ESCRITA (Resultados) 
    fprintf(stderr, "\n Fase de Escrita dos Resultados (stdout) \n"); // <-- ADICIONADO: Log
    for (i = 0; i < k; i++) {
        for (j = 0; j < DIM; j++)
            printf("%5.2f ", mean[i*DIM+j]); // Isso vai para o CONSOLE (stdout)
        printf("\n");
    }

    //  4. CÁLCULO E IMPRESSÃO DO TEMPO 
    fim = clock();
    tempo_total = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // Imprime o tempo total no arquivo de log (stderr)
    fprintf(stderr, "\nTempo de CPU total (Sequencial): %f segundos\n", tempo_total);


    #ifdef DEBUG
    // ... (código debug omitido) ...
    #endif

    // Libera a memória
    free(x);
    free(mean);
    free(sum);
    free(cluster);
    free(count);

    // freopen fecha o arquivo stderr automaticamente no fim do programa

    return(0);
}