
#include "Scene.h"

#include "Render.h"
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include "sstream"

void Scene::CreateDefaultScene()
{
    glm::vec3 albedo;

    // Blue Sphere
    {
        std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>();
        sphere->objectName = "Blue Sphere";
        sphere->position = {0.3f, 0.3f, 0.2f};
        albedo = {0.0f, 0.0f, 0.9f};
        sphere->radius = {0.3};
        sphere->material_ptr = std::make_shared<Lambertian>(albedo);
        sphere->isVisible = true;
        sphere->inReflections = true;

        AddToScene(sphere);
    }

    // Green Sphere
    {
        std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>();
        sphere->objectName = "Green Sphere";
        sphere->position = {-0.2f, 0.1f, 0.8f};
        albedo = {0.0f, 0.9f, 0.0f};
        sphere->radius = {0.1f};
        sphere->material_ptr = std::make_shared<Lambertian>(albedo);
        sphere->isVisible = true;
        sphere->inReflections = true;

        AddToScene(sphere);
    }

    // Large Metal Sphere
    {
        std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>();
        sphere->objectName = "Big Metal Sphere";
        sphere->position = {-0.7f, 0.5f, -0.1f};
        albedo = {0.75f, 0.75f, 0.75f};
        sphere->radius = {0.7f};
        sphere->material_ptr  = std::make_shared<Metal>(albedo, 0.01f);
        sphere->isVisible = true;
        sphere->inReflections = true;

        AddToScene(sphere);
    }

    // Metal Sphere Gold
    {
        std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>();
        sphere->objectName = "Small Metal Sphere";
        sphere->position = {0.6f, 0.1f, 0.8f};
        albedo = {0.9f, 0.91f, 0.12f};
        sphere->radius = {0.3f};
        sphere->material_ptr = std::make_shared<Metal>(albedo, 0.4f);
        sphere->isVisible = true;
        sphere->inReflections = true;

        AddToScene(sphere);
    }

    // Glass Sphere
    {
        std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>();
        sphere->objectName = "Glass Sphere";
        sphere->position = {0.225f, 0.49f, 1.55f};
        albedo = {1.0f, 1.0f, 1.0f};
        sphere->radius = {0.12f};
        sphere->material_ptr = std::make_shared<Dielectric>(albedo, 1.52f);
        sphere->isVisible = true;
        sphere->inReflections = true;

        AddToScene(sphere);
    }

    // Ground Sphere
    {
        std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>();
        sphere->objectName    = "Ground";
        sphere->position = {0.0f, -75.2f, 0.0f};
        sphere->radius = {75.0f};
        std::shared_ptr<CheckerTexture> checker = std::make_shared<CheckerTexture>(glm::vec3(0.25f, 0.5f, 0.1f),
                                                                                   glm::vec3(0.95f, 0.95f, 0.95f));
        sphere->material_ptr = std::make_shared<Lambertian>(checker);
        sphere->isVisible = true;
        sphere->inReflections = true;

        AddToScene(sphere);
    }

    // xAxis Line
    {
        std::shared_ptr<Line> xAxis = std::make_shared<Line>();
        xAxis->objectName = "xAxis";
        xAxis->position = {-1000, 0.0f, 0.0f};
        xAxis->destination = {1000, 0.0f, 0.0f};
        albedo = {1.0f, 0.0f, 0.0f};
        xAxis->material_ptr = std::make_shared<Lambertian>(albedo);
        xAxis->thickness = 0.0015f;
        xAxis->isVisible = true;
        xAxis->inReflections = false;

        //AddToScene(xAxis);
    }

    // yAxis Line
    {
        std::shared_ptr<Line> yAxis = std::make_shared<Line>();
        yAxis->objectName = "yAxis";
        yAxis->position = {0.0f, -1000, 0.0f};
        yAxis->destination = {0.0f, 1000, 0.0f};
        albedo = {0.0f, 1.0f, 0.0f};
        yAxis->material_ptr = std::make_shared<Lambertian>(albedo);
        yAxis->thickness = 0.0015f;
        yAxis->isVisible = true;
        yAxis->inReflections = false;

        //AddToScene(yAxis);
    }

    // zAxis Line
    {
        std::shared_ptr<Line> zAxis = std::make_shared<Line>();
        zAxis->objectName = "zAxis";
        zAxis->position = {0.0f, 0.0f, -1000};
        zAxis->destination = {0.0f, 0.0f, 1000};
        albedo = {0.0f, 0.0f, 1.0f};
        zAxis->material_ptr = std::make_shared<Lambertian>(albedo);
        zAxis->thickness = 0.0015f;
        zAxis->isVisible = true;
        zAxis->inReflections = false;

        //AddToScene(zAxis);
    }

}

void Scene::CreateNewSphere()
{
    std::shared_ptr<Sphere> newSphere = std::make_shared<Sphere>();
    std::string name        = "New_Sphere";
    newSphere->objectName    = name;
    newSphere->position      = {0.0f, 0.0f, 0.0f};
    newSphere->radius        = {0.5f};
    glm::vec3 albedo        = {0.8f, 0.8f, 0.8f};
    newSphere->material_ptr  = std::make_shared<Lambertian>(albedo);
    newSphere->isVisible     = true;
    newSphere->inReflections = true;

    AddToScene(newSphere);
}

void Scene::RayPathToLine(Renderer &pRender)
{
    std::shared_ptr<Core::Image> image = pRender.getFinalImage();

    const ImVec2 cursor = ImGui::GetCurrentContext()->IO.MousePos;
    uint32_t cursorX = cursor.x;
    uint32_t cursorY = (image->GetHeight() - (cursor.y - 87));

    pRender.GetSettings().renderSinglePixel = true;
    pRender.PerPixel(cursorX, cursorY);

    for(int i = 0; i < rayToLine.size(); i++)
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

        std::shared_ptr<Line> newLine = std::make_shared<Line>();
        newLine->position = rayToLine[i].Origin;

        Ray ray = rayToLine[i];
        glm::vec3 Destination;

        if ((i + 1) > rayToLine.size())
        {
            if (ray.HitDistance <= 0.0f) {
                Destination = ray.Origin + (ray.Direction * 5.0f);
            } else {
                Destination = ray.Origin + (ray.Direction * ray.HitDistance);
            }
            newLine->destination = Destination;
        }
        else
        {
            if (ray.HitDistance <= 0.0f) {
                Destination = ray.Origin + (ray.Direction * 5.0f);
            } else {
                Destination = rayToLine[i + 1].Origin;
            }
            newLine->destination = Destination;
        }

        std::ostringstream name;
        name << "Ray_" << rayToLineCount + 1 << "_B-" << i;
        newLine->objectName    = name.str();

        newLine->thickness     = 0.001f;
        newLine->isVisible     = true;
        newLine->inReflections = false;
        glm::vec3 albedo      = {1.0f, 1.0f, 1.0f};
        newLine->material_ptr  = std::make_shared<Lambertian>(albedo);

        AddToScene(newLine);
    }

    pRender.GetSettings().renderSinglePixel = false;
    rayToLine.clear();
    rayToLineCount += 1;
}

void Scene::AddToScene(std::shared_ptr<Object> object)
{
    sceneObjects.push_back(object);
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

void Scene::ClearRays()
{
    for (std::shared_ptr<Object> item : sceneObjects)
    {
        // Use an iterator and not an object index as the sceneObjects vector changes size after .erase()
        auto iter = std::find_if(sceneObjects.begin(),
                               sceneObjects.end(),
                               [](std::shared_ptr<Object> o)->bool {
                                // Return true if "Ray" is found inside the objectName
                                return o->objectName.find("Ray") != std::string::npos;
                                });
        if (iter != sceneObjects.end())
            sceneObjects.erase(iter);
    }
}

bool Scene::intersect(const Ray &ray, tHit &intersector, int &objectIndex) const
{

    for (size_t i = 0; i < sceneObjects.size(); i++)
    {
        std::shared_ptr<Object> object = sceneObjects[i];

        tHit objInter = object->getIntersector();
        objInter.t_near = std::numeric_limits<float>::infinity();
        objInter.t_far  = std::numeric_limits<float>::infinity();

        if (!object->isVisible)
            continue;

        // Way of keeping our Line from rendering in reflections
        if (!ray.isFirstBounce && !object->inReflections) {
            continue;
        }

        if (object->intersect(ray, objInter)
            && objInter.t_near < intersector.t_near)
        {
            intersector.t_near = objInter.t_near;
            objectIndex = (int)i;
        }
    }
}