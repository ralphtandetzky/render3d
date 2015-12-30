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
  std::array<cu::Vec<float,2>,8> transformedPoints;
  for ( std::size_t i = 0; i!= points.size(); ++i )
  {
      const auto vec = systemMatrix * points[i];
      transformedPoints[i] = { 1.5f*scaleFactor * vec[0] / -vec[2] + 0.5f*width,
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
            const auto & P = transformedPoints[ i               ];
            const auto & Q = transformedPoints[ i | bit1        ];
            const auto & R = transformedPoints[ i        | bit2 ];
            const auto & S = transformedPoints[ i | bit1 | bit2 ];
            cu::drawTriangle( img, P, Q, S, (unsigned char)0xFF );
            cu::drawTriangle( img, P, R, S, (unsigned char)0xFF );
        }
  }

  QPainter painter(this);
  painter.fillRect( this->rect(), Qt::black );
  painter.setPen( Qt::white );
  QImage qImg( img.getNCols(), img.getNRows(), QImage::Format_RGB888 );
  qImg.fill( Qt::black );
  for ( std::size_t row = 0; row < img.getNRows(); ++row )
    for ( std::size_t col = 0; col < img.getNCols(); ++col )
      qImg.setPixel( col, row, img[row][col] ? 0xFFFFFFFF : 0xFF );
  painter.drawImage( this->rect(), qImg );
}


MainWindow::~MainWindow() = default;
