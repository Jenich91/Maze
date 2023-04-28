#include <QApplication>
#include <QDir>
#include <QIcon>

#include "controller/controller.h"
#include "view/view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  Q_INIT_RESOURCE(icons);
  a.setWindowIcon(QIcon(":/icons/icon.png"));

  sfleta::Controller controller;
  sfleta::View view(&controller);

  view.setWindowTitle("🧩Maze🧩");
  view.setFixedSize(846, 600);
  view.show();

  return a.exec();
}
