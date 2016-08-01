#include "parser/expression.h"
#include "vm/instruction.h"
#include "vm/instruction-builder.h"
#include "vm/instruction-list.h"

namespace grok {
namespace parser {
using namespace grok::vm;

void ArrayLiteral::emit(std::shared_ptr<InstructionBuilder> builder)
{
    // we generate instructions for the expressions, the result
    // of all those instructions will be stored in stack
    for (auto &i : exprs_) {
        i->emit(builder);
    }
    auto reserve = exprs_.size();
    auto instr = InstructionBuilder::Create<Instructions::res>();

    // TODO : find a better way to store the size
    instr->number_ = static_cast<double>(reserve);
    instr->data_type_ = d_num;
    builder->AddInstruction(std::move(instr));

    // copy the values stored in stack to the reserved array
    instr = InstructionBuilder::Create<Instructions::cpya>();
    instr->data_type_ = d_num;
    instr->number_ = static_cast<double>(reserve);
    builder->AddInstruction(std::move(instr));

    // push the array so created to the stack
    instr = InstructionBuilder::Create<Instructions::pushim>();
    instr->data_type_ = d_null;
    builder->AddInstruction(std::move(instr));
}

}
}
