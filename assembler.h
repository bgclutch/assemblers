#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_


static const char* c_push = "push";
static const char* c_add  = "add";
static const char* c_sub  = "sub";
static const char* c_mul  = "mul";
static const char* c_div  = "div";
static const char* c_in   = "in";
static const char* c_out  = "out";
static const char* c_sqrt = "sqrt";
static const char* c_sin  = "sin";
static const char* c_cos  = "cos";
static const char* c_dump = "dump";
static const char* c_hlt  = "hlt";
static const char* c_pshr = "pushr";
static const char* c_pop  = "pop";
static const char* c_jmp  = "jmp";
static const char* c_ja   = "ja";
static const char* c_jae  = "jae";
static const char* c_jb   = "jb";
static const char* c_jbe  = "jbe";
static const char* c_je   = "je";
static const char* c_jne  = "jne";
static const char* c_ax   = "AX";
static const char* c_bx   = "BX";
static const char* c_cx   = "CX";
static const char* c_dx   = "DX";
static const char lblatr  = ':';

static const char* basic_name = "aboba";
static const size_t b_name_len = sizeof(basic_name);

static const size_t label_num = 10;
static const ssize_t label_ip_null = -1;
static const size_t token_alloc_coef = 8;


#include "../stack_ded/stack_headers/stack.h"

enum Asm_Commands
{
    MY_PUSH = 0x01, // TODO caps or c_ prefix
    MY_ADD  = 0x02,
    MY_SUB  = 0x03,
    MY_MUL  = 0x04,
    MY_DIV  = 0x05,
    MY_IN   = 0x06,
    MY_OUT  = 0x07,
    MY_SQRT = 0x08,
    MY_SIN  = 0x09,
    MY_COS  = 0x0A,
    MY_DUMP = 0x0B,
    MY_JMP  = 0x10,
    MY_JA   = 0x11,
    MY_JAE  = 0x12,
    MY_JB   = 0x13,
    MY_JBE  = 0x14,
    MY_JE   = 0x15,
    MY_JNE  = 0x16,
    MY_PSHR = 0x20,
    MY_POP  = 0x21,
    MATVEY  = 0x6F,
    MY_HLT  = 0xEF,
};

enum Registers
{
    MY_AX  = 0x00,
    MY_BX  = 0x01,
    MY_CX  = 0x02,
    MY_DX  = 0x03,
    REGERR = 0xDF,
};


enum Asm_Errors
{
    SYNT_ERR      = 0x00,
    ALL_OK_RETURN = 0x01,
};


enum Token_Type
{
    OPCODE    = 0x01,
    NUMBER    = 0x02,
    LABEL     = 0x03,
    REGISTER  = 0x04,
    ERROR     = 0xFF,
};


enum Run_Flags
{
    FIRST_RUN  = 0x52,
    SECOND_RUN = 0x53,
};

struct Token
{
    char name[25];
    size_t name_size;
    size_t token_size;
    Token_Type type;
    Asm_Commands opcode;
    Registers register_num;
    ssize_t label_address;
    StackElem_t number;
};


struct Dynamic_Token
{
    struct Token* token_array;
    size_t capacity;
    size_t size;
};


struct Label
{
    char name[25];
    int64_t ip;
};


Token_Type get_token_type(char name[25], size_t name_size);

int opcode_cmp(char name[25], size_t name_size);

int number_cmp(char name[25], size_t name_size);

int register_cmp(char name[25], size_t name_size);

int ctor_dyn_token(Dynamic_Token* token_st);

int dtor_dyn_token(Dynamic_Token* token_st);

int realloc_maker_token(Dynamic_Token* token_st, size_t new_capacity);

size_t realloc_if_up_needed_token(Dynamic_Token token_st);

size_t realloc_if_down_needed_token(Dynamic_Token token_st);

int init_token_st(const char* buffer, size_t file_size, Dynamic_Token* token_st);

int translator(Dynamic_Token* token_st, FILE* file, Label* labels_array, size_t lables_arr_len, Run_Flags translator_flag);

int ctor_labels(Label* labels_array, size_t size);

int dtor_labels(Label* labels_array, size_t size);

void label_dump(Label* labels_array, size_t labels_num, FILE* file);

void token_dump(Dynamic_Token* token, FILE* file);

void token_ctor(Token* token);

int label_to_null(Label* labels_array, size_t index);





size_t command_words_num(char* buffer, size_t size);

size_t strings_num(const char* buffer, size_t size);

Asm_Commands translation_func(const char*, size_t);

char* find_command_word_begin(char* start_address, size_t index, const size_t size);

size_t find_command_word_len(char* start_address, size_t index, const size_t size);

size_t find_string_len(char* start_address, size_t index, const size_t size);

Asm_Commands put_opcode_word(char* buffer, size_t size, size_t* index);

ssize_t put_command_num(char* buffer, size_t size, size_t* index);

Registers choose_register(const char* command_word, size_t wrd_size);

Registers put_register(char* buffer, size_t size, size_t* index);

int synt_err_check(const Asm_Commands command_num, const char* file, size_t line);

int word_toupper_comparer(const char* str_1, const char* str_2, size_t len_1);

ssize_t find_label(char* string, size_t string_len);

int get_label(char* lable_start, size_t lable_len, Label* labels_array, size_t lables_arr_len, size_t instruction_pointer);

ssize_t put_label(char* buffer, size_t size, size_t* index, Label *labels_array, size_t lables_arr_len, size_t counter);

size_t lable_len(char* start_address, size_t index, size_t size);


#endif // ASSEMBLER_H_