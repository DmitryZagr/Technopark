
//Составить программу разложения положительного целого числа на простые сомножители и единицу.
//
//Программа считывает входные данные со стандартного ввода, и печатает результат в стандартный вывод.
//
//Верными входными данными считается только ровно одно положительное число, не превосходящее 2^63 - 1, возможно с предшествующими или последующими пробельными символами.
//
//Хотя единица не входит в каноническое разложение, но в ответе первым элементом всегда необходимо выводить единицу.
//Считать, что разложение самой единицы состоит только из единицы.
//
//Процедура разложения числа должна быть оформлена в виде отдельной функции, которой на вход подается целое число.
//Функция должна возвращать указатель на массив целых чисел, содержащий разложение введенного числа на простые сомножители.
//Последний элемент этого массива должен быть нулевым.
//
//Программа должна уметь обрабатывать ошибки - такие как неверные входные данные(не число, отрицательное число), ошибки выделения памяти и т.п.
//В случае возникновения ошибки нужно вывести сообщение "[error]" (без кавычек) и завершать выполнение программы.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#define INPUT_BUFFSIZE ( 2048 )
#define BUFF_SIZE_NUMBER_SUBSTRING (20)
#define OUTPUT_ARR_SIZE (63)
#define OUTPUT_ERROR_STRING ("[error]")
#define BUFF_SIZE_8192_BAIT (8192)

long long readNumber();
int checkNumber(char* strNumber);
long long* factorization(long long* number);
char* parse_arr_to_str(const long long * input_arr);

int main(int argc, const char * argv[]) {
    
    long long number = 0;
    long long *result_arr = NULL;
    
    if ( !(number = readNumber())) {
        fputs(OUTPUT_ERROR_STRING, stdout);
        return 0;
    }
    
    result_arr = factorization(&number);
    
    if (result_arr == NULL) {
        fputs(OUTPUT_ERROR_STRING, stdout);
        return 0;
    }
    
    char* result = parse_arr_to_str(result_arr);
    
    
    for (int i = 0; i < strlen(result) - 2; i++) {
        fprintf(stdout, "%c", *(result + i));
    }

    
    free(result);
    free(result_arr);
    return 0;
}

long long readNumber() {
    
    long long number = 0;
    int start_index_of_number_substring = 0;
    int end_index_of_number_substring   = 0;
    char buf_for_input_string[INPUT_BUFFSIZE];
    char buf_for_number_substring[BUFF_SIZE_NUMBER_SUBSTRING];
    
    if(fgets(buf_for_input_string, sizeof(char) * (INPUT_BUFFSIZE - 1), stdin) == NULL) {
        fputs(OUTPUT_ERROR_STRING, stdout);
        return 0;
    }
    
    //strlen(buf_for_input_string) - 1, т.к. последним символом мб == '\n'
    if (*(buf_for_input_string + strlen(buf_for_input_string) - 1) == '\n') {
        *(buf_for_input_string + strlen(buf_for_input_string) - 1) = '\0';
    }
    
    //strlen(buf_for_input_string) - 1, т.к. последний символ == '\n'
    for (int i = 0; i < strlen(buf_for_input_string) /*- 1*/; i++) {
        // ищем ' ' или цифру в строке
        if ( *(buf_for_input_string + i) == ' ' ||
                                    isdigit(*(buf_for_input_string + i))) {
            if (*(buf_for_input_string + i) == ' ') {
                start_index_of_number_substring = i;
                // ставми start_index_of_number_substring на индекс цифры, если она идет после ' '
                if (*(buf_for_input_string + i + 1) != ' ') {
                    start_index_of_number_substring = i + 1;
                }
            }
            else {
                end_index_of_number_substring = i;
                if (*(buf_for_input_string + end_index_of_number_substring + 1) == ' ') {
                    // исключаем ситуацию вида '   '_nubmber_'    '_other_char
                    //strlen(buf_for_input_string) - 1, т.к. последний символ == '\n'
                    for (int i = end_index_of_number_substring + 1; i < strlen(buf_for_input_string) - 1; i++) {
                        if (*(buf_for_input_string + i) != ' ') {
                            return 0;
                        }
                    }
                    break;
                }
            }
        }
        else return 0;
    }
    

    for (int i = 0; i <= end_index_of_number_substring - start_index_of_number_substring; i++) {
        *(buf_for_number_substring + i) = *(buf_for_input_string + start_index_of_number_substring + i);
    }
    
    *(buf_for_number_substring + end_index_of_number_substring - start_index_of_number_substring + 1) = '\0';
    
    if (*buf_for_number_substring != '0') {
        number = strtoull(buf_for_number_substring, NULL, 10) ;
    } else {
        return 0;
    }

    return number;
}

// arr[0] == arr.size
// arr[1] == 1
long long* factorization(long long *number) {
    
    long long* result_arr =
        (long long*)malloc(sizeof(long long) * OUTPUT_ARR_SIZE);
    
    memset(result_arr, 0, sizeof(long long) * OUTPUT_ARR_SIZE);
    
    if (*number == 1) {
        *result_arr = 1;
        *(result_arr + 1) = 0;
        return result_arr;
    }
    
    int j = 0;
    
    long long i = 2;
    while(i  <= *number)
    {
        if(*number % i == 0)
        {
            *number = *number / i;
            *(result_arr + j + 1) = i;
            j++;
            *(result_arr) = j;
            i = 2;
        }
        else
            i++;
    }
    
//    printf("\n|%lli|\n", *number);
    
    *(result_arr) = 1;
    j++;
    *(result_arr + j) = 0;
    
    return result_arr;
}



//long long* factorization(long long *t) {
//    
//    long long* result_arr =
//    (long long*)malloc(sizeof(long long) * 63);
//    
//    if (*t == 1) {
//        *result_arr = 1;
//        *(result_arr + 1) = 1;
//        return result_arr;
//    }
//    
//    int j = 0;
//    
//    long long i = 2;
//    while(i  <= *t)
//    {
//        if(*t % i == 0)
//        {
//            //            printf("%llu ",i);
//            *t = (*t / i);
//            //                printf("");
//            *(result_arr + j + 2) = i;
//            j++;
//            *(result_arr) = j;
//            //            *(result_arr + j + 1) = i;
//        }
//        else
//            i++;
//    }
//    
//    *(result_arr + 1) = 1;
//    *result_arr = ++j;
//    *(result_arr) = *(result_arr) + 1;
//    *(result_arr + *(result_arr)) = 0;
//    
//    return result_arr;
//}


char* parse_arr_to_str(const long long * input_arr) {
    if (input_arr == NULL) {
        fputs(OUTPUT_ERROR_STRING, stdout);
    }
    
    char* output_arr = NULL;
    
    output_arr = (char*) malloc(BUFF_SIZE_8192_BAIT);
    
    if (output_arr == NULL) {
        fputs(OUTPUT_ERROR_STRING, stdout);
    }
    
    memset(output_arr, '\0', BUFF_SIZE_8192_BAIT);
    
    char buf[1024];
    memset(buf, '\0', 1024);
    
    int i = 0;
    while (*(input_arr + i) != 0 ) {
        sprintf(buf, "%lli", *(input_arr + i));
//        fprintf(stdout, "%lld ", *(input_arr + i));
        
        int j = 0;
        while (buf[j] != '\0') {
            *(output_arr + strlen(output_arr)) = buf[j];
            j++;
        }
        *(output_arr + strlen(output_arr)) = ' ';
//        fprintf(stdout , " |%s - %li| ", output_arr, strlen(output_arr));
        i++;
    }
    
    *(output_arr + strlen(output_arr)) = '0';
    
    return output_arr;
}































