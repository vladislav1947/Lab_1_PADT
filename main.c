#include "types.h"
#include "status.h"
#include "vector_functions.c"
#include "comparison.c"
#include "read_file.c"
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef struct {
    int total_tests;
    int passed_tests;
    int failed_tests;
} test_results;

void print_test_result(const char* test_name, int passed, test_results* results) {
    results->total_tests++;

    if (passed) {
        results->passed_tests++;
        printf("PASS: %s\n", test_name);
    } else {
        results->failed_tests++;
        printf("FAIL: %s\n", test_name);
    }
}

void print_test_summary(test_results* results) {
    printf("Total tests: %d\n", results->total_tests);
    printf("Passed: %d\n", results->passed_tests);
    printf("Failed: %d\n", results->failed_tests);
}

vector* create_test_vector_int(int* values, int size, STATUS* statusof) {
    struct data_type* item = (struct data_type*)malloc(sizeof(struct data_type));

    if (!item) {
        *statusof = STATUS_MEMORY_ALLOCATION_ERROR;
        return NULL;
    }

    item->size = size;
    item->data = malloc(size * sizeof(void*));

    if (!item->data) {
        free(item);
        *statusof = STATUS_MEMORY_ALLOCATION_ERROR;
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        item->data[i] = malloc(sizeof(int));
        if (!item->data[i]) {
            for (int j = 0; j < i; j++) {
                free(item->data[j]);
            }

            free(item->data);
            free(item);
            *statusof = STATUS_MEMORY_ALLOCATION_ERROR;
            return NULL;
        }

        memcpy(item->data[i], &values[i], sizeof(int));
    }

    item->sum = int_sum;
    item->multiply = int_multiply;
    item->create = create_int;

    vector* vec = vector_init(item, statusof);

    for (int i = 0; i < size; i++) {
        free(item->data[i]);
    }

    free(item->data);
    free(item);

    return vec;
}

vector* create_test_vector_double(double* values, int size, STATUS* statusof) {
    struct data_type* item = (struct data_type*)malloc(sizeof(struct data_type));

    if (!item) {
        *statusof = STATUS_MEMORY_ALLOCATION_ERROR;
        return NULL;
    }

    item->size = size;
    item->data = malloc(size * sizeof(void*));

    if (!item->data) {
        free(item);
        *statusof = STATUS_MEMORY_ALLOCATION_ERROR;
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        item->data[i] = malloc(sizeof(double));

        if (!item->data[i]) {
            for (int j = 0; j < i; j++) {
                free(item->data[j]);
            }

            free(item->data);
            free(item);
            *statusof = STATUS_MEMORY_ALLOCATION_ERROR;
            return NULL;
        }

        memcpy(item->data[i], &values[i], sizeof(double));
    }

    item->sum = double_sum;
    item->multiply = double_multiply;
    item->create = create_double;

    vector* vec = vector_init(item, statusof);

    for (int i = 0; i < size; i++) {
        free(item->data[i]);
    }

    free(item->data);
    free(item);

    return vec;
}

void test_vector_creation(test_results* results) {
    printf("\nTest vector creation\n");
    STATUS statusof = STATUS_OK;

    int values[] = {1, 2, 3};
    vector* vec = create_test_vector_int(values, 3, &statusof);
    print_test_result("Create int vector", vec != NULL && statusof == STATUS_OK, results);

    if (vec) {
        vector_delete(vec);
    }

    double dvalues[] = {1.5, 2.5, 3.5};
    vector* dvec = create_test_vector_double(dvalues, 3, &statusof);
    print_test_result("Create double vector", dvec != NULL && statusof == STATUS_OK, results);

    if (dvec) {
        vector_delete(dvec);
    }

    vector* null_vec = vector_init(NULL, &statusof);
    print_test_result("Got NULL input", null_vec == NULL && statusof == STATUS_NOT_FOUND, results);
}

void test_vector_sum(test_results* results) {
    printf("\nTest vector sum\n");
    STATUS statusof = STATUS_OK;

    int values1[] = {1, 2, 3};
    int values2[] = {4, 5, 6};
    vector* vec1 = create_test_vector_int(values1, 3, &statusof);
    vector* vec2 = create_test_vector_int(values2, 3, &statusof);

    vector* sum = vector_sum(vec1, vec2, &statusof);
    int expected_sum = (vec1 && vec2 && sum && *(int*)sum->value[0]->data == 5 && *(int*)sum->value[1]->data == 7 && *(int*)sum->value[2]->data == 9);
    print_test_result("[1,2,3] + [4,5,6] = [5,7,9]", expected_sum, results);

    if (vec1) {
        vector_delete(vec1);
    }

    if (vec2) {
        vector_delete(vec2);
    }

    if (sum) {
        vector_delete(sum);
    }

    double dvalues1[] = {1.1, 2.2, 3.3};
    double dvalues2[] = {0.9, 0.8, 0.7};
    vector* dvec1 = create_test_vector_double(dvalues1, 3, &statusof);
    vector* dvec2 = create_test_vector_double(dvalues2, 3, &statusof);

    vector* dsum = vector_sum(dvec1, dvec2, &statusof);
    int expected_dsum = (dvec1 && dvec2 && dsum && abs(*(double*)dsum->value[0]->data - 2.0) < 0.001 && abs(*(double*)dsum->value[1]->data - 3.0) < 0.001 && abs(*(double*)dsum->value[2]->data - 4.0) < 0.001);
    print_test_result("[1.1,2.2,3.3] + [0.9,0.8,0.7] = [2.0,3.0,4.0]", expected_dsum, results);

    if (dvec1) {
        vector_delete(dvec1);
    }

    if (dvec2) {
        vector_delete(dvec2);
    }

    if (dsum) {
        vector_delete(dsum);
    }

    int small[] = {1, 2};
    int large[] = {1, 2, 3, 4};
    vector* small_vec = create_test_vector_int(small, 2, &statusof);
    vector* large_vec = create_test_vector_int(large, 4, &statusof);

    vector* mismatch_sum = vector_sum(small_vec, large_vec, &statusof);
    print_test_result("Got size mismatch", mismatch_sum == NULL && statusof == STATUS_NOT_FOUND, results);

    if (small_vec) {
        vector_delete(small_vec);
    }

    if (large_vec) {
        vector_delete(large_vec);
    }
}

void test_scalar_product(test_results* results) {
    printf("\nTest scalar product\n");
    STATUS statusof = STATUS_OK;

    int values1[] = {1, 2, 3};
    int values2[] = {4, 5, 6};
    vector* vec1 = create_test_vector_int(values1, 3, &statusof);
    vector* vec2 = create_test_vector_int(values2, 3, &statusof);

    struct data_type* product = scalar_product(vec1, vec2, &statusof);
    int expected_product = (product && *(int*)product->data == 32);
    print_test_result("[1,2,3] * [4,5,6] = 32", expected_product, results);

    if (vec1) {
        vector_delete(vec1);
    }

    if (vec2) {
        vector_delete(vec2);
    }

    if (product) {
        free(product->data);
        free(product);
    }

    double dvalues1[] = {1.0, 2.0, 3.0};
    double dvalues2[] = {0.5, 1.5, 2.5};
    vector* dvec1 = create_test_vector_double(dvalues1, 3, &statusof);
    vector* dvec2 = create_test_vector_double(dvalues2, 3, &statusof);

    struct data_type* dproduct = scalar_product(dvec1, dvec2, &statusof);
    int expected_dproduct = (dproduct && abs(*(double*)dproduct->data - 11.0) < 0.001); // 0.5 + 3.0 + 7.5 = 11.0
    print_test_result("[1.0,2.0,3.0] * [0.5,1.5,2.5] = 11", expected_dproduct, results);

    if (dvec1) {
        vector_delete(dvec1);
    }

    if (dvec2) {
        vector_delete(dvec2);
    }

    if (dproduct) {
        free(dproduct->data);
        free(dproduct);
    }
}

void test_vector_comparison(test_results* results) {
    printf("\nTest vector comparison\n");
    STATUS statusof = STATUS_OK;

    int values1[] = {1, 2, 3};
    int values2[] = {1, 2, 3};
    vector* vec1 = create_test_vector_int(values1, 3, &statusof);
    vector* vec2 = create_test_vector_int(values2, 3, &statusof);

    int are_equal = compare_vectors(vec1, vec2, &statusof);
    print_test_result("Compare equal vectors", are_equal == 1, results);

    if (vec1) {
        vector_delete(vec1);
    }

    if (vec2) {
        vector_delete(vec2);
    }

    int values3[] = {1, 2, 3};
    int values4[] = {1, 2, 4};
    vector* vec3 = create_test_vector_int(values3, 3, &statusof);
    vector* vec4 = create_test_vector_int(values4, 3, &statusof);

    int are_different = compare_vectors(vec3, vec4, &statusof);
    print_test_result("Compare different vectors", are_different == 0, results);

    if (vec3) {
        vector_delete(vec3);
    }

    if (vec4) {
        vector_delete(vec4);
    }
}

void write_vector_to_file(const char* filename, vector* vec) {
    if (!vec || !filename) {
        return;
    }

    FILE* file = fopen(filename, "w");

    if (!file) {
        printf("Cant create file %s\n", filename);
        return;
    }

    if (vec->value[0]->create == create_int) {
        fprintf(file, "INT\n");

        for (int i = 0; i < vec->size; i++) {
            fprintf(file, "%d", *(int*)vec->value[i]->data);
            if (i < vec->size - 1) fprintf(file, " ");
        }
    } else if (vec->value[0]->create == create_double) {
        fprintf(file, "DOUBLE\n");

        for (int i = 0; i < vec->size; i++) {
            fprintf(file, "%.6f", *(double*)vec->value[i]->data);
            if (i < vec->size - 1) fprintf(file, " ");
        }
    }

    fprintf(file, "\n");
    fclose(file);
}

void write_scalar_to_file(const char* filename, struct data_type* scalar) {
    if (!scalar || !filename) {
        return;
    }

    FILE* file = fopen(filename, "w");

    if (!file) {
        printf("Cant create file %s\n", filename);
        return;
    }

    if (scalar->create == create_int) {
        fprintf(file, "INT\n%d\n", *(int*)scalar->data);
    } else if (scalar->create == create_double) {
        fprintf(file, "DOUBLE\n%.6f\n", *(double*)scalar->data);
    }

    fclose(file);
}

vector* read_vector_from_file(const char* filename, STATUS* statusof) {
    struct data_type* item = read_file(filename);

    if (!item) {
        *statusof = STATUS_NOT_FOUND;
        return NULL;
    }

    vector* vec = vector_init(item, statusof);

    if (item->data) {
        for (int i = 0; i < item->size; i++) {
            if (item->data[i]) {
                free(item->data[i]);
            }
        }

        free(item->data);
    }

    free(item);

    return vec;
}

void print_vector(vector* vec, const char* name) {
    if (!vec) {
        printf("%s: NULL vector\n", name);
        return;
    }

    printf("%s: [", name);

    for (int i = 0; i < vec->size; i++) {
        if (vec->value[i]->create == create_int) {
            printf("%d", *(int*)vec->value[i]->data);
        } else if (vec->value[i]->create == create_double) {
            printf("%.6f", *(double*)vec->value[i]->data);
        }

        if (i < vec->size - 1) {
            printf(", ");
        }
    }

    printf("]\n");
}

void print_scalar(struct data_type* scalar, const char* name) {
    if (!scalar) {
        printf("%s: NULL scalar\n", name);
        return;
    }

    printf("%s: ", name);

    if (scalar->create == create_int) {
        printf("%d\n", *(int*)scalar->data);
    } else if (scalar->create == create_double) {
        printf("%.6f\n", *(double*)scalar->data);
    }
}

int main() {
    test_results results = {0, 0, 0};

    test_vector_creation(&results);
    test_vector_sum(&results);
    test_scalar_product(&results);
    test_vector_comparison(&results);

    print_test_summary(&results);

    printf("\n\nWorking with input:\n");

    STATUS statusof = STATUS_OK;
    vector* vec1 = NULL;
    vector* vec2 = NULL;
    vector* vec_sum = NULL;
    struct data_type* scalar_prod = NULL;

    printf("Reading vector1.txt\n");
    vec1 = read_vector_from_file("input\\vector1.txt", &statusof);

    if (vec1) {
        print_vector(vec1, "Vector 1");
    } else {
        printf("Cant read vector1.txt (Status: %d)\n", &statusof);
    }

    printf("Reading vector2.txt\n");
    vec2 = read_vector_from_file("input\\vector2.txt", &statusof);

    if (vec2) {
        print_vector(vec2, "Vector 2");
    } else {
        printf("Cant read vector2.txt (Status: %d)\n", &statusof);
    }

    if (vec1 && vec2) {
        vec_sum = vector_sum(vec1, vec2, &statusof);

        if (vec_sum) {
            print_vector(vec_sum, "Sum");
        } else {
            printf("Cant compute vector sum (Status: %d)\n", &statusof);
        }

        scalar_prod = scalar_product(vec1, vec2, &statusof);

        if (scalar_prod) {
            print_scalar(scalar_prod, "Scalar Product");
        } else {
            printf("Cant compute scalar product (Status: %d)\n", &statusof);
        }

        printf("\nWriting results to output directory\n");

        write_vector_to_file("output\\vector1.txt", vec1);
        printf("Copied vector1 to output\\vector1.txt\n");

        write_vector_to_file("output\\vector2.txt", vec2);
        printf("Copied vector2 to output\\vector2.txt\n");

        if (vec_sum) {
            write_vector_to_file("output\\vector_sum.txt", vec_sum);
            printf("Saved vector sum to output\\vector_sum.txt\n");
        }

        if (scalar_prod) {
            write_scalar_to_file("output\\vector_product.txt", scalar_prod);
            printf("Saved scalar product to output\\vector_product.txt\n");
        }

    } else {
        printf("Cant perform operations cz missing input vectors.\n");
    }

    if (vec1) {
        vector_delete(vec1);
    }

    if (vec2) {
        vector_delete(vec2);
    }

    if (vec_sum) {
        vector_delete(vec_sum);
    }

    if (scalar_prod) {
        free(scalar_prod->data);
        free(scalar_prod);
    }

    printf("\nPROGRAM COMPLETED\n");
    printf("Test results: %d passed, %d failed out of %d total\n", results.passed_tests, results.failed_tests, results.total_tests);

    return (results.failed_tests == 0) ? 0 : 1;
}