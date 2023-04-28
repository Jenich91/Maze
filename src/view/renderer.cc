#include "renderer.h"

namespace sfleta {
Renderer::Renderer()
    : maze_img_{QSize(500, 500), QImage::Format_ARGB32},
      rout_img_{QSize(500, 500), QImage::Format_ARGB32},
      mouse_img_{QSize(500, 500), QImage::Format_ARGB32} {
  maze_img_.fill(0);
  rout_img_.fill(0);
  mouse_img_.fill(0);
}

void Renderer::DrawRectangle(QRect rect) {
  QPainter painter(&maze_img_);
  painter.fillRect(rect, QBrush(Qt::black, Qt::SolidPattern));
  painter.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::FlatCap));
  painter.drawRect(rect);
}

void Renderer::DrawLine(float x1, float y1, float x2, float y2) {
  QPainter painter(&maze_img_);
  painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));
  painter.drawLine(x1, y1, x2, y2);
}

void Renderer::DrawDot(QRect rect) {
  QPainter painter(&rout_img_);
  QImage point_image{":/icons/point.png"};

  painter.drawImage(
      rect, point_image.scaled(rect.width(), rect.height(), Qt::KeepAspectRatio,
                               Qt::SmoothTransformation));
}

void Renderer::DrawDirections(QPolygon polygon) {
  QPainter painter(&rout_img_);
  QPainterPath path;
  path.addPolygon(polygon);

  painter.setPen(QPen(Qt::green, 2, Qt::SolidLine, Qt::FlatCap));
  painter.drawPath(path);
}

void Renderer::DrawMousePos(QRect rect, bool isClick) {
  mouse_img_.fill(0);
  QPainter painter(isClick ? &rout_img_ : &mouse_img_);
  painter.fillRect(rect, QBrush(QColor::fromRgb(203, 192, 241)));
}
}  // namespace sfleta
