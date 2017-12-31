
#include <string>
#include "isBlankLine.h"

using namespace std;

bool isBlankLine(char const* line)
{
    for ( char const* cp = line; *cp; ++cp )
    {
        if ( !isspace(*cp) ) return false;
    }
    return true;
}

bool isBlankLine(string const& line)
{
   return isBlankLine(line.c_str());
}
