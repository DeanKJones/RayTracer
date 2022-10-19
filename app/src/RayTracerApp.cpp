#include "../../core/src/Application.h"
#include "../../core/src/EntryPoint.h"

#include "../../core/src/Image.h"
#include "../../core/src/Timer.h"

#include "Render.h"
#include "Camera.h"

#include "utils/Utilities.h"
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
            sphere.objectName = "Blue Sphere";
			sphere.position = {0.3f, 0.3f, 0.2f};
            glm::vec3 albedo 	= {0.0f, 0.0f, 0.9f};
			sphere.radius 	= {0.3};
			sphere.material_ptr = std::make_shared<Lambertian>(albedo);
			m_Scene.spheres.push_back(sphere);
		}
		{
			Sphere sphere;
            sphere.objectName = "Green Sphere";
            sphere.position = {0.0f, 0.0f, 1.4f};
            glm::vec3 albedo 	 = {0.0f, 0.9f, 0.0f};
            sphere.radius 	 = {0.1f};
            sphere.material_ptr = std::make_shared<Lambertian>(albedo);
			m_Scene.spheres.push_back(sphere);
		}
		{
			Sphere sphere;
            sphere.objectName = "Big Metal Sphere";
            sphere.position = {-0.7f, 0.5f, -0.1f};
            glm::vec3 albedo   = {0.75f, 0.75f, 0.75f};
            sphere.radius   = {0.7f};
            sphere.material_ptr = std::make_shared<Metal>(albedo, 0.01f);
			m_Scene.spheres.push_back(sphere);
		}
		{
			Sphere sphere;
            sphere.objectName = "Small Metal Sphere";
            sphere.position = {0.6f, 0.1f, 0.8f};
            glm::vec3 albedo   = {0.9f, 0.91f, 0.12f};
            sphere.radius   = {0.3f};
            sphere.material_ptr = std::make_shared<Metal>(albedo, 0.4f);
			m_Scene.spheres.push_back(sphere);
		}
        {
            Sphere sphere;
            sphere.objectName = "Glass Sphere";
            sphere.position = {-0.3f, 0.1f, 0.8f};
            glm::vec3 albedo   = {1.0f, 1.0f, 1.0f};
            sphere.radius   = {0.2f};
            sphere.material_ptr = std::make_shared<Dielectric>(albedo, 1.5f);
            m_Scene.spheres.push_back(sphere);
        }
		{
			Sphere sphere;
            sphere.objectName = "Ground";
            sphere.position = {0.0f, -50.2f, 0.0f};
            glm::vec3 albedo 	= {0.9f, 0.81f, 0.73f};
            sphere.radius 	= {50.0f};
            sphere.material_ptr = std::make_shared<Lambertian>(albedo);
			m_Scene.spheres.push_back(sphere);
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

        // Listbox
        static int currentItem = 1;
        ImGui::ListBox(
                "Scene Objects\n\nSelect an item \nto edit it's \nparameters.",
                &currentItem,
                sphereGetter,
                m_Scene.spheres.data(),
                m_Scene.spheres.size(),
                6
                );
        // Draw Item Parameters
        if(currentItem >= 0) {
            ImGui::Separator();

            Sphere &sphere = m_Scene.spheres[currentItem];

            char *objName = sphere.objectName.data();
            ImGui::Text("%s is selected", objName);

            ImGui::Separator();

            ImGui::ColorEdit3(": Color", glm::value_ptr(sphere.material_ptr->albedo));
            std::string typeidName = typeid(*(sphere.material_ptr)).name();

            if (typeidName.find("Metal") != std::string::npos) {
                auto sphereRoughness = &(reinterpret_cast<Metal *>(sphere.material_ptr.get())->roughness);
                ImGui::DragFloat(": Roughness", sphereRoughness, 0.05f, 0.0f, 1.0f);
            }

            ImGui::DragFloat3(": Position", glm::value_ptr(sphere.position), 0.1f);
            ImGui::DragFloat(": Size", &sphere.radius, 0.1f);

            ImGui::Separator();
            // No need to set and pop IDs - listbox handles it already
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