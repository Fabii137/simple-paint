#include "Tools/BucketFillTool.hpp"
#include "Canvas.hpp"
#include "raylib.h"
#include <array>
#include <queue>
#include <utility>

BucketFillTool::BucketFillTool(Color &color) : m_Color(color) {}

bool BucketFillTool::wantsHiddenCursor() const { return false; }

void BucketFillTool::onPress(Canvas &canvas, Vector2 pos) {
  floodFill(canvas, pos);
}

bool BucketFillTool::colorEquals(Color a, Color b) const {
  return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

int BucketFillTool::cvt2DIndex(Vector2 pos, int width) const {
  return pos.y * width + pos.x;
}

void BucketFillTool::floodFill(Canvas &canvas, Vector2 pos) {
  Image img = canvas.readImage();
  Color *pixels = (Color *)img.data;

  Color targetColor = pixels[cvt2DIndex(pos, img.width)];

  if (colorEquals(targetColor, m_Color)) {
    UnloadImage(img);
    return;
  }

  std::array<std::pair<int, int>, 4> dirs = {{
      {1, 0},
      {-1, 0},
      {0, 1},
      {0, -1},
  }};

  std::queue<std::pair<int, int>> queue;
  queue.push({pos.x, pos.y});
  pixels[cvt2DIndex(pos, img.width)] = m_Color;

  while (!queue.empty()) {
    auto front = queue.front();
    int x = front.first;
    int y = front.second;
    queue.pop();

    for (auto &dir : dirs) {
      int nx = x + dir.first;
      int ny = y + dir.second;
      if (nx < 0 || nx >= img.width || ny < 0 || ny >= img.height)
        continue;

      Color &color = pixels[cvt2DIndex(Vector2(nx, ny), img.width)];
      if (!colorEquals(color, targetColor))
        continue;

      color = m_Color;
      queue.push({nx, ny});
    }
  }

  canvas.writeImage(img);
  UnloadImage(img);
}
