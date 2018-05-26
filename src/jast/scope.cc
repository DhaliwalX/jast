#include "jast/scope.h"
#include "jast/expression.h"
#include "jast/context.h"

namespace jast {

Scope::Scope(Value *root, Scope *parent)
    : root_{root}, parent_{parent}
{
    // in case of global scope the root is empty
    if (root_)
        root_->SetScope(this);
    symbol_table_ = new SymbolTable(this);
}

// when scope is deleted, then so is symbol table
Scope::~Scope() {
    delete symbol_table_;
}

SymbolTable *Scope::symbol_table() {
    return symbol_table_;
}

SymbolTable::SymbolTable(Scope *scope)
    : scope_{scope}
{ }

void SymbolTable::Push(const Name &name, Value *value) {
    entries_[name] = value;
}

bool SymbolTable::Exists(const Name &name) const {
    return !!entries_.count(name);
}

Scope *SymbolTable::scope() {
    return scope_;
}

SymbolTable::Value *SymbolTable::Get(const Name &name) {
    auto it = entries_.find(name);
    if (it == entries_.end()) {
        return nullptr;
    }

    return (*it).second;
}

ScopeManager::ScopeManager(ParserContext *context)
    : context_{context}, global_scope_{context->GetGlobalScope()}, current_{global_scope_}
{ }

void ScopeManager::PushScope(Scope *scope) {
    scope_stack_.push_back(scope);
}

Scope *ScopeManager::NewScope(SymbolTable::Value *value) {
    Scope *scope = new Scope(value, current_);
    PushScope(current_);
    return (current_ = scope);
}

Scope *ScopeManager::PopScope() {
    Scope *scope = current_;
    current_ = scope_stack_.back();
    scope_stack_.pop_back();
    return scope;
}

}
