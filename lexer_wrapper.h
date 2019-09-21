#ifndef LEXER_WRAPPER_H
#define LEXER_WRAPPER_H
#include <experimental/filesystem>
#include <functional>
#include <fstream>
#include <vector>
#include "tokens.h"
#include "FlexLexer.h"

void lex_it(std::experimental::filesystem::path path);

#endif // LEXER_WRAPPER_H
