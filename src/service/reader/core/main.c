#ifndef __ZOST_SERVICE_READER_CORE_MAIN_C__
#define __ZOST_SERVICE_READER_CORE_MAIN_C__

#include <stdio.h>

#include "../../../../vendor/ckastal/include/init.c"

typedef struct Reader {
    FILE* _file;
    size_t _count;
    char* _cursor;
} Reader;

#endif /* __ZOST_SERVICE_READER_CORE_MAIN_C__ */