#include "../../core/src/Application.h"
#include "../../core/src/EntryPoint.h"

#include "../../core/src/Image.h"
#include "../../src/Random.h"
#include "../../src/Timer.h"

using namespace Core;

class ExampleLayer : public Core::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last Render: %.3fms", m_lastRenderTime);
		if (ImGui::Button("Render")) {
			Render();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_viewportWidth = ImGui::GetContentRegionAvail().x;
		m_viewportHeight = ImGui::GetContentRegionAvail().y;

		if (m_image) {
 			ImGui::Image(m_image->GetDescriptorSet(), {(float)m_image->GetWidth(), (float)m_image->GetHeight()});
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Render()
	{
		Timer timer;

		if (!m_image || m_viewportWidth != m_image->GetWidth() || m_viewportHeight != m_image->GetHeight()) 
		{
			m_image = std::make_shared<Image>(m_viewportWidth, m_viewportHeight, ImageFormat::RGBA);
			delete[] m_imageData;
			m_imageData = new uint32_t[m_viewportWidth * m_viewportHeight];
		}

		for (uint32_t i = 0; i < m_viewportWidth * m_viewportHeight; i++) 
		{
			m_imageData[i] = Random::UInt();
			m_imageData[i] |= 0xff000000;
		}

		m_image->SetData(m_imageData);
		m_lastRenderTime = timer.ElapsedMillis();
	}

private:
	std::shared_ptr<Image> m_image;
	uint32_t* m_imageData = nullptr;
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