#include "Paint.hpp"
#include "raylib.h"
#include "raymath.h"
#include <cstddef>
#include <cstdint>
#include <format>
#include <vector>

constexpr int FPS = 240;

constexpr int SCREEN_WIDTH = 1200;
constexpr int SCREEN_HEIGHT = 900;

constexpr int BRUSH_MIN = 2;
constexpr int BRUSH_MAX = 50;
constexpr int BRUSH_SCROLL_STEP = 5;
constexpr float BRUSH_SPACING_FACTOR = 0.5f;

constexpr int COLOR_TAB_HEIGHT = 50;

void Paint::initColorRecs() {
  constexpr float margin = 10.f;
  constexpr float gap = 5.f;
  constexpr float colorRectSize = 30.f;

  for (size_t i = 0; i < c_Colors.size(); i++) {
    m_ColorRecs[i].x = margin + (colorRectSize + gap) * i;
    m_ColorRecs[i].y = (COLOR_TAB_HEIGHT - colorRectSize) / 2;
    m_ColorRecs[i].width = colorRectSize;
    m_ColorRecs[i].height = colorRectSize;
  }
}

void Paint::initTarget() {
  m_Target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
  BeginTextureMode(m_Target);
  ClearBackground(c_Colors[0]);
  EndTextureMode();
}

Paint::Paint() : m_ColorRecs(c_Colors.size()) { initColorRecs(); }

Paint::~Paint() { UnloadRenderTexture(m_Target); }

void Paint::run() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Paint");
  SetTargetFPS(FPS);

  initTarget();

  while (!WindowShouldClose()) {
    handleInput();

    BeginDrawing();
    ClearBackground(RAYWHITE);
    draw();
    EndDrawing();
  }
  CloseWindow();
}

void Paint::handleInput() {
  const Vector2 mousePos = GetMousePosition();
  handleCursor(mousePos);
  handleColorInput(mousePos);
  handleClear();
  handleSave();
  handleBrushInput();
  handleDrawing(mousePos);
}

void Paint::handleCursor(const Vector2 &mousePos) {
  if (isMouseOnCanvas(mousePos)) {
    HideCursor();
    return;
  }

  ShowCursor();
}

void Paint::handleColorInput(const Vector2 &mousePos) {
  // arrow keys color switching
  if (IsKeyPressed(KEY_RIGHT)) {
    m_SelectedColor = (m_SelectedColor + 1) % c_Colors.size();
  } else if (IsKeyPressed(KEY_LEFT)) {
    m_SelectedColor--;
    if (m_SelectedColor < 0) {
      m_SelectedColor = c_Colors.size() - 1;
    }
  }

  // color hover
  m_ColorMouseHover = -1;
  for (size_t i = 0; i < m_ColorRecs.size(); i++) {
    if (CheckCollisionPointRec(mousePos, m_ColorRecs[i])) {
      m_ColorMouseHover = i;
      break;
    }
  }

  // color selection
  if (m_ColorMouseHover >= 0 && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    m_SelectedColor = m_ColorMouseHover;
  }
}
void Paint::handleBrushInput() {
  m_BrushSize += GetMouseWheelMove() * BRUSH_SCROLL_STEP;
  if (m_BrushSize < BRUSH_MIN) {
    m_BrushSize = BRUSH_MIN;
  }
  if (m_BrushSize > BRUSH_MAX) {
    m_BrushSize = BRUSH_MAX;
  }
}

void Paint::handleClear() {
  if (IsKeyPressed(KEY_C)) {
    BeginTextureMode(m_Target);
    ClearBackground(c_Colors[0]);
    EndTextureMode();
  }
}

void Paint::handleSave() {
  if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
    Image image = LoadImageFromTexture(m_Target.texture);
    ImageFlipVertical(&image);
    ImageCrop(&image,
              (Rectangle){0, COLOR_TAB_HEIGHT, static_cast<float>(SCREEN_WIDTH),
                          static_cast<float>(SCREEN_HEIGHT)});
    const uint64_t time_ms = static_cast<uint64_t>(GetTime() * 1000);
    const std::string fileName = std::format("painting_{}.png", time_ms);
    ExportImage(image, fileName.c_str());
    UnloadImage(image);
  }
}

void Paint::handleDrawing(const Vector2 &mousePos) {
  if (!isMouseOnCanvas(mousePos)) {
    m_IsPainting = false;
    return;
  }

  bool downLeft = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
  bool downRight = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);

  bool mouseDown = downLeft || downRight;

  if (mouseDown && !m_IsPainting) {
    m_PrevMousePos = mousePos;
    m_IsPainting = true;
  }

  if (mouseDown) {
    BeginTextureMode(m_Target);
    Color drawColor = c_Colors[m_SelectedColor];

    if (downRight) {
      drawColor = c_Colors[0];
    }

    // draw circles between prev mouse pos and mouse pos
    float distance = Vector2Distance(m_PrevMousePos, mousePos);
    float spacing = m_BrushSize * BRUSH_SPACING_FACTOR;
    int steps = static_cast<int>(distance / (spacing)) + 1;

    for (size_t i = 0; i <= steps; i++) {
      float progress = i / static_cast<float>(steps);
      Vector2 point = {
          m_PrevMousePos.x + (mousePos.x - m_PrevMousePos.x) * progress,
          m_PrevMousePos.y + (mousePos.y - m_PrevMousePos.y) * progress};
      DrawCircleV(point, m_BrushSize, drawColor);
    }
    EndTextureMode();

    m_PrevMousePos = mousePos;
  }

  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) ||
      IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
    m_IsPainting = false;
  }
}

void Paint::draw() {
  drawTarget();
  drawCirclePreview();
  drawColorTab();
}

void Paint::drawTarget() {
  DrawTextureRec(m_Target.texture,
                 (Rectangle){0, 0, static_cast<float>(m_Target.texture.width),
                             static_cast<float>(-m_Target.texture.height)},
                 (Vector2){0, 0}, WHITE);
}

void Paint::drawCirclePreview() {
  const Vector2 mousePos = GetMousePosition();

  if (IsCursorOnScreen() && mousePos.y > COLOR_TAB_HEIGHT) {

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) { // erasing
      DrawCircle(mousePos.x, mousePos.y, m_BrushSize, c_Colors[0]);
      DrawCircleLines(mousePos.x, mousePos.y, m_BrushSize, BLACK);
    } else {
      DrawCircle(mousePos.x, mousePos.y, m_BrushSize,
                 c_Colors[m_SelectedColor]);
    }
  }
}

void Paint::drawColorTab() {
  DrawRectangle(0, 0, SCREEN_WIDTH, COLOR_TAB_HEIGHT, RAYWHITE);
  for (size_t i = 0; i < m_ColorRecs.size(); i++) {
    const Rectangle &rec = m_ColorRecs[i];
    DrawRectangleRec(rec, c_Colors[i]);

    if (i == m_SelectedColor) {
      // selected color border
      DrawRectangleLinesEx(
          (Rectangle){rec.x - 2, rec.y - 2, rec.width + 4, rec.height + 4}, 2.f,
          BLACK);
      continue;
    }

    // color border
    DrawRectangleLinesEx(rec, 1.f, BLACK);
  }

  // hovered color fade
  if (m_ColorMouseHover >= 0) {
    const Rectangle hoveredColorRec = m_ColorRecs[m_ColorMouseHover];
    DrawRectangleRec(hoveredColorRec, Fade(RAYWHITE, 0.6f));
  }
  DrawLine(0, COLOR_TAB_HEIGHT, SCREEN_WIDTH, COLOR_TAB_HEIGHT, BLACK);
}

bool Paint::isMouseOnCanvas(const Vector2 &mousePos) {
  return mousePos.x > 0 && mousePos.x < SCREEN_WIDTH &&
         mousePos.y > COLOR_TAB_HEIGHT && mousePos.y < SCREEN_HEIGHT;
}
