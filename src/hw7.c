#include "hw7.h"

bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
    (void)mat;
    (void)root;

    return NULL;
}

matrix_sf* find_bst_sf(char name, bst_sf *root) {
    (void)name;
    (void)root;
    return NULL;
}

void free_bst_sf(bst_sf *root) {
    (void)root;
}

matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    if(mat1 == NULL || mat2 == NULL)
    {
        return NULL;
    }
    if(mat1->num_rows != mat2->num_rows || mat1->num_cols != mat2->num_cols)
    {
        return NULL;
    }
    size_t count = (size_t)mat1->num_rows * mat1->num_cols;
    matrix_sf *ans = malloc(sizeof(matrix_sf)+ count * (sizeof(int)));
    if(ans == NULL)
    {
        return NULL;
    }
    ans->name = '?';
    ans->num_rows = mat1->num_rows;
    ans->num_cols = mat2->num_cols;
    for (size_t i = 0; i < count; i++)
    {
        ans->values[i] = mat1->values[i] + mat2->values[i];
    }
    return ans;
}

matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    if(mat1 == NULL || mat2== NULL || mat1->num_cols != mat2->num_rows)
    {
        return NULL;
    }
    size_t count = (size_t)mat1->num_rows * mat2->num_cols;
    matrix_sf *ans = malloc(sizeof(matrix_sf)+ count * (sizeof(int)));
    if(ans == NULL)
    {
        return NULL;
    }
    ans->name = '?';
    ans->num_rows = mat1->num_rows;
    ans->num_cols = mat2->num_cols;
    for (unsigned int i = 0; i < mat1->num_rows; i++)
    {
        for (unsigned int j = 0; j < mat1->num_cols; j++)
        {
            int sum=0;
            for (unsigned int k = 0; k < mat1->num_cols; k++)
            {
                sum+= mat1->values[i* mat1->num_cols+k] * mat2->values[k*(mat2->num_rows)+j];
            }
            ans->values[i* ans->num_cols+j] = sum;
        }
    }
    return ans;

}

matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    if(mat == NULL)
    {
        return NULL;
    }
    size_t count = (size_t)mat->num_rows * mat->num_cols;
    matrix_sf *ans = malloc(sizeof(matrix_sf)+ count* sizeof(int));
    if(ans == NULL)
    {
        return NULL;
    }
    ans->name = '?';
    ans->num_rows = mat->num_cols;
    ans->num_cols = mat->num_rows;

    for (unsigned int i = 0; i < mat->num_rows; i++)
    {
        for (unsigned int j = 0; j < mat->num_cols; j++)
        {
            ans->values[ j * ans->num_cols + i]= mat->values[i *mat->num_cols+j];
        }
    }
    return ans;

}

matrix_sf* create_matrix_sf(char name, const char *expr) {
    if (name == '\0' || expr == NULL)
    {
        return NULL;
    }
    const char *p = expr;
    char *end = NULL;

    long r = strtol(p, &end, 10);
    if (end == p || r <= 0) return NULL;
    p = end;

    long c = strtol(p, &end, 10);
    if (end == p || c <= 0) return NULL;
    p = end;
    size_t rows = (size_t)r;
    size_t cols = (size_t)c;
    size_t count = rows * cols;
    matrix_sf *m = malloc(sizeof(*m) + count * sizeof(m->values[0]));
    if (m == NULL)
    {
        return NULL;
    }


    m->name = name;
    m->num_rows = (unsigned int)rows;
    m->num_cols = (unsigned int)cols;
    for (size_t i = 0; i < count; i++)
    {
        long v =strtol(p,&end,10);
        if(end ==p)
        {
            free(m);
            return NULL;
        }
        m->values[i] = (int)v;
        p =end;
    }
    return m;
}

char* infix2postfix_sf(char *infix) {
    (void)infix;
    return NULL;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    (void)name;
    (void)expr;
    (void)root;
    return NULL;
}

matrix_sf *execute_script_sf(char *filename) {
   (void)filename;
   return NULL;
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[]) {
    matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows*num_cols*sizeof(int));
    return m;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
void print_matrix_sf(matrix_sf *mat) {
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    printf("%d %d ", mat->num_rows, mat->num_cols);
    for (unsigned int i = 0; i < mat->num_rows*mat->num_cols; i++) {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows*mat->num_cols-1)
            printf(" ");
    }
    printf("\n");
}
