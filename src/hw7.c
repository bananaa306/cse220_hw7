#include "hw7.h"

bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {

    if(root == NULL)
    {
        bst_sf *new_node = malloc(sizeof(bst_sf));
        new_node->mat = mat;
        new_node->left_child = NULL;
        new_node->right_child = NULL;
        return new_node;
    }    
    if (mat->name < root->mat->name) {
    root->left_child = insert_bst_sf(mat, root->left_child);
    }
    else if (mat->name > root->mat->name) 
    {
        root->right_child = insert_bst_sf(mat, root->right_child);
    }
    return root;
}

matrix_sf* find_bst_sf(char name, bst_sf *root) {
    if(root == NULL)
    {
        return NULL;
    }
    bst_sf *p = root;
    while(p && p->mat->name != name)
    {
        if(name < p->mat->name)
        {
            p = p->left_child;
        }else{
            p = p->right_child;
        }
    }
    return p ? p->mat : NULL;
}

void free_bst_sf(bst_sf *root) {
    if (root == NULL) return;

    free_bst_sf(root->left_child);
    free_bst_sf(root->right_child);

    if (root->mat != NULL) free(root->mat);

    free(root);
}
//helper
void free_bst_except(bst_sf *root, char preserve_name) {
    if (root == NULL) return;
    
    free_bst_except(root->left_child, preserve_name);
    free_bst_except(root->right_child, preserve_name);
    
    // Only free the matrix if it's not the one we want to preserve
    if (root->mat != NULL && root->mat->name != preserve_name) {
        free(root->mat);
    }
    
    free(root);
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
        for (unsigned int j = 0; j < mat2->num_cols; j++)
        {
            int sum=0;
            for (unsigned int k = 0; k < mat1->num_cols; k++)
            {
                sum+= mat1->values[i* mat1->num_cols+k] * mat2->values[k*mat2->num_cols+j];
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
            ans->values[j * ans->num_cols + i] = mat->values[i *mat->num_cols+j];
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

    while (*p && *p != '[') p++;
    if (*p == '[') p++; 

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
        
        while (*p && (*p == ' ' || *p == ';' || *p == '\n' || *p == '\t')) p++;

    }
    return m;
}

char* infix2postfix_sf(char *infix) {
    if (infix == NULL) return NULL;
    //stack implementation as array
    char stack[256];
    int stack_top = -1;
    //allocate mem
    char *result = malloc(strlen(infix) + 1);
    if (result == NULL) return NULL;
    int result_idx = 0;

    for (int i = 0; infix[i] != '\0'; i++)
    {
        char c = infix[i];
        if (c == ' ') continue;
        //letter check
        if (isalpha(c)) 
        {
            result[result_idx++] = c;
            while (i + 1 < (int)strlen(infix) && infix[i + 1] == '\'') {
                result[result_idx++] = '\'';
                i++;
            }
        }
        else if (c == '+' || c == '*') {
            //precendence
            int cur_prec;
            if(c == '*')
            {
                cur_prec=2;
            }
            else {cur_prec =1;}
            while (stack_top >= 0 && stack[stack_top] != '(') 
            {
                char top_op = stack[stack_top];
                int top_prec;
                
                if (top_op == '\'') 
                    top_prec = 3;
                else if (top_op == '*') 
                    top_prec = 2;
                else if (top_op == '+') 
                    top_prec = 1;
                else
                    break;

                if (top_op != '\'' && top_prec >= cur_prec) {
                    result[result_idx++] = stack[stack_top--];
                } 
                else 
                {
                    break;
                }
            }
            stack[++stack_top] = c;
        }
        //paren open
        else if (c == '(') {
            stack[++stack_top] = c;
        }
        //paren close
        else if (c == ')') {
            while (stack_top >= 0 && stack[stack_top] != '(') {
                result[result_idx++] = stack[stack_top--];
            }
            if (stack_top >= 0) stack_top--;
            while (i + 1 < (int)strlen(infix) && infix[i + 1] == '\'') 
            {
                result[result_idx++] = '\'';
                i++;
            }
        } 
    }
    while (stack_top >= 0) 
    {
        result[result_idx++] = stack[stack_top--];
    }
    result[result_idx] = '\0';
    return result;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    if(expr == NULL || root == NULL){return NULL;}
    char *postfix = infix2postfix_sf(expr);
    if (postfix == NULL) return NULL;

    matrix_sf *stack[256];
    int stack_top = -1;
    
    // Track which stack entries are temporary (allocated within this function)
    // vs. from the BST (should not be freed)
    int is_temp[256] = {0};
    
    //
    for (int i = 0; postfix[i] != '\0'; i++) 
    {
        char c = postfix[i];
        //find name
        if (isalpha(c)) {
            matrix_sf *mat = find_bst_sf(c, root);
            if (mat != NULL) {
                stack[++stack_top] = mat;
                is_temp[stack_top] = 0;  // From BST, don't free
            }
        }
        else if (c == '\'') 
        {
            // Unary operator - pop one, transpose, push result
            if (stack_top < 0) {
                free(postfix);
                return NULL;
            }
            matrix_sf *mat = stack[stack_top];
            int mat_is_temp = is_temp[stack_top];
            stack_top--;
            
            matrix_sf *result = transpose_mat_sf(mat);
            
            // Free the temporary matrix we just popped (if it was temporary)
            if (mat_is_temp) {
                free(mat);
            }
            
            if (result != NULL) {
                stack[++stack_top] = result;
                is_temp[stack_top] = 1;  // Result is temporary
            }
        }
        else if (c == '+' || c == '*') {
            // Binary operator - pop two, operate, push result
            if (stack_top < 1) {
                free(postfix);
                return NULL;
            }
            
            matrix_sf *mat2 = stack[stack_top];
            int mat2_is_temp = is_temp[stack_top];
            stack_top--;
            
            matrix_sf *mat1 = stack[stack_top];
            int mat1_is_temp = is_temp[stack_top];
            stack_top--;
            
            matrix_sf *result = NULL;
            if (c == '+') {
                result = add_mats_sf(mat1, mat2);
            } else {
                result = mult_mats_sf(mat1, mat2);
            }

            // Free temporary matrices
            if (mat1_is_temp) {
                free(mat1);
            }
            if (mat2_is_temp) {
                free(mat2);
            }
            
            if (result != NULL) {
                stack[++stack_top] = result;
                is_temp[stack_top] = 1;  // Result is temporary
            }
        }
    }
    matrix_sf *final_result = NULL;
    if (stack_top >= 0) {
        final_result = stack[stack_top];
        final_result->name = name;
    }
    
    free(postfix);
    return final_result;
}

matrix_sf *execute_script_sf(char *filename) {
    if (filename == NULL) return NULL;

    FILE *file = fopen(filename, "r");
    if (file == NULL) return NULL;

    bst_sf *root = NULL;
    matrix_sf *last_matrix = NULL;

    char *line = NULL;
    size_t line_size = 0;
    
    while (getline(&line, &line_size, file) != -1) {
        // Trim trailing newline
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        // Skip empty lines
        if (strlen(line) == 0) continue;
        
        // Find the '=' sign to split name and expression
        char *eq_pos = strchr(line, '=');
        if (eq_pos == NULL) continue;
        
        // Extract matrix name (should be single character before '=')
        char name = line[0];
        
        // Move past '=' and skip whitespace
        char *expr_start = eq_pos + 1;
        while (*expr_start == ' ') expr_start++;
        
        matrix_sf *new_matrix = NULL;
        
        // Check if it's a matrix definition (starts with digit) or expression
        if (isdigit(*expr_start)) {
            // It's a matrix definition: "A = 2 3 1 2 3 4 5 6"
            new_matrix = create_matrix_sf(name, expr_start);
        } else {
            // It's an expression: "C = A*B + D'"
            new_matrix = evaluate_expr_sf(name, expr_start, root);
        }
        
        if (new_matrix != NULL) {
            // Insert into BST
            root = insert_bst_sf(new_matrix, root);
            last_matrix = new_matrix;
        }
    }
    
    free(line);
    fclose(file);
    if (last_matrix != NULL) {
        free_bst_except(root, last_matrix->name);
    } else {
        free_bst_sf(root);
    }

    return last_matrix;
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
