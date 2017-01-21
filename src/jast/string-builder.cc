#include "jast/string-builder.h"

#include <vector>

namespace jast {

// static
String StringFactory::NewString(const char *str)
{
    return String(str);
}

String StringFactory::NewStringFromFile(String name)
{
    std::vector<char> v;
    if (FILE *fp = fopen(name.c_str(), "r"))
    {
        char buf[1024];
        while (size_t len = fread(buf, 1, sizeof(buf), fp))
            v.insert(v.end(), buf, buf + len);
        fclose(fp);
    }

    return String(v.begin(), v.end());
}

String StringFactory::NewStringFromStd(const std::string &str)
{
    return str;
}

}
