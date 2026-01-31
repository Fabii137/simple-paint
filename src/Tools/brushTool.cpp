#include "Tools/BrushTool.hpp"
#include "Canvas.hpp"
#include "raylib.h"
#include "raymath.h"

constexpr int MIN_SIZE = 2;
constexpr int MAX_SIZE = 50;
constexpr int SCROLL_STEP = 5;
constexpr float SPACING_FACTOR = 0.5f;

Brush::Brush(float &brushSize, Color &color)
    : m_BrushSize(brushSize), m_Color(color) {}

void Brush::onPress(Canvas &canvas, Vector2 pos) {
  m_PrevPos = pos;
  m_IsDrawing = true;
}

void Brush::onDrag(Canvas &canvas, Vector2 pos) {
  if (!m_IsDrawing)
    return;

  canvas.begin();
  // draw circles between prev mouse pos and mouse pos
  float distance = Vector2Distance(m_PrevPos, pos);
  float spacing = m_BrushSize * SPACING_FACTOR;
  int steps = static_cast<int>(distance / (spacing)) + 1;

  for (size_t i = 0; i <= steps; i++) {
    float progress = i / static_cast<float>(steps);
    Vector2 point = {m_PrevPos.x + (pos.x - m_PrevPos.x) * progress,
                     m_PrevPos.y + (pos.y - m_PrevPos.y) * progress};
    canvas.drawCircle(point, m_BrushSize, m_Color);
  }
  canvas.end();

  m_PrevPos = pos;
}

void Brush::onRelease(Canvas &canvas, Vector2 pos) { m_IsDrawing = false; }

void Brush::onScroll(float delta) {
  m_BrushSize += delta * SCROLL_STEP;

  if (m_BrushSize > MAX_SIZE) {
    m_BrushSize = MAX_SIZE;
  }
  if (m_BrushSize < MIN_SIZE) {
    m_BrushSize = MIN_SIZE;
  }
}

void Brush::drawPreview(Vector2 pos) const {
  DrawCircleV(pos, m_BrushSize, m_Color);
}

Color Brush::getColor() const { return m_Color; }

float Brush::getBrushSize() const { return m_BrushSize; }
