#pragma once

#include "raylib.h"

class Canvas {
public:
  Canvas(int width, int height, int yOffset);
  ~Canvas();

  void init();
  void clear(Color color);

  void begin();
  void end();

  void drawCircle(Vector2 pos, float radius, Color color);
  void drawToScreen() const;

  bool contains(Vector2 point) const;

  Texture2D getTexture() const;

private:
  RenderTexture2D m_Target;
  int m_Width;
  int m_Height;
  int m_YOffset;
  bool m_IsInitialized = false;
};
