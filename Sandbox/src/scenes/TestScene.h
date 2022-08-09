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
    Ref<Mesh> m_CubeMesh;
    Ref<Mesh> m_BrickwallMesh;
    Entity m_Cubes[8];
    Entity m_LightCube;
    Entity m_Brickwalls[6];
    Entity m_WorldCenter;
    float m_CamSpeed;
    Light m_PointLightParams;
};