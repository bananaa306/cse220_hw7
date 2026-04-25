#include "hw7.h"

bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {

    //insert
    if(root == NULL)
    {
        bst_sf *new_node = malloc(sizeof(bst_sf));
        new_node->mat = mat;
        new_node->left_child = NULL;
        new_node->right_child = NULL;
        return new_node;
    }    
    //recursive find next available
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

    //null check
    if(root == NULL)
    {
        return NULL;
    }
    //pointer of tree
    bst_sf *p = root;
    while(p && p->mat->name != name)
    {
        //move left if less than
        if(name < p->mat->name)
        {
            p = p->left_child;
            //move right if greater
        }else{
            p = p->right_child;
        }
    }
    //if p is null return null
    return p ? p->mat : NULL;
}

void free_bst_sf(bst_sf *root) {
    //null check
    if (root == NULL) return;
    //recursive from leaves to root
    free_bst_sf(root->left_child);
    free_bst_sf(root->right_child);

    if (root->mat != NULL) free(root->mat);

    free(root);
}
//helper
void free_bst_except(bst_sf *root, char preserve_name) {
    //null check
    if (root == NULL) return;
    
    free_bst_except(root->left_child, preserve_name);
    free_bst_except(root->right_child, preserve_name);
    
    // do not free if we want to keep that matrix
    if (root->mat != NULL && root->mat->name != preserve_name) {
        free(root->mat);
    }
    //free entire tree after
    free(root);
}
matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {

    //null checks
    if(mat1 == NULL || mat2 == NULL)
    {
        return NULL;
    }
    //unallowed dimensions
    if(mat1->num_rows != mat2->num_rows || mat1->num_cols != mat2->num_cols)
    {
        return NULL;
    }
    //allocate memory
    size_t count = (size_t)mat1->num_rows * mat1->num_cols;
    matrix_sf *ans = malloc(sizeof(matrix_sf)+ count * (sizeof(int)));
    if(ans == NULL)
    {
        return NULL;
    }
    //fill
    ans->name = '?';
    ans->num_rows = mat1->num_rows;
    ans->num_cols = mat2->num_cols;

    //adding
    for (size_t i = 0; i < count; i++)
    {
        ans->values[i] = mat1->values[i] + mat2->values[i];
    }
    return ans;
}

matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    //null check
    if(mat1 == NULL || mat2== NULL || mat1->num_cols != mat2->num_rows)
    {
        return NULL;
    }
    //allocate
    size_t count = (size_t)mat1->num_rows * mat2->num_cols;
    matrix_sf *ans = malloc(sizeof(matrix_sf)+ count * (sizeof(int)));
    if(ans == NULL)
    {
        return NULL;
    }
    //fill
    ans->name = '?';
    ans->num_rows = mat1->num_rows;
    ans->num_cols = mat2->num_cols;
    // i =row ,j=col, k = addition in multiplication
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
    //null check
    if(mat == NULL)
    {
        return NULL;
    }
    //alloc
    size_t count = (size_t)mat->num_rows * mat->num_cols;
    matrix_sf *ans = malloc(sizeof(matrix_sf)+ count* sizeof(int));
    if(ans == NULL)
    {
        return NULL;
    }
    //fill
    ans->name = '?';
    ans->num_rows = mat->num_cols;
    ans->num_cols = mat->num_rows;

    //i =row, j= col, flip j and i
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
    //null check
    if (name == '\0' || expr == NULL)
    {
        return NULL;
    }
    //pointer to each parse
    const char *p = expr;
    char *end = NULL;
    //row
    long r = strtol(p, &end, 10);
    if (end == p || r <= 0) return NULL;
    p = end;
    //col
    long c = strtol(p, &end, 10);
    if (end == p || c <= 0) return NULL;
    p = end;

    //alloc
    size_t rows = (size_t)r;
    size_t cols = (size_t)c;
    size_t count = rows * cols;
    matrix_sf *m = malloc(sizeof(*m) + count * sizeof(m->values[0]));
    if (m == NULL)
    {
        return NULL;
    }

//fill
    m->name = name;
    m->num_rows = (unsigned int)rows;
    m->num_cols = (unsigned int)cols;

    //matrix
    while (*p && *p != '[') p++;
    if (*p == '[') p++; 
    
    for (size_t i = 0; i < count; i++)
    {
        //parse each value
        long v =strtol(p,&end,10);
        if(end ==p)
        {
            free(m);
            return NULL;
        }
        //add value
        m->values[i] = (int)v;
        p =end;
        //space check
        while (*p && (*p == ' ' || *p == ';' || *p == '\n' || *p == '\t')) p++;
    }
    return m;
}

char* infix2postfix_sf(char *infix) {
    //null
    if (infix == NULL) return NULL;

    //stack
    char stack[256];
    int stack_top = -1;

    //allocate mem
    char *result = malloc(strlen(infix) + 1);
    if (result == NULL) return NULL;
    int result_idx = 0;


    for (int i = 0; infix[i] != '\0'; i++)
    {
        char c = infix[i];
        
        if (c == ' ') continue; //space check

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
                //ordering precedence
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

                //pop if precendence is higher
                if (top_op != '\'' && top_prec >= cur_prec) {
                    result[result_idx++] = stack[stack_top--];
                } 
                else 
                {
                    break;
                }
            }
            //push to stack
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
            //anything after paren
            while (i + 1 < (int)strlen(infix) && infix[i + 1] == '\'') 
            {
                result[result_idx++] = '\'';
                i++;
            }
        } 
    }
    //pop rest
    while (stack_top >= 0) 
    {
        result[result_idx++] = stack[stack_top--];
    }
    //null for end of string
    result[result_idx] = '\0';
    return result;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    //null check
    if(expr == NULL || root == NULL){return NULL;}

    char *postfix = infix2postfix_sf(expr);
    if (postfix == NULL) return NULL;

    //stack
    matrix_sf *stack[256];
    int stack_top = -1;
    

    //which stacks should be freed and shouldnt
    int is_temp[256] = {0};
    
    for (int i = 0; postfix[i] != '\0'; i++) 
    {
        char c = postfix[i];
        //find name
        if (isalpha(c)) {
            matrix_sf *mat = find_bst_sf(c, root);
            if (mat != NULL) {
                stack[++stack_top] = mat;
                is_temp[stack_top] = 0;  // do not free
            }
        }
        else if (c == '\'') 
        {
            // pop ,transpose, push
            if (stack_top < 0) {
                free(postfix);
                return NULL;
            }
            matrix_sf *mat = stack[stack_top];
            int mat_is_temp = is_temp[stack_top];
            stack_top--;
            
            matrix_sf *result = transpose_mat_sf(mat);
            
            //free if temp
            if (mat_is_temp) {
                free(mat);
            }
            
            //poush
            if (result != NULL) {
                stack[++stack_top] = result;
                is_temp[stack_top] = 1;  // temp result
            }
        }
        else if (c == '+' || c == '*') {
            if (stack_top < 1) {
                free(postfix);
                return NULL;
            }
            //pop1
            matrix_sf *mat2 = stack[stack_top];
            int mat2_is_temp = is_temp[stack_top];
            stack_top--;
            //pop2
            matrix_sf *mat1 = stack[stack_top];
            int mat1_is_temp = is_temp[stack_top];
            stack_top--;

            //operate
            matrix_sf *result = NULL;
            if (c == '+') {
                result = add_mats_sf(mat1, mat2);
            } else {
                result = mult_mats_sf(mat1, mat2);
            }

            // free temp matricies
            if (mat1_is_temp) {
                free(mat1);
            }
            if (mat2_is_temp) {
                free(mat2);
            }
            
            //push
            if (result != NULL) {
                stack[++stack_top] = result;
                is_temp[stack_top] = 1;  //temp result
            }
        }
    }
    //final left with one matrix
    matrix_sf *final_result = NULL;
    if (stack_top >= 0) {
        final_result = stack[stack_top];
        final_result->name = name;
    }
    
    //free temp
    free(postfix);

    return final_result;
}

matrix_sf *execute_script_sf(char *filename) {
    //null check
    if (filename == NULL) return NULL;

    FILE *file = fopen(filename, "r");
    if (file == NULL) return NULL;

    bst_sf *root = NULL;
    matrix_sf *last_matrix = NULL;

    char *line = NULL;
    size_t line_size = 0;
    //line check
    while (getline(&line, &line_size, file) != -1) {
        //remove newline
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        // Skip empty lines
        if (strlen(line) == 0) continue;
        
        // find '='
        char *eq_pos = strchr(line, '=');
        if (eq_pos == NULL) continue;
        
        // find matrix name and expression
        char name = line[0];
        
        // go past = and spaces
        char *expr_start = eq_pos + 1;
        while (*expr_start == ' ') expr_start++;
        
        matrix_sf *new_matrix = NULL;
        
        // check if matrix or defenition
        if (isdigit(*expr_start)) {
            //matrix
            new_matrix = create_matrix_sf(name, expr_start);
        } else {
            //expression
            new_matrix = evaluate_expr_sf(name, expr_start, root);
        }
        
        if (new_matrix != NULL) {
            //insert
            root = insert_bst_sf(new_matrix, root);
            last_matrix = new_matrix;
        }
    }
    //cleanup
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
