#ifndef VIEW_H
#define VIEW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QTimer>

#include "QtCore/qpoint.h"
#include "QtGui/qevent.h"
#include "controller/controller.h"
#include "renderer.h"
#include "ui_view.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

namespace sfleta {
class View : public QMainWindow {
  Q_OBJECT

 public:
  explicit View(sfleta::Controller *, QWidget *parent = nullptr);
  ~View() {
    delete render_;
    delete ui_;
  };

 private:
  Ui::View *ui_;
  Controller *ctrl_;
  Renderer *render_;
  QTimer *timer_ = nullptr;

  const size_t window_size_px_ = 500;
  int timer_duration_ = 5000;

  bool next_status_;

  bool first_click = true;
  bool draw_state = false;

  float h_size_;
  float w_size_;

  QString filepath_;

  QRect GetRectForPointImage(QPoint);
  QRect GetRectForCell(QPoint);

  void ChangeCheckedButtonDrawMazeAndClickState(bool);
  void ChangeEnabledButtonDataSourceSectionMaze(bool);
  void ChangeEnabledButtonDataSourceSection(bool);
  void ChangeEnabledButtonNextSection(bool);

  void OpenFileWithChangeButtonIconAndStatus(QCommandLinkButton *, QStatusBar *,
                                             QString *);
  void SaveFileWithChangeButtonIconAndStatus(QCommandLinkButton *,
                                             QStatusBar *);
  void SaveFile(QString);

  void ClearStringAndStatus(QStatusBar *, QString *);
  void ClearMousePosHigthligth();
  void ClearRenderScreen();

  void ResetButtonIconAndEnabled();
  void ResetUiElementsStateMaze();
  void ResetUiElementsState();

  void DrawDirections(const QPoint, const QPoint);
  void DrawCave();
  void DrawMaze();

  void NextStepStatusbar();
  void InitUi();

 private slots:
  void paintEvent(QPaintEvent *) override;
  bool eventFilter(QObject *, QEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;

  void StopDrawMessage() { ui_->statusbar->showMessage("Stoped, it's finish"); }
  void StartDrawMessage() { ui_->statusbar->showMessage("Started"); }
  void NextStep();

  void on_spinBox_rows_amount_maze_valueChanged(int arg1) {
    h_size_ = static_cast<float>(window_size_px_) / arg1;
  }
  void on_spinBox_cols_amount_maze_valueChanged(int arg1) {
    w_size_ = static_cast<float>(window_size_px_) / arg1;
  }

  void on_horizontalSlider_timer_speed_sliderReleased();

  void on_pushButton_clear_maze_clicked();
  void on_pushButton_next_step_clicked();
  void on_pushButton_draw_maze_clicked();
  void on_pushButton_clear_clicked();
  void on_pushButton_draw_clicked();

  void on_radioButton_random_fill_maze_clicked();
  void on_radioButton_from_file_maze_clicked();
  void on_radioButton_random_fill_clicked();
  void on_radioButton_from_file_clicked();
  void on_radioButton_manual_clicked();
  void on_radioButton_auto_clicked();

  void on_commandLinkButton_load_maze_clicked();
  void on_commandLinkButton_save_maze_clicked();
  void on_commandLinkButton_load_clicked();

  void on_tabWidget_currentChanged(int);

 signals:
  void IsTimerAlive();
  void IsTimerDead();
};
}  // namespace sfleta
#endif  // VIEW_H
