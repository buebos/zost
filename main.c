#include "src/phase/lexer/main.c"

#include <stdio.h>

#include "vendor/ckastal/include/syntax_utils.h"

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("Printing help...\n");
        return 0;
    }

    Cli cli = cli_init(&Commands, argc, argv);

    FILE* source_file = fopen(cli_get(&cli, "filename"), "r");
    Status status = {0};

    Scanner source_scanner = scanner_init(source_file);
    Lexer lexer = lexer_init(&source_scanner, &status);

    Ast ast = ast_init();

    Parser parser = parser_init(&ast);
    Analyzer analyzer = analyzer_init(&ast);

    foreach (Lexeme, lexeme, lexer) {
        /**
         * Mutates the abstract syntax tree (AST) or returns an error if there
         * was one.
         */
        status = parser_handle(&parser, lexeme);

        if (status.code != 0) {
            return status.code;
        }

        status = analyzer_handle(&analyzer, lexeme);

        if (status.code != 0) {
            return status.code;
        }
    }

    /**
     * From this part on the compiler has validated that the abstract syntax tree
     * is valid. We could apply optimizations to it and mutate it before
     * synthesizing the three address code.
     */
    // ast_optimize(&ast);

    Writter tac_writter = writter_init(fopen("tac_temp", "w+"));

    /**
     * Synthesizes by chunks the abstract syntax tree to write the three address
     * code.
     */
    tac_from_ast(&tac_writter, &ast);
    /** Maybe also apply optimizations to the tac file generated */
    // tac_optimize(&tac_writter);
    writter_close(&tac_writter);

    /**
     * Synthesizes by chunks the three address code to assembly.
     */
    Writter asm_writter = scanner_init(fopen("asm_temp", "w+"));
    Scanner tac_scanner = scanner_init(fopen("tac_temp", "r"));

    asm_from_tac(&asm_writter, &tac_scanner);
    /** Maybe also apply optimizations to the assembly file generated */
    // asm_optimize(&asm_writter);
    writter_close(&asm_writter);
    scanner_close(&tac_scanner);

    Writter bin_writter = scanner_init(fopen("a.out", "w+"));
    Scanner asm_scanner = scanner_init(fopen("asm_temp", "r"));

    bin_from_asm(&bin_writter, &asm_scanner);
    writter_close(&bin_writter);
    scanner_close(&asm_scanner);

    remove("tac_temp");
    remove("asm_temp");

    return 0;
}
