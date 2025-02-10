#include "../../../src/core/analysis.c"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../../../vendor/clogg/src/clogg.c"

int main(void) {
    assert(strcmp("LITERAL_STRING", lexeme_kind_to_str(LEX_KIND_LITERAL_STRING)) == 0);
    assert(strcmp("ASSIGN", lexeme_kind_to_str(LEX_KIND_ASSIGN)) == 0);
    assert(strcmp("DIVIDE", lexeme_kind_to_str(LEX_KIND_DIVIDE)) == 0);

    lexeme_printf(&(Lexeme){
        .kind = LEX_KIND_AND,
        .value = ck_lenstr_init(ck_lenstr_slp("=")),
    });
    printf("\n");

    lexeme_printf(&(Lexeme){
        .kind = LEX_KIND_SYMBOL,
        .value = ck_lenstr_init(ck_lenstr_slp("username")),
    });
    printf("\n");

    lexeme_printf(&(Lexeme){
        .kind = LEX_KIND_EOF,
        .value = ck_lenstr_init(ck_lenstr_slp("EOF")),
    });
    printf("\n");

    lexeme_printf(&(Lexeme){
        .kind = LEX_KIND_ERROR,
        .value = ck_lenstr_init(ck_lenstr_slp("/fdal\\dsa/")),
    });
    printf("\n");

    cloggln(CLOGG_INFO, "All tests passed");

    return 0;
}
