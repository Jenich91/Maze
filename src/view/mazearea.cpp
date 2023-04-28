#include "view.h"

namespace sfleta {
void View::on_pushButton_draw_maze_clicked() {
  if (ui_->buttonGroup_data_source_maze->checkedButton()) {
    ui_->widget->setMouseTracking(true);
    ChangeCheckedButtonDrawMazeAndClickState(true);
    ChangeEnabledButtonDataSourceSectionMaze(
        ui_->radioButton_random_fill_maze->isChecked());

    ui_->commandLinkButton_save_maze->setEnabled(true);

    if (ui_->radioButton_random_fill_maze->isChecked()) {
      ctrl_->InitMaze(ui_->spinBox_rows_amount_maze->value(),
                      ui_->spinBox_cols_amount_maze->value());
    } else {
      if (ui_->commandLinkButton_load_maze->isChecked() &&
          !filepath_.isEmpty()) {
        ctrl_->InitMaze(filepath_);
      } else {
        return;
      }
    }
    DrawMaze();
  } else {
    draw_state = false;
    ui_->commandLinkButton_save_maze->setEnabled(false);
  }
}

void View::DrawMaze() {
  ClearRenderScreen();
  w_size_ = static_cast<float>(window_size_px_) / ctrl_->GetColsMaze();
  h_size_ = static_cast<float>(window_size_px_) / ctrl_->GetRowsMaze();

  for (size_t i = 0; i < ctrl_->GetRowsMaze(); i++) {
    for (size_t j = 0; j < ctrl_->GetColsMaze(); j++) {
      float x0_w = (j + 1) * w_size_;
      float y0_w = i * h_size_;
      float x1_w = (j + 1) * w_size_;
      float y1_w = (i + 1) * h_size_;

      float x0_f = j * w_size_;
      float y0_f = (i + 1) * h_size_;
      float x1_f = (j + 1) * w_size_;
      float y1_f = (i + 1) * h_size_;

      if (ctrl_->GetBufferMaze()[i][j].right)
        render_->DrawLine(x0_w, y0_w, x1_w, y1_w);
      if (ctrl_->GetBufferMaze()[i][j].down)
        render_->DrawLine(x0_f, y0_f, x1_f, y1_f);
    }
  }
  update();
}

bool View::eventFilter(QObject *target, QEvent *event) {
  QPoint static start_pos{0, 0};

  if (ui_->tabWidget->currentIndex() == 0 && draw_state &&
      ui_->buttonGroup_data_source_maze->checkedButton()) {
    QMouseEvent *e = reinterpret_cast<QMouseEvent *>(event);
    if (event->type() == QEvent::MouseButtonPress &&
        e->button() == Qt::LeftButton) {
      QPoint click_pos(e->pos().x() / w_size_, e->pos().y() / h_size_);

      if (first_click) {
        start_pos.setX(click_pos.y());
        start_pos.setY(click_pos.x());
        render_->ClearRout();
        render_->DrawMousePos(GetRectForCell(click_pos), true);
        render_->DrawDot(GetRectForPointImage(click_pos));
        first_click = false;
      } else {
        QPoint finish_pos;
        finish_pos.setX(click_pos.y());
        finish_pos.setY(click_pos.x());
        render_->DrawMousePos(GetRectForCell(click_pos), true);
        DrawDirections(start_pos, finish_pos);
        render_->DrawDot(GetRectForPointImage(click_pos));
        first_click = true;
      }
      update();
      return true;

    } else if (event->type() == QEvent::MouseMove) {
      mouseMoveEvent(e);
      update();
      return true;
    }
  }
  return false;
}

void View::mouseMoveEvent(QMouseEvent *e) {
  QPoint click_pos(e->pos().x() / w_size_, e->pos().y() / h_size_);
  if (ui_->tabWidget->currentIndex() == 0)
    render_->DrawMousePos(GetRectForCell(click_pos));
}

void View::on_radioButton_random_fill_maze_clicked() {
  ChangeEnabledButtonDataSourceSectionMaze(true);
}

void View::on_radioButton_from_file_maze_clicked() {
  ChangeEnabledButtonDataSourceSectionMaze(false);
}

void View::ChangeEnabledButtonDataSourceSectionMaze(bool state) {
  ui_->spinBox_rows_amount_maze->setEnabled(state);
  ui_->spinBox_cols_amount_maze->setEnabled(state);
  ui_->commandLinkButton_load_maze->setEnabled(!state);

  ResetButtonIconAndEnabled();
}

void View::on_commandLinkButton_load_maze_clicked() {
  OpenFileWithChangeButtonIconAndStatus(ui_->commandLinkButton_load_maze,
                                        ui_->statusbar, &filepath_);
}

void View::SaveFileWithChangeButtonIconAndStatus(QCommandLinkButton *clbtn,
                                                 QStatusBar *sb) {
  QString saveToPath = QFileDialog::getSaveFileName(
      this, "Choose where save file", QDir::homePath());

  if (!saveToPath.isEmpty()) {
    clbtn->setIcon(QIcon(":/icons/check_pass.png"));
    sb->showMessage(QString("File saved in - " + saveToPath));
    clbtn->setChecked(true);

    SaveFile(saveToPath);
  } else {
    clbtn->setIcon(QIcon(":/icons/check_false.png"));
    sb->showMessage(QString("Cannot save file!"));
    clbtn->setChecked(false);
  }
}

void View::SaveFile(QString filepath) {
  QFile File(filepath);
  File.open(QIODevice::WriteOnly | QFile::Text);

  size_t rows_n = ctrl_->GetRowsMaze();
  size_t cols_n = ctrl_->GetColsMaze();

  File.write(
      (std::to_string(rows_n) + ' ' + std::to_string(cols_n) + '\n').c_str());

  auto buff = ctrl_->GetBufferMaze();
  for (auto &i : buff) {
    for (auto elem : i) {
      File.write((std::to_string(elem.right) + ' ').c_str());
    }
    File.write("\n");
  }

  File.write("\n");

  for (auto &i : buff) {
    for (auto elem : i) {
      File.write((std::to_string(elem.down) + ' ').c_str());
    }
    File.write("\n");
  }

  File.close();
}

void View::ChangeCheckedButtonDrawMazeAndClickState(bool state) {
  draw_state = state;
  first_click = state;
}

void View::on_commandLinkButton_save_maze_clicked() {
  SaveFileWithChangeButtonIconAndStatus(ui_->commandLinkButton_save_maze,
                                        ui_->statusbar);
}

void View::ResetUiElementsStateMaze() {
  ui_->buttonGroup_data_source_maze->setExclusive(false);
  ui_->radioButton_random_fill_maze->setChecked(false);
  ui_->radioButton_from_file_maze->setChecked(false);
  ui_->buttonGroup_data_source_maze->setExclusive(true);

  ChangeEnabledButtonDataSourceSectionMaze(false);

  ui_->commandLinkButton_load_maze->setChecked(false);
  ui_->commandLinkButton_load_maze->setIcon(QIcon(":/icons/load.png"));
  ui_->commandLinkButton_load_maze->setEnabled(false);

  ResetButtonIconAndEnabled();

  ClearStringAndStatus(ui_->statusbar, &filepath_);
  ChangeCheckedButtonDrawMazeAndClickState(false);
}

void View::ResetButtonIconAndEnabled() {
  ui_->commandLinkButton_save_maze->setChecked(false);
  ui_->commandLinkButton_save_maze->setIcon(QIcon(":/icons/save.png"));
  ui_->commandLinkButton_save_maze->setEnabled(false);
}

void View::on_pushButton_clear_maze_clicked() {
  ClearRenderScreen();
  ClearMousePosHigthligth();
  ResetUiElementsStateMaze();
  update();
}

void View::ClearMousePosHigthligth() {
  ui_->widget->setMouseTracking(false);
  render_->ClearMouse();
}

void View::DrawDirections(const QPoint start_pos, const QPoint finish_pos) {
  auto checkpoints = ctrl_->FindPathMaze(start_pos, finish_pos);
  QPolygon polygon;

  for (auto point_pos : checkpoints) {
    polygon << QPoint(w_size_ * point_pos.second + w_size_ / 2,
                      h_size_ * point_pos.first + h_size_ / 2);
  }
  render_->DrawDirections(polygon);
  update();
}
}  // namespace sfleta
