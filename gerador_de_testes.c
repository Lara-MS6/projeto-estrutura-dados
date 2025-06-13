#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "leitura_sensor.h"

long data_para_timestamp(const char* string_data);
void gerar_valor_aleatorio(char* valor_saida, const char* tipo);
void embaralhar(LeituraSensor* vetor, int quantidade);

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Uso: %s <data_inicio> <data_fim> <sensor1:tipo> [sensor2:tipo]...\n", argv[0]);
        return 1;
    }

    srand(time(NULL));

    long timestamp_inicio = data_para_timestamp(argv[1]);
    if (timestamp_inicio == -1) {
        fprintf(stderr, "Erro: Data de início inválida: '%s'\n", argv[1]);
        return 1;
    }

    long timestamp_fim = data_para_timestamp(argv[2]);
    if (timestamp_fim == -1) {
        fprintf(stderr, "Erro: Data de fim inválida: '%s'\n", argv[2]);
        return 1;
    }

    int numero_sensores = argc - 3;
    int total_de_leituras = numero_sensores * 2000;
    LeituraSensor* todas_as_leituras = malloc(total_de_leituras * sizeof(LeituraSensor));

    int indice_leitura = 0;
    for (int i = 0; i < numero_sensores; ++i) {
        char* argumento_sensor = strdup(argv[i + 3]);
        char* id_do_sensor = strtok(argumento_sensor, ":");
        if (id_do_sensor == NULL) {
            free(argumento_sensor);
            continue;
        }
        char* tipo_de_dado = strtok(NULL, ":");
        if (tipo_de_dado == NULL) {
            free(argumento_sensor);
            continue;
        }

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
    for (int i = 0; i < indice_leitura; ++i) {
        fprintf(arquivo_saida, "%ld %s %s\n", todas_as_leituras[i].timestamp, todas_as_leituras[i].id_sensor, todas_as_leituras[i].valor_string);
    }
    fclose(arquivo_saida);
    free(todas_as_leituras);

    printf("Arquivo 'dados_brutos.txt' gerado com sucesso.\n");
    return 0;
}

long data_para_timestamp(const char* string_data) {
    struct tm tm = {0};
    int campos_lidos = sscanf(string_data, "%d-%d-%dT%d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
    if (campos_lidos != 6) return -1;

    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    tm.tm_isdst = -1;
    
    time_t tempo = mktime(&tm);
    return (long)tempo;
}

void gerar_valor_aleatorio(char* valor_saida, const char* tipo) {
    if (strcmp(tipo, "int") == 0) { sprintf(valor_saida, "%d", rand() % 1001); } 
    else if (strcmp(tipo, "bool") == 0) { strcpy(valor_saida, (rand() % 2) ? "true" : "false"); }
    else if (strcmp(tipo, "racional") == 0) { sprintf(valor_saida, "%.4f", (float)rand() / RAND_MAX * 500.0); }
    else if (strcmp(tipo, "string") == 0) {
        for (int i = 0; i < 15; ++i) { valor_saida[i] = 'A' + (rand() % 26); }
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