#include "view.h"

namespace sfleta {
void View::on_pushButton_draw_clicked() {
  if (ui_->buttonGroup_data_source->checkedButton() &&
      ui_->buttonGroup_mode_next_step->checkedButton()) {
    ChangeEnabledButtonNextSection(ui_->radioButton_manual->isChecked());

    if (ui_->radioButton_random_fill->isChecked()) {
      ctrl_->InitCaves(ui_->spinBox_rows_amount->value(),
                       ui_->spinBox_cols_amount->value(),
                       ui_->spinBox_chance->value());
    } else {
      if (ui_->commandLinkButton_load->isChecked()) {
        ctrl_->InitCaves(filepath_);
      } else {
        return;
      }
    }

    DrawCave();

    next_status_ = true;
    if (timer_ != nullptr) {
      timer_->stop();
    }
    if (ui_->radioButton_auto->isChecked()) {
      timer_ = new QTimer(this);
      emit IsTimerAlive();
      connect(timer_, SIGNAL(timeout()), this, SLOT(NextStep()));
      timer_->start(timer_duration_ /
                    ui_->horizontalSlider_timer_speed->value());
    }
  }
}

void View::DrawCave() {
  ClearRenderScreen();
  h_size_ = static_cast<float>(window_size_px_) / ctrl_->GetColsCaves();
  w_size_ = static_cast<float>(window_size_px_) / ctrl_->GetRowsCaves();

  for (int i = 0; i < ctrl_->GetRowsCaves(); ++i) {
    for (int j = 0; j < ctrl_->GetColsCaves(); ++j) {
      if (ctrl_->GetElemCaves(i, j)) {
        render_->DrawRectangle(
            QRect(h_size_ * j, w_size_ * i, h_size_, w_size_));
      }
    }
  }
  repaint();
}

void View::on_pushButton_next_step_clicked() {
  if (!ctrl_->IsEmptyCaves()) {
    NextStep();
  }
}

void View::NextStep() {
  bool stop = ctrl_->MetamorphosisCaves(ui_->spinBox_births_limit->value(),
                                        ui_->spinBox_death_limit->value());
  NextStepStatusbar();
  if (ui_->radioButton_auto->isChecked() && stop) {
    timer_->stop();
    emit IsTimerDead();
  }
  DrawCave();
}

void View::NextStepStatusbar() {
  if (next_status_) {
    ui_->statusbar->showMessage("Step by...");
  } else {
    ui_->statusbar->showMessage("...step");
  }
  next_status_ = !next_status_;
}

void View::ResetUiElementsState() {
  ui_->buttonGroup_data_source->setExclusive(false);
  ui_->radioButton_random_fill->setChecked(false);
  ui_->radioButton_from_file->setChecked(false);
  ui_->buttonGroup_data_source->setExclusive(true);

  ChangeEnabledButtonDataSourceSection(false);

  ui_->buttonGroup_mode_next_step->setExclusive(false);
  ui_->radioButton_auto->setChecked(false);
  ui_->radioButton_manual->setChecked(false);
  ui_->buttonGroup_mode_next_step->setExclusive(true);

  ui_->pushButton_next_step->setEnabled(false);
  ui_->horizontalSlider_timer_speed->setEnabled(false);

  ui_->commandLinkButton_load->setChecked(false);
  ui_->commandLinkButton_load->setIcon(QIcon(":/icons/load.png"));
  ui_->commandLinkButton_load->setEnabled(false);

  ClearStringAndStatus(ui_->statusbar, &filepath_);
}

void View::on_pushButton_clear_clicked() {
  if (timer_ != nullptr) {
    timer_->stop();
  }
  ClearRenderScreen();
  ResetUiElementsState();
}

void View::ChangeEnabledButtonNextSection(bool state) {
  ui_->pushButton_next_step->setEnabled(state);
  ui_->horizontalSlider_timer_speed->setEnabled(!state);
}

void View::ChangeEnabledButtonDataSourceSection(bool state) {
  ui_->spinBox_chance->setEnabled(state);
  ui_->spinBox_rows_amount->setEnabled(state);
  ui_->spinBox_cols_amount->setEnabled(state);
  ui_->commandLinkButton_load->setEnabled(!state);
}

void View::on_radioButton_random_fill_clicked() {
  ChangeEnabledButtonDataSourceSection(true);
}

void View::on_radioButton_from_file_clicked() {
  ChangeEnabledButtonDataSourceSection(false);
}

void View::on_radioButton_auto_clicked() {
  ui_->horizontalSlider_timer_speed->setEnabled(true);
  ui_->pushButton_next_step->setEnabled(false);
}

void View::on_radioButton_manual_clicked() {
  ui_->horizontalSlider_timer_speed->setEnabled(false);
}

void View::on_commandLinkButton_load_clicked() {
  OpenFileWithChangeButtonIconAndStatus(ui_->commandLinkButton_load,
                                        ui_->statusbar, &filepath_);
}

void View::on_horizontalSlider_timer_speed_sliderReleased() {
  int speed = timer_duration_ / ui_->horizontalSlider_timer_speed->value();
  ui_->label_speed_test->setText(QString::number(speed));
}
}  // namespace sfleta
