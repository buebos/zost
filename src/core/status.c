#ifndef __ZOST_CORE_STATUS_C__
#define __ZOST_CORE_STATUS_C__

#include "../../vendor/ckastal/include/init.c"

typedef enum StatusType {
    STATUS_TYPE_OK = 0,
    STATUS_TYPE_ERROR = 1,
    STATUS_TYPE_WARNING = 2,
} StatusType;

typedef struct Status {
    StatusType type;
    char *title;
    char *description;

    char *_source_cursor;
    size_t _source_line;
    size_t _source_column;
} Status;

Status status = {
    .type = STATUS_TYPE_OK,
    .title = NULL,
    .description = NULL,

    ._source_line = 0,
    ._source_column = 0,
    ._source_cursor = NULL,
};

#endif /** __ZOST_CORE_STATUS_C__ */