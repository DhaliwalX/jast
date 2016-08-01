#include "parser/expression.h"
#include "vm/instruction-builder.h"

namespace grok {
namespace parser {

using namespace grok::vm;

// Code Generation for Objects
// ==============================

// We will first generate code for the expressions required to create
// the value for a property. Then map the value of the expression (in stack)
// to the name of the property. Thus continuing the same procedure for all
// other properties. Now we have all the properties with their names in stack
// We will be able to store all properties to the object just popping from the
// stack.
void ObjectLiteral::emit(std::shared_ptr<InstructionBuilder> builder)
{
    for (auto &Prop : Props) {
        Prop.second->emit(builder);

        /// \instruction: maps 
        /// This instruction stores the name of the property with the
        /// top of the stack
        auto instr = InstructionBuilder::Create<Instructions::maps>();
        instr->data_type_ = d_name;
        instr->str_ = Prop.first;
        builder->AddInstruction(std::move(instr));
    }

    auto I = InstructionBuilder::Create<Instructions::poprop>();
    I->data_type_ = d_num;
    I->number_ = Props.size();
    builder->AddInstruction(std::move(I));
}

} // parser
} // grok
