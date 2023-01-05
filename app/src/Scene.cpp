
#include "Scene.h"

void Scene::CreateDefaultScene()
{
    Sphere sphere;
    sphere.objectName   = "Blue Sphere";
    sphere.position     = {0.3f, 0.3f, 0.2f};
    glm::vec3 albedo 	= {0.0f, 0.0f, 0.9f};
    sphere.radius 	    = {0.3};
    sphere.material_ptr = std::make_shared<Lambertian>(albedo);
    sphere.isVisible    = true;
    spheres.push_back(sphere);
    sceneObjects.push_back(new Sphere(sphere));

    sphere.objectName   = "Green Sphere";
    sphere.position     = {0.0f, 0.0f, 1.4f};
    albedo 	            = {0.0f, 0.9f, 0.0f};
    sphere.radius 	    = {0.1f};
    sphere.material_ptr = std::make_shared<Lambertian>(albedo);
    sphere.isVisible    = true;
    spheres.push_back(sphere);
    sceneObjects.push_back(new Sphere(sphere));

    sphere.objectName   = "Big Metal Sphere";
    sphere.position     = {-0.7f, 0.5f, -0.1f};
    albedo              = {0.75f, 0.75f, 0.75f};
    sphere.radius       = {0.7f};
    sphere.material_ptr = std::make_shared<Metal>(albedo, 0.01f);
    sphere.isVisible    = true;
    spheres.push_back(sphere);
    sceneObjects.push_back(new Sphere(sphere));

    sphere.objectName   = "Small Metal Sphere";
    sphere.position     = {0.6f, 0.1f, 0.8f};
    albedo              = {0.9f, 0.91f, 0.12f};
    sphere.radius       = {0.3f};
    sphere.material_ptr = std::make_shared<Metal>(albedo, 0.4f);
    sphere.isVisible    = true;
    spheres.push_back(sphere);
    sceneObjects.push_back(new Sphere(sphere));

    sphere.objectName   = "Glass Sphere";
    sphere.position     = {-0.3f, 0.1f, 0.8f};
    albedo              = {1.0f, 1.0f, 1.0f};
    sphere.radius       = {0.2f};
    sphere.material_ptr = std::make_shared<Dielectric>(albedo, 1.52f);
    sphere.isVisible    = false;
    spheres.push_back(sphere);
    sceneObjects.push_back(new Sphere(sphere));

    sphere.objectName   = "Ground";
    sphere.position     = {0.0f, -50.2f, 0.0f};
    albedo           	= {0.9f, 0.81f, 0.73f};
    sphere.radius 	    = {50.0f};
    sphere.material_ptr = std::make_shared<Lambertian>(albedo);
    sphere.isVisible    = true;
    spheres.push_back(sphere);
    sceneObjects.push_back(new Sphere(sphere));

    Line xAxis;
    xAxis.objectName  = "xAxis";
    xAxis.position      = {-1000, 0.0f, 0.0f};
    xAxis.destination = {1000, 0.0f, 0.0f};
    albedo            = {1.0f, 0.0f, 0.0f};
    sphere.material_ptr = std::make_shared<Lambertian>(albedo);
    xAxis.thickness   = 0.05f;
    xAxis.isVisible   = true;
    lines.push_back(xAxis);
    sceneObjects.push_back(new Line(xAxis));

    Line yAxis;
    yAxis.objectName  = "yAxis";
    yAxis.position      = {0.0f, -1000, 0.0f};
    yAxis.destination = {0.0f, 1000, 0.0f};
    albedo            = {0.0f, 1.0f, 0.0f};
    sphere.material_ptr = std::make_shared<Lambertian>(albedo);
    yAxis.thickness   = 0.05f;
    yAxis.isVisible   = true;
    lines.push_back(yAxis);
    sceneObjects.push_back(new Line(yAxis));

    Line zAxis;
    zAxis.objectName  = "zAxis";
    zAxis.position      = {0.0f, 0.0f, -1000};
    zAxis.destination = {0.0f, 0.0f, 1000};
    albedo            = {0.0f, 0.0f, 1.0f};
    sphere.material_ptr = std::make_shared<Lambertian>(albedo);
    zAxis.thickness   = 0.05f;
    zAxis.isVisible   = true;
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

    spheres.push_back(newSphere);
    sceneObjects.push_back(&spheres.back());
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