#pragma once

#include "Tools/Tool.hpp"
#include "raylib.h"
#include <cstdint>
#include <vector>
class Toolbar {
public:
  Toolbar(float width, float height);

  void update(const Vector2 &mousePos);
  void draw() const;

  ToolType getSelectedTool() const;
  bool toolChanged() const;
  void clearToolChanged();

  Color getSelectedColor() const;

private:
  void initColorRecs();
  void handleKeyboardInput();
  void handleMouseInput(const Vector2 &mousePos);

private:
  const std::vector<Color> c_Colors = {
      RAYWHITE, YELLOW,    GOLD,      ORANGE, PINK,     RED,        MAROON,
      GREEN,    LIME,      DARKGREEN, PURPLE, VIOLET,   DARKPURPLE, BEIGE,
      BROWN,    DARKBROWN, LIGHTGRAY, GRAY,   DARKGRAY, BLACK};
  std::vector<Rectangle> m_ColorRecs;

  ToolType m_SelectedTool = ToolType::BRUSH;
  bool m_ToolChanged = false;

  float m_Width;
  float m_Height;

  int8_t m_SelectedColor = c_Colors.size() - 1;
  int8_t m_HoveredColor = 0;
};
