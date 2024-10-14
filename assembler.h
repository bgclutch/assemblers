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
    MATVEY  = 0xFF,
    MY_HLT  = 0x00,

};

size_t command_words_num(char* buffer, size_t size);

Asm_Commands translation_func(const char*, size_t);

int res_checker(const ssize_t num);

int translator(char* buffer, size_t words_num, FILE* file, size_t size);

char* find_command_word_begin(char* start_address, size_t index, const size_t size);

size_t find_command_word_len(char* start_address, size_t index, const size_t size);

Asm_Commands put_command_word(char* buffer, size_t size, size_t* index);

ssize_t put_command_num(char* buffer, size_t size, size_t* index);

#endif // ASSEMBLER_H_