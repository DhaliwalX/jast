#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include <map>
#include <list>

namespace jast {

class ParserContext;
class Expression;
class SymbolTable;

class Scope {
public:
    using Value = Expression;
public:
    Scope(Value *root, Scope *parent);
    ~Scope();

    SymbolTable *symbol_table();

    Scope *parent() { return parent_; }

private:
    // root of tree where scope starts
    Value *root_;
    SymbolTable *symbol_table_;
    Scope *parent_;
};

class SymbolTable {
public:
    using Name = std::string;
    using Value = Expression;

    SymbolTable(Scope *scope);

    void Push(const Name &name, Value *value);

    bool Exists(const Name &name) const;

    Value *Get(const Name &name);

    Scope *scope();
private:
    std::map<Name, Value*> entries_;
    Scope *scope_;
};

// ScopeManager manages stack of scope. It keeps a pointer to the current scope
// and hence current symbolTable.
class ScopeManager {
public:
    ScopeManager(ParserContext *context);

    // NewScope creates a new scope and pushes the current scope to the stack
    // and makes current_ point to new scope
    Scope *NewScope(SymbolTable::Value *value);

    // pops the scope from stack, and current now points to popped scope.
    Scope *PopScope();

    Scope *current() { return current_; }

    Scope *global_scope() { return global_scope_; }

private:
    void PushScope(Scope *scope);

    ParserContext *context_;
    Scope *global_scope_;
    Scope *current_;
    std::list<Scope*> scope_stack_;
};

}

#endif
