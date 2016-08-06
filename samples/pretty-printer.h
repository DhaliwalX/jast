#ifndef PRETTY_PRINTER_H_
#define PRETTY_PRINTER_H_

#include "parser/astvisitor.h"
#include <iostream>

namespace printer {

class PrettyPrinter : public grok::parser::ASTVisitor {
public:
    PrettyPrinter(std::ostream &os, bool tab_or_spaces)
        : os_{ os }, tab_{ 0 }, tab_or_spaces_{tab_or_spaces}
    { }

protected:
    std::ostream &os() { return os_; }
    int &tab() { return tab_; }
    bool tabs_or_spaces() { return tab_or_spaces_; }

public:
#define DECLARE_VISITOR_METHOD(type) void Visit(grok::parser::type *) override;
AST_NODE_LIST(DECLARE_VISITOR_METHOD)
#undef DECLARE_VISITOR_METHOD

private:
    std::ostream &os_;
    int tab_;
    bool tab_or_spaces_;
};

}

#endif
