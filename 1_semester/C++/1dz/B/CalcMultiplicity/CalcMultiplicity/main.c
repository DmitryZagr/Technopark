//Разработайте программу-калькулятор, умеющую вычислять арифметические выражения
//над множествами натуральных чисел.
//Входные данные (выражение с описанием множеств и производимых над ними операций)
//подаются на стандартный поток ввода программы,
//результат вычислений должен подаваться на стандартный поток вывода.
//
//Поддерживаемые операции: 'U' - объединение, '^' - пересечение,
//  '\' - разность множеств, '()' - задание приоритета вычислений.
//Множества задаются в виде [ el1, el2, ... ], где элементами являются обычные натуральные числа.
//Множества могут быть пустыми.
//Между операциями, множествами и элементами множеств может быть любое количество пробелов.
//
//При вычислениях должны учитываться приоритеты операций(в том числе,
//                                    заданные вручную при помощи круглых скобочек).
//
//Результирующее множество должно выводиться без пробелов, в отсортированном порядке.
//В случае, если введенное выражение содержит ошибку,
//необходимо вывести в поток стандартного вывода сообщение "[error]" и завершить выполнение программы.
//
//ВАЖНО! Программа в любом случае должна возвращать 0. Не пишите return -1, exit(1) и т.п.
//Даже если обнаружилась какая-то ошибка, все равно необходимо вернуть 0! (и напечатать [error] в stdout).

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <regex.h>
#include <ctype.h>
#include <stdint.h>

extern int errno;

#define ERROR_OUTPUT_STR ("[error]")
#define BUFF_SIZE_FOR_100_ELEMS (128)
#define MAX_SIZE_OF_INPUT_CALK_STR (2048)
#define LENGHT_STR_IN_STACK_STR (2048)
#define SIZE_OF_BUF_STR (2048)
#define STACK_SIZE (256)


//объединение
char* union_operation(char* input_str1, char* input_str2);

//пересечение
char* traversal_operation(char* input_str1, char* input_str2);

//разность
char* difference_operation(char* input_str1, char* input_str2);

//
char* calculate_one_operation(char* left_operad, char* right_operand, char operation);

//
char* calculate(const char* reverse_polish_notation);

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

char is_correct_symbol(const char ch);

char is_open_bracket(const char ch);

char is_close_bracket(const char ch);

char* parse_l_arr_to_str(long* input_arr);

struct Stack_ch {
    size_t max_size;
    int s_size;
    char* s_elems;
};

typedef struct Stack_ch stack_ch;

//Добавить (положить) в конец стека новый элемент
int push_ch(stack_ch**, char ch);

//Извлечь из стека последний элемент 
char pop_ch(stack_ch*);

stack_ch* create_stack_ch();

void destroy_stack_ch(stack_ch* st);

int is_empty_stack(const stack_ch* st);

struct Stack_str {
//    int s_size;
    char* s_elems;
    struct Stack_str *next;
};

typedef struct Stack_str stack_str;

//Добавить (положить) в конец стека новый элемент
int push_str(stack_str** st, const char* str);

//Извлечь из стека последний элемент
char* pop_str(stack_str**);

//stack_str* create_stack_str();

void destroy_stack_str(stack_str* st);

int is_empty_stack_str(const stack_str* st);

char* get_reverse_polish_notation(const char* input_str);

char is_operation(const char);

char get_operation_prioirity(const char operation);

char* sort_answer(char* input_str);

int funccmp( const void * val1, const void * val2 );


/*------------------------------------------------------------------------------------------------*/

int main(int argc, const char * argv[]) {
    
    fflush(stdout);
    fflush(stdin);
    
    char* intput_str_for_calc = (char*)malloc(sizeof(char) * MAX_SIZE_OF_INPUT_CALK_STR);
    
    if (intput_str_for_calc == NULL) {
        fputs(ERROR_OUTPUT_STR, stdout);
        return 0;
    }
    
    if (fgets(intput_str_for_calc, sizeof(char) * MAX_SIZE_OF_INPUT_CALK_STR, stdin) == NULL) {
        fputs( ERROR_OUTPUT_STR, stdout);
        free(intput_str_for_calc);
        return 0;
    }
    
    if (!validate_str(intput_str_for_calc)) {
        free(intput_str_for_calc);
        fputs( ERROR_OUTPUT_STR, stdout);
        return 0;
    }

    char* reverse_polish_notation = get_reverse_polish_notation(intput_str_for_calc);
    
    if (reverse_polish_notation == NULL) {
        fputs( ERROR_OUTPUT_STR, stdout);
        free(intput_str_for_calc);
        return 0;
    }
    
    char* result = calculate(reverse_polish_notation);
    
    if (result == NULL) {
        fputs( ERROR_OUTPUT_STR, stdout);
        free(intput_str_for_calc);
        free(reverse_polish_notation);
        return 0;
    }
    
    char* sort_result = sort_answer(result);
    
    if (sort_result == NULL) {
        free(intput_str_for_calc);
        free(reverse_polish_notation);
        fputs( ERROR_OUTPUT_STR, stdout);
        return 0;
    }
    
    fputs(sort_result, stdout);
    
    free(result);
    free(sort_result);
    free(intput_str_for_calc);
    free(reverse_polish_notation);
    
    fflush(stdout);
    fflush(stdin);
    
    return 0;
}

/*------------------------------------------------------------------------------------------------*/

//объединение
char* union_operation(char* input_str1, char* input_str2) {
    
//    printf("\nUNION\n");
//    printf("%s & %s\n" , input_str1, input_str2);
    
    if (input_str1 == NULL || input_str2 == NULL) {
//        printf("envwen");
        return NULL;
    }
    
//    printf("%s %s", input_str1, input_str2);
    
    if (strcmp(input_str1, "[]") == 0) {
        char* output_str = (char*)malloc(sizeof(char) * SIZE_OF_BUF_STR);
        memset(output_str, '\0', SIZE_OF_BUF_STR * sizeof(char));
        output_str = strcpy(output_str, input_str2);
        return output_str ;

    }
    
    if (strcmp(input_str2, "[]") == 0) {
        char* output_str = (char*)malloc(sizeof(char) * SIZE_OF_BUF_STR);
        memset(output_str, '\0', SIZE_OF_BUF_STR * sizeof(char));
        output_str = strcpy(output_str, input_str1);
        return output_str ;
    }
    
    long* output_arr     = NULL;
    long* input_str1_arr = NULL;
    long* input_str2_arr = NULL;
    
    output_arr     = alloc_arr_for_union(input_str1, input_str2);
    input_str1_arr = parse_str_to_l_arr(input_str1);
    input_str2_arr = parse_str_to_l_arr(input_str2);
    
    
//  индикатор отсутсвия элемента input_str2_arr в output_arr
    int flag = 0;
    
    if (output_arr == NULL || input_str1_arr == NULL || input_str2_arr == NULL) {
        return NULL;
    }
    
//    printf("%li" , strlen(input_str2_arr));
    
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
    
    if (*(output_arr) == 0) {
        free(output_arr);
        free(input_str1_arr);
        free(input_str2_arr);
        char* output_str = (char*)malloc(sizeof(char) * SIZE_OF_BUF_STR);
        memset(output_str, '\0', SIZE_OF_BUF_STR * sizeof(char));
        output_str = strcpy(output_str, "[]");
        return output_str ;
//        return "[]";
    }
    
    char* result_str = parse_l_arr_to_str(output_arr);
    
    if (result_str == NULL) {
        free(output_arr);
        free(input_str1_arr);
        free(input_str2_arr);
        return NULL;
    }
    
    free(output_arr);
    free(input_str1_arr);
    free(input_str2_arr);
    
    return result_str;
}

/*------------------------------------------------------------------------------------------------*/

//пересечение
char* traversal_operation(char* input_str1, char* input_str2) {
//    printf("\nPERSECHENIE\n");
//    printf("%s & %s\n" , input_str1, input_str2);

    
    if (input_str1 == NULL || input_str2 == NULL) {
        return NULL;
    }
    
    if (strcmp("[]", input_str1) == 0 || strcmp("[]", input_str2) == 0 ) {
        char* output_str = (char*)malloc(sizeof(char) * SIZE_OF_BUF_STR);
        memset(output_str, '\0', SIZE_OF_BUF_STR * sizeof(char));
        output_str = strcpy(output_str, "[]");
        return output_str;
    }
    
//    if (strcmp("[]", input_str2) == 0 /*|| strcmp("[]", input_str2) == 0 */) {
//        char* output_str = (char*)malloc(sizeof(char) * SIZE_OF_BUF_STR);
//        memset(output_str, '\0', SIZE_OF_BUF_STR * sizeof(char));
//        output_str = strcpy(output_str, input_str1);
//        return output_str;
//    }
    
    long* output_arr     = NULL;
    long* input_str1_arr = NULL;
    long* input_str2_arr = NULL;
    
    output_arr     = alloc_arr_for_traversal(input_str1, input_str2);
    input_str1_arr = parse_str_to_l_arr(input_str1);
    input_str2_arr = parse_str_to_l_arr(input_str2);
    
    //  проверка на уникальность
    int flag = 0;
    
    if (output_arr == NULL || input_str1_arr == NULL || input_str2_arr == NULL) {
        return NULL;
    }

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
    
    if (*(output_arr) == 0) {
        free(output_arr);
        free(input_str1_arr);
        free(input_str2_arr);
        char* output_str = (char*)malloc(sizeof(char) * SIZE_OF_BUF_STR);
        memset(output_str, '\0', SIZE_OF_BUF_STR * sizeof(char));
        output_str = strcpy(output_str, "[]");
        return output_str ;
//        return "[]";
    }
    
    char* result_str = parse_l_arr_to_str(output_arr);
    
    if (result_str == NULL) {
        free(output_arr);
        free(input_str1_arr);
        free(input_str2_arr);
        return NULL;
    }
    
    free(output_arr);
    free(input_str1_arr);
    free(input_str2_arr);
    
    return result_str;
    
}

/*------------------------------------------------------------------------------------------------*/

//разность
char* difference_operation(char* input_str1,  char* input_str2) {
//    printf("\nRAZNOST\n");
//    printf("%s & %s\n" , input_str1, input_str2);

    if (input_str1 == NULL || input_str2 == NULL) {
        return NULL;
    }
    
    if (strcmp("[]", input_str1) == 0) {
        char* output_str = (char*)malloc(sizeof(char) * SIZE_OF_BUF_STR);
        memset(output_str, '\0', SIZE_OF_BUF_STR * sizeof(char));
        output_str = strcpy(output_str, "[]");
        return output_str ;
//        return "[]";
    }
    
    if (strcmp("[]", input_str2) == 0) {
        char* output_str = (char*)malloc(sizeof(char) * SIZE_OF_BUF_STR);
        memset(output_str, '\0', SIZE_OF_BUF_STR * sizeof(char));
        strcpy(output_str, input_str1);
//        output_str = strcpy(output_str, "[]");
        return output_str ;
        //        return "[]";
    }
    
    if (strcmp("[]", input_str2) == 0) {
        char* output_str = (char*)malloc(sizeof(char) * SIZE_OF_BUF_STR);
        memset(output_str, '\0', SIZE_OF_BUF_STR * sizeof(char));
        strcpy(output_str, input_str1);
        return output_str;
    }
    
    
    
    long* output_arr = (long*)malloc(sizeof(long) * (count_elems_in_set(input_str1) + 1));
    memset(output_arr, 0, sizeof(long) * (count_elems_in_set(input_str1) + 1));
    long* input_str1_arr = parse_str_to_l_arr(input_str1);
    long* input_str2_arr = parse_str_to_l_arr(input_str2);
    
    if (output_arr == NULL || input_str1_arr == NULL || input_str2_arr == NULL) {
        return NULL;
    }
    
    size_t flag = 0;
    size_t itter_of_output_arr = 0;
    
    for (size_t i = 1; i <= *(input_str1_arr); i++) {
        for (size_t j = 1; j <= *(input_str2_arr); j++) {
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
    
    fflush(stdout);
    
    char* result_str = parse_l_arr_to_str(output_arr);
    
    if (result_str == NULL) {
        free(output_arr);
        free(input_str1_arr);
        free(input_str2_arr);
        return NULL;
    }
    
    free(output_arr);
    free(input_str1_arr);
    free(input_str2_arr);
    
    return result_str;
}

/*------------------------------------------------------------------------------------------------*/

long count_elems_in_set(const char* input_str) {
    long count = 0;
    for (long i = 0; i < strlen(input_str); i++) {
        if ((*(input_str + i) >= 48 && *(input_str + i) <= 57 ) &&
                     !(*(input_str + i + 1) >= 48 && *(input_str + i + 1) <= 57 )) {
                count++;
        }
    }

    return count;
}

/*------------------------------------------------------------------------------------------------*/

//преобразование char* строки в числовой long массив
//нулевой элемент == количесвто элементов в массиве
long* parse_str_to_l_arr(const char* input_str) {
    
    long count_of_elems = count_elems_in_set(input_str);
    
    long* output_arr = (long*)malloc(sizeof(long) * (count_of_elems + 1));
    
    if (output_arr == NULL) {
        return NULL;
    }
    
    memset(output_arr, 0, sizeof(long) * count_of_elems);
    
    if ((*(output_arr) = count_of_elems) == 0) {
        return output_arr;
    }
    
    
// буфер для хранения одного числа из input_str
    char buff_for_one_number[BUFF_SIZE_FOR_100_ELEMS];
    memset(buff_for_one_number, '\0', sizeof(char) * BUFF_SIZE_FOR_100_ELEMS);
    
// счетчик для buff_for_one_number
    int j = 0;
    
//  счетчик для output_arr
    int counter_of_output_arr = 1;
    
    
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
    
    return output_arr;
}

/*------------------------------------------------------------------------------------------------*/

char* calculate_one_operation(char* left_operad, char* right_operand, char operation) {
    
    if (left_operad == NULL || right_operand == NULL) {
        return NULL;
    }
    
    char* result = NULL;
    
    switch (operation) {
            
        case 'U':
            result = union_operation(left_operad, right_operand);
            return result;
            
        case '^':
            result = traversal_operation(left_operad, right_operand);
            return result;
            
        case 92 :
            result = difference_operation(left_operad, right_operand);
            return result;
            
        default:
            return  NULL;
    }
}


/*------------------------------------------------------------------------------------------------*/

char* calculate(const char* reverse_polish_notation) {
//    long polist_str_lenght = strlen(reverse_polish_notation);
    char buf_for_left_operand[SIZE_OF_BUF_STR];
    char buf_for_right_operand[SIZE_OF_BUF_STR];
    char calc_str[SIZE_OF_BUF_STR];
    char buf[SIZE_OF_BUF_STR];
    char *error_flag = NULL;
    
    memset(buf_for_left_operand, '\0', sizeof(char) * SIZE_OF_BUF_STR);
    memset(buf_for_right_operand, '\0', sizeof(char) * SIZE_OF_BUF_STR);
    memset(calc_str, '\0', sizeof(char) * SIZE_OF_BUF_STR);
    memset(buf, '\0', sizeof(char) * SIZE_OF_BUF_STR);
    
    stack_str *st = NULL;
    char operation = 0;
    size_t i = 0, iter_of_buf_str = 0;
    
    while (*(reverse_polish_notation + i ) != '\0') {
        
        if (is_operation( *(reverse_polish_notation + i) )) {
            operation = *(reverse_polish_notation + i);
            error_flag = pop_str(&st);
            if (error_flag == NULL) return NULL;
            strcpy(buf_for_right_operand, error_flag);
            if (error_flag != NULL) free(error_flag);
            error_flag = pop_str(&st);
            if (error_flag == NULL) return NULL;
            strcpy(buf_for_left_operand, error_flag);
            if (error_flag != NULL) free(error_flag);
            
            char *calc_one_oper = calculate_one_operation(buf_for_left_operand,
                                                buf_for_right_operand, operation);

            
            strcpy(calc_str, calc_one_oper/*calculate_one_operation(buf_for_left_operand,
                                                     buf_for_right_operand, operation)*/) ;
            
            if(push_str(&st, calc_str) == -1) {
                destroy_stack_str(st);
                if (calc_one_oper) free(calc_one_oper);
                return NULL;
            }
            if (calc_one_oper != NULL) {
                free(calc_one_oper);
            }
            calc_one_oper = NULL;
            i++;
            continue;
        }
        buf[iter_of_buf_str] = *(reverse_polish_notation + i);
        iter_of_buf_str++;
        
        if (*(reverse_polish_notation + i) == ']') {
            buf[iter_of_buf_str] = '\0';
            if(push_str(&st, buf) == -1) {
                destroy_stack_str(st);
                return NULL;
            }
            iter_of_buf_str = 0;
            memset(buf, '\0', sizeof(char) * SIZE_OF_BUF_STR);
        }
        i++;
    }
    
    char* result_str = NULL;
    result_str = (char*)malloc(sizeof(char)* SIZE_OF_BUF_STR);
    strcpy(result_str, calc_str);
    
    destroy_stack_str(st);
    
    return result_str;
}

/*------------------------------------------------------------------------------------------------*/

// *arr == количество элементов в массиве
long* alloc_arr_for_traversal(const char* input_str1, const char* input_str2) {
    
    if (input_str1 == NULL || input_str2 == NULL) {
        return NULL;
    }
    
//    printf("%s %s\n", input_str1, input_str2);
    
    long count_of_elems_in_str1 = count_elems_in_set(input_str1) + 1;
    long count_of_elems_in_str2 = count_elems_in_set(input_str2) + 1;
    long output_arr_size = 0;
    long flag_compare_str = count_of_elems_in_str1 -
                                        count_of_elems_in_str2;
    long* result_arr = NULL;
    
    if (flag_compare_str == 0) {
        
        output_arr_size = count_of_elems_in_str1;
//        printf("%li\n", output_arr_size);
        
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
//        printf("%li\n", output_arr_size);
        
    } else {
        
        output_arr_size = count_of_elems_in_str2;
        
        if((result_arr = (long*)malloc(sizeof(long) * output_arr_size )) == NULL) {
            return NULL;
        }
        
        memset(result_arr, 0, sizeof(long) * output_arr_size);
        *result_arr = output_arr_size;
        
    }
    
    return result_arr;
}

/*------------------------------------------------------------------------------------------------*/

//получить max число из двух
int max_of_two_numbers(const int num1, const int num2) {
    return num1 >= num2 ? num1 : num2;
}

/*------------------------------------------------------------------------------------------------*/

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

/*------------------------------------------------------------------------------------------------*/

// валидация вводимой строки
int validate_str(const char* input_str) {
    
    size_t i = 0;
    char st_ch = 0;
    char flag = 0;;
    stack_ch *st = create_stack_ch();
    
    while (*(input_str + i) != '\0') {
        if (!is_correct_symbol(*(input_str + i))) {
            destroy_stack_ch(st);
            return 0;
        }
        
        if (is_open_bracket(*(input_str + i))) {
            if ( push_ch(&st, *(input_str + i)) == -1 ) {destroy_stack_ch(st);}
            i++;
            continue;
        }
        
        if ((flag =  is_close_bracket(*(input_str + i)))) {
            
            if (is_empty_stack(st)) { destroy_stack_ch(st);  return 0; }
            
            st_ch = pop_ch(st);
            
            if (flag == ')') {
                flag = flag - 1; /// '('
            } else flag -= 2 ;// '['
            
            if (flag != st_ch) { destroy_stack_ch(st); return 0;}
            
            i++;
            continue;
        }
        
        i++;
    }
    
    if (!is_empty_stack(st)) {
        destroy_stack_ch(st);
        return 0;
    }
    
    destroy_stack_ch(st);
     
    return 1;
            
}

/*------------------------------------------------------------------------------------------------*/

char is_correct_symbol(const char ch) {
    
    if (isdigit(ch) || ch == ',') {
        return 1;
    }
    
    if (ch == ' ' || is_operation(ch) || ch == '[' || ch == ']'
                            || ch == '(' || ch == ')' || ch == '\n') {
        return 1;
    }
    
    return 0;
}

/*------------------------------------------------------------------------------------------------*/

char is_open_bracket(const char ch) {
    
    if (ch == '['  || ch == '(' ) {
        return 1;
    }
    
    return 0;
}

/*------------------------------------------------------------------------------------------------*/

char is_close_bracket(const char ch) {
    
    switch (ch) {
        case ')':
            return ')';
        case ']':
            return ']';
            
        default:
            return 0;
    }
}

/*------------------------------------------------------------------------------------------------*/

int push_ch(stack_ch **st, char ch) {
    
    if (*st == NULL || (*st)->s_elems == NULL) {
        return -1;
    }
    
    if ((*st)->s_size >= (*st)->max_size) {
        (*st)->max_size *= 2;
        stack_ch *buf = *st;
        (*st) = realloc(*st, sizeof(char) * (*st)->max_size );
        if (*(st) == NULL) {
            free(buf);
            return -1;
        }
    }
    
    *((*st)->s_elems + (*st)->s_size) = ch;
    (*st)->s_size++;
    
    return 0;
}

/*------------------------------------------------------------------------------------------------*/

char pop_ch(stack_ch *st) {
    
    if (st == NULL || st->s_elems == NULL) {
        return -1;
    }
    
    if((st->s_size--) < 0)
        return -1;
    return *(st->s_elems + st->s_size);
}

/*------------------------------------------------------------------------------------------------*/

int is_empty_stack(const stack_ch* st) {
    return st->s_size == 0 ? 1 : 0;
}

/*------------------------------------------------------------------------------------------------*/

char* get_reverse_polish_notation(const char* input_str) {
    
    stack_ch* st = create_stack_ch();
    if (st == NULL) {
        return NULL;
    }
    
    char* output_str = (char*)malloc(sizeof(char) * SIZE_OF_BUF_STR);
    if (output_str == NULL) {
        destroy_stack_ch(st);
        return NULL;
    }
    
    size_t i = 0, iter_of_output_str = 0;
    char stack_char = '\0';
    
    while (*(input_str + i) != '\0') {
        
        if (isdigit(*(input_str + i)) || *(input_str + i) == 44 ||
                            *(input_str + i) == 91 || *(input_str + i) == 93 ) {
            *(output_str + iter_of_output_str) = *(input_str + i);
            iter_of_output_str++;
            i++;
            continue;
        }
        
        if (is_operation(*(input_str + i)) || *(input_str + i) == '(' || *(input_str + i) == ')') {
            
            //если стек пуст, то опеpация из входной стpоки пеpеписывается в стек;
            if (is_empty_stack(st)) {
                if(push_ch(&st, *(input_str + i)) == -1) {destroy_stack_ch(st);}
                i++;
                continue;
            }
//            опеpация выталкивает из стека все опеpации с большим или
//            pавным пpиоpитетом в выходную стpоку;
            else {
                
                while (!is_empty_stack(st)) {
                    
                    if((stack_char = pop_ch(st)) == -1) return NULL;
                    
//                    printf("|%c %i |\n", stack_char, st->s_size);
                    if ((get_operation_prioirity(*(input_str + i)) <=
                                    get_operation_prioirity(stack_char)) &&
                                            is_operation(stack_char) &&
                                            is_operation(*(input_str + i))) {
                        *(output_str + iter_of_output_str) = stack_char;
                        iter_of_output_str++;
                    } else {
//                        если условие не выполнилось,
//                        то возвращаем символ в стек и выходим из цикла
                        if(push_ch(&st, stack_char) == -1 ) {destroy_stack_ch(st);}
                        break;
                    }
                }
                if (*(input_str + i) != ')') {
                    if(push_ch(&st, *(input_str + i)) == -1) {destroy_stack_ch(st);}
                    i++;
                    continue;
                }
                
            }
            
//            закpывающая кpуглая скобка выталкивает все опеpации из стека
//            до ближайшей откpывающей скобки,
//            сами скобки в выходную стpоку не пеpеписываются,
//            а уничтожают дpуг дpуга
            
            if (*(input_str + i) == ')') {
                while (1) {
                    if(( stack_char = pop_ch(st)) == -1) {destroy_stack_ch(st); return NULL;}
                    if (stack_char == '(') {
                        break;
                    }
                    *(output_str + iter_of_output_str) = stack_char;
                    iter_of_output_str++;
                    if (iter_of_output_str == SIZE_MAX) {
                        destroy_stack_ch(st);
                        return NULL;
                    }
                }
            }
            
            
        }

        i++;
    }
    
    while (!is_empty_stack(st)) {
        *(output_str + iter_of_output_str) = pop_ch(st);
        if (*(output_str + iter_of_output_str) == '(') {destroy_stack_ch(st); return NULL;}
        iter_of_output_str++;
        
        if (iter_of_output_str == SIZE_MAX) {
            destroy_stack_ch(st);
            return NULL;
        }
    }
    
    *(output_str + iter_of_output_str) = '\0';
    
    destroy_stack_ch(st);
    
    return output_str;
}

/*------------------------------------------------------------------------------------------------*/

stack_ch* create_stack_ch() {
    
    stack_ch* st= (stack_ch*)malloc(sizeof(stack_ch));
    
    if (st == NULL) {
        return NULL;
    }
    
    st->s_size = 0;
    st->max_size = STACK_SIZE;
    st->s_elems = (char*)malloc(sizeof(char) * STACK_SIZE);
    
    if (st->s_elems == NULL) {
        free(st);
        return NULL;
    }
    
    memset(st->s_elems, '\0', sizeof(char) * STACK_SIZE);
    
    return st;
}


/*------------------------------------------------------------------------------------------------*/

void destroy_stack_ch(stack_ch* st) {
    
    if (st != NULL) {
        if (st->s_elems != NULL) free(st->s_elems);
        free(st);
    }
}

/*------------------------------------------------------------------------------------------------*/

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

/*------------------------------------------------------------------------------------------------*/

char get_operation_prioirity(const char operation) {
    switch (operation) {
        case '(':
            return 0;
        case ')':
            return 1;
        case 'U':
            return 2;
        case 92:
            return 2;
        case '^':
            return 3;
//        если символ не является ни одной из приведенных выше операций
        default:
            return -1;
    }
}

/*------------------------------------------------------------------------------------------------*/

//Добавить (положить)  новый элемент
int push_str(stack_str** st,const char* str) {
    
    stack_str *tmp = (stack_str*) malloc(sizeof(stack_str));
    if (tmp == NULL) {
        return -1;
    }
    tmp->s_elems = (char*)malloc(sizeof(char) * LENGHT_STR_IN_STACK_STR);
    if (tmp->s_elems == NULL) {
        free(tmp);
        return -1;
    }
    strcpy(tmp->s_elems, str);
    tmp->next = (*st);
    (*st) = tmp;
//    count_elems_in_st_str++;
//    printf("pop\n");
    return 1;
}

/*------------------------------------------------------------------------------------------------*/

//Извлечь из стека  элемент
char* pop_str(stack_str** st) {
    
    stack_str* prev = NULL;
    char* result_str;
    
    if (*st == NULL) {
        return NULL;
    }
    
    result_str = (char*)malloc(sizeof(char) * LENGHT_STR_IN_STACK_STR);
    prev = (*st);
    strcpy(result_str, prev->s_elems);
    (*st) = (*st)->next;
//    count_elems_in_st_str--;
    if (prev->s_elems != NULL && strlen(prev->s_elems) != 0) {
        free(prev->s_elems);
    }
    free(prev);
    
//    printf("push\n");
    
    return result_str;
}

/*------------------------------------------------------------------------------------------------*/

void destroy_stack_str(stack_str* st) {
    stack_str* s = st;
    while (s) {
        s = s->next;
        free(st->s_elems);
        free(st);
        st = s;
    }
    
//    printf("destroy\n");
}

/*------------------------------------------------------------------------------------------------*/

int is_empty_stack_str(const stack_str* st) {
    
    if (st == NULL) {
        return 1;
    }
    return 0;
}

/*------------------------------------------------------------------------------------------------*/

char* parse_l_arr_to_str(long* input_arr) {
    if (input_arr == NULL) {
        fputs(ERROR_OUTPUT_STR, stdout);
    }
    
    if (*(input_arr) == 0) {
        char* output_str = (char*)malloc(sizeof(char) * SIZE_OF_BUF_STR);
        memset(output_str, '\0', SIZE_OF_BUF_STR * sizeof(char));
        output_str = strcpy(output_str, "[]");
        return output_str ;
    }
    
    char* output_str = NULL;
    
    output_str = (char*) malloc(SIZE_OF_BUF_STR);
    
    if (output_str == NULL) {
        return NULL;
    }
    
    memset(output_str, '\0', SIZE_OF_BUF_STR);
    
    char buf[SIZE_OF_BUF_STR];
    memset(buf, '\0', SIZE_OF_BUF_STR);
    
    *(output_str) = '[';
    
    size_t lenght_of_output_str = strlen(output_str);
    
    for (int i = 1; i <= *input_arr; i++) {
        
        if (i == SIZE_OF_BUF_STR - 1) {
            free(output_str);
            return NULL;
        }
        
        sprintf(buf, "%li", *(input_arr + i));
        int j = 0;
        
        while (buf[j] != '\0') {
            *(output_str + lenght_of_output_str) = buf[j];
            lenght_of_output_str++;
            j++;
        }
        *(output_str + lenght_of_output_str) = ',';
        lenght_of_output_str++;
    }
    
    *(output_str + lenght_of_output_str - 1) = ']';
    *(output_str + lenght_of_output_str ) = '\0';
    
    return output_str;

}

/*------------------------------------------------------------------------------------------------*/

char* sort_answer(char* input_str) {
    char* result_str = NULL;
//    memset(result_str, '\0', sizeof(char) * SIZE_OF_BUF_STR);
    long* buff_arr;
    
    buff_arr = parse_str_to_l_arr(input_str);
    
    qsort(buff_arr + 1, *(buff_arr)  , sizeof(long), funccmp);
    
    result_str = parse_l_arr_to_str(buff_arr);
    
    
    free(buff_arr);
    
    return result_str;
    
}

/*------------------------------------------------------------------------------------------------*/

int funccmp( const void * val1, const void * val2 ) {
//    return ( *(long*)val1 - *(long*)val1 )  ;
    if (*(long*)val1 < * (long*)val2) {
        return -1;
    }
    if (*(long*)val1 > * (long*)val2) {
        return 1;
    }
    return 0;
}

/*------------------------------------------------------------------------------------------------*/
//[][][][][][][]()()()([])

//([[][]])[(([()]))]









































