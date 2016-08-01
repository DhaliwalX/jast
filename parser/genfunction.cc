#include "object/function.h"
#include "parser/functionstatement.h"
#include "vm/instruction-builder.h"
#include "vm/instruction.h"

namespace grok {
namespace parser {

using namespace grok::vm;
using namespace grok::obj;

/// emits code for function definition
/// We don't generate the code for the body now but defer it until
/// it is required i.e. when a call is placed to this function during
/// execution
void FunctionStatement::emit(std::shared_ptr<InstructionBuilder> builder)
{
    std::string Name = proto_->GetName();
    auto F = CreateFunction(std::move(body_), std::move(proto_));

    auto instr = InstructionBuilder::Create<Instructions::push>();
    instr->data_type_ = d_obj;
    instr->data_ = (F);

    builder->AddInstruction(std::move(instr));
    instr = InstructionBuilder::Create<Instructions::news>();
    instr->data_type_ = d_name;
    instr->str_ = Name;
    builder->AddInstruction(std::move(instr));
    
    instr = InstructionBuilder::Create<Instructions::fetch>();
    instr->data_type_ = d_name;
    instr->str_ = Name;
    builder->AddInstruction(std::move(instr));

    instr = InstructionBuilder::Create<Instructions::pushim>();
    builder->AddInstruction(std::move(instr));

    instr = InstructionBuilder::Create<Instructions::store>();
    builder->AddInstruction(std::move(instr));
}

void FunctionPrototype::emit(std::shared_ptr<InstructionBuilder>)
{
    // do nothing
}

} // parser
} // grok
