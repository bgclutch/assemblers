#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "assembler.h"
#include "putargs.h"
#include "../stack_ded/stack_headers/colorised_output.h"
#include "../lib_buffer_proc/buffer.h"
#include "../lib_file_proc/file.h"



size_t command_words_num(char* buffer, size_t size)
{
    size_t counter = 0;
    for(size_t i = 1; i < size; i++)
    {
        if(isspace(buffer[i - 1]) && isalnum(buffer[i]))
            counter++;
    }

    return counter;
}

Asm_Commands translation_func(const char* command_word, size_t wrd_size)
{
         if(strncmp(command_word, c_push, wrd_size) == 0) return MY_PUSH;
    else if(strncmp(command_word, c_add,  wrd_size) == 0) return MY_ADD;
    else if(strncmp(command_word, c_sub,  wrd_size) == 0) return MY_SUB;
    else if(strncmp(command_word, c_mul,  wrd_size) == 0) return MY_MUL;
    else if(strncmp(command_word, c_div,  wrd_size) == 0) return MY_DIV;
    else if(strncmp(command_word, c_in,   wrd_size) == 0) return MY_IN;
    else if(strncmp(command_word, c_out,  wrd_size) == 0) return MY_OUT;
    else if(strncmp(command_word, c_sqrt, wrd_size) == 0) return MY_SQRT;
    else if(strncmp(command_word, c_sin,  wrd_size) == 0) return MY_SIN;
    else if(strncmp(command_word, c_cos,  wrd_size) == 0) return MY_COS;
    else if(strncmp(command_word, c_dump, wrd_size) == 0) return MY_DUMP;
    else if(strncmp(command_word, c_hlt,  wrd_size) == 0) return MY_HLT;
    else if(strncmp(command_word, c_pop,  wrd_size) == 0) return MY_POP;
    else if(strncmp(command_word, c_jmp,  wrd_size) == 0) return MY_JMP;
    else if(strncmp(command_word, c_ja,   wrd_size) == 0) return MY_JA;
    else if(strncmp(command_word, c_jae,  wrd_size) == 0) return MY_JAE;
    else if(strncmp(command_word, c_jb,   wrd_size) == 0) return MY_JB;
    else if(strncmp(command_word, c_jbe,  wrd_size) == 0) return MY_JBE;
    else if(strncmp(command_word, c_je,   wrd_size) == 0) return MY_JE;
    else return MY_JNE;
}

Registers choose_register(const char* command_word, size_t wrd_size)
{
         if(word_toupper_comparer(command_word, c_ax, wrd_size) == 0) return MY_AX;
    else if(word_toupper_comparer(command_word, c_bx, wrd_size) == 0) return MY_BX;
    else if(word_toupper_comparer(command_word, c_cx, wrd_size) == 0) return MY_CX;
    else return MY_DX;
}


Token_Type get_token_type(char name[25], size_t name_size) // FIXME check label addresses
{
         if(opcode_cmp(  name, name_size)) return OPCODE;
    else if(arithm_operand_cmp(name))      return ARITHM;
    else if(push_pop_atr_cmp( name))       return PFLAG;
    else if(number_cmp(  name, name_size)) return NUMBER;
    else if(register_cmp(name, name_size)) return REGISTER;
    else return LABEL;
}


int opcode_cmp(char name[25], size_t name_size)
{
         if(strncmp(name, c_push, name_size) == 0) return 1;
    else if(strncmp(name, c_add,  name_size) == 0) return 1;
    else if(strncmp(name, c_sub,  name_size) == 0) return 1;
    else if(strncmp(name, c_mul,  name_size) == 0) return 1;
    else if(strncmp(name, c_div,  name_size) == 0) return 1;
    else if(strncmp(name, c_in,   name_size) == 0) return 1;
    else if(strncmp(name, c_out,  name_size) == 0) return 1;
    else if(strncmp(name, c_sqrt, name_size) == 0) return 1;
    else if(strncmp(name, c_sin,  name_size) == 0) return 1;
    else if(strncmp(name, c_cos,  name_size) == 0) return 1;
    else if(strncmp(name, c_dump, name_size) == 0) return 1;
    else if(strncmp(name, c_hlt,  name_size) == 0) return 1;
    else if(strncmp(name, c_pop,  name_size) == 0) return 1;
    else if(strncmp(name, c_jmp,  name_size) == 0) return 1;
    else if(strncmp(name, c_ja,   name_size) == 0) return 1;
    else if(strncmp(name, c_jae,  name_size) == 0) return 1;
    else if(strncmp(name, c_jb,   name_size) == 0) return 1;
    else if(strncmp(name, c_jbe,  name_size) == 0) return 1;
    else if(strncmp(name, c_je,   name_size) == 0) return 1;
    else if(strncmp(name, c_jne,  name_size) == 0) return 1;
    else return 0;
}


int number_cmp(char name[25], size_t name_size)
{
    for(size_t i = 0; i < name_size; i++)
    {
        if(!isdigit(name[i]))
            return 0;
    }

    return 1;
}


int register_cmp(char name[25], size_t name_size)
{
         if(word_toupper_comparer(name, c_ax, name_size) == 0) return 1;
    else if(word_toupper_comparer(name, c_bx, name_size) == 0) return 1;
    else if(word_toupper_comparer(name, c_cx, name_size) == 0) return 1;
    else if(word_toupper_comparer(name, c_dx, name_size) == 0) return 1;
    else return 0;
}


Arithm_Op choose_arithm_operation(char name[25])
{
         if(name[0] == add_op) return SUMM;
    else if(name[0] == sub_op) return SUBDIV;
    else if(name[0] == mul_op) return MULTIPL;
    else if(name[0] == div_op) return DIVSION;
    else return DEFAULT;
}

int arithm_operand_cmp(char name[25])
{
         if(name[0] == add_op) return 1;
    else if(name[0] == sub_op) return 1;
    else if(name[0] == mul_op) return 1;
    else if(name[0] == div_op) return 1;
    else return 0;
}

int push_pop_atr_cmp(char name[25])
{
         if(name[0] == ramatrb) return 1;
    else if(name[0] == ramatre) return 1;
    else return 0;
}

int init_token_st(const char* buffer, size_t file_size, Dynamic_Token* token_st)
{
    int all_bytes = 0;
    int cur_bytes = 0;

    while(all_bytes < (ssize_t)file_size)
    {
        //fprintf(stderr, "token array size " BLUE_TEXT("%lu\n"), token_st->size);

        token_st->token_array[token_st->size].name_size = (size_t)sscanf(buffer + all_bytes, "%s%n",
                                                                         token_st->token_array[token_st->size].name,
                                                                         &cur_bytes);

        token_st->token_array[token_st->size].name_size = strlen(token_st->token_array[token_st->size].name);

        //fprintf(stderr, "name " GREEN_TEXT("%s\n")"name size " BLUE_TEXT("%lu\n"), token_st->token_array[token_st->size].name,
                                                                                   //token_st->token_array[token_st->size].name_size);


        all_bytes += cur_bytes;

        //fprintf(stderr, "all bytes " BLUE_TEXT("%d\n") "file size " BLUE_TEXT("%lu\n\n"), all_bytes, file_size);

        token_st->token_array[token_st->size].type = get_token_type(token_st->token_array[token_st->size].name,
                                                              token_st->token_array[token_st->size].name_size);


        switch(token_st->token_array[token_st->size].type)
        {
            case(OPCODE):
            {
                token_st->token_array[token_st->size].opcode = translation_func(token_st->token_array[token_st->size].name,
                                                                                token_st->token_array[token_st->size].name_size);
                token_st->token_array[token_st->size].token_size = sizeof(char);
                token_st->size++;
                break;
            }
            case(NUMBER):
            {
                token_st->token_array[token_st->size].number = strtol(token_st->token_array[token_st->size].name, NULL, 10);
                token_st->token_array[token_st->size].token_size = sizeof(StackElem_t);
                token_st->size++;
                break;
            }
            case(LABEL):
            {
                token_st->token_array[token_st->size].token_size = sizeof(short);
                token_st->size++;
                break;
            }
            case(REGISTER):
            {
                token_st->token_array[token_st->size].register_num = choose_register(token_st->token_array[token_st->size].name,
                                                                                     token_st->token_array[token_st->size].name_size);
                token_st->token_array[token_st->size].token_size = sizeof(char);
                token_st->size++;
                break;
            }
            case(PFLAG):
            {
                token_st->token_array[token_st->size].token_size = sizeof(char);
                token_st->size++;
                break;
            }
            case(ARITHM):
            {
                token_st->token_array[token_st->size].operation = choose_arithm_operation(token_st->token_array[token_st->size].name);
                token_st->token_array[token_st->size].token_size = sizeof(char);
                token_st->size++;
                break;
            }
            case(ERROR):
            {
                assert(0 && "penis");
            }
            default:
            {
                assert(0 && "huynya peredelyvay");
            };
        };

       realloc_maker_token(token_st, realloc_if_up_needed_token(*token_st));
    }

    return 1;
}


int ctor_dyn_token(Dynamic_Token* token_st)
{
    token_st->capacity = token_alloc_coef;
    token_st->size = 0;
    token_st->token_array = (Token*) calloc(token_st->capacity, sizeof(Token));

    for(size_t i = 0; i < token_st->capacity; i++)
    {
        token_ctor(&token_st->token_array[i]);
    }

    return 1;
}


int dtor_dyn_token(Dynamic_Token* token_st)
{
    token_st->capacity = 0;
    token_st->size = 0;
    token_st->token_array = nullptr;

    return 1;
}


int realloc_maker_token(Dynamic_Token* token_st, size_t new_capacity)
{
    if(new_capacity == token_st->capacity)
        return 1;

    Token* new_array = (Token*) realloc(token_st->token_array, new_capacity * sizeof(Token));

    if(new_array == nullptr)
    {
        fprintf(stderr, "stack RED_TEXT(couldn't realloced) at address:YELLOW_TEXT(%p\n)", new_array);
        return 0;
    }

    //fprintf(stderr, "capacity old (realloc) %lu\n" ,token_st->capacity);

    token_st->capacity    = new_capacity;
    token_st->token_array = new_array;

    for(size_t i = token_st->size; i < token_st->capacity; i++)
    {
        token_ctor(&token_st->token_array[i]);
    }

    // fprintf(stderr, "capacity (realloc) %lu\n\n" ,token_st->capacity);

    return 1;

}


size_t realloc_if_up_needed_token(Dynamic_Token token_st)
{
    if(token_st.capacity - token_st.size <= 1)
        return token_st.capacity * 2;
    else
        return token_st.capacity;
}


size_t realloc_if_down_needed_token(Dynamic_Token token_st)
{
    if(4 <= token_st.size && token_st.size * 4 <= token_st.capacity)
        return token_st.capacity / 4;
    else
        return token_st.capacity;
}


int translator(Dynamic_Token* token_st, FILE* file, Label* labels_array, size_t labels_arr_len, Run_Flags translator_flag)
{
    // TODO asserts

    size_t instr_pointer = 0;

    if(translator_flag == FIRST_RUN)
    {
        int flag = 1;

        for(size_t ind = 0; ind < token_st->size; ind++) // iteration by tokens array
        {
            if(token_st->token_array[ind].type == LABEL) // if token type == label
            {
                for(size_t pos = 0; pos < labels_arr_len; pos++) // iteration by labels array
                { // FIXME pizdec peregruz chto delat' (maybe func)
                    if(strncmp((const char*)labels_array[pos].name, basic_name, b_name_len) == 0 && flag == 1 ) // find first free spot
                    {   // copy name to labels array
                        memset(labels_array[pos].name, '\0', sizeof(labels_array[pos].name));
                        strncpy(labels_array[pos].name, token_st->token_array[ind].name, token_st->token_array[ind].name_size);
                        if(token_st->token_array[ind].name[token_st->token_array[ind].name_size - 1] == lblatr)
                        {
                            labels_array[pos].ip = (int64_t)instr_pointer; // else address is -1 (it will be checked in second run)
                            token_st->token_array[ind].label_address = (int64_t)instr_pointer;
                        }
                        else
                        {
                            instr_pointer += token_st->token_array[ind].token_size;
                            label_to_null(labels_array, pos);
                        }
                        flag = 0;
                    }
                    else if(token_st->token_array[ind].name[token_st->token_array[ind].name_size - 1] == lblatr)  // if this pointer already declaired
                    {
                        if(strncmp(labels_array[pos].name, token_st->token_array[ind].name, token_st->token_array[ind].name_size - 2) == 0)
                        {
                            strncpy(labels_array[pos].name, token_st->token_array[ind].name, token_st->token_array[ind].name_size);
                            instr_pointer += token_st->token_array[ind].token_size;
                            token_st->token_array[ind].label_address = (int64_t)instr_pointer;
                            label_to_null(labels_array, pos);

                        }
                    }
                }
            }
            else if(token_st->token_array[ind].type == ARITHM)
            {
                flag = 1;
            }
            else
            {
                instr_pointer += token_st->token_array[ind].token_size;
                flag = 1;
            }
        }
    }
    else if(translator_flag == SECOND_RUN)
    {
        for(size_t ind = 0; ind < token_st->size; ind++)
        {
            switch(token_st->token_array[ind].type)
            {
                case(OPCODE):
                {
                    fwrite(&token_st->token_array[ind].opcode, 1, 1, file);

                    if(strncmp(token_st->token_array[ind].name, c_push, token_st->token_array[ind].name_size) == 0)
                    {
                        char push_arg = putarg_push(token_st, ind);
                        assert(push_arg && "zalupa 2 prohod in push arg");
                        fwrite(&push_arg, 1, sizeof(char), file);
                    }
                    if(strncmp(token_st->token_array[ind].name, c_pop, token_st->token_array[ind].name_size) == 0)
                    {
                        char pop_arg = putarg_pop(token_st, ind);
                        assert(pop_arg && "zalupa 2 prohod in pop arg");
                        fwrite(&pop_arg, 1, sizeof(char), file);
                    }

                    break;
                }
                case(REGISTER):
                {
                    fwrite(&token_st->token_array[ind].register_num, 1, 1, file);
                    break;
                }
                case(NUMBER):
                {
                    fwrite(&token_st->token_array[ind].number, 1, sizeof(StackElem_t), file);
                    break;
                }
                case(LABEL):
                {
                    int flag = 1;

                    for(size_t i = 0; i < labels_arr_len && flag == 1; i++)
                    {
                        if(token_st->token_array[ind].name[token_st->token_array[ind].name_size - 1] != lblatr)
                        {
                            if(strncmp(token_st->token_array[ind].name, labels_array[i].name, token_st->token_array->name_size) != 0)
                            {
                                continue;
                            }

                            //label_dump(labels_array, labels_arr_len, stderr);

                            if(labels_array[i].ip == label_ip_null)
                            {
                                fprintf(stderr, "wrong ip!\n");
                                return 0;
                            }

                            fwrite(&labels_array[i].ip, 1, 2, file);
                            flag = 0;
                        }
                    }
                    break;
                }
                case(PFLAG):
                {
                    break;
                }
                case(ARITHM):
                {
                    break;
                }
                case(ERROR):
                {
                    assert(0 && "penis + penis");
                }
                default:
                {
                    assert(0 && "pizdec vsyo menyat'");
                };
            };
        }
    }
    else
    {
        return 0;
    }
    return 1;
}


int ctor_labels(Label* labels_array, size_t size)
{
    for(size_t index = 0; index < size; index++)
    {
        strncpy(labels_array[index].name, basic_name, b_name_len);
        labels_array[index].ip = label_ip_null;
    }
    return 1;
}


int word_toupper_comparer(const char* str_1, const char* str_2, size_t len_1)
{
    size_t ind_str_1 = 0;
    size_t ind_str_2 = 0;
    int result = 0;

    while((ind_str_1 < len_1) && !isalpha(str_1[ind_str_1]))
    {
        ind_str_1++;
    }
    while((ind_str_2 < len_1) && !isalpha(str_2[ind_str_2]))
    {
        ind_str_2++;
    }

    while(ind_str_1 < len_1 && ind_str_2 < len_1 && result == 0)
    {
        result = toupper(str_1[ind_str_1]) - toupper(str_2[ind_str_2]);
        ind_str_1++;
        ind_str_2++;
    }
    return result;
}


void label_dump(Label* labels_array, size_t labels_num, FILE* file)
{
    for(size_t i = 0; i < labels_num; i++)
    {
        fprintf(file, "array struct elem %lu: %s jump address:%ld", i, labels_array[i].name, labels_array[i].ip);
        fprintf(file, "\n");
    }
    fprintf(file, "\n\n");
}


void token_dump(Dynamic_Token* token, FILE* file)
{
    for(size_t i = 0; i < token->size; i++)
    {
        fprintf(file,   "name: %s\n"
                        "name size %lu\n"
                        "token_size %lu\n"
                        "type %d\n"
                        "opcode %d\n"
                        "register %d\n"
                        "label address %ld\n"
                        "number %ld\n"
                        "arithmetic operation %d\n",
                        token->token_array[i].name,
                        token->token_array[i].name_size,
                        token->token_array[i].token_size,
                        token->token_array[i].type,
                        token->token_array[i].opcode,
                        token->token_array[i].register_num,
                        token->token_array[i].label_address,
                        token->token_array[i].number,
                        token->token_array[i].operation);
        fprintf(file, "\n\n");
    }


    int a = 0;

    while((a = getchar()) != 'c')
    {
        fprintf(stderr, "press 'c' to continue\n\n");
    }
}


int token_ctor(Token* token)
{
    strncpy(token->name, basic_name, b_name_len);
    token->name_size = b_name_len;
    token->token_size = 0;
    token->type = ERROR;
    token->opcode = MATVEY;
    token->register_num = REGERR;
    token->label_address = label_ip_null;
    token->number = -1;
    token->operation = DEFAULT;

    return 1;
}


int label_to_null(Label* labels_array, size_t index)
{
    strncpy(labels_array[index].name, basic_name, b_name_len);
    labels_array[index].ip = label_ip_null;
    return 1;
}