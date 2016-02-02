#ifndef _OPTIONS_PARSER_H
#define _OPTIONS_PARSER_H

#include <string>

struct Parameters
{
    char* pathToFileWithWords;
    std::string firstWord;
    int minWordsCount = 1;
};

Parameters GetParameters (int argc, char **argv);

#endif

