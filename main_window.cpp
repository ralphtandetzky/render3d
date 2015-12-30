#include "main_window.hpp"
#include "ui_main_window.h"

#include "drawing.hpp"
#include "vec.hpp"
#include "mat.hpp"
#include "trafo_mats.hpp"

#include <QPainter>
#include <QTimer>

#include <algorithm>

struct MainWindow::Impl
{
  Ui::MainWindow ui;
  float angle{};
};

MainWindow::MainWindow(QWidget *parent)
  : QWidget(parent)
  , m( std::make_unique<Impl>() )
{
  m->ui.setupUi(this);
  auto timer = new QTimer(this);
  connect( timer, &QTimer::timeout, this, [this](){ m->angle += 0.01; update(); } );
  timer->start(20);
}

void MainWindow::paintEvent( QPaintEvent * )
{
  std::array<cu::Vec<float,4>,8> points =
  {{
    { 1, 1, 1, 1},
    { 1, 1,-1, 1},
    { 1,-1, 1, 1},
    { 1,-1,-1, 1},
    {-1, 1, 1, 1},
    {-1, 1,-1, 1},
    {-1,-1, 1, 1},
    {-1,-1,-1, 1}
  }};
  const auto shiftMat =
          cu::makeTranslationMat( cu::makeVec(0.f,0.f,-6.f) );
  const auto rotMat =
          cu::makeExtendedMat(
          cu::makeRotationMat( cu::makeVec( -0.3f,0.f,0.f ) ) *
          cu::makeRotationMat( m->angle*cu::makeVec(0.f,1.f,0.f) ) );
  const auto width = this->width();
  const auto height = this->height();
  const auto scaleFactor = std::min( width, height );
  const auto systemMatrix =
          shiftMat *
          rotMat;
  std::array<cu::Vec<float,2>,8> transformedPoints2d;
  std::array<cu::Vec<float,3>,8> transformedPoints3d;
  for ( std::size_t i = 0; i!= points.size(); ++i )
  {
    transformedPoints3d[i] = cu::popBack( systemMatrix * points[i] );
    const auto & vec = transformedPoints3d[i];
    transformedPoints2d[i] = { 1.5f*scaleFactor * vec[0] / -vec[2] + 0.5f*width,
                               1.5f*scaleFactor * vec[1] / -vec[2] + 0.5f*height };
  }

  cu::Mat<unsigned char> img( this->height(), this->width() );
  std::fill_n( img.data(), img.getNRows()*img.getNCols(), 0 );
  for ( std::size_t i = 0; i!= points.size(); ++i )
  {
    for ( auto bit1 : { 1, 2, 4 } )
      for ( auto bit2 : { 1, 2, 4 } )
      {
        if ( bit1 >= bit2 )
            continue;
        if ( i & ( bit1 | bit2 ) )
            continue;
        const auto & P2d = transformedPoints2d[ i               ];
        const auto & Q2d = transformedPoints2d[ i | bit1        ];
        const auto & R2d = transformedPoints2d[ i        | bit2 ];
        const auto & S2d = transformedPoints2d[ i | bit1 | bit2 ];
        const auto & P3d = transformedPoints3d[ i               ];
        const auto & Q3d = transformedPoints3d[ i | bit1        ];
        const auto & R3d = transformedPoints3d[ i        | bit2 ];
        const auto normalVec = cu::normalVector( P3d, Q3d, R3d );
        const auto lightVec = cu::normalize( cu::makeVec( 1.f, 1.f, -2.f ) );
        const auto absCos = std::abs( normalVec * lightVec );
        const unsigned char color = (0.8*absCos*absCos+0.2) * 0xFF;
        cu::drawTriangle( img, P2d, Q2d, S2d, color );
        cu::drawTriangle( img, P2d, R2d, S2d, color );
      }
  }

  QPainter painter(this);
  painter.fillRect( this->rect(), Qt::black );
  painter.setPen( Qt::white );
  QImage qImg( img.getNCols(), img.getNRows(), QImage::Format_RGB888 );
  qImg.fill( Qt::black );
  for ( std::size_t row = 0; row < img.getNRows(); ++row )
    for ( std::size_t col = 0; col < img.getNCols(); ++col )
      qImg.setPixel( col, row, img[row][col] * 0x10101 + 0xFF000000 );
  painter.drawImage( this->rect(), qImg );
}


MainWindow::~MainWindow() = default;
