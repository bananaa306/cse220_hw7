#include "unit_tests.h"
#include "hw7.h"



static matrix_sf *mk(unsigned int rows, unsigned int cols, int values[]) {
    matrix_sf *m = copy_matrix(rows, cols, values);
    cr_assert_not_null(m);
    return m;
}

static matrix_sf *mk_named(char name, unsigned int rows, unsigned int cols, int values[]) {
    matrix_sf *m = mk(rows, cols, values);
    m->name = name;
    return m;
}

static matrix_sf *mk_scalar(char name, int value) {
    int v[] = {value};
    return mk_named(name, 1, 1, v);
}

static char *make_temp_script(const char *text) {
    char templ[] = "/tmp/hw7_student_XXXXXX";
    int fd = mkstemp(templ);
    cr_assert_neq(fd, -1);
    FILE *fp = fdopen(fd, "w");
    cr_assert_not_null(fp);
    fprintf(fp, "%s", text);
    fclose(fp);
    return strdup(templ);
}

static void remove_temp_script(char *path) {
    if (path == NULL) return;
    unlink(path);
    free(path);
}

/* add_mats_sf: 10 cases */
Test(student_operator_return, add_edge01, .description="add_mats_sf handles NULL left input") {
    matrix_sf *b = mk_scalar('B', 2);
    cr_expect_null(add_mats_sf(NULL, b));
    free(b);
}

Test(student_operator_return, add_edge02, .description="add_mats_sf handles NULL right input") {
    matrix_sf *a = mk_scalar('A', 2);
    cr_expect_null(add_mats_sf(a, NULL));
    free(a);
}

Test(student_operator_return, add_edge03, .description="add_mats_sf rejects different row counts") {
    matrix_sf *a = mk_named('A', 1, 2, (int[]){1, 2});
    matrix_sf *b = mk_named('B', 2, 2, (int[]){3, 4, 5, 6});
    cr_expect_null(add_mats_sf(a, b));
    free(a);
    free(b);
}

Test(student_operator_return, add_edge04, .description="add_mats_sf rejects different column counts") {
    matrix_sf *a = mk_named('A', 2, 2, (int[]){1, 2, 3, 4});
    matrix_sf *b = mk_named('B', 2, 3, (int[]){5, 6, 7, 8, 9, 10});
    cr_expect_null(add_mats_sf(a, b));
    free(a);
    free(b);
}

Test(student_operator_return, add_edge05, .description="add_mats_sf adds 1x1 matrices") {
    matrix_sf *a = mk_scalar('A', -3);
    matrix_sf *b = mk_scalar('B', 10);
    matrix_sf *c = add_mats_sf(a, b);
    expect_matrices_equal(c, 1, 1, (int[]){7});
    free(a);
    free(b);
    free(c);
}

Test(student_operator_return, add_edge06, .description="add_mats_sf adds rectangular matrices") {
    matrix_sf *a = mk_named('A', 2, 3, (int[]){1, 2, 3, 4, 5, 6});
    matrix_sf *b = mk_named('B', 2, 3, (int[]){6, 5, 4, 3, 2, 1});
    matrix_sf *c = add_mats_sf(a, b);
    expect_matrices_equal(c, 2, 3, (int[]){7, 7, 7, 7, 7, 7});
    free(a);
    free(b);
    free(c);
}

Test(student_operator_return, add_edge07, .description="add_mats_sf handles negative values") {
    matrix_sf *a = mk_named('A', 2, 2, (int[]){-1, -2, 3, 4});
    matrix_sf *b = mk_named('B', 2, 2, (int[]){5, -6, -7, 8});
    matrix_sf *c = add_mats_sf(a, b);
    expect_matrices_equal(c, 2, 2, (int[]){4, -8, -4, 12});
    free(a);
    free(b);
    free(c);
}

Test(student_operator_return, add_edge08, .description="add_mats_sf result name is question mark") {
    matrix_sf *a = mk_scalar('A', 1);
    matrix_sf *b = mk_scalar('B', 1);
    matrix_sf *c = add_mats_sf(a, b);
    cr_expect_eq(c->name, '?');
    free(a);
    free(b);
    free(c);
}

Test(student_operator_return, add_edge09, .description="add_mats_sf does not mutate inputs") {
    int av[] = {1, 2, 3, 4};
    int bv[] = {5, 6, 7, 8};
    matrix_sf *a = mk_named('A', 2, 2, av);
    matrix_sf *b = mk_named('B', 2, 2, bv);
    matrix_sf *c = add_mats_sf(a, b);
    cr_expect_arr_eq(a->values, av, sizeof(av));
    cr_expect_arr_eq(b->values, bv, sizeof(bv));
    free(a);
    free(b);
    free(c);
}

Test(student_operator_return, add_edge10, .description="add_mats_sf handles zero-sized matrices") {
    matrix_sf *a = malloc(sizeof(matrix_sf));
    matrix_sf *b = malloc(sizeof(matrix_sf));
    cr_assert_not_null(a);
    cr_assert_not_null(b);
    a->name = 'A'; a->num_rows = 0; a->num_cols = 0;
    b->name = 'B'; b->num_rows = 0; b->num_cols = 0;
    matrix_sf *c = add_mats_sf(a, b);
    cr_expect_not_null(c);
    cr_expect_eq(c->num_rows, 0);
    cr_expect_eq(c->num_cols, 0);
    free(a);
    free(b);
    free(c);
}

/* mult_mats_sf: 10 cases */
Test(student_operator_return, mult_edge01, .description="mult_mats_sf handles NULL left input") {
    matrix_sf *b = mk_scalar('B', 2);
    cr_expect_null(mult_mats_sf(NULL, b));
    free(b);
}

Test(student_operator_return, mult_edge02, .description="mult_mats_sf handles NULL right input") {
    matrix_sf *a = mk_scalar('A', 2);
    cr_expect_null(mult_mats_sf(a, NULL));
    free(a);
}

Test(student_operator_return, mult_edge03, .description="mult_mats_sf rejects incompatible dimensions") {
    matrix_sf *a = mk_named('A', 2, 2, (int[]){1, 2, 3, 4});
    matrix_sf *b = mk_named('B', 1, 3, (int[]){1, 2, 3});
    cr_expect_null(mult_mats_sf(a, b));
    free(a);
    free(b);
}

Test(student_operator_return, mult_edge04, .description="mult_mats_sf multiplies 1x1 matrices") {
    matrix_sf *a = mk_scalar('A', -2);
    matrix_sf *b = mk_scalar('B', 9);
    matrix_sf *c = mult_mats_sf(a, b);
    expect_matrices_equal(c, 1, 1, (int[]){-18});
    free(a);
    free(b);
    free(c);
}

Test(student_operator_return, mult_edge05, .description="mult_mats_sf computes dot product") {
    matrix_sf *a = mk_named('A', 1, 3, (int[]){2, 3, 4});
    matrix_sf *b = mk_named('B', 3, 1, (int[]){5, 6, 7});
    matrix_sf *c = mult_mats_sf(a, b);
    expect_matrices_equal(c, 1, 1, (int[]){56});
    free(a);
    free(b);
    free(c);
}

Test(student_operator_return, mult_edge06, .description="mult_mats_sf with identity matrix") {
    matrix_sf *i = mk_named('I', 2, 2, (int[]){1, 0, 0, 1});
    matrix_sf *m = mk_named('M', 2, 2, (int[]){9, 8, 7, 6});
    matrix_sf *c = mult_mats_sf(i, m);
    expect_matrices_equal(c, 2, 2, (int[]){9, 8, 7, 6});
    free(i);
    free(m);
    free(c);
}

Test(student_operator_return, mult_edge07, .description="mult_mats_sf with zero matrix produces zero") {
    matrix_sf *z = mk_named('Z', 2, 2, (int[]){0, 0, 0, 0});
    matrix_sf *m = mk_named('M', 2, 2, (int[]){9, 8, 7, 6});
    matrix_sf *c = mult_mats_sf(z, m);
    expect_matrices_equal(c, 2, 2, (int[]){0, 0, 0, 0});
    free(z);
    free(m);
    free(c);
}

Test(student_operator_return, mult_edge08, .description="mult_mats_sf handles signs correctly") {
    matrix_sf *a = mk_named('A', 2, 2, (int[]){-1, 2, -3, 4});
    matrix_sf *b = mk_named('B', 2, 2, (int[]){5, -6, 7, -8});
    matrix_sf *c = mult_mats_sf(a, b);
    expect_matrices_equal(c, 2, 2, (int[]){9, -10, 13, -14});
    free(a);
    free(b);
    free(c);
}

Test(student_operator_return, mult_edge09, .description="mult_mats_sf result name is question mark") {
    matrix_sf *a = mk_scalar('A', 3);
    matrix_sf *b = mk_scalar('B', 4);
    matrix_sf *c = mult_mats_sf(a, b);
    cr_expect_eq(c->name, '?');
    free(a);
    free(b);
    free(c);
}

Test(student_operator_return, mult_edge10, .description="mult_mats_sf handles zero-row by zero-col") {
    matrix_sf *a = malloc(sizeof(matrix_sf));
    matrix_sf *b = malloc(sizeof(matrix_sf));
    cr_assert_not_null(a);
    cr_assert_not_null(b);
    a->name = 'A'; a->num_rows = 0; a->num_cols = 3;
    b->name = 'B'; b->num_rows = 3; b->num_cols = 0;
    matrix_sf *c = mult_mats_sf(a, b);
    cr_expect_not_null(c);
    cr_expect_eq(c->num_rows, 0);
    cr_expect_eq(c->num_cols, 0);
    free(a);
    free(b);
    free(c);
}

/* transpose_mat_sf: 10 cases */
Test(student_operator_return, trans_edge01, .description="transpose_mat_sf handles NULL input") {
    cr_expect_null(transpose_mat_sf(NULL));
}

Test(student_operator_return, trans_edge02, .description="transpose_mat_sf transposes 1x1") {
    matrix_sf *a = mk_scalar('A', 42);
    matrix_sf *t = transpose_mat_sf(a);
    expect_matrices_equal(t, 1, 1, (int[]){42});
    free(a);
    free(t);
}

Test(student_operator_return, trans_edge03, .description="transpose_mat_sf transposes 2x2") {
    matrix_sf *a = mk_named('A', 2, 2, (int[]){1, 2, 3, 4});
    matrix_sf *t = transpose_mat_sf(a);
    expect_matrices_equal(t, 2, 2, (int[]){1, 3, 2, 4});
    free(a);
    free(t);
}

Test(student_operator_return, trans_edge04, .description="transpose_mat_sf transposes rectangular matrix") {
    matrix_sf *a = mk_named('A', 2, 3, (int[]){1, 2, 3, 4, 5, 6});
    matrix_sf *t = transpose_mat_sf(a);
    expect_matrices_equal(t, 3, 2, (int[]){1, 4, 2, 5, 3, 6});
    free(a);
    free(t);
}

Test(student_operator_return, trans_edge05, .description="transpose_mat_sf converts row vector to column") {
    matrix_sf *a = mk_named('A', 1, 3, (int[]){7, 8, 9});
    matrix_sf *t = transpose_mat_sf(a);
    expect_matrices_equal(t, 3, 1, (int[]){7, 8, 9});
    free(a);
    free(t);
}

Test(student_operator_return, trans_edge06, .description="transpose_mat_sf converts column vector to row") {
    matrix_sf *a = mk_named('A', 3, 1, (int[]){7, 8, 9});
    matrix_sf *t = transpose_mat_sf(a);
    expect_matrices_equal(t, 1, 3, (int[]){7, 8, 9});
    free(a);
    free(t);
}

Test(student_operator_return, trans_edge07, .description="transpose_mat_sf keeps sign values correctly") {
    matrix_sf *a = mk_named('A', 2, 3, (int[]){-1, -2, -3, -4, -5, -6});
    matrix_sf *t = transpose_mat_sf(a);
    expect_matrices_equal(t, 3, 2, (int[]){-1, -4, -2, -5, -3, -6});
    free(a);
    free(t);
}

Test(student_operator_return, trans_edge08, .description="transpose_mat_sf result name is question mark") {
    matrix_sf *a = mk_scalar('A', 5);
    matrix_sf *t = transpose_mat_sf(a);
    cr_expect_eq(t->name, '?');
    free(a);
    free(t);
}

Test(student_operator_return, trans_edge09, .description="transpose_mat_sf does not mutate input") {
    int vals[] = {1, 2, 3, 4};
    matrix_sf *a = mk_named('A', 2, 2, vals);
    matrix_sf *t = transpose_mat_sf(a);
    cr_expect_arr_eq(a->values, vals, sizeof(vals));
    free(a);
    free(t);
}

Test(student_operator_return, trans_edge10, .description="transpose_mat_sf handles zero dimensions") {
    matrix_sf *a = malloc(sizeof(matrix_sf));
    cr_assert_not_null(a);
    a->name = 'A';
    a->num_rows = 0;
    a->num_cols = 5;
    matrix_sf *t = transpose_mat_sf(a);
    cr_expect_not_null(t);
    cr_expect_eq(t->num_rows, 5);
    cr_expect_eq(t->num_cols, 0);
    free(a);
    free(t);
}

/* insert_bst_sf: 10 cases */
Test(student_insert_bst, insert_edge01, .description="insert_bst_sf inserts into empty tree") {
    matrix_sf *a = mk_scalar('A', 1);
    bst_sf *root = insert_bst_sf(a, NULL);
    cr_expect_not_null(root);
    cr_expect_eq(root->mat, a);
    free_bst_sf(root);
}

Test(student_insert_bst, insert_edge02, .description="insert_bst_sf inserts left child") {
    bst_sf *root = insert_bst_sf(mk_scalar('M', 1), NULL);
    root = insert_bst_sf(mk_scalar('A', 2), root);
    cr_expect_eq(root->left_child->mat->name, 'A');
    free_bst_sf(root);
}

Test(student_insert_bst, insert_edge03, .description="insert_bst_sf inserts right child") {
    bst_sf *root = insert_bst_sf(mk_scalar('M', 1), NULL);
    root = insert_bst_sf(mk_scalar('Z', 2), root);
    cr_expect_eq(root->right_child->mat->name, 'Z');
    free_bst_sf(root);
}

Test(student_insert_bst, insert_edge04, .description="insert_bst_sf inserts at depth") {
    bst_sf *root = NULL;
    root = insert_bst_sf(mk_scalar('D', 1), root);
    root = insert_bst_sf(mk_scalar('B', 1), root);
    root = insert_bst_sf(mk_scalar('F', 1), root);
    root = insert_bst_sf(mk_scalar('C', 1), root);
    cr_expect_eq(root->left_child->right_child->mat->name, 'C');
    free_bst_sf(root);
}

Test(student_insert_bst, insert_edge05, .description="insert_bst_sf ignores duplicate key") {
    matrix_sf *orig = mk_scalar('K', 1);
    matrix_sf *dup = mk_scalar('K', 9);
    bst_sf *root = insert_bst_sf(orig, NULL);
    root = insert_bst_sf(dup, root);
    cr_expect_eq(root->mat, orig);
    free(dup);
    free_bst_sf(root);
}

Test(student_insert_bst, insert_edge06, .description="insert_bst_sf returns original root pointer") {
    bst_sf *root = insert_bst_sf(mk_scalar('Q', 5), NULL);
    bst_sf *ret = insert_bst_sf(mk_scalar('P', 6), root);
    cr_expect_eq(ret, root);
    free_bst_sf(root);
}

Test(student_insert_bst, insert_edge07, .description="insert_bst_sf uses ASCII ordering") {
    bst_sf *root = insert_bst_sf(mk_scalar('a', 1), NULL);
    root = insert_bst_sf(mk_scalar('Z', 2), root);
    cr_expect_eq(root->left_child->mat->name, 'Z');
    free_bst_sf(root);
}

Test(student_insert_bst, insert_edge08, .description="insert_bst_sf works with many insertions") {
    bst_sf *root = NULL;
    char names[] = "GTHRNBVCUJELMOPSQZA";
    for (size_t i = 0; i < strlen(names); i++) {
        root = insert_bst_sf(mk_scalar(names[i], (int)i), root);
    }
    cr_expect_not_null(find_bst_sf('A', root));
    free_bst_sf(root);
}

Test(student_insert_bst, insert_edge09, .description="insert_bst_sf stores non-square matrix pointers") {
    matrix_sf *m = mk_named('R', 2, 3, (int[]){1, 2, 3, 4, 5, 6});
    bst_sf *root = insert_bst_sf(m, NULL);
    cr_expect_eq(root->mat->num_rows, 2);
    cr_expect_eq(root->mat->num_cols, 3);
    free_bst_sf(root);
}

Test(student_insert_bst, insert_edge10, .description="insert_bst_sf supports mixed key set") {
    bst_sf *root = NULL;
    char names[] = "MATRIX";
    for (size_t i = 0; i < strlen(names); i++) {
        root = insert_bst_sf(mk_scalar(names[i], (int)names[i]), root);
    }
    cr_expect_not_null(find_bst_sf('M', root));
    cr_expect_not_null(find_bst_sf('X', root));
    free_bst_sf(root);
}

/* find_bst_sf: 10 cases */
Test(student_find_bst, find_edge01, .description="find_bst_sf returns NULL for NULL root") {
    cr_expect_null(find_bst_sf('A', NULL));
}

Test(student_find_bst, find_edge02, .description="find_bst_sf finds root") {
    bst_sf *root = insert_bst_sf(mk_scalar('M', 7), NULL);
    matrix_sf *m = find_bst_sf('M', root);
    cr_expect_not_null(m);
    cr_expect_eq(m->values[0], 7);
    free_bst_sf(root);
}

Test(student_find_bst, find_edge03, .description="find_bst_sf finds left leaf") {
    bst_sf *root = NULL;
    root = insert_bst_sf(mk_scalar('M', 1), root);
    root = insert_bst_sf(mk_scalar('A', 2), root);
    cr_expect_eq(find_bst_sf('A', root)->name, 'A');
    free_bst_sf(root);
}

Test(student_find_bst, find_edge04, .description="find_bst_sf finds right leaf") {
    bst_sf *root = NULL;
    root = insert_bst_sf(mk_scalar('M', 1), root);
    root = insert_bst_sf(mk_scalar('Z', 2), root);
    cr_expect_eq(find_bst_sf('Z', root)->name, 'Z');
    free_bst_sf(root);
}

Test(student_find_bst, find_edge05, .description="find_bst_sf miss on left branch") {
    bst_sf *root = insert_bst_sf(mk_scalar('M', 1), NULL);
    cr_expect_null(find_bst_sf('A', root));
    free_bst_sf(root);
}

Test(student_find_bst, find_edge06, .description="find_bst_sf miss on right branch") {
    bst_sf *root = insert_bst_sf(mk_scalar('M', 1), NULL);
    cr_expect_null(find_bst_sf('Z', root));
    free_bst_sf(root);
}

Test(student_find_bst, find_edge07, .description="find_bst_sf handles left-skewed tree") {
    bst_sf *root = NULL;
    root = insert_bst_sf(mk_scalar('D', 1), root);
    root = insert_bst_sf(mk_scalar('C', 2), root);
    root = insert_bst_sf(mk_scalar('B', 3), root);
    root = insert_bst_sf(mk_scalar('A', 4), root);
    cr_expect_eq(find_bst_sf('A', root)->values[0], 4);
    free_bst_sf(root);
}

Test(student_find_bst, find_edge08, .description="find_bst_sf handles right-skewed tree") {
    bst_sf *root = NULL;
    root = insert_bst_sf(mk_scalar('A', 1), root);
    root = insert_bst_sf(mk_scalar('B', 2), root);
    root = insert_bst_sf(mk_scalar('C', 3), root);
    root = insert_bst_sf(mk_scalar('D', 4), root);
    cr_expect_eq(find_bst_sf('D', root)->values[0], 4);
    free_bst_sf(root);
}

Test(student_find_bst, find_edge09, .description="find_bst_sf is case-sensitive") {
    bst_sf *root = insert_bst_sf(mk_scalar('A', 1), NULL);
    cr_expect_null(find_bst_sf('a', root));
    free_bst_sf(root);
}

Test(student_find_bst, find_edge10, .description="find_bst_sf returns original on duplicate insert") {
    matrix_sf *orig = mk_scalar('K', 10);
    matrix_sf *dup = mk_scalar('K', 20);
    bst_sf *root = insert_bst_sf(orig, NULL);
    root = insert_bst_sf(dup, root);
    cr_expect_eq(find_bst_sf('K', root), orig);
    free(dup);
    free_bst_sf(root);
}

/* free_bst_sf: 10 cases */
Test(student_free_bst, free_edge01, .description="free_bst_sf handles NULL tree") {
    free_bst_sf(NULL);
    cr_expect(1);
}

Test(student_free_bst, free_edge02, .description="free_bst_sf frees single node tree") {
    bst_sf *root = insert_bst_sf(mk_scalar('A', 1), NULL);
    free_bst_sf(root);
    cr_expect(1);
}

Test(student_free_bst, free_edge03, .description="free_bst_sf frees two-node left tree") {
    bst_sf *root = NULL;
    root = insert_bst_sf(mk_scalar('B', 1), root);
    root = insert_bst_sf(mk_scalar('A', 2), root);
    free_bst_sf(root);
    cr_expect(1);
}

Test(student_free_bst, free_edge04, .description="free_bst_sf frees two-node right tree") {
    bst_sf *root = NULL;
    root = insert_bst_sf(mk_scalar('A', 1), root);
    root = insert_bst_sf(mk_scalar('B', 2), root);
    free_bst_sf(root);
    cr_expect(1);
}

Test(student_free_bst, free_edge05, .description="free_bst_sf frees balanced tree") {
    bst_sf *root = NULL;
    char keys[] = "DBFACEG";
    for (size_t i = 0; i < strlen(keys); i++) {
        root = insert_bst_sf(mk_scalar(keys[i], (int)i), root);
    }
    free_bst_sf(root);
    cr_expect(1);
}

Test(student_free_bst, free_edge06, .description="free_bst_sf frees left-skewed tree") {
    bst_sf *root = NULL;
    char keys[] = "EDCBA";
    for (size_t i = 0; i < strlen(keys); i++) {
        root = insert_bst_sf(mk_scalar(keys[i], (int)i), root);
    }
    free_bst_sf(root);
    cr_expect(1);
}

Test(student_free_bst, free_edge07, .description="free_bst_sf frees right-skewed tree") {
    bst_sf *root = NULL;
    char keys[] = "ABCDE";
    for (size_t i = 0; i < strlen(keys); i++) {
        root = insert_bst_sf(mk_scalar(keys[i], (int)i), root);
    }
    free_bst_sf(root);
    cr_expect(1);
}

Test(student_free_bst, free_edge08, .description="free_bst_sf handles node with NULL matrix") {
    bst_sf *root = malloc(sizeof(bst_sf));
    cr_assert_not_null(root);
    root->mat = NULL;
    root->left_child = NULL;
    root->right_child = NULL;
    free_bst_sf(root);
    cr_expect(1);
}

Test(student_free_bst, free_edge09, .description="free_bst_sf frees larger tree") {
    bst_sf *root = NULL;
    char keys[] = "QWERTYUIOPASDFGHJKLZXCVBNM";
    for (size_t i = 0; i < strlen(keys); i++) {
        root = insert_bst_sf(mk_scalar(keys[i], (int)i), root);
    }
    free_bst_sf(root);
    cr_expect(1);
}

Test(student_free_bst, free_edge10, .description="free_bst_sf frees tree with mixed matrix sizes") {
    bst_sf *root = NULL;
    root = insert_bst_sf(mk_named('M', 2, 2, (int[]){1, 2, 3, 4}), root);
    root = insert_bst_sf(mk_named('A', 2, 3, (int[]){5, 6, 7, 8, 9, 10}), root);
    root = insert_bst_sf(mk_scalar('Z', 99), root);
    free_bst_sf(root);
    cr_expect(1);
}

/* create_matrix_sf: 10 cases */
Test(student_create_matrix, create_edge01, .description="create_matrix_sf handles NULL expr") {
    cr_expect_null(create_matrix_sf('A', NULL));
}

Test(student_create_matrix, create_edge02, .description="create_matrix_sf rejects null name") {
    cr_expect_null(create_matrix_sf('\0', "1 1 [5]"));
}

Test(student_create_matrix, create_edge03, .description="create_matrix_sf rejects non-numeric rows") {
    cr_expect_null(create_matrix_sf('A', "x 1 [5]"));
}

Test(student_create_matrix, create_edge04, .description="create_matrix_sf rejects non-numeric cols") {
    cr_expect_null(create_matrix_sf('A', "1 x [5]"));
}

Test(student_create_matrix, create_edge05, .description="create_matrix_sf rejects zero rows") {
    cr_expect_null(create_matrix_sf('A', "0 1 [5]"));
}

Test(student_create_matrix, create_edge06, .description="create_matrix_sf rejects zero cols") {
    cr_expect_null(create_matrix_sf('A', "1 0 [5]"));
}

Test(student_create_matrix, create_edge07, .description="create_matrix_sf rejects insufficient values") {
    cr_expect_null(create_matrix_sf('A', "2 2 [1 2 3]"));
}

Test(student_create_matrix, create_edge08, .description="create_matrix_sf parses plain format") {
    matrix_sf *m = create_matrix_sf('B', "2 2 [1 2 3 4]");
    cr_assert_not_null(m);
    expect_matrices_equal(m, 2, 2, (int[]){1, 2, 3, 4});
    cr_expect_eq(m->name, 'B');
    free(m);
}

Test(student_create_matrix, create_edge09, .description="create_matrix_sf parses semicolons and newline") {
    matrix_sf *m = create_matrix_sf('C', "2 3 [1 2 3;\n4 5 6;]");
    cr_assert_not_null(m);
    expect_matrices_equal(m, 2, 3, (int[]){1, 2, 3, 4, 5, 6});
    free(m);
}

Test(student_create_matrix, create_edge10, .description="create_matrix_sf tolerates trailing text") {
    matrix_sf *m = create_matrix_sf('D', "1 2 [7 8] trailing words");
    cr_assert_not_null(m);
    expect_matrices_equal(m, 1, 2, (int[]){7, 8});
    free(m);
}

/* infix2postfix_sf: 10 cases */
Test(student_infix2postfix, post_edge01, .description="infix2postfix_sf handles NULL input") {
    cr_expect_null(infix2postfix_sf(NULL));
}

Test(student_infix2postfix, post_edge02, .description="infix2postfix_sf handles single variable") {
    char expr[] = "A";
    char *p = infix2postfix_sf(expr);
    cr_expect_str_eq(p, "A");
    free(p);
}

Test(student_infix2postfix, post_edge03, .description="infix2postfix_sf converts simple addition") {
    char expr[] = "A+B";
    char *p = infix2postfix_sf(expr);
    cr_expect_str_eq(p, "AB+");
    free(p);
}

Test(student_infix2postfix, post_edge04, .description="infix2postfix_sf converts simple multiplication") {
    char expr[] = "A*B";
    char *p = infix2postfix_sf(expr);
    cr_expect_str_eq(p, "AB*");
    free(p);
}

Test(student_infix2postfix, post_edge05, .description="infix2postfix_sf applies precedence") {
    char expr[] = "A+B*C";
    char *p = infix2postfix_sf(expr);
    cr_expect_str_eq(p, "ABC*+");
    free(p);
}

Test(student_infix2postfix, post_edge06, .description="infix2postfix_sf applies parentheses") {
    char expr[] = "(A+B)*C";
    char *p = infix2postfix_sf(expr);
    cr_expect_str_eq(p, "AB+C*");
    free(p);
}

Test(student_infix2postfix, post_edge07, .description="infix2postfix_sf handles transpose on symbol") {
    char expr[] = "A'";
    char *p = infix2postfix_sf(expr);
    cr_expect_str_eq(p, "A'");
    free(p);
}

Test(student_infix2postfix, post_edge08, .description="infix2postfix_sf handles transpose on group") {
    char expr[] = "(A+B)'";
    char *p = infix2postfix_sf(expr);
    cr_expect_str_eq(p, "AB+'");
    free(p);
}

Test(student_infix2postfix, post_edge09, .description="infix2postfix_sf ignores spaces") {
    char expr[] = " A + B ";
    char *p = infix2postfix_sf(expr);
    cr_expect_str_eq(p, "AB+");
    free(p);
}

Test(student_infix2postfix, post_edge10, .description="infix2postfix_sf handles complex expression") {
    char expr[] = "A*B'+(C+D)'";
    char *p = infix2postfix_sf(expr);
    cr_expect_str_eq(p, "AB'*CD+'+");
    free(p);
}

/* evaluate_expr_sf: 10 cases */
Test(student_evaluate_expr, eval_edge01, .description="evaluate_expr_sf returns NULL for NULL expr") {
    bst_sf *root = insert_bst_sf(mk_scalar('A', 1), NULL);
    cr_expect_null(evaluate_expr_sf('R', NULL, root));
    free_bst_sf(root);
}

Test(student_evaluate_expr, eval_edge02, .description="evaluate_expr_sf returns NULL for NULL root") {
    char expr[] = "A+B";
    cr_expect_null(evaluate_expr_sf('R', expr, NULL));
}

Test(student_evaluate_expr, eval_edge03, .description="evaluate_expr_sf evaluates addition") {
    bst_sf *root = NULL;
    root = insert_bst_sf(mk_named('A', 2, 2, (int[]){1, 2, 3, 4}), root);
    root = insert_bst_sf(mk_named('B', 2, 2, (int[]){5, 6, 7, 8}), root);
    char expr[] = "A+B";
    matrix_sf *r = evaluate_expr_sf('X', expr, root);
    cr_assert_not_null(r);
    expect_matrices_equal(r, 2, 2, (int[]){6, 8, 10, 12});
    cr_expect_eq(r->name, 'X');
    free(r);
    free_bst_sf(root);
}

Test(student_evaluate_expr, eval_edge04, .description="evaluate_expr_sf evaluates multiplication") {
    bst_sf *root = NULL;
    root = insert_bst_sf(mk_named('C', 2, 3, (int[]){1, 2, 3, 4, 5, 6}), root);
    root = insert_bst_sf(mk_named('D', 3, 2, (int[]){7, 8, 9, 10, 11, 12}), root);
    char expr[] = "C*D";
    matrix_sf *r = evaluate_expr_sf('Y', expr, root);
    cr_assert_not_null(r);
    expect_matrices_equal(r, 2, 2, (int[]){58, 64, 139, 154});
    free(r);
    free_bst_sf(root);
}

Test(student_evaluate_expr, eval_edge05, .description="evaluate_expr_sf evaluates transpose") {
    bst_sf *root = insert_bst_sf(mk_named('C', 2, 3, (int[]){1, 2, 3, 4, 5, 6}), NULL);
    char expr[] = "C'";
    matrix_sf *r = evaluate_expr_sf('T', expr, root);
    cr_assert_not_null(r);
    expect_matrices_equal(r, 3, 2, (int[]){1, 4, 2, 5, 3, 6});
    free(r);
    free_bst_sf(root);
}

Test(student_evaluate_expr, eval_edge06, .description="evaluate_expr_sf handles grouped transpose") {
    bst_sf *root = NULL;
    root = insert_bst_sf(mk_named('A', 2, 2, (int[]){1, 2, 3, 4}), root);
    root = insert_bst_sf(mk_named('B', 2, 2, (int[]){10, 20, 30, 40}), root);
    char expr[] = "(A+B)'";
    matrix_sf *r = evaluate_expr_sf('R', expr, root);
    cr_assert_not_null(r);
    expect_matrices_equal(r, 2, 2, (int[]){11, 33, 22, 44});
    free(r);
    free_bst_sf(root);
}

Test(student_evaluate_expr, eval_edge07, .description="evaluate_expr_sf fails when symbol is missing") {
    bst_sf *root = insert_bst_sf(mk_scalar('A', 1), NULL);
    char expr[] = "A+Z";
    cr_expect_null(evaluate_expr_sf('R', expr, root));
    free_bst_sf(root);
}

Test(student_evaluate_expr, eval_edge08, .description="evaluate_expr_sf fails on incompatible add") {
    bst_sf *root = NULL;
    root = insert_bst_sf(mk_named('A', 2, 2, (int[]){1, 2, 3, 4}), root);
    root = insert_bst_sf(mk_named('C', 2, 3, (int[]){1, 2, 3, 4, 5, 6}), root);
    char expr[] = "A+C";
    cr_expect_null(evaluate_expr_sf('R', expr, root));
    free_bst_sf(root);
}

Test(student_evaluate_expr, eval_edge09, .description="evaluate_expr_sf handles chained operations") {
    bst_sf *root = NULL;
    root = insert_bst_sf(mk_named('A', 2, 2, (int[]){1, 1, 1, 1}), root);
    root = insert_bst_sf(mk_named('B', 2, 2, (int[]){2, 2, 2, 2}), root);
    char expr[] = "A+B+B";
    matrix_sf *r = evaluate_expr_sf('S', expr, root);
    cr_assert_not_null(r);
    expect_matrices_equal(r, 2, 2, (int[]){5, 5, 5, 5});
    free(r);
    free_bst_sf(root);
}

Test(student_evaluate_expr, eval_edge10, .description="evaluate_expr_sf ignores spaces in expression") {
    bst_sf *root = NULL;
    root = insert_bst_sf(mk_named('A', 2, 2, (int[]){1, 2, 3, 4}), root);
    root = insert_bst_sf(mk_named('B', 2, 2, (int[]){5, 6, 7, 8}), root);
    char expr[] = " A * B ";
    matrix_sf *r = evaluate_expr_sf('M', expr, root);
    cr_assert_not_null(r);
    expect_matrices_equal(r, 2, 2, (int[]){19, 22, 43, 50});
    free(r);
    free_bst_sf(root);
}

/* execute_script_sf: 10 cases */
Test(student_execute_script, exec_edge01, .description="execute_script_sf returns NULL for NULL filename") {
    cr_expect_null(execute_script_sf(NULL));
}

Test(student_execute_script, exec_edge02, .description="execute_script_sf returns NULL for missing file") {
    char path[] = "/tmp/does_not_exist_hw7_12345";
    cr_expect_null(execute_script_sf(path));
}

Test(student_execute_script, exec_edge03, .description="execute_script_sf returns NULL for empty file") {
    char *path = make_temp_script("");
    cr_expect_null(execute_script_sf(path));
    remove_temp_script(path);
}

Test(student_execute_script, exec_edge04, .description="execute_script_sf handles one definition") {
    char *path = make_temp_script("A = 1 3 [1 2 3]\n");
    matrix_sf *m = execute_script_sf(path);
    cr_assert_not_null(m);
    cr_expect_eq(m->name, 'A');
    expect_matrices_equal(m, 1, 3, (int[]){1, 2, 3});
    free(m);
    remove_temp_script(path);
}

Test(student_execute_script, exec_edge05, .description="execute_script_sf returns last successful assignment") {
    char *path = make_temp_script(
        "A = 1 1 [5]\n"
        "B = 1 1 [9]\n"
    );
    matrix_sf *m = execute_script_sf(path);
    cr_assert_not_null(m);
    cr_expect_eq(m->name, 'B');
    expect_matrices_equal(m, 1, 1, (int[]){9});
    free(m);
    remove_temp_script(path);
}

Test(student_execute_script, exec_edge06, .description="execute_script_sf evaluates expression lines") {
    char *path = make_temp_script(
        "A = 2 2 [1 2 3 4]\n"
        "B = 2 2 [5 6 7 8]\n"
        "C = A+B\n"
    );
    matrix_sf *m = execute_script_sf(path);
    cr_assert_not_null(m);
    cr_expect_eq(m->name, 'C');
    expect_matrices_equal(m, 2, 2, (int[]){6, 8, 10, 12});
    free(m);
    remove_temp_script(path);
}

Test(student_execute_script, exec_edge07, .description="execute_script_sf ignores lines without equals") {
    char *path = make_temp_script(
        "not an assignment\n"
        "A = 1 1 [3]\n"
    );
    matrix_sf *m = execute_script_sf(path);
    cr_assert_not_null(m);
    cr_expect_eq(m->name, 'A');
    expect_matrices_equal(m, 1, 1, (int[]){3});
    free(m);
    remove_temp_script(path);
}

Test(student_execute_script, exec_edge08, .description="execute_script_sf keeps last matrix on bad expression") {
    char *path = make_temp_script(
        "A = 1 1 [3]\n"
        "B = A+Z\n"
    );
    matrix_sf *m = execute_script_sf(path);
    cr_assert_not_null(m);
    cr_expect_eq(m->name, 'A');
    expect_matrices_equal(m, 1, 1, (int[]){3});
    free(m);
    remove_temp_script(path);
}

Test(student_execute_script, exec_edge09, .description="execute_script_sf accepts extra space after equals") {
    char *path = make_temp_script("A =  1 2 [9 8]\n");
    matrix_sf *m = execute_script_sf(path);
    cr_assert_not_null(m);
    expect_matrices_equal(m, 1, 2, (int[]){9, 8});
    free(m);
    remove_temp_script(path);
}

Test(student_execute_script, exec_edge10, .description="execute_script_sf supports transpose then multiply") {
    char *path = make_temp_script(
        "A = 2 3 [1 2 3 4 5 6]\n"
        "B = A'\n"
        "C = A*B\n"
    );
    matrix_sf *m = execute_script_sf(path);
    cr_assert_not_null(m);
    cr_expect_eq(m->name, 'C');
    expect_matrices_equal(m, 2, 2, (int[]){14, 32, 32, 77});
    free(m);
    remove_temp_script(path);
}
