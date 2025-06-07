#include "types.h"
#include "status.h"

int compare_vectors(vector *Vector1, vector *Vector2, STATUS *statusof) {
    if (Vector1->size != Vector2->size) {
        *statusof = STATUS_UNDEFINED_TYPE;
        return 0; 
    }
    
    for (int i = 0; i < Vector1->size; i++) {
        if (Vector1->value[i]->sum != Vector2->value[i]->sum || Vector1->value[i]->multiply != Vector2->value[i]->multiply || Vector1->value[i]->create != Vector2->value[i]->create) return 0;

        if (Vector1->value[i]->create == create_int) {
            int value1 = *(int*)Vector1->value[i]->data;
            int value2 = *(int*)Vector2->value[i]->data;
            if (value1 != value2) return 0;
        } else if (Vector1->value[i]->create == create_double) {
            double value1 = *(double*)Vector1->value[i]->data;
            double value2 = *(double*)Vector2->value[i]->data;
            if (value1 != value2) return 0;
        }
    }
    return 1;
}

int compare_scalar(struct data_type *item1, struct data_type *item2, STATUS *statusof) {
    if (!item1 || !item2) {
        *statusof = STATUS_NOT_FOUND;
        return 0;
    }

    if (item1->sum != item2->sum 
        || item1->multiply != item2->multiply 
        || item1->create != item2->create) {
            *statusof = STATUS_UNDEFINED_TYPE;
            return 0;
        }

    if (item1->create == create_int) {
        int item1_value = *(int*)item1->data;
        int item2_value = *(int*)item2->data;
        return (item1_value == item2_value);
    } else if (item1->create == create_double) {
        double item1_value = *(double*)item1->data;
        double item2_value = *(double*)item2->data;
        return (item1_value == item2_value);
    } else {
        return 0;
    }
}