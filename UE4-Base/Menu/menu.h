#pragma once
#include "../Includes.h"

inline void renderIMenu(ImGuiWindow& window)
{
	window.DrawList->PushClipRectFullScreen();
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);

	if (Settings.ShowMenu) {
		IRendering::ColorAndStyle();
		ImGui::SetNextWindowSize({ 709, 470 });
		ImGuiStyle* style = &ImGui::GetStyle();
		static int maintabs = 0;
		static int esptabs = 0;
		bool test;
		if (ImGui::Begin(("GC Pro"), &Settings.ShowMenu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::Tab(0, "Aimbot", &maintabs, 55.f);
			ImGui::Tab(1, "Visuals", &maintabs, 55.f);
			ImGui::Tab(2, "Misc", &maintabs, 55.f);
			ImGui::Tab(3, "Settings", &maintabs, 55.f);
			ImGui::Separator();

			if (maintabs == 0) {
				ImGui::Text("Aimbot");
				ImGui::Checkbox("test", &test);
				ImGui::SameLine();
				ImGui::TextColored(ImColor(255, 231, 94, 255), "[?]");
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text("test");
					ImGui::EndTooltip();
				}
			}
			else if (maintabs == 1) {
				ImGui::Tab(0, "Players", &esptabs, 100);
				ImGui::Tab(1, "......", &esptabs, 100);
				ImGui::Tab(2, "......", &esptabs, 100);
				ImGui::Tab(3, "Others", &esptabs, 100);

				ImGui::Separator();

				if (esptabs == 0) {
					ImGui::Text("Players");
					ImGui::Checkbox("test", &test);
				}
				else if (esptabs == 1) {
					ImGui::Text(".........");
					ImGui::Checkbox("Test", &test);
				}
				else if (esptabs == 2) {
					ImGui::Text(".........");
					ImGui::Checkbox("test", &test);
				}
				else if (esptabs == 3) {
					ImGui::Text("Other Visuals");
					ImGui::Checkbox("RGB", &Settings.Visuals.RGB);
					ImGui::Checkbox("Crosshair", &Settings.Visuals.Crosshair);
					ImGui::SliderFloat("Crosshair Thikkness", &Settings.Visuals.CrosshairThikkness, 0.1f, 5.f);
					ImGui::Checkbox("FOV Circle", &Settings.Visuals.FOVCircle);
					ImGui::SliderFloat("FOV Sides", &Settings.Visuals.FOVCricleSides, 3.f, 120.f);
					ImGui::SliderFloat("FOV Thikkness", &Settings.Visuals.FOVCircleThikkness, 0.1f, 5.f);
					ImGui::SliderFloat("FOV Size", &Settings.Visuals.FOV, 0.f, 500.f);
				}
			}
			else if (maintabs == 2) {
				ImGui::Text("Misc");
				ImGui::Checkbox("Test", &test);
			}
			else if (maintabs == 3) {
				ImGui::Text("Settings");
				ImGui::Checkbox("Test", &test);
				ImGui::ColorEdit3("FOV", (float*)&Settings.Colors.FovCircle, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoOptions);
				ImGui::ColorEdit3("Crosshair", (float*)&Settings.Colors.Crosshair, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoOptions);
			}
		}
	}
}