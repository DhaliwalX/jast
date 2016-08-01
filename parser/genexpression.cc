#include "parser/expression.h"
#include "parser/ifstatement.h"
#include "parser/forstatement.h"
#include "parser/blockstatement.h"
#include "parser/returnstatement.h"
#include "vm/instruction.h"
#include "vm/instruction-builder.h"
#include "lexer/token.h"

namespace grok {
namespace parser {

using namespace grok::vm;

void NullLiteral::emit(std::shared_ptr<InstructionBuilder> builder)
{
    auto instr = InstructionBuilder::Create<Instructions::push>();
    instr->data_type_ = d_null;

    builder->AddInstruction(std::move(instr));
}

void ThisHolder::emit(std::shared_ptr<InstructionBuilder> builder)
{
    auto instr = InstructionBuilder::Create<Instructions::fetch>();
    instr->data_type_ = d_name;
    instr->str_ = "this";

    builder->AddInstruction(std::move(instr));
    
    instr = InstructionBuilder::Create<Instructions::pushim>();
    instr->data_type_ = d_null;
    builder->AddInstruction(std::move(instr));
}

void IntegralLiteral::emit(std::shared_ptr<InstructionBuilder> builder)
{
    auto instr = InstructionBuilder::Create<Instructions::push>();
    instr->data_type_ = d_num;
    instr->number_ = value_;

    builder->AddInstruction(std::move(instr));
}

void StringLiteral::emit(std::shared_ptr<InstructionBuilder> builder)
{
    auto instr = InstructionBuilder::Create<Instructions::push>();
    instr->data_type_ = d_str;
    instr->str_ = str_;

    builder->AddInstruction(std::move(instr));
}

void BooleanLiteral::emit(std::shared_ptr<InstructionBuilder> builder)
{
    auto instr = InstructionBuilder::Create<Instructions::push>();
    instr->data_type_ = d_bool;
    instr->boolean_ = pred_;

    builder->AddInstruction(std::move(instr));
}

void Identifier::emit(std::shared_ptr<InstructionBuilder> builder)
{
    auto instr = InstructionBuilder::Create<Instructions::fetch>();
    instr->data_type_ = d_name;
    instr->str_ = name_;

    builder->AddInstruction(std::move(instr));
    
    instr = InstructionBuilder::Create<Instructions::pushim>();
    instr->data_type_ = d_null;
    builder->AddInstruction(std::move(instr));
}

void EmitBinaryOperator(BinaryExpression::Operator op,
        std::shared_ptr<InstructionBuilder> builder)
{
    auto instr = InstructionBuilder::Create<Instructions::noop>();
    switch (op) {
    default:
        throw TypeError("unknown operator");
    case PLUS:
        instr->kind_ = Instructions::adds;
        break;
    case MINUS:
        instr->kind_ = Instructions::subs;
        break;
    case MUL:
        instr->kind_ = Instructions::muls;
        break;
    case DIV:
        instr->kind_ = Instructions::divs;
        break;
    case SHR:
        instr->kind_ = Instructions::shrs;
        break;
    case SHL:
        instr->kind_ = Instructions::shls;
        break;
    case MOD:
        instr->kind_ = Instructions::rems;
        break;
    case LT:
        instr->kind_ = Instructions::lts;
        break;
    case GT:
        instr->kind_ = Instructions::gts;
        break;
    case LTE:
        instr->kind_ = Instructions::ltes;
        break;
    case GTE:
        instr->kind_ = Instructions::gtes;
        break;
    case EQUAL:
        instr->kind_ = Instructions::eqs;
        break;
    case NOTEQ:
        instr->kind_ = Instructions::neqs;
        break;
    case BAND:
        instr->kind_ = Instructions::bands;
        break;
    case BOR:
        instr->kind_ = Instructions::bors;
        break;
    case OR:
        instr->kind_ = Instructions::ors;
        break;
    case AND:
        instr->kind_ = Instructions::ands;
        break;
    case XOR:
        instr->kind_ = Instructions::xors;
        break;
    }
    instr->data_type_ = d_null;
    builder->AddInstruction(std::move(instr));
}

void PrefixExpression::emit(std::shared_ptr<InstructionBuilder> builder)
{
    if (expr_->ProduceRValue() && (tok_ == INC || tok_ == DEC))
        throw ReferenceError("can't apply prefix operator on "
            "r-value");

    expr_->emit(builder);
    if (tok_ == INC) {
        auto instr = InstructionBuilder::Create<Instructions::inc>();
        builder->AddInstruction(std::move(instr));
    } else if (tok_ == DEC) {
        auto instr = InstructionBuilder::Create<Instructions::dec>();
        builder->AddInstruction(std::move(instr));
    } else if (tok_ == NOT) {
        auto instr = InstructionBuilder::Create<Instructions::snot>();
        builder->AddInstruction(std::move(instr));
    } else if (tok_ == BNOT) {
        auto instr = InstructionBuilder::Create<Instructions::bnot>();
        builder->AddInstruction(std::move(instr));
    }
}

void PostfixExpression::emit(std::shared_ptr<InstructionBuilder> builder)
{
    if (expr_->ProduceRValue())
        throw ReferenceError("can't apply postfix operator on "
            "r-value");

    expr_->emit(builder);
    if (tok_ == INC) {
        auto instr = InstructionBuilder::Create<Instructions::pinc>();
        builder->AddInstruction(std::move(instr));
    } else if (tok_ == DEC) {
        auto instr = InstructionBuilder::Create<Instructions::pdec>();
        builder->AddInstruction(std::move(instr));    
    }
}

void BinaryExpression::emit(std::shared_ptr<InstructionBuilder> builder)
{
    lhs_->emit(builder);
    rhs_->emit(builder);

    EmitBinaryOperator(op_, builder);
}

void AssignExpression::emit(std::shared_ptr<InstructionBuilder> builder)
{
    // generate code for rhs
    rhs_->emit(builder);

    if (lhs_->ProduceRValue())
        throw ReferenceError("can't assign to an rvalue");

    // first we check whether the lhs_ is an Identifier
    auto maybe = dynamic_cast<Identifier*>(lhs_.get());

    if (maybe) {
        auto ns = InstructionBuilder::Create<Instructions::news>();
        ns->data_type_ = d_name;
        ns->str_ = maybe->GetName();
        builder->AddInstruction(std::move(ns));
        lhs_->emit(builder);
    } else {
        lhs_->emit(builder);
    }
    auto instr = InstructionBuilder::Create<Instructions::store>();
    instr->data_type_ = d_null;
    builder->AddInstruction(std::move(instr));
}

void TernaryExpression::emit(std::shared_ptr<InstructionBuilder> builder)
{
    // now build the code for conditional expression
    // result will be stored in the flags
    first_->emit(builder);

    auto popinstr = InstructionBuilder::Create<Instructions::pop>();
    builder->AddInstruction(std::move(popinstr));
    // add a jmpz instruction at the end of this block
    auto instr = InstructionBuilder::Create<Instructions::jmpz>();
    instr->data_type_ = d_null;
    instr->number_ = 0.0;
    instr->jmp_addr_ = 0;
    builder->AddInstruction(std::move(instr));

    // now create a block that will handle the instructions for second_
    builder->CreateBlock();
    second_->emit(builder);

    // add jmp instruction at the end of current block
    instr = InstructionBuilder::Create<Instructions::jmp>();
    builder->AddInstruction(std::move(instr));

    // update the jmpz instruction which is currently in stack
    builder->UpdateStackedJump();

    // create another block for third_
    builder->CreateBlock();
    third_->emit(builder);
    builder->EndBlockForJump();

    // end the block
    builder->EndBlock();
}

void CommaExpression::emit(std::shared_ptr<InstructionBuilder> builder)
{
    for (auto &expr : exprs_) {
        expr->emit(builder);
    }
}

void IfStatement::emit(std::shared_ptr<InstructionBuilder> builder)
{
    // code generation for if statement is almost same as that of ternary
    condition_->emit(builder);

    auto popinstr = InstructionBuilder::Create<Instructions::pop>();
    builder->AddInstruction(std::move(popinstr));

    auto instr = InstructionBuilder::Create<Instructions::jmpz>();
    instr->data_type_ = d_null;
    instr->jmp_addr_ = 0;
    builder->AddInstruction(std::move(instr));

    // create a block that will hold if body
    builder->CreateBlock();
    body_->emit(builder);
    builder->EndBlockForJump();
}

void IfElseStatement::emit(std::shared_ptr<InstructionBuilder> builder)
{
    // same as that of ternary expression
    // result of if condition will be stored in the flags
    condition_->emit(builder);

    auto popinstr = InstructionBuilder::Create<Instructions::pop>();
    builder->AddInstruction(std::move(popinstr));

    // add a jmpz instruction at the end of current block
    auto instr = InstructionBuilder::Create<Instructions::jmpz>();
    instr->data_type_ = d_null;
    instr->number_ = 0.0;
    instr->jmp_addr_ = 0;
    builder->AddInstruction(std::move(instr));

    // now create a block that will hold instruction for `if` body
    builder->CreateBlock();
    body_->emit(builder);

    // add jmp instruction at the end of current block used for skipping `else`
    instr = InstructionBuilder::Create<Instructions::jmp>();
    builder->AddInstruction(std::move(instr));
    
    // update the jmpz instruction which is currently in stack
    builder->UpdateStackedJump();

    // create another block for `else` body
    builder->CreateBlock();
    else_->emit(builder);
    builder->EndBlockForJump();

    // end the block
    builder->EndBlock();
}

void ForStatement::emit(std::shared_ptr<InstructionBuilder> builder)
{
    // init code for `for`
    init_->emit(builder);

    // start of the condition_ instructions
    auto cmp_blk_start = builder->CurrentLength();
    condition_->emit(builder);

    auto popinstr = InstructionBuilder::Create<Instructions::pop>();
    builder->AddInstruction(std::move(popinstr));

    // insert a jmpz instruction at the end of the condition block
    auto instr = InstructionBuilder::Create<Instructions::jmpz>();
    instr->data_type_ = d_null;
    instr->jmp_addr_ = 0; // to be calculate
    auto instr_ptr = instr.get();

    builder->AddInstruction(std::move(instr));

    // end of the condition instructions
    auto cmp_blk_end = builder->CurrentLength();

    body_->emit(builder);
    update_->emit(builder);

    popinstr = InstructionBuilder::Create<Instructions::pop>();
    builder->AddInstruction(std::move(popinstr));
    
    // insert a jmp back instruction
    instr = InstructionBuilder::Create<Instructions::jmp>();
    instr->data_type_ = d_null;
    instr->jmp_addr_ = 0;   // to be calculated later
    auto jmp_back_ptr = instr.get();
    builder->AddInstruction(std::move(instr));

    // mark the end of the for loop
    auto for_loop_end = builder->CurrentLength();

    // update the jump instructions
    instr_ptr->jmp_addr_ = for_loop_end - cmp_blk_end;
    jmp_back_ptr->jmp_addr_ = -(for_loop_end - cmp_blk_start);
}

void WhileStatement::emit(std::shared_ptr<InstructionBuilder> builder)
{
    // start of the condition_ instructions
    auto cmp_blk_start = builder->CurrentLength();
    condition_->emit(builder);

    auto popinstr = InstructionBuilder::Create<Instructions::pop>();
    builder->AddInstruction(std::move(popinstr));

    // insert a jmpz instruction at the end of the condition block
    auto instr = InstructionBuilder::Create<Instructions::jmpz>();
    instr->data_type_ = d_null;
    instr->jmp_addr_ = 0; // to be calculate
    auto instr_ptr = instr.get();

    builder->AddInstruction(std::move(instr));

    // end of the condition instructions
    auto cmp_blk_end = builder->CurrentLength();

    // generate code for while's body
    body_->emit(builder);

    popinstr = InstructionBuilder::Create<Instructions::pop>();
    builder->AddInstruction(std::move(popinstr));

    // insert a jmp back instruction
    instr = InstructionBuilder::Create<Instructions::jmp>();
    instr->data_type_ = d_null;
    instr->jmp_addr_ = 0;   // to be calculated later
    auto jmp_back_ptr = instr.get();
    builder->AddInstruction(std::move(instr));

    // mark the end of the while loop
    auto loop_end = builder->CurrentLength();

    // update the jump instructions
    instr_ptr->jmp_addr_ = loop_end - cmp_blk_end;
    jmp_back_ptr->jmp_addr_ = -(loop_end - cmp_blk_start);
}

void DoWhileStatement::emit(std::shared_ptr<InstructionBuilder> builder)
{
    auto cmp_blk_start = builder->CurrentLength();

    // generate code for body
    body_->emit(builder);

    auto popinstr = InstructionBuilder::Create<Instructions::pop>();
    builder->AddInstruction(std::move(popinstr));

    condition_->emit(builder);
    popinstr = InstructionBuilder::Create<Instructions::pop>();
    builder->AddInstruction(std::move(popinstr));

    // insert a jmp back instruction
    auto instr = InstructionBuilder::Create<Instructions::jmpnz>();
    instr->data_type_ = d_num;
    instr->jmp_addr_ = 0;   // to be calculated later
    auto jmp_back_ptr = instr.get();
    builder->AddInstruction(std::move(instr));

    // mark the end of the while loop
    auto loop_end = builder->CurrentLength();

    jmp_back_ptr->jmp_addr_ = -(loop_end - cmp_blk_start);
}

void BlockStatement::emit(std::shared_ptr<InstructionBuilder> builder)
{
    for (auto &stmt : stmts_) {
        stmt->emit(builder);
    }
}

void ArgumentList::emit(std::shared_ptr<InstructionBuilder> builder)
{
    for (auto &arg : args_) {
        arg->emit(builder);
    }
    auto inst = InstructionBuilder::Create<Instructions::mem_call>();
    builder->AddInstruction(std::move(inst));
    auto size = args_.size();
    auto call_instr = InstructionBuilder::Create<Instructions::call>();
    call_instr->data_type_ = d_num;

    // number of args passed
    call_instr->number_ = static_cast<double>(size);

    builder->AddInstruction(std::move(call_instr));
}

void FunctionCallExpression::emit(std::shared_ptr<InstructionBuilder> builder)
{
    // args will be on the top of the stack
    // now store the address of the function
    func_->emit(builder);

    // generate code for args
    for (auto &arg : args_) {
        arg->emit(builder);
    }

    auto size = args_.size();

    auto call_instr = InstructionBuilder::Create<Instructions::call>();
    call_instr->data_type_ = d_num;

    // number of args passed this will help in getting the actual
    // function from the stack
    call_instr->number_ = static_cast<double>(size);

    builder->AddInstruction(std::move(call_instr));
}

void DotMemberExpression::emit(std::shared_ptr<InstructionBuilder> builder)
{
    auto Property = mem_->GetName();

    // now the base object must lie in the stack, so we will
    // replace the object with its property Property
    auto repl = InstructionBuilder::Create<Instructions::replprop>();
    repl->data_type_ = d_name;
    repl->str_ = Property;
    builder->AddInstruction(std::move(repl));
}

void IndexMemberExpression::emit(std::shared_ptr<InstructionBuilder> builder)
{
    // now our array or the object lies in stack
    // evaluating the expression inside bracket will add one more element 
    // to the stack. That element will be used in index operator
    expr_->emit(builder);   // result of this expression is on the stack

    auto index = InstructionBuilder::Create<Instructions::index>();
    index->data_type_ = d_null;
    builder->AddInstruction(std::move(index));
}

void CallExpression::emit(std::shared_ptr<InstructionBuilder> builder)
{
    func_->emit(builder);

    if (members_.size() == 0)
        return;

    for (auto member = members_.begin();
                member != members_.end(); ++member) {
        (*member)->emit(builder);
    }
}

void MemberExpression::emit(std::shared_ptr<InstructionBuilder> builder)
{
    for (auto member = members_.begin();
                member != members_.end(); ++member) {
        (*member)->emit(builder);
    }
}

void ReturnStatement::emit(std::shared_ptr<InstructionBuilder> builder)
{
    if (builder->InsideFunction())
        expr_->emit(builder);
    else 
        throw TypeError("return statement was not inside function");

    auto ret = InstructionBuilder::Create<Instructions::ret>();
    builder->AddInstruction(std::move(ret));
}

void NewExpression::emit(std::shared_ptr<InstructionBuilder> builder)
{
    auto inst = InstructionBuilder::Create<Instructions::markst>();
    builder->AddInstruction(std::move(inst));
    member_->emit(builder);

    // auto instr = InstructionBuilder::Create<Instructions::pushthis>();
    // builder->AddInstruction(std::move(instr));
}

void Declaration::emit(std::shared_ptr<InstructionBuilder> builder)
{
    if (init_) {
        init_->emit(builder);
    }
    auto ns = InstructionBuilder::Create<Instructions::news>();
    ns->data_type_ = d_name;
    ns->str_ = name_;
    ns->number_ = 1;
    builder->AddInstruction(std::move(ns));

    auto instr = InstructionBuilder::Create<Instructions::fetch>();
    instr->data_type_ = d_name;
    instr->str_ = name_;

    builder->AddInstruction(std::move(instr));

    instr = InstructionBuilder::Create<Instructions::pushim>();
    instr->data_type_ = d_null;
    builder->AddInstruction(std::move(instr));

    instr = InstructionBuilder::Create<Instructions::store>();
    instr->data_type_ = d_null;
    builder->AddInstruction(std::move(instr));

}

void DeclarationList::emit(std::shared_ptr<InstructionBuilder> builder)
{
    for (auto &expr : exprs_) {
        expr->emit(builder);
    }
}

}
}
