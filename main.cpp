#include "vec3d.h"
#include "main_window.hpp"
#include "mat.hpp"
#include "vec.hpp"

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


static void testVec()
{
    using cu::Vec;

    Vec<double,2> a = {3,4};
    Vec<double,2> b = {5,12};
    Vec<double,2> c = b - 2.*a;
    assert( l2Norm(a) == 5 );
    assert( a*b == 63 );
    assert( c == (Vec<double,2>({-1,4})));
}


static void testMat()
{
    using cu::Vec;
    using cu::Mat;

    Vec<double,3> v = { 1, 2, 3 };
    Mat<double,2,3> m = {
    { 0, 0, 1 },
    { 0, 1, 0 }
    };
    assert( m*v == (Vec<double,2>{ 3, 2 }) );
    assert( m*transpose(m) == ( Mat<double,2,2>{ {1,0}, {0,1} } ) );
}


int main(int argc, char *argv[])
{
    testVec();
    testMat();
    test();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
