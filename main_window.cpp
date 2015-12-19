#include "main_window.h"

#include "ui_main_window.h"

#include <QTimer>
#include <memory>

struct MainWindow::Impl
{
    Ui::MainWindow ui;
};

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m( std::make_unique<Impl>() )
{
    m->ui.setupUi( this );
    auto timer = std::make_unique<QTimer>( this );
    connect( timer.get(), &QTimer::timeout, m->ui.display, [this]
    {
        QPixmap pixmap(this->width(),this->height());
        pixmap.fill( Qt::black );
        m->ui.display->setPixmap( pixmap );
    });
    timer->start(40);
    timer.release();
}

MainWindow::~MainWindow()
{
}
