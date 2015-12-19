#pragma once

#include <QWidget>
#include <memory>

class MainWindow: public QWidget
{
    Q_OBJECT

public:
    MainWindow( QWidget *parent = nullptr );
    ~MainWindow();

private:
    struct Impl;
    std::unique_ptr<Impl> m;
};
