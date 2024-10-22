#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "putargs.h"
#include "../lib_buffer_proc/buffer.h"
#include "../lib_file_proc/file.h"



char putarg_push(Dynamic_Token* token, size_t ind)
{
    ind++; // first incr
    char push_dest_byte = 0;

    switch(token->token_array[ind].type) // check first token type after push, many branches
    {
        case(PFLAG):
        {
            if(token->token_array[ind].name[0] != ramatrb && token->token_array[ind + 4].name[0] != ramatre)
            {
                fprintf(stderr, "huynya, dobav' skobku v svoy ebuchiy push or pop, eblan\n");
                assert(0); // FIXME CHANGE KAKASHECHKU?
            }

            push_dest_byte |= push_dest_byte | RAM_bit;

            ind++; //second incr

            if(token->token_array[ind].type == REGISTER)
            {
                push_dest_byte |= register_bit;

                ind++; // third incr

                if(token->token_array[ind].type == ARITHM) // first branch
                {
                    push_dest_byte |= (char)token->token_array[ind].operation;

                    ind++; // fourth incr

                    if(token->token_array[ind].type != NUMBER)
                    {
                        fprintf(stderr, "dolboeb, kuda vtoroy registr\n");
                        assert(0);
                    }

                    push_dest_byte |= number_bit;
                }
            }
            else if(token->token_array[ind].type == NUMBER)
            {
                push_dest_byte |= number_bit;

                ind++; // third incr

                if(token->token_array[ind].type == ARITHM) // second branch
                {
                    push_dest_byte |= (char)token->token_array[ind].operation;

                    ind++; // fourth incr

                    if(token->token_array[ind].type != REGISTER)
                    {
                        fprintf(stderr, "dolboeb, kuda vtoroy number\n");
                        assert(0);
                    }

                    push_dest_byte |= register_bit;

                    swap_tokens(&token->token_array[ind], &token->token_array[ind - 2]);
                }
            }

            return push_dest_byte;

            break;
        }
        case(REGISTER):
        {
            push_dest_byte |= register_bit;

            ind++; // second incr

            if(token->token_array[ind].type == ARITHM) // first branch
            {
                push_dest_byte |= (char)token->token_array[ind].operation;

                ind++; // third incr

                if(token->token_array[ind].type != NUMBER)
                {
                    fprintf(stderr, "dolboeb, kuda vtoroy registr\n");
                    assert(0);
                }

                push_dest_byte |= number_bit;
            }

            return push_dest_byte;

            break;
        }
        case(NUMBER):
        {
            push_dest_byte |= number_bit;

            ind++; // third incr

            if(token->token_array[ind].type == ARITHM) // first branch
            {
                push_dest_byte |= (char)token->token_array[ind].operation;

                ind++; // fourth incr

                if(token->token_array[ind].type != REGISTER)
                {
                    fprintf(stderr, "dolboeb, kuda vtoroy number\n");
                    assert(0);
                }

                push_dest_byte |= register_bit;

                swap_tokens(&token->token_array[ind], &token->token_array[ind - 2]);
            }

            return push_dest_byte;

            break;
        }
        case(OPCODE):
        {
            assert(0 && "opcode push");
        }
        case(LABEL):
        {
            assert(0 && "label push");
        }
        case(ARITHM):
        {
            assert(0 && "arithm push");
        }
        case(ERROR):
        {
            assert(0 && "error push");
        }
        default:
        {
            assert(0 && "otsosi big huy");
        }
    }
}


char putarg_pop(Dynamic_Token* token, size_t ind)
{
    ind++;

    char pop_dest_byte = 0;

    switch(token->token_array[ind].type)
    {
        case(PFLAG):
        {
            if(token->token_array[ind].name[0] != ramatrb && (token->token_array[ind + 4].name[0] != ramatre || token->token_array[ind + 2].name[0] != ramatre))
            {
                fprintf(stderr, "huynya, dobav' skobku v svoy ebuchiy push or pop, eblan\n");
                assert(0); // FIXME CHANGE KAKASHECHKU?
            }

            pop_dest_byte |= pop_dest_byte | RAM_bit;

            ind++;

            if(token->token_array[ind].type == NUMBER)
            {
                pop_dest_byte |= pop_dest_byte | number_bit;

                ind++;

                if(token->token_array[ind].type == ARITHM)
                {
                    pop_dest_byte |= pop_dest_byte | (char)token->token_array[ind].operation;

                    ind++;

                    if(token->token_array[ind].type != REGISTER)
                    {
                        fprintf(stderr, "ueban nahuya tebe 2 numbers\n");
                        assert(0 && "peredelyvay\n");
                    }

                    pop_dest_byte |= pop_dest_byte | register_bit;

                    swap_tokens(&token->token_array[ind], &token->token_array[ind - 2]);
                }
            }
            else if(token->token_array[ind].type == REGISTER)
            {
                pop_dest_byte |= pop_dest_byte | register_bit;

                ind++;

                if(token->token_array[ind].type != ARITHM)
                {
                    fprintf(stderr, "dobav' arithmetic operation to pop pls\n");
                    assert(0 && "pososi moy huy\n");
                }

                pop_dest_byte |= pop_dest_byte | (char)token->token_array[ind].operation;

                ind++;

                if(token->token_array[ind].type != NUMBER)
                {
                    fprintf(stderr, "uberi second register eblan\n");
                    assert(0);
                }

                pop_dest_byte |= pop_dest_byte | number_bit;
            }

            return pop_dest_byte;

            break;
        }
        case(REGISTER):
        {
            return register_bit;

            break;
        }
        case(OPCODE):
        {
            assert(0 && "opcode pop");
        }
        case(LABEL):
        {
            assert(0 && "label pop");
        }
        case(ARITHM):
        {
            assert(0 && "arithm pop");
        }
        case(ERROR):
        {
            assert(0 && "error pop");
        }
        case(NUMBER):
        {
            assert(0 && "number pop");
        }
        default:
        {
            assert(0 && "da ty zaebal pisat huynu\n");
        }
    }
}


int swap_tokens(Token* token_1, Token* token_2)
{
    Token temp = {};

    temp = *token_2;
    *token_2 = *token_1;
    *token_1 = temp;

    return 1;
}