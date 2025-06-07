#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include "status.h"

struct data_type {
    size_t size;
    void **data;
    struct data_type *(*sum)(struct data_type*, struct data_type*, STATUS*);
    struct data_type *(*multiply)(struct data_type*, struct data_type*, STATUS*);
    struct data_type *(*create)(void *number, STATUS*);
};

typedef struct {
    struct data_type **value;
    size_t size;
} vector;

struct data_type *create_int(void *number, STATUS *statusof);
struct data_type *create_double(void *number, STATUS *statusof);
struct data_type *int_sum(struct data_type *item1, struct data_type *item2, STATUS *statusof);
struct data_type *int_multiply(struct data_type *item1, struct data_type *item2, STATUS *statusof);
struct data_type *double_sum(struct data_type *item1, struct data_type *item2, STATUS *statusof);
struct data_type *double_multiply(struct data_type *item1, struct data_type *item2, STATUS *statusof);

#endif