#include "types.h"
#include "status.h"
#include <stdlib.h>
#include <string.h>

struct data_type *create_int(void *number, STATUS *statusof) {
    struct data_type *result = (struct data_type*)malloc(sizeof(struct data_type));

    if (!result) {
        *statusof = STATUS_MEMORY_ALLOCATION_ERROR;
        return NULL;
    }

    result->size = sizeof(int);

    result->data = malloc(result->size);

    if (!result->data) {
        *statusof = STATUS_MEMORY_ALLOCATION_ERROR;
        free(result);
        return NULL;
    }

    memcpy(result->data, number, result->size);

    result->sum = int_sum;
    result->multiply = int_multiply;
    result->create = create_int;

    return result;
}

struct data_type *int_sum(struct data_type *item1, struct data_type *item2, STATUS *statusof) {
    if (!item1 || !item2) {
        *statusof = STATUS_NOT_FOUND;
        return NULL;
    }

    int value1 = *(int*)item1->data;
    int value2 = *(int*)item2->data;
    int sum = value1 + value2;
    struct data_type *result = create_int(&sum, statusof);
    return result;
}

struct data_type *int_multiply(struct data_type *item1, struct data_type *item2, STATUS *statusof) {
    if (!item1 || !item2) {
        *statusof = STATUS_NOT_FOUND;
        return NULL;
    }

    int value1 = *(int*)item1->data;
    int value2 = *(int*)item2->data;
    int product = value1 * value2;
    struct data_type *result = create_int(&product, statusof);
    return result;
}

struct data_type *create_double(void *number, STATUS *statusof) {
    struct data_type *result = (struct data_type*)malloc(sizeof(struct data_type));

    if (!result) {
        *statusof = STATUS_MEMORY_ALLOCATION_ERROR;
        return NULL;
    }

    result->size = sizeof(double);

    result->data = malloc(result->size);

    if (!result->data) {
        *statusof = STATUS_MEMORY_ALLOCATION_ERROR;
        free(result);
        return NULL;
    }

    memcpy(result->data, number, result->size);

    result->sum = double_sum;
    result->multiply = double_multiply;
    result->create = create_double;

    return result;
}

struct data_type *double_sum(struct data_type *item1, struct data_type *item2, STATUS *statusof) {
    if (!item1 || !item2) {
        *statusof = STATUS_NOT_FOUND;
        return NULL;
    }

    double value1 = *(double*)item1->data;
    double value2 = *(double*)item2->data;
    double sum = value1 + value2;
    struct data_type *result = create_double(&sum, statusof);
    return result;
}

struct data_type *double_multiply(struct data_type *item1, struct data_type *item2, STATUS *statusof) {
    if (!item1 || !item2) {
        *statusof = STATUS_NOT_FOUND;
        return NULL;
    }
    
    double value1 = *(double*)item1->data;
    double value2 = *(double*)item2->data;
    double product = value1 * value2;
    struct data_type *result = create_double(&product, statusof);
    return result;
}