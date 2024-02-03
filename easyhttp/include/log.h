//
// Created by asxe on 2023/12/29.
//

#ifndef C_LOG_H
#define C_LOG_H

#include "utils.h"

#define POST "\033[0m"  //结束
#define GREEN_PRE "\033[32;1m" //绿色前缀
#define RED_PRE "\033[31;1m" //红色前缀
#define GREEN_WEIGHT_PRE "\033[36;1m" //深绿色前缀
#define YELLOW_PRE "\033[33;1m"

void info(cString s);
void warning(cString s);
void error(cString s);

#endif //C_LOG_H
