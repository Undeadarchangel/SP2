#pragma once
#include <MatrixStack.h>
#include "Scene.h"
#include "Mesh.h"
#include "Light.h"
#include "ShaderManager.hpp"
#include "ParticleSystem.h"
#include "Object.h"

class MotorScene final: public Scene{
	enum class MESH{
		BULLET, LEFT, RIGHT, FRONT, BACK, TOP, BOTTOM, LIGHT_SPHERE, TEXT_ON_SCREEN, NUM_GEOMETRY
	};
	enum OBJECT_INSTANCES
	{
		UFO,
		NUM_INSTANCES,
	};
	bool showDebugInfo, showLightSphere;
	char keys[7] = {'1', '2', '3', '4', '8', '9', '0'};
	double bulletBounceTime, debugBounceTime, lightBounceTime;
	double CalcFrameRate() const;
	Object object[NUM_INSTANCES];
	Light light[1]{Light(0.f, 192.4f, 0.f)};
	Mesh* meshList[static_cast<unsigned int>(MESH::NUM_GEOMETRY)];
	MS modelStack, viewStack, projectionStack;
	ParticleEmitter bulletGenerator;
	ShaderManager* shMan;
	unsigned m_vertexArrayID;
	void InitMeshes(), RenderLight(), RenderMeshOnScreen(Mesh*, float, float, float, float, int, int), RenderSkybox(bool), RenderTextOnScreen(Mesh*, std::string, Color, float, float, float, int, int);
	void InitLight() const, RenderParticle(Mesh*, GLfloat) const, RenderMesh(Mesh*, bool) const, RenderAnimation(Mesh*, std::string, Color) const, RenderText(Mesh*, std::string, Color) const;
public:
	~MotorScene() override{}
	void Init() override, Update(double, float) override, Render(double, int, int) override, Exit(Scene*) override;
};