#pragma once
#include <MatrixStack.h>
#include "Scene.h"
#include "Mesh.h"
#include "Light.h"

class Scene5: public Scene{
	enum class MESH{
		AXES, GROUND, GROUND_CIRCLE, GROUND_RING, HEAD, HAIR_PART, EYEBALL, PUPIL, NOSE_TOP, NOSE_BOTTOM, MOUTH_PART, NECK, TORSO,
		SHOULDER, ARM_PART, HAND_PART, FINGER_PART, FINGER_TIP, LEG_PART, FOOT, HANDLE, CHAIN_PART, STRIKING_HEAD, SPIKE, JOINT, MAGNET, LIGHT_SPHERE, NUM_GEOMETRY
	};
	enum class UNIFORM_TYPE{
		U_MVP, U_MODELVIEW, U_MODELVIEW_INVERSE_TRANSPOSE, U_MATERIAL_AMBIENT, U_MATERIAL_DIFFUSE, U_MATERIAL_SPECULAR, U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION, U_LIGHT0_COLOR, U_LIGHT0_POWER, U_LIGHT0_KC, U_LIGHT0_KL, U_LIGHT0_KQ, U_LIGHTENABLED, U_LIGHT0_TYPE, U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF, U_LIGHT0_COSINNER, U_LIGHT0_EXPONENT, U_NUMLIGHTS, U_TOTAL
	};
	private:
		Mesh* meshList[static_cast<unsigned int>(MESH::NUM_GEOMETRY)];
		MS modelStack, viewStack, projectionStack;
		Light light[1];
		bool begin, jumpOver, showLightBall, zActivate;
		char keys[13] = {'1', '2', '3', '4', '5', '6', '7', 'I', 'J', 'K', 'L', 'O', 'U'};
		double startTime, timePressed;
		float a1[20], a2[20], angle, angle1, angle2, angle3, angle4, change, currentAngle, dir, dist, flailTranslate, footTilt, leftSwing, rightSwing, phi, theta, yTranslate, yTranslate2, zTranslate;
		unsigned m_vertexArrayID, m_programID, m_parameters[(unsigned int)UNIFORM_TYPE::U_TOTAL], animation;
		short state;
		Vector3 initialLightPos;
		void RenderMesh(MESH, bool), RenderBaldi(), RenderHead(), RenderHair(), RenderEyes(), RenderNose(), RenderMouth(), RenderBody(),
			RenderLeftUpperLimb(), RenderRightUpperLimb(), RenderLeftLowerLimb(), RenderRightLowerLimb(), RenderFlail();
	public:
		Scene5();
		~Scene5();
		void Reset(), Update(double, float), Render(Camera2&);
};