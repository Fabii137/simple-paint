#include "Tools/RectangleTool.hpp"
#include "Canvas.hpp"
#include "raylib.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

RectangleTool::RectangleTool(Color &color) : m_Color(color) {}

bool RectangleTool::wantsHiddenCursor() const { return false; }

void RectangleTool::onPress(Canvas &canvas, Vector2 pos) {
  m_Start = pos;
  m_End = pos;
  m_Dragging = true;
}

void RectangleTool::onDrag(Canvas &canvas, Vector2 pos) {
  if (!m_Dragging)
    return;

  m_End = pos;
}

void RectangleTool::onRelease(Canvas &canvas, Vector2 pos) {
  if (!m_Dragging)
    return;

  m_End = pos;
  m_Dragging = false;

  canvas.begin();
  DrawRectangleRec(makeRect(m_Start, m_End), m_Color);
  canvas.end();
}

void RectangleTool::drawPreview(Vector2 pos) const {
  if (!m_Dragging)
    return;

  DrawRectangleLinesEx(makeRect(m_Start, m_End), m_LineThickness, m_Color);
}

void RectangleTool::cancel() { m_Dragging = false; }

Rectangle RectangleTool::makeRect(Vector2 start, Vector2 end) const {
  float x = std::min(start.x, end.x);
  float y = std::min(start.y, end.y);
  float width = std::abs(end.x - start.x);
  float height = std::abs(end.y - start.y);

  return {x, y, width, height};
}
