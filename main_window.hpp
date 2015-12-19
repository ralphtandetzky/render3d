#pragma once

#include <QWidget>
#include <memory>

class MainWindow final : public QWidget
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow() override;

  virtual void paintEvent( QPaintEvent * event );

private:
  struct Impl;
  std::unique_ptr<Impl> m;
};
