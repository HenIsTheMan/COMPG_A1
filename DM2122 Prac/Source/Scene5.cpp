#include "Scene5.h"
#include "GL\glew.h"
#include "Application.h"
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"

constexpr auto LSPEED = 10.f;
double lightBounceTime = 0.0;
extern double elapsedTime;

Scene5::Scene5(){ //Default constructor for initialising member vars of scene obj
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE); //Enable back-face culling
	glEnable(GL_DEPTH_TEST); //Enable depth test
	m_programID = LoadShaders("Shader//Shading.vertexshader", "Shader//LightSource.fragmentshader"); //Load shaders
	glUseProgram(m_programID);
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f); //Set bg colour

	m_parameters[unsigned int(UNIFORM_TYPE::U_NUMLIGHTS)] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_TYPE)] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_SPOTDIRECTION)] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_COSCUTOFF)] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_COSINNER)] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_EXPONENT)] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[unsigned int(UNIFORM_TYPE::U_MVP)] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[unsigned int(UNIFORM_TYPE::U_MODELVIEW)] = glGetUniformLocation(m_programID, "MV");
	m_parameters[unsigned int(UNIFORM_TYPE::U_MODELVIEW_INVERSE_TRANSPOSE)] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[unsigned int(UNIFORM_TYPE::U_MATERIAL_AMBIENT)] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[unsigned int(UNIFORM_TYPE::U_MATERIAL_DIFFUSE)] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[unsigned int(UNIFORM_TYPE::U_MATERIAL_SPECULAR)] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[unsigned int(UNIFORM_TYPE::U_MATERIAL_SHININESS)] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_POSITION)] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_COLOR)] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_POWER)] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_KC)] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_KL)] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_KQ)] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHTENABLED)] = glGetUniformLocation(m_programID, "lightEnabled");

	glUniform1i(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_TYPE)], GLint(light[0].type));
	glUniform3fv(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_COLOR)], 1, &light[0].color.r);
	glUniform1f(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_POWER)], light[0].power);
	glUniform1f(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_KC)], light[0].kC);
	glUniform1f(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_KL)], light[0].kL);
	glUniform1f(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_KQ)], light[0].kQ);
	glUniform3fv(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_SPOTDIRECTION)], 1, &light[0].spotDirection.x);
	glUniform1f(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_COSCUTOFF)], light[0].cosCutoff);
	glUniform1f(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_COSINNER)], light[0].cosInner);
	glUniform1f(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_EXPONENT)], light[0].exponent);
	glUniform1i(m_parameters[unsigned int(UNIFORM_TYPE::U_NUMLIGHTS)], 1);

	//Meshes to be rendered
	meshList[unsigned int(MESH::AXES)] = MeshBuilder::GenerateAxes(1000.f, 1000.f, 1000.f);
	meshList[unsigned int(MESH::GROUND)] = MeshBuilder::GenerateQuad(Color(0.8f, 1, 0.3f), 1000.f, 1000.f);
	meshList[unsigned int(MESH::GROUND_CIRCLE)] = MeshBuilder::GenerateCircle(Color(0.75f, 0.85f, 0.5f), 40, 40.f);
	meshList[unsigned int(MESH::GROUND_RING)] = MeshBuilder::GenerateRing(Color(0.26f, 0.89f, 0.24f), 40, 40.f, 35.f);
	meshList[unsigned int(MESH::HEAD)] = MeshBuilder::GenerateSphere(Color(0.79f, 0.89f, 0.53f), 30, 30, 1.f);
	meshList[unsigned int(MESH::HAIR_PART)] = MeshBuilder::GenerateCuboid(Color(0.8f, 0.4f, 0.0f), 1.f, 1.f, 1.f);
	meshList[unsigned int(MESH::EYEBALL)] = MeshBuilder::GenerateSphere(Color(.8f, .8f, .8f), 30, 30, 1.f);
	meshList[unsigned int(MESH::PUPIL)] = MeshBuilder::GenerateHemisphere(Color(0, 0, 0), 40, 40, 1.f);
	meshList[unsigned int(MESH::NOSE_TOP)] = MeshBuilder::GenerateCone(Color(0.75f, 0.85f, 0.5f), 30, 1.f, 2.f);
	meshList[unsigned int(MESH::NOSE_BOTTOM)] = MeshBuilder::GenerateSphere(Color(0.75f, 0.85f, 0.5f), 30, 30, 1.f);
	meshList[unsigned int(MESH::MOUTH_PART)] = MeshBuilder::GenerateCuboid(Color(.9f, .1f, .1f), 1.f, 1.f, 1.f);
	meshList[unsigned int(MESH::NECK)] = MeshBuilder::GenerateConicalFrustum(Color(0.79f, 0.89f, 0.53f), 30, 2, 1.2f, 4.f);
	meshList[unsigned int(MESH::TORSO)] = MeshBuilder::GenerateCuboid(Color(0.22f, 0.84f, 0.26f), 3.f, 7.f, 1.f);
	meshList[unsigned int(MESH::SHOULDER)] = MeshBuilder::GenerateSphere(Color(0.26f, 0.89f, 0.24f), 20, 20, 1.f);
	meshList[unsigned int(MESH::ARM_PART)] = MeshBuilder::GenerateCylinder(Color(0.26f, 0.89f, 0.24f), 50, 1.f, 1.f);
	meshList[unsigned int(MESH::HAND_PART)] = MeshBuilder::GenerateCuboid(Color(0.79f, 0.89f, 0.53f), 1.f, 1.f, 1.f);
	meshList[unsigned int(MESH::FINGER_PART)] = MeshBuilder::GenerateCylinder(Color(0.79f, 0.89f, 0.53f), 30, .5f, 4.f);
	meshList[unsigned int(MESH::FINGER_TIP)] = MeshBuilder::GenerateHemisphere(Color(0.79f, 0.89f, 0.53f), 30, 30, .5f);
	meshList[unsigned int(MESH::LEG_PART)] = MeshBuilder::GenerateCylinder(Color(.43f, .35f, .94f), 50, 1.f, 3.8f);
	meshList[unsigned int(MESH::FOOT)] = MeshBuilder::GenerateCylinder(Color(0.71f, 0.40f, 0.12f), 50, 0.8f, 2.3f);
	meshList[unsigned int(MESH::HANDLE)] = MeshBuilder::GenerateCylinder(Color(0.2f, 0.2f, 0.2f), 50, 2.f, 5.f);
	meshList[unsigned int(MESH::CHAIN_PART)] = MeshBuilder::GenerateTorus(Color(.25f, 0.25f, .25f), 40, 40, 0.9f, 0.2f);
	meshList[unsigned int(MESH::STRIKING_HEAD)] = MeshBuilder::GenerateSphere(Color(0.2f, 0.2f, 0.2f), 30, 30, 1.f);
	meshList[unsigned int(MESH::SPIKE)] = MeshBuilder::GenerateCone(Color(.25f, .25f, .25f), 30, 1.f, 2.f);
	meshList[unsigned int(MESH::JOINT)] = MeshBuilder::GenerateSphere(Color(.43f, .35f, .94f), 20, 20, 1.f);
	meshList[unsigned int(MESH::MAGNET)] = MeshBuilder::GenerateCuboid(Color(0.3f, 0.3f, 0.3f), 1.f, 1.f, 1.f);
	meshList[unsigned int(MESH::LIGHT_SPHERE)] = MeshBuilder::GenerateSphere(Color(1.f, 1.f, 1.f), 30, 30, 1.f);
	
	dist = 1.9f;
	phi = -90.f;
	theta = -180.f;

	//Angles for positioning flail spikes
	a1[0] = 0.f;
	a2[0] = 0.f;
	a1[1] = 45.f;
	a2[1] = 0.f;
	a1[2] = 90.f;
	a2[2] = 0.f;
	a1[3] = 135.f;
	a2[3] = 0.f;
	a1[4] = 225.f;
	a2[4] = 0.f;
	a1[5] = 270.f;
	a2[5] = 0.f;
	a1[6] = 315.f;
	a2[6] = 0.f;
	a1[7] = 0.f;
	a2[7] = 45.f;
	a1[8] = 0.f;
	a2[8] = 90.f;
	a1[9] = 0.f;
	a2[9] = 135.f;
	a1[10] = 0.f;
	a2[10] = 225.f;
	a1[11] = 0.f;
	a2[11] = 270.f;
	a1[12] = 0.f;
	a2[12] = 315.f;

	begin = zActivate = showLightBall = animation = state = 0;
	jumpOver = 1;

	angle = angle1 = angle2 = angle3 = angle4 = change = currentAngle = flailTranslate = footTilt = leftSwing = rightSwing = yTranslate = zTranslate = yTranslate2 = 0.0f;
	//angle - angle used for rotations during jumping animation
	//angle1, angle2, angle3 and angle4 - angles used for rotations during walking animation
	//yTranslate - amt to translate Baldi by in the y-axis during jumping animation
	//zTranslate - amt to translate Baldi by in the z-axis during walking animation
	//yTranslate2 - amt to translate Baldi by in the y-axis during prepare-to-jump "animation"
	
	dir = 1.f;
	startTime = timePressed = 0.0;
	initialLightPos = Vector3(light[0].position.x, light[0].position.y, light[0].position.z);
}

Scene5::~Scene5(){ //Destructor
	for(int i = 0; i < int(MESH::NUM_GEOMETRY); ++i){
		if(meshList[i] != NULL){
			delete meshList[i];
		}
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void Scene5::Reset(){ //Reset scene
	begin = zActivate = animation = state = 0;
	jumpOver = 1;
	angle = angle1 = angle2 = angle3 = angle4 = change = currentAngle = flailTranslate = footTilt = leftSwing = rightSwing = yTranslate = zTranslate = yTranslate2 = 0.0f;
	dir = 1.f;
	startTime = timePressed = 0.0;
}

void Scene5::Update(double dt, float FOV){ //Update scene
	for(int i = 0; i < 13; ++i){
		if(Application::IsKeyPressed(keys[i])){
			switch(keys[i]){
				case '1': glDisable(GL_CULL_FACE); break; //Disable back-face culling
				case '2': glEnable(GL_CULL_FACE); break; //Enable back-face culling
				case '3': glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break; //Set polygon mode to GL_FILL (default mode)
				case '4': glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break; //Set polygon mode to GL_LINE (wireframe mode)
				case '5': { //Change to point light
					light[0].type = Light::LIGHT_TYPE::LIGHT_POINT;
					glUniform1i(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_TYPE)], GLint(light[0].type));
					break;
				}
				case '6': { //Change to directional light
					light[0].type = Light::LIGHT_TYPE::LIGHT_DIRECTIONAL;
					glUniform1i(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_TYPE)], GLint(light[0].type));
					break;
				}
				case '7': { //Change to spotlight
					light[0].type= Light::LIGHT_TYPE::LIGHT_SPOT;
					glUniform1i(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_TYPE)], GLint(light[0].type));
					break;
				}
				case 'I': initialLightPos.z -= float(LSPEED * dt); break; //Move light in the direction of the negative z-axis
				case 'J': initialLightPos.x -= float(LSPEED * dt); break; //Move light in the direction of the negative x-axis
				case 'K': initialLightPos.z += float(LSPEED * dt); break; //Move light in the direction of the positive z-axis
				case 'L': initialLightPos.x += float(LSPEED * dt); break; //Move light in the direction of the positive x-axis
				case 'O': initialLightPos.y -= float(LSPEED * dt); break; //Move light in the direction of the negative y-axis
				case 'U': initialLightPos.y += float(LSPEED * dt); break; //Move light in the direction of the positive y-axis
			}
		}
	}

	if(Application::IsKeyPressed('8') && animation == 0){ //Reset character animation
		Reset();
	} else if(Application::IsKeyPressed('M') && (animation == 0 || animation == 1)){ //Triggers magnetic attraction animation
		if(animation == 0){
			Reset();
		}
		if(startTime == 0.0){
			startTime = elapsedTime;
		}
		animation = 1;
	} else if(Application::IsKeyPressed('9') && (animation == 0 || animation == 2)){ //Triggers jumping animation
		if(animation == 0){
			Reset();
		}
		animation = 2;
	} else if(Application::IsKeyPressed('0') && (animation == 0 || animation == 3)){ //Triggers walking animation
		if(animation == 0){
			Reset();
		}
		animation = 3;
	} else{ //Set animation to 0
		animation = 0;
	}
	if(Application::IsKeyPressed('P') && lightBounceTime <= elapsedTime){ //Show/Hide light sphere
		showLightBall = !showLightBall;
		lightBounceTime = elapsedTime + 0.4;
	}
	if(startTime != 0.0){
		timePressed = elapsedTime - startTime; //Calculates time pressed
	}
	if(animation == 1){ //Magnetic attraction animation
		if(timePressed >= 0.5f && timePressed < 1.31f){
			angle1 -= .5f;
			angle3 -= .5f;
			zTranslate += .5f * float(timePressed);
		} else if(timePressed >= 1.31f && timePressed < 1.54f){
			dir = -1;
			angle1 += .8f * float(timePressed) * 2;
			angle3 += .8f * float(timePressed) * 2;
		}
	}
	if(animation == 2){ //Prepare-to-jump "animation"
		jumpOver = 0;
		if(angle < 30.f){
			angle += 1.f;
			yTranslate2 -= 0.05f;
		}
	} else if(animation == 0 && angle != 0.0f){ //Jump animation
		if(currentAngle == 0.0f){
			currentAngle = angle;
			change = currentAngle / 30.f;
		}
		angle -= 1.f;
		footTilt = 25.f;
		yTranslate2 += 0.05f;
	}
	yTranslate += change;
	flailTranslate += change;
	change -= 0.04f; //Gives gravity effect
	if(yTranslate < 0.0f){ //Makes Baldi be in his initial position
		if(animation != 2){
			jumpOver = 1;
		}
		yTranslate = 0.0f;
		flailTranslate = 0.0f;
		change = 0.0f;
		currentAngle = 0.0f;
		footTilt = 0.0f;
	}
	if(animation == 3){ //Walking animation
		if(state == 0){ //Runs for 48 frames
			begin = 1;
			angle1 = ++angle2;
			if(angle1 == 48.0f){ //angle1 = angle2 = 48.f
				++state;
			}
		}
		if(state == 1){ //Runs for 48 frames
			zActivate = 1;
			angle1 -= .5f;
			angle2 -= 1.f;
			if(begin){
				angle3 -= .5f;
			} else{
				angle3 -= .25f;
			}
			if(angle2 == 0.0f){ //angle1 == 24.f, angle2 == 0.f, angle3 == -24.f
				++state;
			}
		}
		if(state == 2){ //Runs for 48 frames
			angle1 -= .75f;
			angle3 += 1.5f;
			angle4 += 1.f;
			if(angle3 == 48.f){ //angle1 == -12.f, angle3 == 48.f, angle4 == 48.f
				++state;
			}
		}
		if(state == 3){ //Runs for 48 frames
			angle1 -= .25f;
			angle3 -= .5f;
			angle4 -= 1.f;
			if(angle4 == 0.0f){ //angle1 == -24.f, angle3 == 24.f, angle4 == 0.f
				++state;
			}
		}
		if(state == 4){ //Runs for 48 frames
			angle1 += 1.5f;
			angle2 += 1.f;
			angle3 -= 0.75;
			if(angle1 == 48.0f){ //angle1 == 48.f, angle2 == 48.f, angle3 == -12.f
				begin = 0;
				state = 1;
			}
		}
		if(zActivate){
			zTranslate += .1f; //Cause Baldi to move forward
		}
	}
	Mtx44 projection;
	projection.SetToPerspective(FOV, 4.f / 3.f, 0.1f, 1000.f); //FOV value affects cam zoom
	projectionStack.LoadMatrix(projection);
}

void Scene5::RenderMesh(MESH obj, bool enableLight = 1){ //For rendering meshes to form Baldi
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[unsigned int(unsigned int(UNIFORM_TYPE::U_MVP))], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[unsigned int(unsigned int(UNIFORM_TYPE::U_MODELVIEW))], 1, GL_FALSE, &modelView.a[0]);
	if(enableLight){
		glUniform1i(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHTENABLED)], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[unsigned int(UNIFORM_TYPE::U_MODELVIEW_INVERSE_TRANSPOSE)], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//Load material
		glUniform3fv(m_parameters[unsigned int(UNIFORM_TYPE::U_MATERIAL_AMBIENT)], 1, &meshList[unsigned int(obj)]->material.kAmbient.r);
		glUniform3fv(m_parameters[unsigned int(UNIFORM_TYPE::U_MATERIAL_DIFFUSE)], 1, &meshList[unsigned int(obj)]->material.kDiffuse.r);
		glUniform3fv(m_parameters[unsigned int(UNIFORM_TYPE::U_MATERIAL_SPECULAR)], 1, &meshList[unsigned int(obj)]->material.kSpecular.r);
		glUniform1f(m_parameters[unsigned int(UNIFORM_TYPE::U_MATERIAL_SHININESS)], meshList[unsigned int(obj)]->material.kShininess);
	} else{
		glUniform1i(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHTENABLED)], 0);
	}
	meshList[unsigned int(obj)]->Render();
}

void Scene5::RenderBaldi(){ //Render Baldi
	modelStack.PushMatrix();
		modelStack.Translate(0, yTranslate, zTranslate);
		modelStack.PushMatrix();
			modelStack.Translate(0, yTranslate2, 0);
			RenderHead();
		modelStack.PopMatrix();
		RenderBody();
	modelStack.PopMatrix();
}

void Scene5::RenderHead(){ //Render Baldi's head
	modelStack.PushMatrix();
		modelStack.Translate(0, 15.2f, 0.f);
		modelStack.Scale(3.8f, 4.2f, 3.8f);
		RenderMesh(MESH::HEAD);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
		modelStack.Translate(0, 16.6f, 3.4f);
		RenderEyes();
		RenderNose();
		RenderHair();
	modelStack.PopMatrix();
	RenderMouth();
}

void Scene5::RenderHair(){ //Render Baldi's hair
	//Strand of hair
	modelStack.PushMatrix();
	modelStack.Translate(.4f, 2.3f, -1.2f);
	modelStack.Rotate(35.f, 0, 0, 1.f);
	modelStack.Scale(.8f, .1f, .1f);
	RenderMesh(MESH::HAIR_PART);
	modelStack.PopMatrix();

	//Left eyebrow
	modelStack.PushMatrix();
	modelStack.Translate(1.5f, 1.3f, -0.8f);
	modelStack.Rotate(-10.f, 0, 0, 1.f);
	modelStack.Scale(.8f, .1f, .5f);
	RenderMesh(MESH::HAIR_PART);
	modelStack.PopMatrix();

	//Right eyebrow
	modelStack.PushMatrix();
	modelStack.Translate(-1.5f, 1.3f, -0.8f);
	modelStack.Rotate(10.f, 0, 0, 1.f);
	modelStack.Scale(.8f, .1f, .5f);
	RenderMesh(MESH::HAIR_PART);
	modelStack.PopMatrix();
}

void Scene5::RenderEyes(){ //Render Baldi's eyes
	modelStack.PushMatrix();
		modelStack.Translate(-1.5f, 0, 0);
		RenderMesh(MESH::EYEBALL);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
		modelStack.Translate(-1.5f, 0, .5f);
		modelStack.Rotate(90.f, 1.f, 0, 0);
		modelStack.Scale(.5f, .8f, .5f);
		RenderMesh(MESH::PUPIL);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
		modelStack.Translate(1.5f, 0, 0);
		RenderMesh(MESH::EYEBALL);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
		modelStack.Translate(1.5f, 0, .5f);
		modelStack.Rotate(90.f, 1.f, 0, 0);
		modelStack.Scale(.5f, .8f, .5f);
		RenderMesh(MESH::PUPIL);
	modelStack.PopMatrix();
}

void Scene5::RenderNose(){ //Render Baldi's nose
	modelStack.PushMatrix();
	modelStack.Translate(0, -1.f, .4f);
	modelStack.Scale(.7f, .7f, .7f);
	RenderMesh(MESH::NOSE_TOP);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -2.f, .5f);
	modelStack.Scale(.7f, .7f, .7f);
	RenderMesh(MESH::NOSE_BOTTOM);
	modelStack.PopMatrix();
}

void Scene5::RenderMouth(){ //Render Baldi's mouth
	modelStack.PushMatrix();
	modelStack.Translate(1.2f, 13.f + .25f * dir, 3.4f);
	modelStack.Rotate(25.f * dir, 0, 0, 1.f);
	modelStack.Scale(.9f, .5f, .5f);
	RenderMesh(MESH::MOUTH_PART);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1.2f, 13.f + .25f * dir, 3.4f);
	modelStack.Rotate(-25.f * dir, 0, 0, 1.f);
	modelStack.Scale(.9f, .5f, .5f);
	RenderMesh(MESH::MOUTH_PART);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(.4f, 13.f, 3.4f);
	modelStack.Rotate(10.f * dir, 0, 0, 1.f);
	modelStack.Scale(.9f, .5f, .5f);
	RenderMesh(MESH::MOUTH_PART);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-0.4f, 13.f, 3.4f);
	modelStack.Rotate(-10.f * dir, 0, 0, 1.f);
	modelStack.Scale(.9f, .5f, .5f);
	RenderMesh(MESH::MOUTH_PART);
	modelStack.PopMatrix();
}

void Scene5::RenderBody(){ //Render Baldi's body
	modelStack.PushMatrix();
		modelStack.PushMatrix();
			modelStack.Translate(0, yTranslate2, 0);
			modelStack.Translate(0, 11.1f, 0.f);
			modelStack.Scale(.8f, 1.f, .8f);
			RenderMesh(MESH::NECK);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
			modelStack.Translate(0, yTranslate2, 0);
			modelStack.Scale(2.9f, 2.8f, 3.5f);
			RenderMesh(MESH::TORSO);
		modelStack.PopMatrix();
		if(animation == 1){
			modelStack.PushMatrix();
				modelStack.Translate(0.f, 0.f, 1.5f);
				modelStack.Scale(3.f, 5.f, 3.5f);
				RenderMesh(MESH::MAGNET);
			modelStack.PopMatrix();
		}
	modelStack.PopMatrix();
	modelStack.PushMatrix();
		modelStack.Translate(0, yTranslate2, 0);
		RenderLeftUpperLimb();
		RenderRightUpperLimb();
	modelStack.PopMatrix();
	RenderLeftLowerLimb();
	RenderRightLowerLimb();
}

void Scene5::RenderLeftUpperLimb(){ //Render Baldi's left arm
	modelStack.PushMatrix();
		modelStack.Translate(5.6f, 8.4f, 0.f);
		modelStack.Rotate(-angle / 2, 1.f, 0, 0);
		modelStack.Rotate((angle1 < 0 ? -angle1 : -angle1 / 1.2f), 1.f, 0, 0);
		modelStack.PushMatrix();
			modelStack.Scale(1.35f, 1.35f, 1.35f);
			RenderMesh(MESH::SHOULDER);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
			modelStack.Translate(0, -4.0f, 0.f);
			modelStack.Scale(1.3f, 8.1f, 1.3f);
			RenderMesh(MESH::ARM_PART); //Upper arm
		modelStack.PopMatrix();
		modelStack.PushMatrix();
			modelStack.Translate(0, -10.0f, 0.f);
			modelStack.PushMatrix();
				modelStack.Scale(1.3f, 6.1f, 1.3f);
				RenderMesh(MESH::ARM_PART); //Lower arm
			modelStack.PopMatrix();
			modelStack.PushMatrix();
				modelStack.Translate(.5f, -3.7f, 0);
				modelStack.PushMatrix();
					modelStack.Scale(.7f, 1.9f, 1.9f);
					RenderMesh(MESH::HAND_PART);
				modelStack.PopMatrix();
				for(int i = 0; i < 5; ++i){
					modelStack.PushMatrix();
						modelStack.Translate(-0.5f, -.85f, -0.8f + i * 0.4f);
						modelStack.Rotate(90, 0, 0, 1.f);
						modelStack.Scale(.3f, .4f, .3f);
						RenderMesh(MESH::FINGER_PART);
					modelStack.PopMatrix();
					modelStack.PushMatrix();
						modelStack.Translate(-1.4f, -0.38f, -0.8f + i * 0.4f);
						modelStack.Scale(.3f, .3f, .3f);
						RenderMesh(MESH::FINGER_PART);
						modelStack.PushMatrix();
							modelStack.Translate(0, 1.9f, 0);
							modelStack.Scale(1.f, 1.6f, 1.f);
							RenderMesh(MESH::FINGER_TIP);
						modelStack.PopMatrix();
					modelStack.PopMatrix();
				}
			modelStack.PopMatrix();
			RenderFlail();
		modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void Scene5::RenderRightUpperLimb(){ //Render Baldi's right arm
	modelStack.PushMatrix();
		modelStack.Translate(-5.6f, 8.4f, 0.f);
		modelStack.Rotate(-angle / 2, 1.f, 0, 0);
		modelStack.Rotate((angle3 < 0 ? -angle3 : -angle3 / 1.2f), 1.f, 0, 0);
		modelStack.PushMatrix();
			modelStack.Scale(1.35f, 1.35f, 1.35f);
			RenderMesh(MESH::SHOULDER);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
			modelStack.Translate(0, -4.0f, 0.f);
			modelStack.Scale(1.3f, 8.1f, 1.3f);
			RenderMesh(MESH::ARM_PART); //Upper arm
		modelStack.PopMatrix();
		modelStack.PushMatrix();
			modelStack.Translate(0, -10.0f, 0.f);
			modelStack.PushMatrix();
				modelStack.Scale(1.3f, 6.1f, 1.3f);
				RenderMesh(MESH::ARM_PART); //Lower arm
			modelStack.PopMatrix();
			modelStack.PushMatrix();
				modelStack.Translate(0, -3.7f, 0);
				modelStack.PushMatrix();
					modelStack.Scale(.7f, 1.9f, 1.85f);
					RenderMesh(MESH::HAND_PART);
				modelStack.PopMatrix();
				for(int i = 0; i < 5; ++i){
					modelStack.PushMatrix();
						modelStack.Translate(0, -1.5f, -0.8f + i * 0.4f);
						modelStack.Scale(.3f, .4f, .3f);
						RenderMesh(MESH::FINGER_PART);
						modelStack.PushMatrix();
							modelStack.Translate(.3f, -3.5f, 0);
							modelStack.Rotate(10.f, 0, 0, 1.f);
							RenderMesh(MESH::FINGER_PART);
							modelStack.PushMatrix();
								modelStack.Translate(0, -1.9f, 0);
								modelStack.Rotate(180.f, 0, 0, 1.f);
								modelStack.Scale(1.f, 1.6f, 1.f);
								RenderMesh(MESH::FINGER_TIP);
							modelStack.PopMatrix();
						modelStack.PopMatrix();
					modelStack.PopMatrix();
				}
			modelStack.PopMatrix();
		modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void Scene5::RenderLeftLowerLimb(){ //Render Baldi's left leg
	modelStack.PushMatrix();
		modelStack.Translate(2.2f, -8.6f, 0.f);
		modelStack.Rotate(-angle3, 1.f, 0, 0);
		modelStack.PushMatrix();
			modelStack.Translate(0, yTranslate2, 0);
			modelStack.Rotate(-angle, 1.f, 0, 0);
			RenderMesh(MESH::JOINT); //Upper joint
			modelStack.PushMatrix();
				modelStack.Translate(0.f, -2.3f, 0.f);
				modelStack.PushMatrix();
					modelStack.Scale(1.f, 1.9f, 1.f);
					RenderMesh(MESH::LEG_PART); //Upper leg
				modelStack.PopMatrix();
				modelStack.PushMatrix();
					modelStack.Translate(0.f, -3.6f, 0.f);
					RenderMesh(MESH::JOINT); //Lower joint
					modelStack.Rotate(angle4, 1.f, 0, 0);
					modelStack.Rotate(angle * 2, 1.f, 0, 0);
					modelStack.PushMatrix();
						modelStack.Translate(0.f, -3.6f, 0.f);
						modelStack.Scale(1.f, 1.9f, 1.f);
						RenderMesh(MESH::LEG_PART); //Lower leg
					modelStack.PopMatrix();
					if(jumpOver){
						modelStack.PushMatrix();
						modelStack.Translate(0, -7.65f, .6f);
						modelStack.Rotate(90, 1.f, 0, 0);
						modelStack.Scale(1.4f, 1.6f, .8f);
						RenderMesh(MESH::FOOT);
						modelStack.PopMatrix();
					}
				modelStack.PopMatrix();
			modelStack.PopMatrix();
		modelStack.PopMatrix();
	modelStack.PopMatrix();
	if(!jumpOver){
		modelStack.PushMatrix();
			modelStack.Translate(2.2f, -22.3f, .6f);
			modelStack.Rotate(90 + footTilt, 1.f, 0, 0);
			modelStack.Scale(1.4f, 1.6f, .8f);
			RenderMesh(MESH::FOOT);
		modelStack.PopMatrix();
	}
}

void Scene5::RenderRightLowerLimb(){ //Render Baldi's right leg
	modelStack.PushMatrix();
		modelStack.Translate(-2.2f, -8.6f, 0.f);
		modelStack.Rotate(-angle1, 1.f, 0, 0);
		modelStack.PushMatrix();
			modelStack.Translate(0, yTranslate2, 0);
			modelStack.Rotate(-angle, 1.f, 0, 0);
			RenderMesh(MESH::JOINT); //Upper joint
			modelStack.PushMatrix();
				modelStack.Translate(0.f, -2.3f, 0.f);
				modelStack.PushMatrix();
					modelStack.Scale(1.f, 1.9f, 1.f);
					RenderMesh(MESH::LEG_PART); //Upper leg
				modelStack.PopMatrix();
				modelStack.PushMatrix();
					modelStack.Translate(0.f, -3.6f, 0.f);
					RenderMesh(MESH::JOINT); //Lower joint
					modelStack.Rotate(angle2, 1.f, 0, 0);
					modelStack.Rotate(angle * 2, 1.f, 0, 0);
					modelStack.PushMatrix();
						modelStack.Translate(0.f, -3.6f, 0.f);
						modelStack.Scale(1.f, 1.9f, 1.f);
						RenderMesh(MESH::LEG_PART); //Lower leg
					modelStack.PopMatrix();
					if(jumpOver){
						modelStack.PushMatrix();
						modelStack.Translate(0, -7.65f, .6f);
						modelStack.Rotate(90, 1.f, 0, 0);
						modelStack.Scale(1.4f, 1.6f, .8f);
						RenderMesh(MESH::FOOT);
						modelStack.PopMatrix();
					}
				modelStack.PopMatrix();
			modelStack.PopMatrix();
		modelStack.PopMatrix();
	modelStack.PopMatrix();
	if(!jumpOver){
		modelStack.PushMatrix();
			modelStack.Translate(-2.2f, -22.3f, .6f);
			modelStack.Rotate(90 + footTilt, 1.f, 0, 0);
			modelStack.Scale(1.4f, 1.6f, .8f);
			RenderMesh(MESH::FOOT);
		modelStack.PopMatrix();
	}
}

void Scene5::RenderFlail(){ //Render flail that Baldi is holding
	modelStack.PushMatrix();
		modelStack.Translate(-0.3f, -4.0f, 0.7f);
		modelStack.PushMatrix();
			modelStack.Rotate(90, 1.f, 0, 0);
			modelStack.Scale(.2f, .9f, .2f);
			RenderMesh(MESH::HANDLE);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
			modelStack.Translate(0, 0, 0.7f);
			modelStack.Rotate(angle / 2, 1.f, 0, 0);
			modelStack.Rotate((angle1 < 0 ? -angle1 : -angle1 / 2.5f) * 1.05f, 1.f, 0, 0);
			for(int i = 0; i < 4; ++i){
				modelStack.PushMatrix();
					modelStack.Translate(0, flailTranslate * i / 4, 0);
					modelStack.Translate(0, -0.6f - 1.8f * i, 1.2f);
					modelStack.Rotate(90, 1.f, 0.f, 0.f);
					modelStack.Scale(.45f, .65f, .6f);
					RenderMesh(MESH::CHAIN_PART);
				modelStack.PopMatrix();
				modelStack.PushMatrix();
					modelStack.Translate(0, flailTranslate * i / 4, 0);
					modelStack.Translate(0, -1.5f - 1.8f * i, 1.2f);
					modelStack.Rotate(90, 1.f, 0.f, 0.f);
					modelStack.Rotate(90, 0.f, 0.f, 1.f);
					modelStack.Scale(.45f, .65f, .6f);
					RenderMesh(MESH::CHAIN_PART);
				modelStack.PopMatrix();
			}
			modelStack.PushMatrix();
				modelStack.Translate(0, flailTranslate, 0);
				modelStack.Translate(0, -9.2f, 1.2f);
				modelStack.PushMatrix();
					modelStack.Scale(2.f, 2.f, 2.f);
					RenderMesh(MESH::STRIKING_HEAD);
				modelStack.PopMatrix();
				for(int i = 0; i < 13; ++i){
					modelStack.PushMatrix();
						modelStack.Rotate(a2[i], 0, 0, 1.f);
						modelStack.Rotate(a1[i], 1.f, 0, 0);
						modelStack.Translate(dist * cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta)),
							dist * sin(Math::DegreeToRadian(phi)), dist * cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta)));
						modelStack.Rotate(phi, 0, 1.f, 0);
						modelStack.Rotate(theta, 1.f, 0, 0);
						modelStack.Scale(.3f, .3f, .3f);
						modelStack.Translate(0, 1.f, 0);
						RenderMesh(MESH::SPIKE);
					modelStack.PopMatrix();
				}
			modelStack.PopMatrix();
		modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void Scene5::Render(Camera2 &camera){ //Render scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();
	if(light[0].type == Light::LIGHT_TYPE::LIGHT_DIRECTIONAL){
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_POSITION)], 1, &lightDirection_cameraspace.x); 
	} else if(light[0].type == Light::LIGHT_TYPE::LIGHT_SPOT){ 
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_POSITION)], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_SPOTDIRECTION)], 1, &spotDirection_cameraspace.x); 
	} else{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[unsigned int(UNIFORM_TYPE::U_LIGHT0_POSITION)], 1, &lightPosition_cameraspace.x); 
	}
	modelStack.PushMatrix();
		light[0].position.x = initialLightPos.x;
		light[0].position.y = initialLightPos.y + yTranslate;
		light[0].position.z = initialLightPos.z + zTranslate;
		modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
		if(showLightBall){
			RenderMesh(MESH::LIGHT_SPHERE, 0);
		}
	modelStack.PopMatrix();
	modelStack.PushMatrix();
		modelStack.Translate(0.f, -23.f, 0.f);
		modelStack.PushMatrix();
			modelStack.Rotate(-90, 1.f, 0.f, 0.f);
			RenderMesh(MESH::GROUND);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
			modelStack.Translate(0.f, .1f, 0.f);
			RenderMesh(MESH::GROUND_CIRCLE);
			modelStack.PushMatrix();
				modelStack.Translate(0.f, .1f, 0.f);
				RenderMesh(MESH::GROUND_RING);
			modelStack.PopMatrix();
		modelStack.PopMatrix();
	modelStack.PopMatrix();
	RenderMesh(MESH::AXES, 0);
	RenderBaldi();
	if(animation == 1){
		modelStack.PushMatrix();
			modelStack.Translate(0.f, -12.f, 30.f);
			modelStack.Scale(15.f, 40.f, 10.f);
			RenderMesh(MESH::MAGNET);
		modelStack.PopMatrix();
	}
}