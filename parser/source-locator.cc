#include "parser/source-locator.h"
#include "parser/lexinfo.h"
namespace grok {
namespace parser {

SourceLocation &SourceLocator::loc()
{
    auto pos = LexerInfo::GetSeek();
    cache_ = SourceLocation(pos);
    return cache_;
}

}
}
