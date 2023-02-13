
#include "Scene.h"

#include "Render.h"
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include "sstream"

void Scene::CreateDefaultScene()
{
    Sphere sphere;
    sphere.objectName    = "Blue Sphere";
    sphere.position      = {0.3f, 0.3f, 0.2f};
    glm::vec3 albedo 	 = {0.0f, 0.0f, 0.9f};
    sphere.radius 	     = {0.3};
    sphere.material_ptr  = std::make_shared<Lambertian>(albedo);
    sphere.isVisible     = true;
    sphere.inReflections = true;
    spheres.push_back(sphere);
    sceneObjects.push_back(new Sphere(sphere));

    sphere.objectName    = "Green Sphere";
    sphere.position      = {0.0f, 0.0f, 1.4f};
    albedo 	             = {0.0f, 0.9f, 0.0f};
    sphere.radius 	     = {0.1f};
    sphere.material_ptr  = std::make_shared<Lambertian>(albedo);
    sphere.isVisible     = true;
    sphere.inReflections = true;
    spheres.push_back(sphere);
    sceneObjects.push_back(new Sphere(sphere));

    sphere.objectName    = "Big Metal Sphere";
    sphere.position      = {-0.7f, 0.5f, -0.1f};
    albedo               = {0.75f, 0.75f, 0.75f};
    sphere.radius        = {0.7f};
    sphere.material_ptr  = std::make_shared<Metal>(albedo, 0.01f);
    sphere.isVisible     = true;
    sphere.inReflections = true;
    spheres.push_back(sphere);
    sceneObjects.push_back(new Sphere(sphere));

    sphere.objectName    = "Small Metal Sphere";
    sphere.position      = {0.6f, 0.1f, 0.8f};
    albedo               = {0.9f, 0.91f, 0.12f};
    sphere.radius        = {0.3f};
    sphere.material_ptr  = std::make_shared<Metal>(albedo, 0.4f);
    sphere.isVisible     = true;
    sphere.inReflections = true;
    spheres.push_back(sphere);
    sceneObjects.push_back(new Sphere(sphere));

    sphere.objectName    = "Glass Sphere";
    sphere.position      = {-0.3f, 0.1f, 0.8f};
    albedo               = {1.0f, 1.0f, 1.0f};
    sphere.radius        = {0.2f};
    sphere.material_ptr  = std::make_shared<Dielectric>(albedo, 1.52f);
    sphere.isVisible     = false;
    sphere.inReflections = true;
    spheres.push_back(sphere);
    sceneObjects.push_back(new Sphere(sphere));

    sphere.objectName    = "Ground";
    sphere.position      = {0.0f, -50.2f, 0.0f};
    albedo           	 = {0.9f, 0.81f, 0.73f};
    sphere.radius 	     = {50.0f};
    sphere.material_ptr  = std::make_shared<Lambertian>(albedo);
    sphere.isVisible     = true;
    sphere.inReflections = true;
    spheres.push_back(sphere);
    sceneObjects.push_back(new Sphere(sphere));

    Line xAxis;
    xAxis.objectName    = "xAxis";
    xAxis.position      = {-1000, 0.0f, 0.0f};
    xAxis.destination   = {1000, 0.0f, 0.0f};
    albedo              = {1.0f, 0.0f, 0.0f};
    xAxis.material_ptr  = std::make_shared<Lambertian>(albedo);
    xAxis.thickness     = 0.0015f;
    xAxis.isVisible     = true;
    xAxis.inReflections = false;
    lines.push_back(xAxis);
    sceneObjects.push_back(new Line(xAxis));

    Line yAxis;
    yAxis.objectName    = "yAxis";
    yAxis.position      = {0.0f, -1000, 0.0f};
    yAxis.destination   = {0.0f, 1000, 0.0f};
    albedo              = {0.0f, 1.0f, 0.0f};
    yAxis.material_ptr  = std::make_shared<Lambertian>(albedo);
    yAxis.thickness     = 0.0015f;
    yAxis.isVisible     = true;
    yAxis.inReflections = false;
    lines.push_back(yAxis);
    sceneObjects.push_back(new Line(yAxis));

    Line zAxis;
    zAxis.objectName    = "zAxis";
    zAxis.position      = {0.0f, 0.0f, -1000};
    zAxis.destination   = {0.0f, 0.0f, 1000};
    albedo              = {0.0f, 0.0f, 1.0f};
    zAxis.material_ptr  = std::make_shared<Lambertian>(albedo);
    zAxis.thickness     = 0.0015f;
    zAxis.isVisible     = true;
    zAxis.inReflections = false;
    lines.push_back(zAxis);
    sceneObjects.push_back(new Line(zAxis));
}


void Scene::CreateNewSphere()
{
    Sphere newSphere;
    std::string name        = "New_Sphere";
    newSphere.objectName    = name;
    newSphere.position      = {0.0f, 0.0f, 0.0f};
    newSphere.radius        = {0.5f};
    glm::vec3 albedo        = {0.8f, 0.8f, 0.8f};
    newSphere.material_ptr  = std::make_shared<Lambertian>(albedo);
    newSphere.isVisible     = true;
    newSphere.inReflections = true;

    spheres.push_back(newSphere);
    sceneObjects.push_back(&spheres.back());
}

void Scene::RayPathToLine(Renderer &pRender, uint32_t &viewportWidth)
{
    std::shared_ptr<Core::Image> image = pRender.getFinalImage();

    const ImVec2 cursor = ImGui::GetCurrentContext()->IO.MousePos;
    uint32_t cursorX = cursor.x;
    uint32_t cursorY = (image->GetHeight() - (cursor.y - 108.0f));

    pRender.GetSettings().renderSinglePixel = true;
    pRender.PerPixel(cursorX, cursorY);

    for(int i = 0; i <= rayToLine.size(); i++)
    {

#define log 0
#if log
        std::cout << "Pixel Coordinates: " << cursorX << ", " << cursorY << "\n";

        std::cout << "Vector Size: " << rayToLine.size() << "\n";
        std::cout << "Ray Hit Distance: " << ray.HitDistance << "\n";

        std::cout << "Ray: " << rayToLine[i].Origin.x << ", "
                             << rayToLine[i].Origin.y << ", "
                             << rayToLine[i].Origin.z << "\n";
#endif

        Line newLine;
        newLine.position = rayToLine[i].Origin;
        if ((i + 1) > rayToLine.size()) {
            Ray ray = rayToLine[i];
            glm::vec3 FinalDest;

            if (ray.HitDistance <= 0.0f) {
                FinalDest = ray.Origin + (ray.Direction * 10.0f);
            } else {
                FinalDest = ray.Origin + (ray.Direction * ray.HitDistance);
            }
            newLine.destination = FinalDest;
        } else {
            newLine.destination = rayToLine[i + 1].Origin;
        }

        std::ostringstream name;
        name << "Ray_" << rayToLineCount + 1 << "_B-" << i;
        newLine.objectName    = name.str();

        newLine.thickness     = 0.001f;
        newLine.isVisible     = true;
        newLine.inReflections = false;
        glm::vec3 albedo      = {1.0f, 1.0f, 1.0f};
        newLine.material_ptr  = std::make_shared<Lambertian>(albedo);

        lines.push_back(newLine);
        sceneObjects.push_back(new Line(newLine));
    }

    pRender.GetSettings().renderSinglePixel = false;
    rayToLineCount += 1;
    rayToLine.clear();
}

void Scene::RemoveItem(int objectIndex)
{
    sceneObjects.erase(sceneObjects.begin() + (objectIndex));
}

// Does Nothing
Object Scene::GetItem(int objectIndex)
{
    sceneObjects[objectIndex];
}