#pragma once
#include "Tools/Tool.hpp"

class CircleTool : public Tool {
public:
  CircleTool(Color &color);

  bool wantsHiddenCursor() const override;

  void onPress(Canvas &canvas, Vector2 pos) override;
  void onDrag(Canvas &canvas, Vector2 pos) override;
  void onRelease(Canvas &canvas, Vector2 pos) override;
  void onScroll(float delta) override {}

  void cancel() override;

  void drawPreview(Vector2 pos) const override;

private:
  Vector2 m_Start{};
  Vector2 m_End{};

  Color &m_Color;

  bool m_Dragging = false;
};
