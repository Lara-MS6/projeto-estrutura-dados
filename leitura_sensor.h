#ifndef LEITURA_SENSOR_H
#define LEITURA_SENSOR_H

typedef struct {
    long timestamp;
    char id_sensor[50];
    char valor_string[100];
} LeituraSensor;

#endif
