#include "main_window.hpp"
#include "ui_main_window.h"


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


MainWindow::~MainWindow() = default;
