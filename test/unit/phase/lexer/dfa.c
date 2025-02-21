#include <assert.h>
#include <stdlib.h>

/**/
#include "../../../../src/phase/lexer/dfa.c"
#include "../../../../src/service/scanner/provider/string.c"
#include "../../../../vendor/ckastal/include/init.c"
#include "../../../../vendor/clogg/src/clogg.c"

bool a_to_d(char c) {
    return (c >= 'a' && c <= 'h') || (c >= 'j' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool b_to_d(char c) {
    return (c >= 'a' && c <= 'e') || (c >= 'g' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_digit(char c) {
    return (c >= '0' && c <= '9');
}
bool is_alpnum_snake_case(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

int main(int argc, char** argv) {
    LexerDfaState A = {LEX_KIND_INCOMPLETE, (LexerDfaTransition[5]){0}, 5};
    LexerDfaState B = {LEX_KIND_SYMBOL, (LexerDfaTransition[2]){0}, 2};
    LexerDfaState C = {LEX_KIND_IF, (LexerDfaTransition[1]){0}, 1};
    LexerDfaState D = {LEX_KIND_SYMBOL, (LexerDfaTransition[1]){0}, 1};
    LexerDfaState E = {LEX_KIND_INCOMPLETE, (LexerDfaTransition[1]){0}, 1};
    LexerDfaState F = {LEX_KIND_INCOMPLETE, (LexerDfaTransition[2]){0}, 2};
    LexerDfaState G = {LEX_KIND_LITERAL_INT, (LexerDfaTransition[3]){0}, 3};
    LexerDfaState H = {LEX_KIND_LITERAL_FLOAT, (LexerDfaTransition[2]){0}, 2};
    LexerDfaState I = {LEX_KIND_INCOMPLETE, (LexerDfaTransition[2]){0}, 2};
    LexerDfaState J = {LEX_KIND_INCOMPLETE, (LexerDfaTransition[1]){0}, 1};
    LexerDfaState K = {LEX_KIND_LITERAL_FLOAT, (LexerDfaTransition[1]){0}, 1};

    A.transitions[0] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_STRING, "i", &B);
    A.transitions[1] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_FN, a_to_d, &D);
    A.transitions[2] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_STRING, ".", &E);
    A.transitions[3] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_STRING, "+-", &F);
    A.transitions[4] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_FN, is_digit, &G);

    B.transitions[0] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_STRING, "f", &C);
    B.transitions[1] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_FN, b_to_d, &D);

    D.transitions[0] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_FN, is_alpnum_snake_case, &D);

    E.transitions[0] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_FN, is_digit, &H);

    F.transitions[0] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_FN, is_digit, &G);
    F.transitions[1] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_STRING, ".", &E);

    G.transitions[0] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_FN, is_digit, &G);
    G.transitions[1] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_STRING, "eE", &I);
    G.transitions[2] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_STRING, ".", &H);

    H.transitions[0] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_FN, is_digit, &H);
    H.transitions[1] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_STRING, "eE", &I);

    I.transitions[0] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_STRING, "+-", &J);
    I.transitions[1] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_FN, is_digit, &K);

    J.transitions[0] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_FN, is_digit, &K);

    K.transitions[0] = lexer_dfa_transition_init(LEX_DFA_TRANSITION_TYPE_FN, is_digit, &K);

    Ck_LenStr inputs[] = {
        ck_lenstr_init(ck_lenstr_slp("hello")),
        ck_lenstr_init(ck_lenstr_slp("3.1416")),
        ck_lenstr_init(ck_lenstr_slp("3")),
        ck_lenstr_init(ck_lenstr_slp("10e-20")),
        ck_lenstr_init(ck_lenstr_slp("10.03e+20")),
        ck_lenstr_init(ck_lenstr_slp("if")),
        ck_lenstr_init(ck_lenstr_slp("this will cause an error")),
    };
    LexemeKind expected[] = {
        LEX_KIND_SYMBOL,
        LEX_KIND_LITERAL_FLOAT,
        LEX_KIND_LITERAL_INT,
        LEX_KIND_LITERAL_FLOAT,
        LEX_KIND_LITERAL_FLOAT,
        LEX_KIND_IF,
        LEX_KIND_ERROR,
    };

    for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++) {
        Scanner scanner = scanner_string_init(&inputs[i]);
        LexerDfa lexer = lexer_dfa_init(&scanner, &A);

        Lexeme* lexeme = lexer_dfa_next(&lexer);

        assert(lexeme->kind == expected[i]);
    }

    cloggln(CLOGG_INFO, "All tests passed");

    return 0;
}
