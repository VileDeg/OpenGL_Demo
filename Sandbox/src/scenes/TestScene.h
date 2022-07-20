#pragma once
#include "OpenGL_Demo.h"

class TestScene : virtual public Scene
{
public:
    TestScene(Window& window, Camera& camera);
	~TestScene() override;

    void OnUpdate(float deltaTime) override;
    void OnImGuiRender() override;
private:
	void SetLightParams();
private:
    Window& m_Window;
    Camera& m_Camera;
    Mesh m_CubeMesh;
    Mesh* m_BrickwallMesh;
    Entity m_Cubes[125];
    Entity m_LightCubes[3];
    Entity m_Room;
    Entity m_Brickwalls[6];
    Entity m_WorldCenter;
    std::vector<Entity> m_ImportedModels;
    std::vector<Model> m_Models;

    float m_CamSpeed;
    glm::vec3 m_LightPositions[3];

    Light m_DirLightParams;
    Light m_SpotLightParams;
    Light m_PointLightParams;
};