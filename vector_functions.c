#include "types.h"
#include "status.h"
#include "default_functions.c"

vector *vector_init(struct data_type *item, STATUS *statusof) {
    if (!item) {
        *statusof = STATUS_NOT_FOUND;
        return NULL;
    }

    vector *Vector = (vector*)malloc(sizeof(vector));

    if (!Vector) {
        *statusof = STATUS_MEMORY_ALLOCATION_ERROR;
        return NULL;
    }

    Vector->size = item->size;

    Vector->value = (struct data_type**)malloc(Vector->size * sizeof(struct data_type*));

    if (!Vector->value) {
        *statusof = STATUS_MEMORY_ALLOCATION_ERROR;
        free(Vector);
        return NULL;
    }

    for (int i = 0; i < Vector->size; i++) {
        Vector->value[i] = item->create(item->data[i], statusof);
        if (!Vector->value[i]) {
            *statusof = STATUS_UNDEFINED_TYPE;

            for (int j = 0; j < i; j++) {
                free(Vector->value[j]);
            }

            free(Vector->value);
            free(Vector);
            return NULL;
        }
    }

    return Vector;
}

void vector_delete(vector *Vector) {
    if (Vector->value) {
        for (int i = 0; i < Vector->size; i++) {
            if (Vector->value[i]) {
                free(Vector->value[i]->data);
                free(Vector->value[i]);
            }
        }

        free(Vector->value);
        free(Vector);
    }
}

vector *vector_sum(vector *Vector1, vector *Vector2, STATUS *statusof) {
    if (!Vector1 || !Vector2 || Vector1->size != Vector2->size) {
        *statusof = STATUS_NOT_FOUND;
        return NULL;
    }

    vector *Vector_sum = (vector*)malloc(sizeof(vector));
    if (!Vector_sum) {
        *statusof = STATUS_MEMORY_ALLOCATION_ERROR;
        return NULL;
    }

    Vector_sum->size = Vector1->size;
    Vector_sum->value = (struct data_type**)malloc(Vector_sum->size * sizeof(struct data_type*));

    if (!Vector_sum->value) {
        *statusof = STATUS_MEMORY_ALLOCATION_ERROR;
        free(Vector_sum);
        return NULL;
    }

    for (int i = 0; i < Vector1->size; i++) {
        Vector_sum->value[i] = Vector1->value[i]->sum(Vector1->value[i], Vector2->value[i], statusof);

        if (!Vector_sum->value[i]) {
            *statusof = STATUS_UNDEFINED_TYPE;

            for (int j = 0; j < i; j++) {
                free(Vector_sum->value[j]);
            }

            free(Vector_sum->value);
            free(Vector_sum);
            return NULL;
        }
    }
    return Vector_sum;
}

struct data_type *scalar_product(vector *Vector1, vector *Vector2, STATUS *statusof) {
    if (!Vector1 || !Vector2 || Vector1->size != Vector2->size || Vector1->size == 0) {
        *statusof = STATUS_NOT_FOUND;
        return NULL;
    }

    if (Vector1->value[0]->sum != Vector2->value[0]->sum 
        || Vector1->value[0]->multiply != Vector2->value[0]->multiply 
        || Vector1->value[0]->create != Vector2->value[0]->create) {
        *statusof = STATUS_UNDEFINED_TYPE;
        return NULL;
    }

    struct data_type *result = NULL;

    struct data_type *product = Vector1->value[0]->multiply(Vector1->value[0], Vector2->value[0], statusof);

    if (!product) {
        return NULL;
    }

    result = product;

    for (int i = 1; i < Vector1->size; i++) {
        struct data_type *current_product = Vector1->value[i]->multiply(Vector1->value[i], Vector2->value[i], statusof);

        if (!current_product) {
            free(result->data);
            free(result);
            return NULL;
        }

        struct data_type *temp_sum = result->sum(result, current_product, statusof);

        if (!temp_sum) {
            free(current_product->data);
            free(current_product);
            free(result->data);
            free(result);
            return NULL;
        }

        free(result->data);
        free(result);
        free(current_product->data);
        free(current_product);

        result = temp_sum;
    }

    return result;
}