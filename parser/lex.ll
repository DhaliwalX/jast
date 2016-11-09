%{
#include "parser/lexinfo.h"
#include <iostream>

#define YY_INPUT(buf,result,max_size) \
    { \
        int c = grok::parser::LexerInfo::ReadChar();  \
        result = (c == -1) ? YY_NULL : (buf[0] = c, 1); \
    }

%}
%start COMMENT1 COMMENT2
LNUM    [0-9]+
DNUM    ([0-9]*[\.][0-9]+)|([0-9]+[\.][0-9]*)
BNUM    "0b"[01]+
EXPONENT_DNUM   (({LNUM}|{DNUM})[eE][+-]?{LNUM})
HNUM    "0x"[0-9a-fA-F]+
IDENT   [a-zA-Z_$][a-zA-Z_$0-9]*
STR     (["].*["])|(['].*['])
%%
<COMMENT1>.+    ;
<COMMENT1>\n    BEGIN 0;
<COMMENT2>[^*]+     ;
<COMMENT2>\*[^/]    ;
<COMMENT2>\*\/  BEGIN 0;
[ ]|[\t]       ;
"//"        BEGIN COMMENT1;
"/*"        BEGIN COMMENT2;
"("         { return LPAREN; }
")"         { return RPAREN; }
"["         { return LBRACK; }
"]"         { return RBRACK; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
":"         { return COLON; }
";"         { return SEMICOLON; }
"."         { return PERIOD; }
"..."       { return ELLIPSIS; }
"?"         { return CONDITIONAL; }
"++"        { return INC; }
"--"        { return DEC; }
"=>"        { return ARROW; }
"="         { return ASSIGN; }
"|="        { return ASSIGN_BIT_OR; }
"^="        { return ASSIGN_BIT_XOR; }
"&="        { return ASSIGN_BIT_AND; }
"<<="       { return ASSIGN_SHL; }
">>="       { return ASSIGN_SAR; }
">>>="      { return ASSIGN_SHR; }
"+="        { return ASSIGN_ADD; }
"-="        { return ASSIGN_SUB; }
"*="        { return ASSIGN_MUL; }
"/="        { return ASSIGN_DIV; }
"%="        { return ASSIGN_MOD; }
","         { return COMMA; }
"||"        { return OR; }
"&&"        { return AND; }
"|"         { return BIT_OR; }
"&"         { return BIT_AND; }
"^"         { return BIT_XOR; }
"<<"        { return SHL; }
">>"        { return SAR; }
">>>"       { return SHR; }
"+"         { return ADD; }
"-"         { return SUB; }
"*"         { return MUL; }
"/"         { return DIV; }
"%"         { return MOD; }
"=="        { return EQ; }
"!="        { return NE; }
"==="       { return EQ_STRICT; }
"!=="       { return NE_STRICT; }
"<"         { return LT; }
">"         { return GT; }
"<="        { return LTE; }
">="        { return GTE; }
"instanceof"    { return INSTANCEOF; }
"in"        { return IN; }
"~"         { return BIT_NOT; }
"delete"    { return DELETE; }
"typeof"    { return TYPEOF; }
"void"      { return VOID; }
"break"     { return BREAK; }
"case"      { return CASE; }
"catch"     { return CATCH; }
"continue"  { return CONTINUE; }
"debugger"  { return DEBUGGER; }
"default"   { return DEFAULT; }
"do"        { return DO; }
"else"      { return ELSE; }
"finally"   { return FINALLY; }
"for"       { return FOR; }
"function"  { return FUNCTION; }
"if"        { return IF; }
"new"       { return NEW; }
"return"    { return RETURN; }
"switch"    { return SWITCH; }
"this"      { return THIS; }
"throw"     { return THROW; }
"try"       { return TRY; }
"var"       { return VAR; }
"while"     { return WHILE; }
"with"      { return WITH; }
"null"      { return NULL_LITERAL; }
"true"      { return TRUE_LITERAL; }
"false"     { return FALSE_LITERAL; }
"async"     { return ASYNC; }
"await"     { return AWAIT; }
"class"     { return CLASS; }
"const"     { return CONST; }
"enum"      { return ENUM; }
"export"    { return EXPORT; }
"extends"   { return EXTENDS; }
"import"    { return IMPORT; }
"let"       { return LET; }
"static"    { return STATIC; }
"super"     { return SUPER; }
<<EOF>>     { return EOS; }
{LNUM}|{DNUM}|{BNUM}|{EXPONENT_DNUM}|{HNUM}   { return NUMBER; }
{IDENT}     { return IDENTIFIER; }
{STR}       { return STRING; }
[\n]        { yylineno++; }
<*>(.)         { return ILLEGAL; }
%%

int yywrap() { return 1; }

namespace grok {
namespace parser {

TokenType LexerInfo::current_tok_;
String LexerInfo::current_matched_;
size_t LexerInfo::seek_;
Scanner *LexerInfo::scanner_;

TokenType LexerInfo::peek()
{
    return current_tok_;
}

void LexerInfo::advance()
{
    current_tok_ = (TokenType) ::yylex();
    current_matched_.assign(::yytext, ::yyleng);

    // TODO: seek_ should contain current position of yylex
    seek_ = 0;
}

bool LexerInfo::MatchNext(String str)
{
    LexerInfo::advance();
    bool ret = (current_matched_ == str);
    LexerInfo::advance();
    return ret;
}

size_t LexerInfo::GetSeek() { return seek_; }

char Scanner::ReadChar()
{
    return is_ ? is_.get() : EOF;
}

}
}

