#include "main_window.hpp"
#include "ui_main_window.h"

#include "vec.hpp"
#include "mat.hpp"

#include <QPainter>

struct MainWindow::Impl
{
  Ui::MainWindow ui;
};

MainWindow::MainWindow(QWidget *parent)
  : QWidget(parent)
  , m( std::make_unique<Impl>() )
{
  m->ui.setupUi(this);
}

void MainWindow::paintEvent( QPaintEvent * )
{
  QPainter painter(this);
  painter.fillRect( this->rect(), Qt::black );
  std::array<cu::Vec<double,4>,8> points =
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
  const cu::Mat<double,4,4> trafoMat = {
    { 1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 1,-8 },
    { 0, 0, 0, 1 }
  };
}


MainWindow::~MainWindow() = default;
