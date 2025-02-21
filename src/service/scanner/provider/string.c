#ifndef __ZOST_SERVICE_SCANNER_PROVIDER_STRING_C__
#define __ZOST_SERVICE_SCANNER_PROVIDER_STRING_C__

#include <stdio.h>

#include "../../../../vendor/ckastal/include/prefixed_length_string.c"
#include "../core/main.c"

char _scanner_string_current(Scanner* scanner) {
    return scanner->_current;
}

char _scanner_string_next(Scanner* scanner) {
    Ck_LenStr* string = (Ck_LenStr*)scanner->_source;

    if (scanner->_count >= string->_length) {
        scanner->_current = EOF;
        return EOF;
    }

    char c = string->_str[scanner->_count];

    scanner->_count++;

    if (c == '\n') {
        scanner->_line += 1;
        scanner->_column = 0;
    } else {
        scanner->_column += 1;
    }

    scanner->_current = c;

    return c;
}

char _scanner_string_peek(Scanner* scanner) {
    Ck_LenStr* string = (Ck_LenStr*)scanner->_source;

    if (scanner->_count >= string->_length) {
        return EOF;
    }

    return string->_str[scanner->_count];
}

void _scanner_string_lookahead(Scanner* scanner, char* buffer, size_t count) {
    Ck_LenStr* string = (Ck_LenStr*)scanner->_source;
    size_t i = 0;

    for (i = 0; i < count && (scanner->_count + i) < string->_length; i++) {
        buffer[i] = string->_str[scanner->_count + i];
    }

    buffer[i] = '\0';
}

Scanner scanner_string_init(Ck_LenStr* source) {
    Scanner scanner = {
        ._source = source,
        ._count = 0,

        ._count = 0,
        ._line = 0,
        ._column = 0,

        ._current = EOF,
        .current = _scanner_string_current,
        .next = _scanner_string_next,
        .peek = _scanner_string_peek,
        .lookahead = _scanner_string_lookahead,
    };

    return scanner;
}

#endif /* __ZOST_SERVICE_SCANNER_PROVIDER_STRING_C__ */
