#include "../../core/src/Application.h"
#include "../../core/src/EntryPoint.h"

#include "../../core/src/Image.h"
#include "../../core/src/Timer.h"

#include "Render.h"
#include "Camera.h"

#include "string.h"

using namespace Core;

class ExampleLayer : public Core::Layer
{
public:
	ExampleLayer()
		: m_Camera(45.0f, 0.1f, 100.0f) {}

	virtual void OnUpdate(float ts) override
	{
		m_Camera.OnUpdate(ts);
	}

	virtual void OnUIRender() override
	{
		// Settings //
		ImGui::Begin("Settings");
		ImGui::Text("Last Render: %.3fms", m_lastRenderTime);
		if (ImGui::Button("Render")) {
			RenderImage();
		}
		ImGui::End();

		// Scene //
		ImGui::Begin("Scene");
		for(int i = 0; i < m_Objects.size(); i++){
			ImGui::PushID(i);

			int objectID = m_Objects[i]->ID;

			ImGui::ColorEdit3(": Color", (float*)&Sphere::color[objectID]);
			ImGui::DragFloat3(": Position", (float*)&Sphere::sphereCenter[objectID], 0.1f);
			ImGui::DragFloat(": Size", (float*)&Sphere::radius[objectID], 0.1f);
			
			ImGui::Separator();
			ImGui::PopID();
		}

		ImGui::Text("Light Direction: ");
		ImGui::InputFloat3("", (float*)&Renderer::lightDirection);
		ImGui::End();

		// Viewport //
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_viewportWidth = ImGui::GetContentRegionAvail().x;
		m_viewportHeight = ImGui::GetContentRegionAvail().y;

		auto image = m_Render.getFinalImage();
		if (image) {
 			ImGui::Image(image->GetDescriptorSet(), {(float)image->GetWidth(), (float)image->GetHeight()},
						 ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();
		ImGui::PopStyleVar();

		RenderImage();
	}

	void RenderImage()
	{
		Timer m_timer;

		m_Render.onResize(m_viewportWidth, m_viewportHeight);
		m_Camera.OnResize(m_viewportWidth, m_viewportHeight);

		// Create Objects
		if(m_Objects.size() == 0){
			m_Objects.push_back(std::unique_ptr<Object>(new Sphere(glm::vec3(0.0f, 0.0f, 0.0f), 0.2f, glm::vec3(1.0f, 0.0f, 0.0f))));
			m_Objects.push_back(std::unique_ptr<Object>(new Sphere(glm::vec3(-1.0f, 0.0f, 0.0f), 0.2f, glm::vec3(0.0f, 1.0f, 0.0f))));
			m_Objects.push_back(std::unique_ptr<Object>(new Sphere(glm::vec3(1.0f, 0.0f, 0.0f), 0.2f, glm::vec3(0.0f, 0.0f, 1.0f))));
		}
		m_Render.Render(m_Camera, m_Objects);

		m_lastRenderTime = m_timer.ElapsedMillis();
	}

private:
	Renderer m_Render;
	Camera m_Camera;
	std::vector<std::unique_ptr<Object>> m_Objects;

	float m_lastRenderTime = 0.0f;
	uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
};

Core::Application* Core::CreateApplication(int argc, char** argv)
{
	Core::ApplicationSpecification spec;
	spec.Name = "Ray Tracer Application";

	Core::Application* app = new Core::Application(spec);
	app->PushLayer<ExampleLayer>();
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