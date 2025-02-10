#ifndef __ZOST_SERVICE_SCANNER_PROVIDER_FILE_C__
#define __ZOST_SERVICE_SCANNER_PROVIDER_FILE_C__

#include <stdio.h>

#include "../core/main.c"

char _scanner_file_current(Scanner* scanner_file) {
    return scanner_file->_current;
}

char _scanner_file_next(Scanner* scanner_file) {
    char c = fgetc((FILE*)scanner_file->_source);

    if (c == '\n') {
        scanner_file->_line += 1;
        scanner_file->_column = 0;
    } else {
        scanner_file->_column += 1;
    }

    scanner_file->_current = c;

    return c;
}

char _scanner_file_peek(Scanner* scanner_file) {
    FILE* file = (FILE*)scanner_file->_source;
    char c = fgetc(file);
    fseek(file, -1L, SEEK_CUR);
    return c;
}

void _scanner_file_lookahead(Scanner* scanner_file, char* buffer, size_t count) {
    for (size_t i = 0; i < count; i++) {
        buffer[i] = _scanner_file_peek(scanner_file);
    }
}

Scanner scanner_file_init(FILE* file) {
    Scanner scanner = {
        ._source = (void*)file,

        ._count = 0,
        ._line = 0,
        ._column = 0,

        ._current = 0,
        .current = _scanner_file_current,

        .next = _scanner_file_next,
        .peek = _scanner_file_peek,

        .lookahead = _scanner_file_lookahead,
    };

    return scanner;
}

#endif /* __ZOST_SERVICE_SCANNER_PROVIDER_FILE_C__ */