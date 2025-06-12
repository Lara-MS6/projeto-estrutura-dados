#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "leitura_sensor.h"

long data_para_timestamp(const char* string_data) {
    struct tm tm = {0};
    sscanf(string_data, "%d-%d-%dT%d:%d:%d", 
           &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
           &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    tm.tm_isdst = -1;
    return mktime(&tm);
}

void gerar_valor_aleatorio(char* valor_saida, const char* tipo) {
    if (strcmp(tipo, "int") == 0) {
        sprintf(valor_saida, "%d", rand() % 1001);
    } else if (strcmp(tipo, "bool") == 0) {
        strcpy(valor_saida, (rand() % 2) ? "true" : "false");
    } else if (strcmp(tipo, "racional") == 0) {
        sprintf(valor_saida, "%.4f", (float)rand() / RAND_MAX * 500.0);
    } else if (strcmp(tipo, "string") == 0) {
        for (int i = 0; i < 15; ++i) {
            valor_saida[i] = 'A' + (rand() % 26);
        }
        valor_saida[15] = '\0';
    }
}

void embaralhar(LeituraSensor* vetor, int quantidade) {
    for (int i = quantidade - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        LeituraSensor temporario = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = temporario;
    }
}

int main(int argc, char *argv[]) {
    const char* string_inicio = "2025-06-12T08:00:00";
    const char* string_fim = "2025-06-12T18:00:00";
    const char* sensores[] = {
        "TEMPERATURA_SALA:racional",
        "UMIDADE_AR:int",
        "PORTA_ABERTA:bool"
    };
    int numero_sensores = 3;

    printf("Gerando dados\n");
    printf("De: %s\nAté: %s\n", string_inicio, string_fim);

    srand(time(NULL));

    long timestamp_inicio = data_para_timestamp(string_inicio);
    long timestamp_fim = data_para_timestamp(string_fim);
    int total_de_leituras = numero_sensores * 2000;

    LeituraSensor* todas_as_leituras = malloc(total_de_leituras * sizeof(LeituraSensor));
    if (!todas_as_leituras) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        return 1;
    }

    int indice_leitura = 0;
    for (int i = 0; i < numero_sensores; ++i) {
        char* argumento_sensor = strdup(sensores[i]);
        char* id_do_sensor = strtok(argumento_sensor, ":");
        char* tipo_de_dado = strtok(NULL, ":");

        for (int j = 0; j < 2000; ++j) {
            todas_as_leituras[indice_leitura].timestamp = timestamp_inicio + rand() % (timestamp_fim - timestamp_inicio);
            strcpy(todas_as_leituras[indice_leitura].id_sensor, id_do_sensor);
            gerar_valor_aleatorio(todas_as_leituras[indice_leitura].valor_string, tipo_de_dado);
            indice_leitura++;
        }
        free(argumento_sensor);
    }

    embaralhar(todas_as_leituras, total_de_leituras);

    FILE* arquivo_saida = fopen("dados_brutos.txt", "w");
    if (!arquivo_saida) {
        fprintf(stderr, "Erro ao criar dados_brutos.txt\n");
        free(todas_as_leituras);
        return 1;
    }

    for (int i = 0; i < total_de_leituras; ++i) {
        fprintf(arquivo_saida, "%ld %s %s\n", 
                todas_as_leituras[i].timestamp, 
                todas_as_leituras[i].id_sensor, 
                todas_as_leituras[i].valor_string);
    }

    fclose(arquivo_saida);
    free(todas_as_leituras);

    printf("\nArquivo 'dados_brutos.txt' gerado com %d leituras.\n", total_de_leituras);
    return 0;
}