#include "view.h"

namespace sfleta {
View::View(sfleta::Controller *ctrl, QWidget *parent)
    : QMainWindow(parent),
      ui_(new Ui::View),
      render_{new Renderer},
      ctrl_{ctrl} {
  InitUi();
}

void View::InitUi() {
  ui_->setupUi(this);

  ui_->commandLinkButton_load->setIcon(QIcon(":/icons/load.png"));
  ui_->commandLinkButton_load->setCheckable(true);

  ui_->pushButton_next_step->setCheckable(true);

  connect(this, SIGNAL(IsTimerAlive()), this, SLOT(StartDrawMessage()));
  connect(this, SIGNAL(IsTimerDead()), this, SLOT(StopDrawMessage()));

  ui_->commandLinkButton_load_maze->setIcon(QIcon(":/icons/load.png"));
  ui_->commandLinkButton_load_maze->setCheckable(true);

  ui_->commandLinkButton_save_maze->setIcon(QIcon(":/icons/save.png"));
  ui_->commandLinkButton_save_maze->setCheckable(true);
  ui_->commandLinkButton_save_maze->setStyleSheet("text-align:center;");

  ui_->widget->installEventFilter(this);

  ui_->widget->setCursor(QCursor(
      QPixmap(":/icons/cave.png")
          .scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation),
      -1, 15));
}

void View::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  QRect rect_(ui_->widget->pos().x(), ui_->widget->pos().y(), window_size_px_,
              window_size_px_);

  painter.drawImage(rect_, render_->GetMousePosImg());
  painter.drawImage(rect_, render_->GetRoutImage());
  painter.drawImage(rect_, render_->GetMazeImage());

  painter.drawRect(ui_->widget->pos().x() + 4, ui_->widget->pos().y() + 3,
                   window_size_px_ - 8, window_size_px_ - 7);
}

void View::ClearRenderScreen() {
  render_->ClearRout();
  render_->ClearModel();
  repaint();
}

void View::ClearStringAndStatus(QStatusBar *sb, QString *filepath) {
  sb->clearMessage();
  filepath->clear();
}

void View::OpenFileWithChangeButtonIconAndStatus(QCommandLinkButton *clbtn_load,
                                                 QStatusBar *sb,
                                                 QString *filepath) {
  *filepath =
      QFileDialog::getOpenFileName(this, "Choose data file", QDir::homePath());

  if (!filepath->isEmpty()) {
    clbtn_load->setIcon(QIcon(":/icons/check_pass.png"));
    sb->showMessage(QString("Opened file from - " + *filepath));
    clbtn_load->setChecked(true);
  } else {
    clbtn_load->setIcon(QIcon(":/icons/check_false.png"));
    sb->showMessage(QString("Cannot open file!"));
    clbtn_load->setChecked(false);
  }
}

void View::on_tabWidget_currentChanged(int index) {
  if (index == 0) {
    on_pushButton_clear_clicked();
    ui_->widget->setCursor(QCursor(
        QPixmap(":/icons/point.png")
            .scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        -1, 15));
  } else {
    on_pushButton_clear_maze_clicked();
    ui_->widget->setCursor(QCursor(
        QPixmap(":/icons/cave.png")
            .scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        -1, 15));
  }
}

QRect View::GetRectForPointImage(QPoint click_pos) {
  float img_size = std::min(w_size_, h_size_) / 2.;
  float shift_x = w_size_ >= h_size_ ? w_size_ / 2 - img_size / 2
                                     : w_size_ / 2 - w_size_ / 4;
  float shift_y = w_size_ < h_size_ ? h_size_ / 2 - img_size : 0;

  return QRect(int(click_pos.x()) * w_size_ + shift_x,
               int(click_pos.y()) * h_size_ + shift_y + img_size * 0.105,
               img_size, img_size);
}

QRect View::GetRectForCell(QPoint click_pos) {
  return QRect(int(click_pos.x()) * w_size_, int(click_pos.y()) * h_size_,
               w_size_, h_size_);
}
}  // namespace sfleta
