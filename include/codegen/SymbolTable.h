#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include <llvm/IR/Module.h>

namespace jast {

// TODO: FixMe should be valid for every data type

using SymbolTable = std::map<std::string, llvm::Value*>;

}

#endif
