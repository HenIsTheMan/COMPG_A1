#pragma once
#include <GL\glew.h>
#include <vector>
#include "Mesh.h"

class MeshBuilder{ //Class MeshBuilder: Provides methods to generate meshes of diff shapes
	public:
		static Mesh *CreateMesh(Mesh::DRAW_MODE, std::vector<Vertex>, std::vector<GLuint>);
		static Mesh *GenerateAxes(float, float, float);
		static Mesh *GenerateQuad(Color, float, float);
		static Mesh *GenerateCuboid(Color, float, float, float);
		static Mesh *GenerateSphere(Color, unsigned, unsigned, float);
		static Mesh *GenerateHemisphere(Color, unsigned, unsigned, float);
		static Mesh *GenerateRing(Color, unsigned, float, float);
		static Mesh *GenerateCircle(Color, unsigned, float);
		static Mesh *GenerateCylinder(Color, unsigned, float, float);
		static Mesh *GenerateCone(Color, unsigned, float, float);
		static Mesh *GenerateConicalFrustum(Color, unsigned, float, float, float);
		static Mesh *GenerateTorus(Color, unsigned, unsigned, float, float);
};