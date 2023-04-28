#ifndef RENDERER_H
#define RENDERER_H

#include <QDir>
#include <QImage>
#include <QPainter>
#include <QPainterPath>

namespace sfleta {
class Renderer {
 public:
  explicit Renderer();
  ~Renderer(){};

  void DrawMousePos(QRect, bool isClick = false);
  void DrawLine(float, float, float, float);
  void DrawDirections(QPolygon);
  void DrawDot(QRect);
  void DrawRectangle(QRect);

  void ClearMouse() { mouse_img_.fill(0); }
  void ClearModel() { maze_img_.fill(0); }
  void ClearRout() { rout_img_.fill(0); }

  const QImage &GetMousePosImg() { return mouse_img_; }
  const QImage &GetMazeImage() { return maze_img_; }
  const QImage &GetRoutImage() { return rout_img_; }

 private:
  QImage maze_img_;
  QImage rout_img_;
  QImage mouse_img_;
};
}  // namespace sfleta
#endif  // RENDERER_H
