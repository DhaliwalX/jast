#include "jast/context.h"
#include "jast/scope.h"

#include <memory>

namespace jast {

class ParserContextImpl {
public:
    ParserContextImpl()
        : global_scope_{ std::make_unique<Scope>(nullptr, nullptr) }
    { }

    Scope *global_scope() { return global_scope_.get(); }
private:
    std::unique_ptr<Scope> global_scope_;
};

ParserContext::ParserContext()
    : impl_{ new ParserContextImpl() }
{}

ParserContext::~ParserContext()
{
    delete impl_;
}

Scope *ParserContext::GetGlobalScope() {
    return impl_->global_scope();
}

}
