#include <assert.h>
#include <stdlib.h>

/** */
#include "../../../../src/phase/lexer/core.c"
#include "../../../../src/service/scanner/provider/file.c"
#include "../../../../vendor/clogg/src/clogg.c"

int main(void) {
    Scanner scanner = scanner_file_init(fopen("test/fixture/input/numbers.zt", "r"));
    Lexer lexer = lexer_init(&scanner);

    Lexeme* lexeme = NULL;

    while ((lexeme = lexer_next(&lexer)) && lexeme->kind != LEX_KIND_EOF) {
        assert(lexeme->kind == LEX_KIND_LITERAL_NUMBER);
    }

    fclose((FILE*)scanner._source);

    cloggln(CLOGG_INFO, "All tests passed");

    return 0;
}
