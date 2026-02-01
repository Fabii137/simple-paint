#pragma once

#include "Tools/Tool.hpp"
#include "raylib.h"

class RectangleTool : public Tool {
public:
  RectangleTool(Color &color);

  bool wantsHiddenCursor() const override;

  void onPress(Canvas &canvas, Vector2 pos) override;
  void onDrag(Canvas &canvas, Vector2 pos) override;
  void onRelease(Canvas &canvas, Vector2 pos) override;
  void onScroll(float delta) override {}

  void cancel() override;

  void drawPreview(Vector2 pos) const override;

private:
  Rectangle makeRect(Vector2 a, Vector2 b) const;

private:
  Vector2 m_Start{};
  Vector2 m_End{};
  Color &m_Color;
  float m_LineThickness = 1.f;
  bool m_Dragging = false;
};
