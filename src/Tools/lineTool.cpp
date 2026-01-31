#include "Tools/LineTool.hpp"
#include "Canvas.hpp"
#include "raylib.h"

LineTool::LineTool(Color &color) : m_Color(color) {}

bool LineTool::wantsHiddenCursor() const { return false; }

void LineTool::onPress(Canvas &canvas, Vector2 pos) {
  m_Dragging = true;
  m_Start = pos;
  m_End = pos;
}

void LineTool::onDrag(Canvas &canvas, Vector2 pos) {
  if (!m_Dragging)
    return;

  m_End = pos;
}

void LineTool::onRelease(Canvas &canvas, Vector2 pos) {
  if (!m_Dragging)
    return;

  m_End = pos;
  m_Dragging = false;

  canvas.begin();
  DrawLineEx(m_Start, m_End, m_LineThickness, m_Color);
  canvas.end();
}

void LineTool::drawPreview(Vector2 pos) const {
  if (!m_Dragging)
    return;

  DrawLineEx(m_Start, m_End, m_LineThickness, m_Color);
}
