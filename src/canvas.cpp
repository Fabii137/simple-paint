#include "Canvas.hpp"
#include "raylib.h"

Canvas::Canvas(int width, int height, int yOffset)
    : m_Width(width), m_Height(height), m_YOffset(yOffset) {}

Canvas::~Canvas() {
  if (m_IsInitialized)
    UnloadRenderTexture(m_Target);
}

void Canvas::init() {
  if (m_IsInitialized)
    return;

  m_Target = LoadRenderTexture(m_Width, m_Height);

  begin();
  ClearBackground(WHITE);
  end();

  m_IsInitialized = true;
}

void Canvas::clear(Color color) {
  begin();
  ClearBackground(color);
  end();
}

void Canvas::begin() { BeginTextureMode(m_Target); }

void Canvas::end() { EndTextureMode(); }

void Canvas::drawCircle(Vector2 pos, float radius, Color color) {
  DrawCircleV(pos, radius, color);
}

void Canvas::drawToScreen() const {
  DrawTextureRec(m_Target.texture,
                 {0, 0, static_cast<float>(m_Target.texture.width),
                  static_cast<float>(-m_Target.texture.height)},
                 {0, 0}, WHITE);
}

bool Canvas::contains(Vector2 point) const {
  return point.x >= 0 && point.x < m_Width && point.y > m_YOffset &&
         point.y < m_Height;
}

Texture2D Canvas::getTexture() const { return m_Target.texture; }
