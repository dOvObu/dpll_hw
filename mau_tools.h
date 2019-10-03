#ifndef MAU_TOOLS_H
#define MAU_TOOLS_H

#include <experimental/filesystem>

#include "vis_base.h"
#include "vis_exec.h"
#include "parser_nodes.h"
#include "vis_printer.h"
#include "tokens.h"
#include "parser.h"
#include "lexer_wrapper.h"

void run_script(std::experimental::filesystem::path path);

extern std::map<string, void(*)(VIS_EXEC*, VEC<EXPR*>&)> call_backs;

#endif // MAU_TOOLS_H