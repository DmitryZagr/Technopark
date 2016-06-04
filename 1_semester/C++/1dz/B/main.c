//
//  main.c
//  CalcMultiplicity
//
//  Created by Дмитрий  Загребаев on 28.02.16.
//  Copyright © 2016 Дмитрий  Загребаев. All rights reserved.
//  charset - UTF-8

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <regex.h>
#include <ctype.h>

extern int errno;

#define ERROR_OUTPUT_STR ("[error]")
#define BUFF_SIZE_FOR_100_ELEMS (100)
#define MAX_SIZE_OF_INPUT_CALK_STR (2048)
#define COUNT_OF_BRACKETS_IN_STACK (128)
#define SIZE_OF_BUF_STR (2048)
#define STACK_SIZE (128)
#define INITIAL_SIZE_MAP (10)
//коэффициент расширения map
#define EXPANSIVITY_MAP (2)

//объединение
char* union_operation(char* input_str1, char* input_str2);

//пересечение
char* traversal_operation(char* input_str1, char* input_str2);

//разность
char* difference_operation(char* input_str1, char* input_str2);

//
char* calculate(char* left_operad, char* right_operand, char operation);

//количесвто элементов в множестве
long count_elems_in_set(const char* input_str);

//преобразование char* строки в числовой long массив
//нулевой элемент == количесвто элементов в массиве
long* parse_str_to_l_arr(const char* input_str);

// выделение памяти под массив long, основываясь на строке наименьшей длины для операции пересечения
long* alloc_arr_for_traversal(const char* input_str1, const char* input_str2);

// выделение памяти под массив long для операции объединения
long* alloc_arr_for_union(const char* input_str1, const char* input_str2);

//получить max число из двух
int max_of_two_numbers(const int num1, const int num2);

// валидация вводимой строки
int validate_str(const char* input_str);

struct key_value {
    char value;
    char key;
    struct key_value* next;
};

typedef struct key_value list;

list* create_map(int count_of_elems);

list* destroy_map(list* l);

list* add_elem_in_map(list* l,char value, char key);

list* remove_elem(char key);

list* get_check_list();



struct Stack {
    int s_size;
    char* s_elems;
};

typedef struct Stack stack;

//Добавить (положить) в конец стека новый элемент
int pop(stack*, char ch);

//Извлечь из стека последний элемент 
char push(stack*);

struct Stack* create_stack();

void destroy_stack(stack* st);

//struct Operations {
//    int  priority;
//    char operation;
//    struct Operations* next;
//    char* lefOperand;
//    char* rightOperand;
//    char* result;
//} *oper_head;

//typedef struct Operations operations;

// составление списка операций
//operations* get_operation_list(char* operation_str);
//
//Создание нового узла struct Operations*
//operations* create_new_node_list(int priority, char operation, char* lefOperand, char* rightOperand);

//добавить новый узел в список struct Operations*
//int add_new_node_to_list(operations* newNode);
//
//struct Node_of_tree_objs {
//    char* lec;
//    char* result;
//    char operation;
//    struct Node_of_tree_objs* left;
//    struct Node_of_tree_objs* right;
//    struct Node_of_tree_objs* parent;
//};

//typedef struct Node_of_tree_objs node_of_tree_objs;

//конструктор по умолчанию
//node_of_tree_objs* create_default_node_of_tree_objs();

//создание конкретизированного узла
//node_of_tree_objs*  create_node_of_tree_objs(node_of_tree_objs* current_node,
//                                             char* lec, char* result, char operation,
//                                             node_of_tree_objs* left, node_of_tree_objs* right,
//                                             node_of_tree_objs* parent);

//node_of_tree_objs* buid_tree(node_of_tree_objs* root, char* input_str);

//char* get_substring_arroud_with_rounf_brakets(const char* input_str);

char* get_reverse_polish_notation(const char* input_str);

char is_operation(const char);


/*-------------------------------------------------------------------------------------------------------*/

int main(int argc, const char * argv[]) {
    
    char* intput_str_for_calc = (char*)malloc(sizeof(char) * MAX_SIZE_OF_INPUT_CALK_STR);
    
    
    
//    errno = validate_str( fgets(intput_str_for_calc, sizeof(char) * MAX_SIZE_OF_INPUT_CALK_STR - 1, stdin));
//
//    if (errno != 0) {
//        fputs("[error]", stdout);
//        return 0;
//    }
    
//    long *arr;
//    
//    fprintf(stdout, "%s", intput_str_for_calc);
//    
//    arr = parse_str_to_l_arr(intput_str_for_calc);
//    if (arr == NULL) {
//        fputs("[error]", stdout);
//    }
    
    
    char* intput_str_for_calc1 = (char*)malloc(sizeof(char) * MAX_SIZE_OF_INPUT_CALK_STR);
    
    fgets(intput_str_for_calc1, sizeof(char) * MAX_SIZE_OF_INPUT_CALK_STR - 1, stdin);
    
    char* str = get_reverse_polish_notation(intput_str_for_calc1);
    fputs(str, stdout);
    
    
//    difference_operation(intput_str_for_calc, intput_str_for_calc1);
    
    
//    fprintf(stdout, "%li", *(arr));
//    for (int i = 1; i <= *arr; i++) {
//        fprintf(stdout, "arr = %li", *(arr + i));
//    }
    
    return 0;
}

/*-------------------------------------------------------------------------------------------------------*/

//объединение
char* union_operation(char* input_str1, char* input_str2) {
    
    if (input_str1 == NULL || input_str2 == NULL) {
        return NULL;
    }
    
    long* output_arr     = NULL;
    long* input_str1_arr = NULL;
    long* input_str2_arr = NULL;
    
    output_arr     = alloc_arr_for_union(input_str1, input_str2);
    input_str1_arr = parse_str_to_l_arr(input_str1);
    input_str2_arr = parse_str_to_l_arr(input_str2);
    
    printf("__________\n");
    
    for (int i = 0; i <= *(input_str1_arr); i++) {
        printf("%li ",  *(input_str1_arr + i));
    } printf("\n");
    
    for (int i = 0; i <= *(input_str2_arr); i++) {
        printf("%li ",  *(input_str2_arr + i));
    } printf("\n");
    
    printf("__________\n");
    
//  индикатор отсутсвия элемента input_str2_arr в output_arr
    int flag = 0;
    
    if (output_arr == NULL || input_str1_arr == NULL || input_str2_arr == NULL) {
        return NULL;
    }
    
    for (long i = 0; i <= *(input_str1_arr); i++) {
        *(output_arr + i) = *(input_str1_arr + i);
    }
    
    for (long i = 1; i <= *(input_str2_arr); i++) {
        for (long j = 1; j <= *(output_arr); j++) {
            flag = 1;
            if (*(input_str2_arr + i) == *(output_arr + j)) {
                flag = 0;
                break;
            }
        }
        
        if (flag == 1) {
            *(output_arr + *(output_arr) + 1) = *(input_str2_arr + i);
            *(output_arr) = *(output_arr) + 1;
        }
        
    }
    
    
// -1, т.к. без этой опрерации *(output_arr) будет показывать общее количесвто элементов
// в массиве, включая *output_arr
//    *(output_arr) = iter_of_output_arr - 1;
    
    realloc(output_arr, sizeof(long) * (*(output_arr) + 1));
    
//    *(output_arr) = iter_of_output_arr;
    
    for (int i = 0; i <= *(output_arr) ; i++) {
        printf("%li ",  *(output_arr + i));
    } printf("\n");
    
    for (int i = 0; i <= *(input_str1_arr); i++) {
        printf("%li ",  *(input_str1_arr + i));
    } printf("\n");
    
    for (int i = 0; i <= *(input_str2_arr); i++) {
        printf("%li ",  *(input_str2_arr + i));
    } printf("\n");

    
    return 0;
}

/*-------------------------------------------------------------------------------------------------------*/

//пересечение
char* traversal_operation(char* input_str1, char* input_str2) {
    
    if (input_str1 == NULL || input_str2 == NULL) {
        return NULL;
    }
    
    long* output_arr     = NULL;
    long* input_str1_arr = NULL;
    long* input_str2_arr = NULL;
    
    output_arr     = alloc_arr_for_traversal(input_str1, input_str2);
    input_str1_arr = parse_str_to_l_arr(input_str1);
    input_str2_arr = parse_str_to_l_arr(input_str2);
    
    printf("__________\n");
    
    for (int i = 0; i <= *(input_str1_arr); i++) {
        printf("%li ",  *(input_str1_arr + i));
    } printf("\n");
    
    for (int i = 0; i <= *(input_str2_arr); i++) {
        printf("%li ",  *(input_str2_arr + i));
    } printf("\n");
    
    printf("__________\n");
    
    //  проверка на уникальность
    int flag = 0;
    
    if (output_arr == NULL || input_str1_arr == NULL || input_str2_arr == NULL) {
        return NULL;
    }
    
//    memset(output_arr,     0, sizeof(long) * *(output_arr));
    long iter_of_output_arr = 0;
    
    for (long i = 1; i <= *input_str1_arr; i++) {
        for (long j = 1; j <= *input_str2_arr; j++) {
            if (*(input_str1_arr + i) == *(input_str2_arr + j)) {
                flag = 1;
                for (long k = 1; k <= iter_of_output_arr; k++) {
                    if (*(output_arr + k) == *(input_str2_arr + j)) {
                        flag = 0;
                    }
                }
                if (flag) {
                    *(output_arr + iter_of_output_arr + 1) = *(input_str1_arr + i);
                    iter_of_output_arr ++;
                }
                
            }
        }
    }
    
    *(output_arr) = iter_of_output_arr;
    
    for (int i = 0; i <= *(output_arr); i++) {
        printf("%li ",  *(output_arr + i));
    }
    
    realloc(output_arr, sizeof(long) * (*(output_arr) + 1));
    
    return 0;
    
}

/*-------------------------------------------------------------------------------------------------------*/

//разность
char* difference_operation(char* input_str1,  char* input_str2) {
    
    if (input_str1 == NULL || input_str2 == NULL) {
        return NULL;
    }
    
    long* output_arr = (long*)malloc(sizeof(long) * count_elems_in_set(input_str1) + 1);
    long* input_str1_arr = parse_str_to_l_arr(input_str1);
    long* input_str2_arr = parse_str_to_l_arr(input_str2);
    
    if (output_arr == NULL || input_str1_arr == NULL || input_str2_arr == NULL) {
        return NULL;
    }
    
    printf("__________\n");
    
    for (int i = 0; i <= *(input_str1_arr); i++) {
        printf("%li ",  *(input_str1_arr + i));
    } printf("\n");
    
    for (int i = 0; i <= *(input_str2_arr); i++) {
        printf("%li ",  *(input_str2_arr + i));
    } printf("\n");
    
    printf("__________\n");
    
    int flag = 0;
    long itter_of_output_arr = 0;
    
    for (long i = 1; i <= *(input_str1_arr); i++) {
        for (long j = 1; j <= *(input_str2_arr); j++) {
            flag = 1;
            if (*(input_str1_arr + i) == *(input_str2_arr + j)) {
                flag = 0;
                break;
            }
        }
        
        if (flag == 1) {
            itter_of_output_arr++;
            *(output_arr) = itter_of_output_arr;
            *(output_arr + itter_of_output_arr) = *(input_str1_arr + i);
        }
    }
    
    realloc(output_arr, sizeof(long) * (*(output_arr) + 1));
    
    
    
    for (int i = 0; i <= *(output_arr) ; i++) {
        printf("%li ",  *(output_arr + i));
    } printf("\n");
    
    for (int i = 0; i <= *(input_str1_arr); i++) {
        printf("%li ",  *(input_str1_arr + i));
    } printf("\n");
    
    for (int i = 0; i <= *(input_str2_arr); i++) {
        printf("%li ",  *(input_str2_arr + i));
    }
    
    
    return 0;
}

/*-------------------------------------------------------------------------------------------------------*/

long count_elems_in_set(const char* input_str) {
    long count = 0;
    for (long i = 0; i < strlen(input_str); i++) {
        if ((*(input_str + i) >= 48 && *(input_str + i) <= 57 ) &&
                     !(*(input_str + i + 1) >= 48 && *(input_str + i + 1) <= 57 )) {
                count++;
        }
    }
    
//    printf("count = %li || %s\n", count, input_str);
    return count;
}

/*-------------------------------------------------------------------------------------------------------*/

//преобразование char* строки в числовой long массив
//нулевой элемент == количесвто элементов в массиве
long* parse_str_to_l_arr(const char* input_str) {
    
    printf("parse_str_to_l_arr\n");
    
    long count_of_elems = count_elems_in_set(input_str);
    
//    fprintf(stdout, "\nline122 %li\n", count_of_elems);
    
    long* output_arr = (long*)malloc(sizeof(long) * (count_of_elems + 1));
    memset(output_arr, 0, sizeof(long) * count_of_elems);
    
// буфер для хранения одного числа из input_str
    char buff_for_one_number[BUFF_SIZE_FOR_100_ELEMS];
    memset(buff_for_one_number, '\0', sizeof(char) * BUFF_SIZE_FOR_100_ELEMS);
    
// счетчик для buff_for_one_number
    int j = 0;
    
//  счетчик для output_arr
    int counter_of_output_arr = 1;
    
// признак числа
//    int flag = 1;
    
    if (output_arr == NULL) {
        return NULL;
    }
    
    *(output_arr) = count_of_elems;
//    memset(output_arr, 0, sizeof(long) * count_of_elems);
//    memset(buff_for_one_number, '\0', sizeof(char) * BUFF_SIZE_FOR_100_ELEMS);
    
    for (long i = 0; i < strlen(input_str); i++) {
        if (*(input_str + i) >= 48 && *(input_str + i) <= 57) {
            buff_for_one_number[j] = *(input_str + i);
//            flag = 1;
            j++;
        } else if(*buff_for_one_number != '\0'){
            buff_for_one_number[j] = '\0';
            output_arr[counter_of_output_arr] = atol(buff_for_one_number);
            counter_of_output_arr ++;
            memset(buff_for_one_number, '\0', sizeof(char) * BUFF_SIZE_FOR_100_ELEMS);
            j = 0;
        }
    }
    
    for (int i = 0; i <= *output_arr; i++) {
        printf("%li ", *(output_arr + i));
    }
    
    printf("\n");
    
    return output_arr;
}

/*-------------------------------------------------------------------------------------------------------*/

char* calculate(char* left_operad, char* right_operand, char operation) {
    
    if (left_operad == NULL || right_operand == NULL) {
        return NULL;
    }
    
    switch (operation) {
        case 'U':
            return union_operation(left_operad, right_operand);
            
        case '^':
            return traversal_operation(left_operad, right_operand);
            
        case 92 :
            return difference_operation(left_operad, right_operand);
            
        default:
            return  NULL;
    }
}


/*-------------------------------------------------------------------------------------------------------*/

// *arr == количество элементов в массиве
long* alloc_arr_for_traversal(const char* input_str1, const char* input_str2) {
    
    if (input_str1 == NULL || input_str2 == NULL) {
        return NULL;
    }
    
    long count_of_elems_in_str1 = count_elems_in_set(input_str1);
    long count_of_elems_in_str2 = count_elems_in_set(input_str2);
    long output_arr_size = 0;
    long flag_compare_str = count_of_elems_in_str1 -
                                        count_of_elems_in_str2;
    long* result_arr = NULL;
    
    if (flag_compare_str == 0) {
        
        output_arr_size = count_of_elems_in_str1;
        
        if((result_arr = (long*)malloc(sizeof(long) * output_arr_size)) == NULL) {
            return NULL;
        }
        
        memset(result_arr, 0, sizeof(long) * output_arr_size);
        *result_arr = output_arr_size;
        
    } else if(flag_compare_str < 0) {
        
        output_arr_size = count_of_elems_in_str1;
        
        if((result_arr = (long*)malloc(sizeof(long) * output_arr_size)) == NULL) {
            return NULL;
        }
        
        memset(result_arr, 0, sizeof(long) * count_of_elems_in_str1);
        *result_arr = output_arr_size;
        
        
    } else {
        
        output_arr_size = count_of_elems_in_str2;
        
        if((result_arr = (long*)malloc(sizeof(long) * output_arr_size)) == NULL) {
            return NULL;
        }
        
        memset(result_arr, 0, sizeof(long) * output_arr_size);
        *result_arr = output_arr_size;
        
    }
    
    return result_arr;
}

/*-------------------------------------------------------------------------------------------------------*/

//получить max число из двух
int max_of_two_numbers(const int num1, const int num2) {
    return num1 >= num2 ? num1 : num2;
}

/*-------------------------------------------------------------------------------------------------------*/

// выделение памяти под массив long для операции объединения
long* alloc_arr_for_union(const char* input_str1, const char* input_str2) {
    
    if (input_str1 == NULL || input_str2 == NULL) {
        return NULL;
    }
    
    long arr_size = count_elems_in_set(input_str2) + count_elems_in_set(input_str1);
    long* output_arr = (long*)malloc(sizeof(long) * (arr_size + 1));
    
    if (output_arr == NULL) {
        return NULL;
    }
    
    memset(output_arr, 0, sizeof(long) * (arr_size + 1));
    
    return output_arr;
}

/*-------------------------------------------------------------------------------------------------------*/

// валидация вводимой строки
int validate_str(const char* input_str) {
    stack *st = (stack*)malloc(sizeof(stack));
    st->s_size = 0;
    if (st == NULL) {
        return -1;
    }
    st->s_elems = (char*)malloc(sizeof(char) * COUNT_OF_BRACKETS_IN_STACK);
    if (st->s_elems == NULL) {
        return -1;
    }
    
    int i = 0;
    long count_of_nums_in_str = 0;
    long count_of_commas_in_str = 0;
    
    
    while (*(input_str + i) != '\0') {
        
        if ( (*(input_str + i) >= 48 && *(input_str + i) <= 57)) {
            count_of_nums_in_str++;
            i++;
            continue;
        }
        
        if (*(input_str + i) == 44) {
            count_of_commas_in_str++;
            i++;
            continue;
        }
        
        if (*(input_str + i) == 40 || *(input_str + i) == 41 ||
                    *(input_str + i) == 91 || *(input_str + i) == 93 ||
                    *(input_str + i) == 92 || *(input_str + i) == 94 ||
                                                    *(input_str + i) == 85) {
            
            switch (*(input_str + i)) {
                case '[':
                    if(pop(st, *(input_str + i)) == -1) return -1;
                    break;
                    
                case '(':
                    if(pop(st, *(input_str + i)) == -1) return -1;
                    break;
                    
                case ')':
                    if(push(st) == -1) return -1;
                    break;
                    
                case ']':
                    if(push(st) == -1) return -1;
                    break;
            }
            i++;
            
        } else {
            free(st->s_elems);
            free(st);
            return -1;
        }

        i++;
        
    } 
        
        
    
//    
//    while (*(input_str + i) != '\0') {
//        
//        if ( (*(input_str + i) >= 48 && *(input_str + i) <= 57) ||
//                        *(input_str + i) == 40 || *(input_str + i) == 41 ||
//                        *(input_str + i) == 44 || *(input_str + i) == 91 ||
//                        *(input_str + i) == 93 || *(input_str + i) == 92 ||
//                        *(input_str + i) == 94 || *(input_str + i) == 85 ||
//                        *(input_str + i) == '\n') {
//            
//            switch (*(input_str + i)) {
//                case '[':
//                    if(pop(st, *(input_str + i)) == -1) return -1;
//                    break;
//                    
//                case '(':
//                    if(pop(st, *(input_str + i)) == -1) return -1;
//                    break;
//                    
//                case ')':
//                    if(push(st) == -1) return -1;
//                    break;
//                    
//                case ']':
//                    if(push(st) == -1) return -1;
//                    break;
//                    
//            }
//            i++;
//            
//        } else {
//            free(st->s_elems);
//            free(st);
//            return -1;//        }
//        
//        
//    }
    
    if (st->s_size != 0 || (count_of_nums_in_str - count_of_commas_in_str) != 1) {
        free(st->s_elems);
        free(st);
        return -1;
    }
    
    free(st->s_elems);
    free(st);
    
    return 0;
}

/*-------------------------------------------------------------------------------------------------------*/

int pop(stack *st, char ch) {
    
    if (st == NULL || st->s_size >= STACK_SIZE || st->s_elems == NULL) {
        return -1;
    }
    
    *(st->s_elems + st->s_size) = ch;
    st->s_size++;
    
    return 0;
}

/*-------------------------------------------------------------------------------------------------------*/

char push(stack *st) {
    
    if (st == NULL || st->s_elems == NULL) {
        return -1;
    }
    
    if((st->s_size--) < 0)
        return -1;
    return *(st->s_elems + st->s_size);
}

/*-------------------------------------------------------------------------------------------------------*/
//
//operations* get_operation_list(char* operation_str) {
//    
//    int i = 0;
//    
//    while (*(operation_str + i) != '\0') {
//        
//        
//        i++;
//    }
//}
//
/*-------------------------------------------------------------------------------------------------------*/

operations* create_new_node_list(int priority, char operation, char* lefOperand, char* rightOperand) {
    
    operations* new_operation = (operations*)malloc(sizeof(operations));
    new_operation->priority = priority;
    new_operation->next = NULL;
    new_operation->operation = operation;
    new_operation->lefOperand = lefOperand;
    new_operation->rightOperand = rightOperand;
    
    return new_operation;
}

/*-------------------------------------------------------------------------------------------------------*/

int add_new_node_to_list(operations* newNode) {
    if (oper_head == NULL) {
        oper_head = newNode;
    } else {
        operations* oper = oper_head;
        while (oper->next) {
            oper = oper->next;
        }
        oper->next = newNode;
    }
    
    return 0;
}

/*-------------------------------------------------------------------------------------------------------*/

node_of_tree_objs* create_default_node_of_tree_objs() {
    node_of_tree_objs* new_node = (node_of_tree_objs*)malloc(sizeof(node_of_tree_objs));
    new_node->lec       = NULL;
    new_node->result    = NULL;
    new_node->operation = '\0';
    new_node->parent    = NULL;
    new_node->left      = NULL;
    new_node->right     = NULL;
    
    return new_node;
}

/*-------------------------------------------------------------------------------------------------------*/

//создание конкретизированного узла
node_of_tree_objs*  create_node_of_tree_objs(node_of_tree_objs* current_node,
                                             char* lec, char* result, char operation,
                                             node_of_tree_objs* left, node_of_tree_objs* right,
                                             node_of_tree_objs* parent) {
    if (current_node == NULL) {
        current_node = create_default_node_of_tree_objs();
    }
    
    current_node->lec = (char*)malloc(sizeof(char) * (strlen(lec) + 1));
    strcpy(current_node->lec, lec);
    
    current_node->result = (char*)malloc(sizeof(char) * (strlen(result) + 1));
    strcpy(current_node->result, result);
    
    current_node->operation = operation;
    
    current_node->left   = left;
    current_node->right  = right;
    current_node->parent = parent;
    
    return current_node;
}

/*-------------------------------------------------------------------------------------------------------*/

node_of_tree_objs* buid_tree(node_of_tree_objs* root, char* input_str) {
    
    if (root == NULL) {
        root = create_node_of_tree_objs(root, input_str, NULL, '\0', NULL, NULL, NULL);
    }
    
//    stack* st = (stack*)malloc(sizeof(char) * 1);
    
//    long lenght_of_input_str = 0;
//    lenght_of_input_str = strlen(input_str);
//    int i = 0;
//    int start_of_first_substring = 0;
//    int end_of_secong_substring  = 0;
//
//    while (*(input_str + i) != '\0') {
//        
//        
//        i++;
//    }
    
    return root;
}

/*-------------------------------------------------------------------------------------------------------*/

char* get_substring_arroud_with_rounf_brakets(const char* input_str) {
    
    if (input_str == NULL) {
        return NULL;
    }
    
    int i = 0;
    int index_of_substring_with_round_brakets = 0;
    
    long start_index = 0;
    
    char* substring_with_round_brakets = NULL;
    substring_with_round_brakets = (char*)malloc(sizeof(char) * SIZE_OF_BUF_STR);
    memset(substring_with_round_brakets, '\0', sizeof(char) * SIZE_OF_BUF_STR);
    
    while (*(input_str + i) && *(input_str + i) != ')') {
        
        
        
        if ( *(input_str + i) == '(' ) {
            start_index = i;
        }
        
        if (start_index != 0) {
            *(substring_with_round_brakets + index_of_substring_with_round_brakets) = *(input_str + i);
            index_of_substring_with_round_brakets++;
        }
        
        i++;
    }
    
    *(substring_with_round_brakets + index_of_substring_with_round_brakets) = '\0';
    
    return substring_with_round_brakets;
}

/*-------------------------------------------------------------------------------------------------------*/

char* get_reverse_polish_notation(const char* input_str) {
    
    stack* st = create_stack();
    if (st == NULL) {
        return NULL;
    }
    
    char* output_str = (char*)malloc(sizeof(char) * SIZE_OF_BUF_STR);
    if (output_str == NULL) {
        destroy_stack(st);
        return NULL;
    }
    
    int i = 0, iter_of_output_str = 0;
    char stack_char = '\0';
    
    while (*(input_str + i) != '\0') {
        
        if (isdigit(*(input_str + i)) || *(input_str + i) == 44 ||
                            *(input_str + i) == 91 || *(input_str + i) == 93 ) {
            *(output_str + iter_of_output_str) = *(input_str + i);
            iter_of_output_str++;
            i++;
            continue;
        }
        
        if (is_operation(*(input_str + i)) || *(input_str + i) == '(') {
            printf("is_operation");
            pop(st, *(input_str + i));
            i++;
            continue;
        }
        
        if (*(input_str + i) == ')') {
            while (st->s_size) {
                stack_char = push(st);
//                printf("%c\n", stack_char);
                if (stack_char != '(') {
                    printf("%c\n", stack_char);
                    *(output_str + iter_of_output_str) = stack_char;
                    iter_of_output_str++;
                } else break;
            }
        }
        
        
        i++;
    }
    
    *(output_str + iter_of_output_str) = '\0';
    
    destroy_stack(st);
    
    return output_str;
}

/*-------------------------------------------------------------------------------------------------------*/

struct Stack* create_stack() {
    
    stack* st= (stack*)malloc(sizeof(stack));
    
    if (st == NULL) {
        return NULL;
    }
    
    st->s_size = 0;
    st->s_elems = (char*)malloc(sizeof(char) * STACK_SIZE);
    
    if (st->s_elems == NULL) {
        free(st->s_elems);
        free(st);
        return NULL;
    }
    
    memset(st->s_elems, '\0', sizeof(char) * STACK_SIZE);
    
    return st;
}


/*-------------------------------------------------------------------------------------------------------*/

void destroy_stack(stack* st) {
    free(st->s_elems);
    free(st);
}

/*-------------------------------------------------------------------------------------------------------*/

char is_operation(const char ch) {
    
    switch (ch) {
        case 'U':
            return ch;
            
        case '^':
            return ch;
            
        case 92:
            return ch;
            
        default:
            return 0;
    }
    
}

/*-------------------------------------------------------------------------------------------------------*/

list* create_map() {
    
    list* l = (list*)malloc(sizeof(list) );
//    list* p = NULL;
    
    if (l == NULL) {
        return NULL;
    }
    
//    p = l;
//    
//    for (int i = 0; i < INITIAL_SIZE_MAP; i++) {
//        (p + i)->key = 0;
//        (p + i)->value = 0;
//        (p + i)->next = (p)
//    }
    
    return l;
}

/*-------------------------------------------------------------------------------------------------------*/

list* destroy_map(list* m) {
    
    list* next_l = NULL;
    list* curr_el = NULL;
    next_l = m;
    while (next_l) {
        curr_el = next_l;
        next_l = next_l->next;
        free(curr_el);
    }
    
    return m;
}

/*-------------------------------------------------------------------------------------------------------*/

list* add_elem_in_map(list* l, char value, char key) {
    
    list* new_el = (list*)malloc(sizeof(list));
    
    if (new_el == NULL) {
        return NULL;
    }
    
    list* curr_el = l;
    
    while (curr_el) {
        
        if (curr_el->key == key) {
            return NULL;
        }
        
        curr_el = curr_el->next;
        
        if (curr_el == NULL) {
            curr_el = new_el;
            curr_el->next = NULL;
            break;
        }
    }
    return l;
}

/*-------------------------------------------------------------------------------------------------------*/

list* remove_elem(char key);


/*-------------------------------------------------------------------------------------------------------*/

//list* get_check_list(){
//    list* l = create_map(10);
//    add_elem_in_map(l, '(', 0);
//    add_elem_in_map(l, ')', 1);
//    add_elem_in_map(l, 'U', 2);
////    add_elem_in_map(l, '\', 2);
//}

/*-------------------------------------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------------------------------------*/
//[][][][][][][]()()()([])

//([[][]])[(([()]))]































