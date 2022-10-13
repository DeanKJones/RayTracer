#include "../../core/src/Application.h"
#include "../../core/src/EntryPoint.h"

#include "../../core/src/Image.h"
#include "../../core/src/Timer.h"

#include "Render.h"
#include "Camera.h"

#include "string.h"
#include "glm/gtc/type_ptr.hpp"

using namespace Core;

class ExampleLayer : public Core::Layer
{
public:
	ExampleLayer()
		: m_Camera(45.0f, 0.1f, 100.0f) 
	{
		// Scene Description
		{
			Sphere sphere;
			sphere.position = {0.3f, 0.3f, 0.2f};
			sphere.albedo 	= {0.0f, 0.0f, 0.9f};
			sphere.radius 	= {0.3};
			sphere.material_ptr = std::make_shared<Lambertian>(sphere.albedo);
			m_Scene.spheres.push_back(sphere);
		}
		{
			Sphere sphereSmall;
			sphereSmall.position = {0.0f, 0.0f, 1.4f};
			sphereSmall.albedo 	 = {0.0f, 0.9f, 0.0f};
			sphereSmall.radius 	 = {0.1f};
			sphereSmall.material_ptr = std::make_shared<Lambertian>(sphereSmall.albedo);
			m_Scene.spheres.push_back(sphereSmall);
		}
		{
			Sphere sphereBig;
			sphereBig.position = {-0.7f, 0.5f, -0.1f};
			sphereBig.albedo   = {0.75f, 0.75f, 0.75f};
			sphereBig.radius   = {0.7f};
			sphereBig.material_ptr = std::make_shared<Metal>(sphereBig.albedo, 0.01f);
			m_Scene.spheres.push_back(sphereBig);
		}
		{
			Sphere metalSmall;
			metalSmall.position = {0.6f, 0.1f, 0.8f};
			metalSmall.albedo   = {0.9f, 0.91f, 0.12f};
			metalSmall.radius   = {0.3f};
			metalSmall.material_ptr = std::make_shared<Metal>(metalSmall.albedo, 0.4f);
			m_Scene.spheres.push_back(metalSmall);
		}
		{
			Sphere ground;
			ground.position = {0.0f, -50.2f, 0.0f};
			ground.albedo 	= {0.9f, 0.81f, 0.73f};
			ground.radius 	= {50.0f};
			ground.material_ptr = std::make_shared<Lambertian>(ground.albedo);
			m_Scene.spheres.push_back(ground);
		}
	}

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
		ImGui::Checkbox("Render each frame: ", (bool*)&Renderer::renderEachFrame);
		if (Renderer::renderEachFrame) {
			RenderImage();
		}
		ImGui::End();

		// Scene //
		ImGui::Begin("Scene");
		for(int i = 0; i < m_Scene.spheres.size(); i++){
			ImGui::PushID(i);

			Sphere& sphere = m_Scene.spheres[i];

			// Changing albedo here does nothing as the albedo is now stored in the Material pointer
			ImGui::ColorEdit3(": Color", glm::value_ptr(sphere.albedo));
			ImGui::DragFloat3(": Position", glm::value_ptr(sphere.position), 0.1f);
			ImGui::DragFloat(": Size", &sphere.radius, 0.1f);
			
			ImGui::Separator();
			ImGui::PopID();
		}

		ImGui::Text("Turn on Light Bouncing: ");
		ImGui::Checkbox(": GI", (bool*)&Renderer::doGI);

		ImGui::Text("Use Lambert Hemisphere Model: ");
		ImGui::Checkbox(": Scattering Type", (bool*)&Lambertian::lambertHemi);
		ImGui::Separator();

		ImGui::Text("Light Direction: ");
		ImGui::DragFloat3(": Direction", (float*)&Renderer::lightDirection);

		// Samples per pixel lock at 1, values below 1 will be ignored
		ImGui::Text("Samples per pixel ");
		ImGui::InputInt(": Samples", (int*)&Renderer::samplesPerPixel);

		ImGui::Text("Ray Bounce Depth ");
		ImGui::InputInt(": Ray Bounces", (int*)&Renderer::bounceDepth);

		// End Window
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