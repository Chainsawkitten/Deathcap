#pragma once

#include <imgui.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

#include <cmath>
#include <vector>
#include <Video/ParticleSystemRenderer.hpp>

struct MyCurve
{
    std::string curve_name;
    float maxTime = 1.0f;
    ImVec2 value[10];
    ImGuiID id;
    int item = 0;
    float value_you_care_about = 0.0f;

    // Variables for controlling particle.
    bool editVelocityX = false;
    bool editVelocityY = false;
    bool editVelocityZ = false;
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

    void AddMyCurve(std::string& curve_name, ImGuiID uniqueId, int item);
    void UpdateCurves(float deltaTime, float totalTime);
    void RenderCurveEditor();

    const std::vector<MyCurve>& GetAllCurves() const;

private:
    bool visible = false;
    ImGuiID addedCurve = 0;
    std::string curvename = "Default";
    char curveBuf[10];
    std::string editor_name;
    std::vector<MyCurve> curves;
    float time = 0.0f;
    Video::ParticleSystemRenderer::EmitterSettings emitterSettings;
    bool goBack = false;
    bool play = false;

};