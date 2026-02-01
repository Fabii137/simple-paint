#pragma once
#include "Tools/Tool.hpp"

class BucketFillTool : public Tool {
public:
  BucketFillTool(Color &m_Color);

  bool wantsHiddenCursor() const override;

  void onPress(Canvas &canvas, Vector2 pos) override;

private:
  bool colorEquals(Color a, Color b) const;
  int cvt2DIndex(Vector2 pos, int width) const;
  void floodFill(Canvas &canvas, Vector2 pos);

private:
  Color &m_Color;
};
