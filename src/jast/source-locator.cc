#include "jast/source-locator.h"
namespace jast {

Position &SourceLocator::loc()
{
    auto pos = parent_->currentToken().position();
    cache_ = pos;
    return cache_;
}

}
