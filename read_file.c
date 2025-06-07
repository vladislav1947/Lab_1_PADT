#include "types.h"
#include "status.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct data_type *read_file(const char *filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    char type[10];
    struct data_type *item = NULL;

    while (fscanf(file, "%s", type) == 1) {
        if (strcmp(type, "INT") == 0) {
            int values[100];
            int count = 0;

            while (fscanf(file, "%d", &values[count]) == 1) {
                count++;
            }

            item = (struct data_type*)malloc(sizeof(struct data_type));

            if (!item) {
                fclose(file);
                return NULL;
            }

            item->size = count;
            item->data = malloc(count * sizeof(void *));

            if (!item->data) {
                free(item);
                fclose(file);
                return NULL;
            }

            for (int i = 0; i < count; i++) {
                item->data[i] = malloc(sizeof(int));

                if (!item->data[i]) {
                    for (int j = 0; j < i; j++) free(item->data[j]);
                    free(item->data);
                    free(item);
                    fclose(file);
                    return NULL;
                }

                memcpy(item->data[i], &values[i], sizeof(int));
            }

            item->sum = int_sum;
            item->multiply = int_multiply;
            item->create = create_int;
        } else if (strcmp(type, "DOUBLE") == 0) {
            double values[100];
            int count = 0;

            while (fscanf(file, "%lf", &values[count]) == 1) {
                count++;
            }

            item = (struct data_type*)malloc(sizeof(struct data_type));

            if (!item) {
                fclose(file);
                return NULL;
            }

            item->size = count;
            item->data = malloc(count * sizeof(void *));

            if (!item->data) {
                free(item);
                fclose(file);
                return NULL;
            }

            for (int i = 0; i < count; i++) {
                item->data[i] = malloc(sizeof(double));

                if (!item->data[i]) {
                    for (int j = 0; j < i; j++) free(item->data[j]);
                    free(item->data);
                    free(item);
                    fclose(file);
                    return NULL;
                }

                memcpy(item->data[i], &values[i], sizeof(double));
            }

            item->sum = double_sum;
            item->multiply = double_multiply;
            item->create = create_double;
        } else {
            fprintf(stderr, "Unknown type: %s\n", type);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    return item;
}