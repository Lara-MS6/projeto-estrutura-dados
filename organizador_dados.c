#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitura_sensor.h"

void ordenar_por_bubble_sort(LeituraSensor* vetor, int quantidade) {
    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = 0; j < quantidade - i - 1; j++) {
            int comparacao_id = strcmp(vetor[j].id_sensor, vetor[j + 1].id_sensor);
            
            int deve_trocar = 0;
            if (comparacao_id > 0) {
                deve_trocar = 1;
            } else if (comparacao_id == 0) {
                if (vetor[j].timestamp > vetor[j + 1].timestamp) {
                    deve_trocar = 1;
                }
            }

            if (deve_trocar) {
                LeituraSensor temporario = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temporario;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    FILE* arquivo_entrada = fopen(argv[1], "r");
    if (!arquivo_entrada) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", argv[1]);
        return 1;
    }

    int capacidade = 1000;
    int contador = 0;
    LeituraSensor* leituras = malloc(capacidade * sizeof(LeituraSensor));

    while (fscanf(arquivo_entrada, "%ld %49s %49s", &leituras[contador].timestamp, leituras[contador].id_sensor, leituras[contador].valor_string) == 3) {
        contador++;
        if (contador >= capacidade) {
            capacidade *= 2;
            leituras = realloc(leituras, capacidade * sizeof(LeituraSensor));
        }
    }
    fclose(arquivo_entrada);

    ordenar_por_bubble_sort(leituras, contador);

    if (contador > 0) {
        char arquivo_atual[60];
        sprintf(arquivo_atual, "%s.txt", leituras[0].id_sensor);
        FILE* arquivo_saida = fopen(arquivo_atual, "w");
        printf("Arquivo '%s' criado e ordenado com sucesso.\n", arquivo_atual);

        for (int i = 0; i < contador; ++i) {
            if (i > 0 && strcmp(leituras[i].id_sensor, leituras[i - 1].id_sensor) != 0) {
                fclose(arquivo_saida);
                sprintf(arquivo_atual, "%s.txt", leituras[i].id_sensor);
                arquivo_saida = fopen(arquivo_atual, "w");
                printf("Arquivo '%s' criado e ordenado com sucesso.\n", arquivo_atual);
            }
            fprintf(arquivo_saida, "%ld %s %s\n", leituras[i].timestamp, leituras[i].id_sensor, leituras[i].valor_string);
        }
        fclose(arquivo_saida);
    }

    free(leituras);
    return 0;
}