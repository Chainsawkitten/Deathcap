#pragma once

#include <imgui.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

#include <cmath>
#include <vector>

struct MyCurve
{
    std::string curve_name;
    float maxTime = 1.0f;
    ImVec2 value[10];
    ImGuiID id;
    int item = 0;
};

class CurveEditor
{
public:
    CurveEditor();
    ~CurveEditor();

    /// Show the editor.
    void Show();

    bool IsVisible() const;

    void SetVisible(bool visible);

    void AddMyCurve(std::string curve_name, ImGuiID uniqueId, int item);
    void UpdateCurves(float deltaTime);
    void RenderCurveEditor();

private:
    bool visible = false;
    ImGuiID addedCurve = 0;
    char curvename[10] = "Default";
    std::string editor_name;
    std::vector<MyCurve> curves;
    float time = 0.0f;

};