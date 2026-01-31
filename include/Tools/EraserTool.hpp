#pragma once

#include "Tools/BrushTool.hpp"
#include "Tools/Tool.hpp"
#include "raylib.h"

class Eraser : public Tool {
public:
  Eraser(float &brushSize);

  void onPress(Canvas &canvas, Vector2 pos) override;
  void onDrag(Canvas &canvas, Vector2 pos) override;
  void onRelease(Canvas &canvas, Vector2 pos) override;
  void onScroll(float delta) override;

  void drawPreview(Vector2 pos) const override;

private:
  Color m_Color = WHITE;
  Brush m_Brush;
};
