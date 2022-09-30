#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Mandelbrot.h"
#include <Walnut/Timer.h>

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Time Taken: %.3fms", m_TimeTaken);
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::InputInt("Max Iterations", &m_MaxIteration);
		ImGui::InputDouble("Zoom", &zoom, 1.0);
		ImGui::DragFloat("X offset", &xoffset, 0.001f, 0.0f, 0.0f, "%.6f");
		ImGui::DragFloat("Y offset", &yoffset, 0.001f, 0.0f, 0.0f, "%.6f");
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		auto finalImage = m_Mandelbrot.GetFinalImage();
		if (finalImage)
			ImGui::Image(finalImage->GetDescriptorSet(), { (float)finalImage->GetWidth(), (float)finalImage->GetHeight() });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Render()
	{
		Timer timer;

		m_Mandelbrot.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Mandelbrot.Render(m_MaxIteration, zoom, xoffset, yoffset);

		m_TimeTaken = timer.ElapsedMillis();
	}
private:
	Mandelbrot m_Mandelbrot;
	double m_ViewportWidth = 0, m_ViewportHeight = 0;
	double m_TimeTaken = 0;
	double zoom = 1.0f;
	float xoffset = 0.0f;
	float yoffset = 0.0f;
	int m_MaxIteration = 1000;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";
	spec.Width = 800;
	spec.Height = 600;

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	/*app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});*/
	return app;
}