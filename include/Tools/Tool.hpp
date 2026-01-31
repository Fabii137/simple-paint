#pragma once

#include "raylib.h"

class Canvas;

enum class ToolType {
  BRUSH,
  ERASER,
  RECTANGLE,
};

class Tool {
public:
  virtual ~Tool() = default;

  virtual bool wantsHiddenCursor() const;

  virtual void onPress(Canvas &canvas, Vector2 pos) = 0;
  virtual void onDrag(Canvas &canvas, Vector2 pos) = 0;
  virtual void onRelease(Canvas &canvas, Vector2 pos) = 0;
  virtual void onScroll(float delta) = 0;

  virtual void drawPreview(Vector2 pos) const = 0;
};
