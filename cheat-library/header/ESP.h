#pragma once
#include "../header/Config.h"
#include "../header/Memory.h"

#include "../imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui_internal.h"


bool WorldToScreen(const Vector3& in, Vector3& out);

void DrawBox(Matrix3x3 rotation, Vector3 bbmin, Vector3 bbmax, Vector3 position, bool Invulnerable, Vector3& bbcenter);
void DrawBox(Matrix3x3 rotation, Vector3 bbmin, Vector3 bbmax, Vector3 position);

void DrawOffscreenIndicator(Vector3 origin, float distance);

void ESP();
void debug();