#include "../../core/src/Application.h"
#include "../../core/src/EntryPoint.h"

#include "../../core/src/Image.h"
#include "../../core/src/Timer.h"

#include "Render.h"
#include "Camera.h"

#include "utils/Utilities.h"
#include "utils/Magnifier.h"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include <string>

using namespace Core;

class RenderLayer : public Core::Layer
{
public:
	RenderLayer() // Scene definition
		: m_Camera(45.0f, 0.1f, 100.0f)
	{
        m_Scene.CreateDefaultScene();
	}

	virtual void OnUpdate(float ts) override
	{
        if (m_Camera.OnUpdate(ts)) {
            m_Render.ResetFrameIndex();
        }
	}

	virtual void OnUIRender() override
	{
		// Settings //
		ImGui::Begin("Settings");

		ImGui::Text("Last Render: %.3fms", m_lastRenderTime);
		if (ImGui::Button("Render")) {
			RenderImage();
		}

        ImGui::SameLine();
        ImGui::Checkbox(": Render each frame ", &m_Render.GetSettings().renderEachFrame);
        if (m_Render.GetSettings().renderEachFrame
                && m_Render.GetSettings().samples > m_Render.GetFrameIndex()) {
            RenderImage();
        }

        if (ImGui::Button("Reset")) {
            m_Render.ResetFrameIndex();
        }

        ImGui::Separator();

        ImGui::Text("Turn on Light Bouncing: ");
        ImGui::Checkbox(": GI", &m_Render.GetSettings().doGI);

        ImGui::Text("Accumulate samples over time: ");
        ImGui::Checkbox(": Accumulate", &m_Render.GetSettings().accumulate);

        ImGui::Text("Use Lambert Hemisphere Model: ");
        ImGui::Checkbox(": Scattering Type", (bool*)&Lambertian::lambertHemi);
        ImGui::Separator();
        ImGui::Checkbox(": Display Normals", &m_Render.GetSettings().renderNormals);
        ImGui::Checkbox(": Facing Ratio", &m_Render.GetSettings().renderFacingRatio);

        ImGui::Separator();

        // Samples per pixel lock at 1, values below 1 will be ignored
        ImGui::Text("Maximum number of samples: ");
        ImGui::InputInt(": Samples", &m_Render.GetSettings().samples);

        ImGui::Text("Ray Bounce Depth: ");
        ImGui::InputInt(": Ray Bounces", &m_Render.GetSettings().bounceDepth);

		ImGui::End();

        // Camera
        m_Camera.GetUI();

		// Scene //
		ImGui::Begin("Scene");
        // Listbox
        static int currentItem = 0;
        ImGui::ListBox(
                "Scene Objects\n\nSelect an item \nto edit it's \nparameters.",
                &currentItem,
                objectGetter,
                m_Scene.sceneObjects.data(),
                m_Scene.sceneObjects.size(),
                6
                );

        // Draw Item Parameters
        if(currentItem >= 0) {
            ImGui::Separator();

            m_Scene.sceneObjects[currentItem]->getUI();
            // Remove Objects
            if (ImGui::Button("Remove")) {
                m_Scene.RemoveItem(currentItem);
            }
        }
        // Add Objects
        if (ImGui::Button("Add Sphere")) {
            m_Scene.CreateNewSphere();
        }

		// End Window
		ImGui::End();

		// Viewport //
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_viewportWidth = ImGui::GetContentRegionAvail().x;
		m_viewportHeight = ImGui::GetContentRegionAvail().y;

		std::shared_ptr<Image> image = m_Render.getFinalImage();
		if (image) {
 			ImGui::Image(image->GetDescriptorSet(), {(float)image->GetWidth(), (float)image->GetHeight()},
						 ImVec2(0, 1), ImVec2(1, 0));

            // Render zoomed in image at the position of the cursor.
            if(ImGui::IsItemHovered())
            {
                const ImVec2 cursor = ImGui::GetCurrentContext()->IO.MousePos;
                const ImVec2 offset = ImGui::GetItemRectMin();

                float width  = -(float)image->GetWidth();
                auto height = (float)image->GetHeight();

                auto size = ImVec2(ImGui::GetContentRegionAvail().x, (ImGui::GetContentRegionAvail().x * height) / width);
                // Pixel position
                const ImVec2 center = (ImVec2(width, height) * (cursor - offset)) / size;
                glm::vec2 cursorPos(center.x, center.y);

                //std::shared_ptr<Image> magnifiedImage = magnify(image, cursorPos);

                const ImVec2 uv0 = (center + ImVec2(4, 4)) / ImVec2(width, height);
                const ImVec2 uv1 = (center - ImVec2(4, 4)) / ImVec2(width, height);

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                ImGui::SetNextWindowPos(cursor - ImVec2(-15, 69));

                ImGui::BeginTooltip();
                // Flipped y axis
                ImGui::Image(image->GetDescriptorSet(), ImVec2(64, 64), uv0, uv1);

                ImGui::Text("center: %.1f", center.x);
                ImGui::SameLine();
                ImGui::Text(", %.1f", center.y);
                ImGui::Text("uv0: %.1f", uv0.x);
                ImGui::SameLine();
                ImGui::Text(", %.1f", uv0.y);
                ImGui::Text("uv1: %.1f", uv1.x);
                ImGui::SameLine();
                ImGui::Text(", %.1f", uv1.y);

                ImGui::End();
                ImGui::PopStyleVar();
            }
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void RenderImage()
	{
		Timer m_timer;

		m_Render.onResize(m_viewportWidth, m_viewportHeight);
		m_Camera.OnResize(m_viewportWidth, m_viewportHeight);
		m_Render.Render(m_Camera, m_Scene);

		m_lastRenderTime = m_timer.ElapsedMillis();
	}

private:
	Renderer m_Render;
	Camera m_Camera;
	Scene m_Scene;

	float m_lastRenderTime = 0.0f;
	uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
};

Core::Application* Core::CreateApplication(int argc, char** argv)
{
	Core::ApplicationSpecification spec;
	spec.Name = "Ray Tracer Application";

	Core::Application* app = new Core::Application(spec);
	app->PushLayer<RenderLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}