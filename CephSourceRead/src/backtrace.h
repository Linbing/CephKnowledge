// Last Update:2016-10-21 15:15:54


#ifndef BACKTRACE_H
#define BACKTRACE_H

#include <iosfwd>
#include <execinfo.h>
#include <stdlib.h>

struct BackTrace {
  const static int max = 100;

  int skip;
  void *array[max];
  size_t size;
  char **strings;

  BackTrace(int s) : skip(s) {
    size = backtrace(array, max);
    strings = backtrace_symbols(array, size);
  }
  ~BackTrace() {
    free(strings);
  }

  BackTrace(const BackTrace& other);
  const BackTrace& operator=(const BackTrace& other);

  void print(std::ostream& out);
};

#endif  /*BACKTRACE_H*/
