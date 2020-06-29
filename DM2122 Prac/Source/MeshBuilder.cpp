#include "MeshBuilder.h"

Mesh *MeshBuilder::CreateMesh(Mesh::DRAW_MODE drawMode, std::vector<Vertex> vertex_buffer_data, std::vector<GLuint> index_buffer_data){ //For generating mesh
	Mesh *mesh = new Mesh;
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = drawMode;
	return mesh; //Stores VBO and IBO
}

Mesh *MeshBuilder::GenerateAxes(float lengthX, float lengthY, float lengthZ){ //Axes mesh generator, use red for x-axis, green for... then gen VBO/IBO and store them in Mesh obj, x-axis should start at -lengthX / 2 and end at lengthX / 2, y-axis...
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	v.color.Set(1, 0, 0);
	v.pos.Set(-lengthX * 0.5f, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX * 0.5f, 0, 0);
	vertex_buffer_data.push_back(v);

	v.color.Set(0, 1, 0);
	v.pos.Set(0, -lengthY * 0.5f, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, lengthY * 0.5f, 0);
	vertex_buffer_data.push_back(v);

	v.color.Set(0, 0, 1);
	v.pos.Set(0, 0, -lengthZ * 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, lengthZ * 0.5f);
	vertex_buffer_data.push_back(v);

	for(int i = 0; i < 6; ++i){
		index_buffer_data.push_back(i);
	}
	return CreateMesh(Mesh::DRAW_MODE::DRAW_LINES, vertex_buffer_data, index_buffer_data);
}

Mesh *MeshBuilder::GenerateQuad(Color c, float lengthX, float lengthY){ //Quad mesh generator
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.color.Set(c.r, c.g, c.b);
	v.normal.Set(0, 0, 1.0f);

	float midX = lengthX * 0.5f, midY = lengthY * 0.5f;
	v.pos.Set(-midX, midY, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-midX, -midY, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(midX, midY, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(midX, -midY, 0);
	vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	return CreateMesh(Mesh::DRAW_MODE::DRAW_TRIANGLES, vertex_buffer_data, index_buffer_data);
}

Mesh *MeshBuilder::GenerateCuboid(Color c, float lengthX, float lengthY, float lengthZ){ //Cuboid mesh generator (uses 24 vertices)
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.color.Set(c.r, c.g, c.b);
	float midX = lengthX * 0.5f, midY = lengthY * 0.5f, midZ = lengthZ * 0.5f;

	//0 - 7
	v.pos.Set(-midX, -midY, -midZ);
	v.normal = Vector3(-1.f, 0.f, 0.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(midX, -midY, -midZ);
	v.normal = Vector3(1.f, 0.f, 0.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(midX, midY, -midZ);
	v.normal = Vector3(1.f, 0.f, 0.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-midX, midY, -midZ);
	v.normal = Vector3(-1.f, 0.f, 0.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-midX, -midY, midZ);
	v.normal = Vector3(-1.f, 0.f, 0.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(midX, -midY, midZ);
	v.normal = Vector3(1.f, 0.f, 0.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(midX, midY, midZ);
	v.normal = Vector3(1.f, 0.f, 0.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-midX, midY, midZ);
	v.normal = Vector3(-1.f, 0.f, 0.f);
	vertex_buffer_data.push_back(v);

	//8 - 15
	v.pos.Set(-midX, -midY, -midZ);
	v.normal = Vector3(0.f, -1.f, 0.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(midX, -midY, -midZ);
	v.normal = Vector3(0.f, -1.f, 0.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(midX, midY, -midZ);
	v.normal = Vector3(0.f, 1.f, 0.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-midX, midY, -midZ);
	v.normal = Vector3(0.f, 1.f, 0.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-midX, -midY, midZ);
	v.normal = Vector3(0.f, -1.f, 0.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(midX, -midY, midZ);
	v.normal = Vector3(0.f, -1.f, 0.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(midX, midY, midZ);
	v.normal = Vector3(0.f, 1.f, 0.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-midX, midY, midZ);
	v.normal = Vector3(0.f, 1.f, 0.f);
	vertex_buffer_data.push_back(v);

	//16 - 23
	v.pos.Set(-midX, -midY, -midZ);
	v.normal = Vector3(0.f, 0.f, -1.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(midX, -midY, -midZ);
	v.normal = Vector3(0.f, 0.f, -1.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(midX, midY, -midZ);
	v.normal = Vector3(0.f, 0.f, -1.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-midX, midY, -midZ);
	v.normal = Vector3(0.f, 0.f, -1.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-midX, -midY, midZ);
	v.normal = Vector3(0.f, 0.f, 1.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(midX, -midY, midZ);
	v.normal = Vector3(0.f, 0.f, 1.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(midX, midY, midZ);
	v.normal = Vector3(0.f, 0.f, 1.f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-midX, midY, midZ);
	v.normal = Vector3(0.f, 0.f, 1.f);
	vertex_buffer_data.push_back(v);

	//Front
	index_buffer_data.push_back(23);
	index_buffer_data.push_back(20);
	index_buffer_data.push_back(22);
	index_buffer_data.push_back(21);
	index_buffer_data.push_back(22);
	index_buffer_data.push_back(20);

	//Right
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(5);

	//Top
	index_buffer_data.push_back(11);
	index_buffer_data.push_back(15);
	index_buffer_data.push_back(10);
	index_buffer_data.push_back(14);
	index_buffer_data.push_back(10);
	index_buffer_data.push_back(15);

	//Back
	index_buffer_data.push_back(18);
	index_buffer_data.push_back(17);
	index_buffer_data.push_back(19);
	index_buffer_data.push_back(16);
	index_buffer_data.push_back(19);
	index_buffer_data.push_back(17);

	//Left
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(0);

	//Bottom
	index_buffer_data.push_back(12);
	index_buffer_data.push_back(8);
	index_buffer_data.push_back(13);
	index_buffer_data.push_back(9);
	index_buffer_data.push_back(13);
	index_buffer_data.push_back(8);
	return CreateMesh(Mesh::DRAW_MODE::DRAW_TRIANGLES, vertex_buffer_data, index_buffer_data);
}

Mesh *MeshBuilder::GenerateSphere(Color c, unsigned numStack, unsigned numSlice, float r){ //Sphere mesh generator
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.color.Set(c.r, c.g, c.b);
	float degreePerStack = 180.f / numStack, degreePerSlice = 360.f / numSlice;
	for(unsigned stack = 0; stack < numStack + 1; ++stack){
		float phi = -90 + stack * degreePerStack;
		for(unsigned slice = 0; slice < numSlice + 1; ++slice){
			float theta = slice * degreePerSlice;
			v.pos.Set(r * cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta)),
				r * sin(Math::DegreeToRadian(phi)), r * cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta)));
			Vector3 normal = Vector3(v.pos.x, v.pos.y, v.pos.z).Normalized();
			v.normal.Set(normal.x, normal.y, normal.z);
			vertex_buffer_data.push_back(v);
			index_buffer_data.push_back(stack * (numSlice + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlice + 1) + slice);
		}
	}
	return CreateMesh(Mesh::DRAW_MODE::DRAW_TRIANGLE_STRIP, vertex_buffer_data, index_buffer_data);
}

Mesh *MeshBuilder::GenerateHemisphere(Color c, unsigned numStack, unsigned numSlice, float r){ //Hemisphere mesh generator
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.color.Set(c.r, c.g, c.b);
	v.normal.Set(0, -1.0f, 0);
	float degreePerStack = 90.f / numStack, degreePerSlice = 360.f / numSlice;
	for(unsigned slice = 0; slice < numSlice + 1; ++slice){
		float theta = slice * degreePerSlice;
		v.pos.Set(r * cos(Math::DegreeToRadian(theta)), 0, r * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
		v.pos.Set(0, 0, 0);
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(slice * 2);
		index_buffer_data.push_back(slice * 2 + 1);
	}
	for(unsigned stack = 0; stack < numStack + 1; ++stack){
		float phi = stack * degreePerStack;
		for(unsigned slice = 0; slice < numSlice + 1; ++slice){
			float theta = slice * degreePerSlice;
			v.pos.Set(r * cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta)),
				r * sin(Math::DegreeToRadian(phi)), r * cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta)));
			Vector3 normal = Vector3(v.pos.x, v.pos.y, v.pos.z).Normalized();
			v.normal.Set(normal.x, normal.y, normal.z);
			vertex_buffer_data.push_back(v);
			index_buffer_data.push_back(stack * (numSlice + 1) + slice + numSlice * 2 + 2);
			index_buffer_data.push_back((stack + 1) * (numSlice + 1) + slice + numSlice * 2 + 2);
		}
	}
	return CreateMesh(Mesh::DRAW_MODE::DRAW_TRIANGLE_STRIP, vertex_buffer_data, index_buffer_data);
}

Mesh *MeshBuilder::GenerateRing(Color c, unsigned numSlice, float outerR, float innerR){ //Ring mesh generator
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.color.Set(c.r, c.g, c.b);
	v.normal.Set(0, 1.0f, 0);
	float degreePerSlice = -360.f / numSlice;
	for(unsigned slice = 0; slice < numSlice + 1; ++slice){
		float theta = slice * degreePerSlice;
		v.pos.Set(innerR * cos(Math::DegreeToRadian(theta)), 0, innerR * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
		v.pos.Set(outerR * cos(Math::DegreeToRadian(theta)), 0, outerR * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(slice * 2);
		index_buffer_data.push_back(slice * 2 + 1);
	}
	return CreateMesh(Mesh::DRAW_MODE::DRAW_TRIANGLE_STRIP, vertex_buffer_data, index_buffer_data);
}

Mesh *MeshBuilder::GenerateCircle(Color c, unsigned numSlice, float r){ //Circle mesh generator
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.color.Set(c.r, c.g, c.b);
	v.normal.Set(0, 1.0f, 0);
	float degreePerSlice = -360.f / numSlice;
	for(unsigned slice = 0; slice < numSlice + 1; ++slice){
		float theta = slice * degreePerSlice;
		v.pos.Set(0, 0, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(r * cos(Math::DegreeToRadian(theta)), 0, r * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(slice * 2);
		index_buffer_data.push_back(slice * 2 + 1);
	}
	return CreateMesh(Mesh::DRAW_MODE::DRAW_TRIANGLE_STRIP, vertex_buffer_data, index_buffer_data);
}

Mesh *MeshBuilder::GenerateCylinder(Color c, unsigned numSlice, float r, float h){ //Cylinder mesh generator
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.color.Set(c.r, c.g, c.b);
	v.normal.Set(0, 1.0f, 0);
	float degreePerSlice = -360.f / numSlice;
	for(unsigned slice = 0; slice < numSlice + 1; ++slice){ //top
		float theta = slice * degreePerSlice;
		v.pos.Set(0, h / 2, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(r * cos(Math::DegreeToRadian(theta)), h / 2, r * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(slice * 2);
		index_buffer_data.push_back(slice * 2 + 1);
	}
	for(unsigned slice = 0; slice < numSlice + 1; ++slice){
		float theta = slice * degreePerSlice;
		v.pos.Set(r * cos(Math::DegreeToRadian(theta)), h / 2, r * sin(Math::DegreeToRadian(theta)));
		Vector3 normal = Vector3(v.pos.x, 0, v.pos.z).Normalized();
		v.normal.Set(normal.x, normal.y, normal.z);
		vertex_buffer_data.push_back(v);
		v.pos.Set(r * cos(Math::DegreeToRadian(theta)), -h / 2, r * sin(Math::DegreeToRadian(theta)));
		normal = Vector3(v.pos.x, 0, v.pos.z).Normalized();
		v.normal.Set(normal.x, normal.y, normal.z);
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(slice * 2 + numSlice * 2 + 2);
		index_buffer_data.push_back(slice * 2 + numSlice * 2 + 2 + 1);
	}
	v.normal.Set(0, -1.0f, 0);
	for(unsigned slice = 0; slice < numSlice + 1; ++slice){ //bottom
		float theta = slice * degreePerSlice;
		v.pos.Set(0, -h / 2, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(r * cos(Math::DegreeToRadian(theta)), -h / 2, r * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(slice * 2 + numSlice * 2 + numSlice * 2 + 2 + 2);
		index_buffer_data.push_back(slice * 2 + numSlice * 2 + numSlice * 2 + 2 + 2 + 1);
	}
	return CreateMesh(Mesh::DRAW_MODE::DRAW_TRIANGLE_STRIP, vertex_buffer_data, index_buffer_data);
}

Mesh *MeshBuilder::GenerateCone(Color c, unsigned numSlice, float r, float h){ //Cone mesh generator
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.color.Set(c.r, c.g, c.b);
	float degreePerSlice = -360.f / numSlice;
	for(unsigned slice = 0; slice < numSlice + 1; ++slice){
		float theta = slice * degreePerSlice;
		v.pos.Set(0, h / 2, 0);
		v.normal.Set(0, 1.0f, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(r * cos(Math::DegreeToRadian(theta)), -h / 2, r * sin(Math::DegreeToRadian(theta)));
		Vector3 normal = Vector3(v.pos.x, 0, v.pos.z).Normalized();
		v.normal.Set(normal.x, normal.y, normal.z);
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(slice * 2);
		index_buffer_data.push_back(slice * 2 + 1);
	}
	v.normal.Set(0, -1.0f, 0);
	for(unsigned slice = 0; slice < numSlice + 1; ++slice){
		float theta = slice * degreePerSlice;
		v.pos.Set(0, -h / 2, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(r * cos(Math::DegreeToRadian(theta)), -h / 2, r * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(slice * 2 + numSlice * 2 + 2);
		index_buffer_data.push_back(slice * 2 + numSlice * 2 + 2 + 1);
	}
	return CreateMesh(Mesh::DRAW_MODE::DRAW_TRIANGLE_STRIP, vertex_buffer_data, index_buffer_data);
}

Mesh *MeshBuilder::GenerateConicalFrustum(Color c, unsigned numSlice, float outerR, float innerR, float h){ //Conical frustum mesh generator
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.color.Set(c.r, c.g, c.b);
	v.normal.Set(0, 1.0f, 0);
	float degreePerSlice = -360.f / numSlice;
	for(unsigned slice = 0; slice < numSlice + 1; ++slice){ //top
		float theta = slice * degreePerSlice;
		v.pos.Set(0, h / 2, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(innerR * cos(Math::DegreeToRadian(theta)), h / 2, innerR * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(slice * 2);
		index_buffer_data.push_back(slice * 2 + 1);
	}
	for(unsigned slice = 0; slice < numSlice + 1; ++slice){
		float theta = slice * degreePerSlice;
		v.pos.Set(innerR * cos(Math::DegreeToRadian(theta)), h / 2, innerR * sin(Math::DegreeToRadian(theta)));
		Vector3 normal = Vector3(h * v.pos.x, innerR, h * v.pos.z).Normalized();
		v.normal.Set(normal.x, normal.y, normal.z);
		vertex_buffer_data.push_back(v);
		v.pos.Set(outerR * cos(Math::DegreeToRadian(theta)), -h / 2, outerR * sin(Math::DegreeToRadian(theta)));
		normal = Vector3(h * v.pos.x, outerR, h * v.pos.z).Normalized();
		v.normal.Set(normal.x, normal.y, normal.z);
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(slice * 2 + numSlice * 2 + 2);
		index_buffer_data.push_back(slice * 2 + numSlice * 2 + 2 + 1);
	}
	v.normal.Set(0, -1.0f, 0);
	for(unsigned slice = 0; slice < numSlice + 1; ++slice){ //bottom
		float theta = slice * degreePerSlice;
		v.pos.Set(0, -h / 2, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(outerR * cos(Math::DegreeToRadian(theta)), -h / 2, outerR * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(slice * 2 + numSlice * 2 + numSlice * 2 + 2 + 2);
		index_buffer_data.push_back(slice * 2 + numSlice * 2 + numSlice * 2 + 2 + 2 + 1);
	}
	return CreateMesh(Mesh::DRAW_MODE::DRAW_TRIANGLE_STRIP, vertex_buffer_data, index_buffer_data);
}

Mesh *MeshBuilder::GenerateTorus(Color c, unsigned numStack, unsigned numSlice, float outerR, float innerR){ //Torus mesh generator
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.color.Set(c.r, c.g, c.b);
	float degreePerStack = 360.f / numStack, degreePerSlice = 360.f / numSlice;
	float x1, z1, x2, y2, z2; //y1 is always 0
	for(unsigned stack = 0; stack < numStack + 1; ++stack){
		for(unsigned slice = 0; slice < numSlice + 1; ++slice){
			z1 = outerR * cos(Math::DegreeToRadian(stack * degreePerStack));
			x1 = outerR * sin(Math::DegreeToRadian(stack * degreePerStack));
			x2 = (outerR + innerR * cos(Math::DegreeToRadian(slice * degreePerSlice))) * sin(Math::DegreeToRadian(stack * degreePerStack));
			y2 = innerR * sin(Math::DegreeToRadian(slice * degreePerSlice));
			z2 = (outerR + innerR * cos(Math::DegreeToRadian(slice * degreePerSlice))) * cos(Math::DegreeToRadian(stack * degreePerStack));
			v.pos.Set(x2, y2, z2);
			Vector3 normal = Vector3(x2 - x1, y2, z2 - z1).Normalized();
			v.normal.Set(normal.x, normal.y, normal.z);
			vertex_buffer_data.push_back(v);
		}
	}
	for(unsigned stack = 0; stack < numStack; ++stack){
		for(unsigned slice = 0; slice < numSlice + 1; ++slice){
			index_buffer_data.push_back(stack * (numSlice + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlice + 1) + slice);
		}
	}
	return CreateMesh(Mesh::DRAW_MODE::DRAW_TRIANGLE_STRIP, vertex_buffer_data, index_buffer_data);
}