#pragma once

#include "raylib.h"
#include <vector>

class Paint {
public:
  Paint();
  ~Paint();
  void run();

private:
  void initColorRecs();
  void initTarget();

  void handleInput();
  void handleColorInput(const Vector2 &mousePos);
  void handleBrushInput();
  void handleClear();
  void handleSave();
  void handleDrawing(const Vector2 &mousePos);

  void draw();
  void drawTarget();
  void drawCirclePreview();
  void drawColorTab();

private:
  const std::vector<Color> c_Colors = {
      RAYWHITE, YELLOW,    GOLD,      ORANGE, PINK,     RED,        MAROON,
      GREEN,    LIME,      DARKGREEN, PURPLE, VIOLET,   DARKPURPLE, BEIGE,
      BROWN,    DARKBROWN, LIGHTGRAY, GRAY,   DARKGRAY, BLACK};

  std::vector<Rectangle> m_ColorRecs;
  RenderTexture2D m_Target;

  Vector2 m_PrevMousePos;
  bool m_IsPainting = false;
  int m_ColorMouseHover = 0;
  int m_SelectedColor = c_Colors.size() - 1;
  float m_BrushSize = 10.f;
};
