#include "../../core/src/Application.h"
#include "../../core/src/EntryPoint.h"

#include "../../core/src/Image.h"
#include "../../core/src/Timer.h"

#include "Render.h"
#include "Camera.h"

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
		ImGui::Begin("Settings");
		ImGui::Text("Last Render: %.3fms", m_lastRenderTime);
		if (ImGui::Button("Render")) {
			RenderImage();
		}

		ImGui::Separator();

		ImGui::Text("Sphere Color: ");
		//ImGui::ColorPicker3("", (float*)&Renderer::sphereColor);
		ImGui::NewLine();

		ImGui::Text("Light Direction: ");
		ImGui::InputFloat3("", (float*)&Renderer::lightDirection);
	
		ImGui::End();

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

		std::vector<std::unique_ptr<Object>> objects;

		// Bug where the sphere at 0,0,0 renders above the two outer spheres
		// Not sure where this comes from 
		objects.push_back(std::unique_ptr<Object>(new Sphere(glm::vec3(0.0f, 0.0f, 0.0f), 0.2f, glm::vec3(1.0f, 0.0f, 0.0f))));
		objects.push_back(std::unique_ptr<Object>(new Sphere(glm::vec3(-1.0f, 0.0f, 0.0f), 0.2f, glm::vec3(1.0f, 0.0f, 0.0f))));
		objects.push_back(std::unique_ptr<Object>(new Sphere(glm::vec3(1.0f, 0.0f, 0.0f), 0.2f, glm::vec3(1.0f, 0.0f, 0.0f))));

		m_Render.Render(m_Camera, objects);

		m_lastRenderTime = m_timer.ElapsedMillis();
	}

private:
	Renderer m_Render;
	Camera m_Camera;

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