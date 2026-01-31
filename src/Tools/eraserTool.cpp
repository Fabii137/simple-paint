#include "Tools/EraserTool.hpp"
#include "Canvas.hpp"
#include "raylib.h"

Eraser::Eraser(float &brushSize) : m_Brush(brushSize, m_Color) {}

void Eraser::onPress(Canvas &canvas, Vector2 pos) {
  m_Brush.onPress(canvas, pos);
}

void Eraser::onDrag(Canvas &canvas, Vector2 pos) {
  m_Brush.onDrag(canvas, pos);
}

void Eraser::onRelease(Canvas &canvas, Vector2 pos) {
  m_Brush.onRelease(canvas, pos);
}

void Eraser::onScroll(float delta) { m_Brush.onScroll(delta); }

void Eraser::drawPreview(Vector2 pos) const {
  DrawCircleV(pos, m_Brush.getBrushSize(), m_Brush.getColor());
  DrawCircleLinesV(pos, m_Brush.getBrushSize(), BLACK);
}
