// main.cpp — Application entry point: initializes Qt and shows the Matrix Calculator main window.

#include "MainWindow.hpp"
#include <QApplication>
#include <QFont>
#include <QPalette>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  app.setApplicationName(QStringLiteral("Matrix Calculator"));
  app.setApplicationVersion(QStringLiteral("1.0"));

  QFont font = app.font();
  font.setPointSize(font.pointSize() >= 10 ? font.pointSize() : 10);
  app.setFont(font);

  MainWindow mainWin;
  mainWin.show();
  return app.exec();
}
