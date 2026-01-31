#include "Tools/CircleTool.hpp"
#include "Canvas.hpp"
#include "raylib.h"
#include "raymath.h"

CircleTool::CircleTool(Color &color) : m_Color(color) {}

bool CircleTool::wantsHiddenCursor() const { return false; }

void CircleTool::onPress(Canvas &canvas, Vector2 pos) {
  m_Dragging = true;
  m_Start = pos;
  m_End = pos;
}

void CircleTool::onDrag(Canvas &canvas, Vector2 pos) {
  if (!m_Dragging)
    return;

  m_End = pos;
}

void CircleTool::onRelease(Canvas &canvas, Vector2 pos) {
  if (!m_Dragging)
    return;

  m_End = pos;
  m_Dragging = false;

  canvas.begin();
  DrawCircleV(m_Start, Vector2Distance(m_Start, m_End), m_Color);
  canvas.end();
}

void CircleTool::drawPreview(Vector2 pos) const {
  if (!m_Dragging)
    return;

  DrawCircleLinesV(m_Start, Vector2Distance(m_Start, m_End), m_Color);
}
