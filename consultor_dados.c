#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "leitura_sensor.h"

long data_para_timestamp(const char* string_data);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <id_do_sensor> <data_hora>\n", argv[0]);
        return 1;
    }
    
    char* id_consulta = argv[1];
    long timestamp_consulta = data_para_timestamp(argv[2]);
    if (timestamp_consulta == -1) {
        fprintf(stderr, "Erro: Data de consulta inválida: '%s'\n", argv[2]);
        return 1;
    }

    char nome_arquivo[60];
    sprintf(nome_arquivo, "%s.txt", id_consulta);

    FILE* arquivo_sensor = fopen(nome_arquivo, "r");
    if (!arquivo_sensor) {
        fprintf(stderr, "Erro: Arquivo '%s' não encontrado.\n", nome_arquivo);
        return 1;
    }

    int capacidade = 1000;
    int contador = 0;
    LeituraSensor* leituras = malloc(capacidade * sizeof(LeituraSensor));
    while (fscanf(arquivo_sensor, "%ld %49s %49s", &leituras[contador].timestamp, leituras[contador].id_sensor, leituras[contador].valor_string) == 3) {
        contador++;
        if (contador >= capacidade) {
            capacidade *= 2;
            leituras = realloc(leituras, capacidade * sizeof(LeituraSensor));
        }
    }
    fclose(arquivo_sensor);
    
    if (contador == 0) {
        printf("Nenhuma leitura encontrada no arquivo '%s'.\n", nome_arquivo);
        free(leituras);
        return 0;
    }

    int baixo = 0;
    int alto = contador - 1;
    int indice_candidato = 0;
    while (baixo <= alto) {
        int meio = baixo + (alto - baixo) / 2;
        if (leituras[meio].timestamp < timestamp_consulta) {
            baixo = meio + 1;
        } else {
            alto = meio - 1;
        }
    }
    indice_candidato = baixo;

    int indice_resultado;
    if (indice_candidato == 0) {
        indice_resultado = 0;
    } else if (indice_candidato >= contador) {
        indice_resultado = contador - 1;
    } else {
        long diff_anterior = timestamp_consulta - leituras[indice_candidato - 1].timestamp;
        long diff_atual = leituras[indice_candidato].timestamp - timestamp_consulta;
        if (diff_anterior <= diff_atual) {
            indice_resultado = indice_candidato - 1;
        } else {
            indice_resultado = indice_candidato;
        }
    }
    
    printf("\nLeitura mais próxima encontrada:\n");
    printf("%ld %s %s\n", leituras[indice_resultado].timestamp, leituras[indice_resultado].id_sensor, leituras[indice_resultado].valor_string);

    free(leituras);
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