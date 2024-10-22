#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "assembler.h"
#include "../lib_file_proc/file.h"
#include "../lib_buffer_proc/buffer.h"


int main(void)
{
    FILE* file_read = nullptr;
    file_read = file_read_open(file_read, "default_assembler_code.asm");

    if(!res_checker((ssize_t)file_read))
        return 0;


    size_t file_size = symbols_number(file_read);

    if(!res_checker((ssize_t)file_size))
        return 0;


    char* buffer = buffer_create(file_size, sizeof(char), file_read);

    if(!res_checker((ssize_t)buffer))
        return 0;


    FILE* file_write = nullptr;
    file_write = file_write_open(file_write, "machine_code.asm");

    if(!res_checker((ssize_t)file_write))
        return 0;


    FILE* dumpik = nullptr;
    dumpik = file_write_open(dumpik, "dump_file.txt");

    if(!res_checker((ssize_t)dumpik))
        return 0;

    Label labels_array[label_num] = {};

    int ctor_labels_arr_result = ctor_labels(labels_array, label_num);

    if(!res_checker(ctor_labels_arr_result))
        return 0;

    Dynamic_Token token = {};
    ctor_dyn_token(&token);

    init_token_st(buffer, file_size, &token);

    translator(&token, file_write, labels_array, label_num, FIRST_RUN);

    translator(&token, file_write, labels_array, label_num, SECOND_RUN);

    int f_read_res = file_close(file_read);

    if(!res_checker(f_read_res))
        return 0;

    int f_write_cl = file_close(file_write);

    if(!res_checker(f_write_cl))
        return 0;

    int f_dump_cl = file_close(dumpik);

    if(!res_checker(f_dump_cl))
        return 0;

    free(buffer);
    buffer = nullptr;

    free(token.token_array);
    token.token_array = nullptr;

    return 0;
}