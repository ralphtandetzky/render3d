#include "main_window.hpp"
#include "ui_main_window.h"

#include "vec.hpp"
#include "mat.hpp"
#include "trafo_mats.hpp"

#include <QPainter>
#include <QTimer>

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

  QPainter painter(this);
  painter.fillRect( this->rect(), Qt::black );
  painter.setPen( Qt::white );
  for ( std::size_t i = 0; i!= points.size(); ++i )
  {
      for ( auto bit : { 1, 2, 4 } )
      {
          if ( i & bit )
              continue;
          const auto & P = transformedPoints[ i       ];
          const auto & Q = transformedPoints[ i | bit ];
          painter.drawLine( QPointF( P[0], P[1] ), QPointF( Q[0], Q[1] ) );
      }
  }
}


MainWindow::~MainWindow() = default;
