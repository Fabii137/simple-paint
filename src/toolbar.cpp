#include "Toolbar.hpp"
#include "Tools/Tool.hpp"
#include "raylib.h"

constexpr float COLOR_MARGIN = 10.f;
constexpr float COLOR_GAP = 5.f;
constexpr float COLOR_SIZE = 30.f;

Toolbar::Toolbar(float width, float height)
    : m_Width(width), m_Height(height), m_ColorRecs(c_Colors.size()) {
  initColorRecs();
}

void Toolbar::initColorRecs() {
  for (size_t i = 0; i < c_Colors.size(); i++) {
    m_ColorRecs[i].x = COLOR_MARGIN + (COLOR_SIZE + COLOR_GAP) * i;
    m_ColorRecs[i].y = (m_Height - COLOR_SIZE) / 2;
    m_ColorRecs[i].width = COLOR_SIZE;
    m_ColorRecs[i].height = COLOR_SIZE;
  }
}

void Toolbar::update(const Vector2 &mousePos) {
  handleKeyboardInput();
  handleMouseInput(mousePos);
}

void Toolbar::handleKeyboardInput() {
  // tool switching
  if (IsKeyPressed(KEY_B)) {
    m_SelectedTool = ToolType::BRUSH;
    m_ToolChanged = true;
  }
  if (IsKeyPressed(KEY_E)) {
    m_SelectedTool = ToolType::ERASER;
    m_ToolChanged = true;
  }
  if (IsKeyPressed(KEY_R)) {
    m_SelectedTool = ToolType::RECTANGLE;
    m_ToolChanged = true;
  }

  // arrow key color switching
  if (IsKeyPressed(KEY_RIGHT)) {
    m_SelectedColor = (m_SelectedColor + 1) % c_Colors.size();
  } else if (IsKeyPressed(KEY_LEFT)) {
    m_SelectedColor--;
    if (m_SelectedColor < 0) {
      m_SelectedColor = c_Colors.size() - 1;
    }
  }
}

void Toolbar::handleMouseInput(const Vector2 &mousePos) {
  m_HoveredColor = -1;
  for (size_t i = 0; i < m_ColorRecs.size(); i++) {
    if (CheckCollisionPointRec(mousePos, m_ColorRecs[i])) {
      m_HoveredColor = i;
      break;
    }
  }

  if (m_HoveredColor >= 0 && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    m_SelectedColor = m_HoveredColor;
  }
}

void Toolbar::draw() const {
  DrawRectangle(0, 0, m_Width, m_Height, RAYWHITE);
  for (size_t i = 0; i < m_ColorRecs.size(); i++) {
    const Rectangle &rec = m_ColorRecs[i];
    DrawRectangleRec(rec, c_Colors[i]);

    if (i == m_SelectedColor) {
      // selected border
      DrawRectangleLinesEx(
          (Rectangle){rec.x - 2, rec.y - 2, rec.width + 4, rec.height + 4}, 2.f,
          BLACK);
      continue;
    }

    // border
    DrawRectangleLinesEx(rec, 1.f, BLACK);
  }

  // hovered
  if (m_HoveredColor >= 0) {
    const Rectangle hoveredColorRec = m_ColorRecs[m_HoveredColor];
    DrawRectangleRec(hoveredColorRec, Fade(RAYWHITE, 0.6f));
  }

  DrawLine(0, m_Height, m_Width, m_Height, BLACK);
}

ToolType Toolbar::getSelectedTool() const { return m_SelectedTool; }

bool Toolbar::toolChanged() const { return m_ToolChanged; }

void Toolbar::clearToolChanged() { m_ToolChanged = false; }

Color Toolbar::getSelectedColor() const { return c_Colors[m_SelectedColor]; }
