// Last Update:2016-10-21 15:18:00

#include <ostream>
#include <cxxabi.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>

#include "backtrace.h"

#define _STR(x) #x
#define STRINGIFY(x) _STR(x)

std::string const pretty_version_to_str(void)
{
  std::ostringstream oss;
  oss << "test: " << "1.0";
  return oss.str();
}

void BackTrace::print(std::ostream& out)
{
  out << " " << pretty_version_to_str() << std::endl;
  for (size_t i = skip; i < size; i++) {
    out << " " << (i-skip+1) << ": " << strings[i] << std::endl;

    size_t sz = 1024; // just a guess, template names will go much wider
    char *function = (char *)malloc(sz);
    if (!function)
      return;
    char *begin = 0, *end = 0;

    // find the parentheses and address offset surrounding the mangled name
    for (char *j = strings[i]; *j; ++j) {
      if (*j == '(')
        begin = j+1;
      else if (*j == '+')
        end = j;
    }
    if (begin && end) {
      int len = end - begin;
      char *foo = (char *)malloc(len+1);
      if (!foo) {
        free(function);
        return;
      }
      memcpy(foo, begin, len);
      foo[len] = 0;

      int status;
      char *ret = abi::__cxa_demangle(foo, function, &sz, &status);
      if (ret) {
        // return value may be a realloc() of the input
        function = ret;
      }
      else {
        // demangling failed, just pretend it's a C function with no args
        strncpy(function, foo, sz);
        strncat(function, "()", sz);
        function[sz-1] = 0;
      }
      out << " " << (i-skip+1) << ": (" << function << end << std::endl;
      //fprintf(out, "    %s:%s\n", stack.strings[i], function);
      free(foo);
    } else {
      // didn't find the mangled name, just print the whole line
      out << " " << (i-skip+1) << ": " << strings[i] << std::endl;
    }
    free(function);
  }
}
