#include "main_window.h"

#include "vec3d.h"

#include <QApplication>

#include <cassert>

void test()
{
    Vec<double,3> v1 = {{{3.0,2.0,1.0}}};
    Vec<double,3> v2, v3;
    v2 += v1;
    v3 = v1 * 2. - v2 / 2.;
    v3 = {{{3.0,4.0,12.0}}};
    const auto x = v1*v2;
    const auto y = sqrNorm(v1);
    const auto z = norm(v3);
    assert( x == y );
    assert( z == x-1 );
}


int main(int argc, char *argv[])
{
    test();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
