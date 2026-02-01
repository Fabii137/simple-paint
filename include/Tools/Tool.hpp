#pragma once

#include "raylib.h"

class Canvas;

enum class ToolType {
  BRUSH,
  ERASER,
  RECTANGLE,
  LINE,
  CIRCLE,
  BUCKETFILL,
};

class Tool {
public:
  virtual ~Tool() = default;

  virtual bool wantsHiddenCursor() const;

  virtual void onPress(Canvas &canvas, Vector2 pos) {}
  virtual void onDrag(Canvas &canvas, Vector2 pos) {}
  virtual void onRelease(Canvas &canvas, Vector2 pos) {}
  virtual void onScroll(float delta) {}

  virtual void cancel() {};

  virtual void onMouseEnterWhileDragging(Canvas &canvas, Vector2 pos) {};

  virtual void drawPreview(Vector2 pos) const {}
};
