#include "Paint.hpp"
#include "Tools/BrushTool.hpp"
#include "Tools/BucketFillTool.hpp"
#include "Tools/CircleTool.hpp"
#include "Tools/EraserTool.hpp"
#include "Tools/LineTool.hpp"
#include "Tools/RectangleTool.hpp"
#include "Tools/Tool.hpp"
#include "raylib.h"
#include <cstdint>
#include <format>
#include <memory>

constexpr int FPS = 240;

constexpr int SCREEN_WIDTH = 1200;
constexpr int SCREEN_HEIGHT = 900;

constexpr int TOOLBAR_HEIGHT = 50;

Paint::Paint()
    : m_Toolbar(SCREEN_WIDTH, TOOLBAR_HEIGHT),
      m_Canvas(SCREEN_WIDTH, SCREEN_HEIGHT, TOOLBAR_HEIGHT) {
  m_DrawColor = BLACK;
  m_ActiveTool = std::make_unique<Brush>(m_BrushSize, m_DrawColor);
}

void Paint::run() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Paint");
  SetTargetFPS(FPS);

  m_Canvas.init();

  while (!WindowShouldClose()) {
    handleInput();

    BeginDrawing();
    ClearBackground(WHITE);
    draw();
    EndDrawing();
  }
  CloseWindow();
}

void Paint::handleInput() {
  const Vector2 mousePos = GetMousePosition();
  m_DrawColor = m_Toolbar.getSelectedColor();

  m_Toolbar.update(mousePos);
  if (m_Toolbar.toolChanged()) {
    selectTool(m_Toolbar.getSelectedTool());
    m_Toolbar.clearToolChanged();
  }

  handleCursor(mousePos);
  handleClear();
  handleSave();
  handleToolInput(mousePos);
}

void Paint::handleCursor(const Vector2 &mousePos) {
  if (m_Canvas.contains(mousePos) && m_ActiveTool->wantsHiddenCursor()) {
    HideCursor();
    return;
  }

  ShowCursor();
}

void Paint::handleClear() {
  if (IsKeyPressed(KEY_C)) {
    m_Canvas.clear(WHITE);
  }
}

void Paint::handleSave() {
  if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
    Image image = LoadImageFromTexture(m_Canvas.getTexture());
    ImageFlipVertical(&image);
    ImageCrop(&image,
              (Rectangle){0, TOOLBAR_HEIGHT, static_cast<float>(SCREEN_WIDTH),
                          static_cast<float>(SCREEN_HEIGHT)});
    const uint64_t time_ms = static_cast<uint64_t>(GetTime() * 1000);
    const std::string fileName = std::format("painting_{}.png", time_ms);
    ExportImage(image, fileName.c_str());
    UnloadImage(image);
  }
}

void Paint::handleToolInput(const Vector2 &mousePos) {
  if (!m_Canvas.contains(mousePos)) {
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
      m_ActiveTool->cancel();

    return;
  }

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    m_ActiveTool->onPress(m_Canvas, mousePos);
  }

  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    m_ActiveTool->onDrag(m_Canvas, mousePos);
  }

  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    m_ActiveTool->onRelease(m_Canvas, mousePos);
  }

  if (GetMouseWheelMove() != 0) {
    m_ActiveTool->onScroll(GetMouseWheelMove());
  }
}

void Paint::selectTool(ToolType type) {
  if (m_ActiveToolType == type)
    return;

  if (m_ActiveTool)
    m_ActiveTool->cancel();

  m_ActiveToolType = type;

  switch (type) {
  case ToolType::BRUSH:
    m_ActiveTool = std::make_unique<Brush>(m_BrushSize, m_DrawColor);
    break;
  case ToolType::ERASER:
    m_ActiveTool = std::make_unique<Eraser>(m_BrushSize);
    break;
  case ToolType::RECTANGLE:
    m_ActiveTool = std::make_unique<RectangleTool>(m_DrawColor);
    break;
  case ToolType::LINE:
    m_ActiveTool = std::make_unique<LineTool>(m_DrawColor);
    break;
  case ToolType::BUCKETFILL:
    m_ActiveTool = std::make_unique<BucketFillTool>(m_DrawColor);
    break;
  case ToolType::CIRCLE:
    m_ActiveTool = std::make_unique<CircleTool>(m_DrawColor);
    break;
  }
}

void Paint::draw() {
  m_Canvas.drawToScreen();

  Vector2 mousePos = GetMousePosition();
  if (m_Canvas.contains(mousePos)) {
    m_ActiveTool->drawPreview(mousePos);
  }

  m_Toolbar.draw();
}
