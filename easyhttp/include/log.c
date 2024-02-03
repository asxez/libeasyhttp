#include <stdio.h>
#include "log.h"

void info(cString s) {
    fprintf(stdout, "%s%s%s %s|%s INFO     %s|%s %s%s:%s:%d%s - %s\n",
            GREEN_PRE,
            __TIMESTAMP__,
            POST,
            RED_PRE,
            POST,
            RED_PRE,
            POST,
            GREEN_WEIGHT_PRE,
            __FILE__,
            __FUNCTION__,
            __LINE__,
            POST,
            s
    );
}

void warning(cString s) {
    fprintf(stdout, "%s%s%s %s|%s %sWARNING%s  %s|%s %s%s:%s:%d%s - %s%s%s\n",
            GREEN_PRE,
            __TIMESTAMP__,
            POST,
            RED_PRE,
            POST,
            YELLOW_PRE,
            POST,
            RED_PRE,
            POST,
            GREEN_WEIGHT_PRE,
            __FILE__,
            __FUNCTION__,
            __LINE__,
            POST,
            YELLOW_PRE,
            s,
            POST
    );
}

void error(cString s) {
    fprintf(stderr, "%s%s%s %s|%s %sERROR%s    %s|%s %s%s:%s:%d%s - %s%s%s\n",
            GREEN_PRE,
            __TIMESTAMP__,
            POST,
            RED_PRE,
            POST,
            RED_PRE,
            POST,
            RED_PRE,
            POST,
            GREEN_WEIGHT_PRE,
            __FILE__,
            __FUNCTION__,
            __LINE__,
            POST,
            RED_PRE,
            s,
            POST
    );
}
