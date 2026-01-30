#pragma once

#include "Canvas.hpp"
#include "Toolbar.hpp"
#include "Tools/Tool.hpp"
#include "raylib.h"
#include <memory>

class Paint {
public:
  Paint();
  void run();

private:
  void handleInput();
  void handleCursor(const Vector2 &mousePos);
  void handleToolInput(const Vector2 &mousePos);
  void handleClear();
  void handleSave();

  void selectTool(ToolType type);

  void draw();
  void drawCirclePreview();

private:
  Canvas m_Canvas;
  Toolbar m_Toolbar;

  Color m_DrawColor = BLACK;
  ToolType m_ActiveToolType = ToolType::BRUSH;
  std::unique_ptr<Tool> m_ActiveTool;

  Vector2 m_PrevMousePos;
  float m_BrushSize = 10.f;
  bool m_IsPainting = false;
};
