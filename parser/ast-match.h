#ifndef AST_MATCH_H_
#define AST_MATCH_H_

namespace grok {
namespace parser {

class Expression;

// FastASTMatcher ::= matches the two AST
// fast because it just compares the types of AST nodes
class FastASTMatcher {
public:
    static bool match(Expression *a, Expression *b);
};

// LazyASTMatcher ::= matches two AST's and also checks for corresponding
// child nodes for equal types and values
class LazyASTMatcher {
public:
    static bool match(Expression *a, Expression *b);
};

}
}


#endif
