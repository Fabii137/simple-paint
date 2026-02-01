#pragma once

#include "Tool.hpp"
#include "raylib.h"

class Brush : public Tool {
public:
  Brush(float &brushSize, Color &color);

  void onPress(Canvas &canvas, Vector2 pos) override;
  void onDrag(Canvas &canvas, Vector2 pos) override;
  void onRelease(Canvas &canvas, Vector2 pos) override;
  void onScroll(float delta) override;

  void cancel() override;

  void drawPreview(Vector2 pos) const override;

  Color getColor() const;
  float getBrushSize() const;

private:
  Vector2 m_PrevPos{};
  Color &m_Color;
  float &m_BrushSize;
  bool m_IsDrawing = false;
};
