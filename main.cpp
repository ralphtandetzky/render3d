#include "main_window.hpp"
#include "vec.hpp"

#include <QApplication>
#include <cassert>

void testVec()
{
  using cu::Vec;

  Vec<double,2> a = {3,4};
  Vec<double,2> b = {5,12};
  Vec<double,2> c = b - 2.*a;
  assert( l2Norm(a) == 5 );
  assert( a*b == 63 );
  assert( c == (Vec<double,2>({-1,4})));
}


int main(int argc, char *argv[])
{
  testVec();
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
