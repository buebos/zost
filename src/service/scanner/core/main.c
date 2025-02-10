#ifndef __ZOST_SERVICE_SCANNER_CORE_MAIN_C__
#define __ZOST_SERVICE_SCANNER_CORE_MAIN_C__

#include <stdio.h>

#include "../../../../vendor/ckastal/include/init.c"

/**
 * A generic byte serial scanner; over files, strings, etc.
 */
typedef struct Scanner {
    void* _source;

    size_t _count;
    size_t _line;
    size_t _column;

    char _current;

    char (*current)(struct Scanner*);

    char (*next)(struct Scanner*);
    char (*peek)(struct Scanner*);

    void (*lookahead)(struct Scanner*, char* buffer, size_t count);
} Scanner;

#endif /* __ZOST_SERVICE_SCANNER_CORE_MAIN_C__ */