// Last Update:2016-10-21 15:23:43

#include <assert.h>

#include <ostream>
#include <fstream>

#include "backtrace.h"

int main(int argc, const char *argv[])
{
  BackTrace *bt = new BackTrace(1);
  std::filebuf fb;

  fb.open("test.log", std::ios::out);
  std::ostream dout(&fb);
  dout << "test: \n";

  bt->print(dout);
  assert(0);
  fb.close();

  return 0;
}
