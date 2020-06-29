#pragma once
#include "Vertex.h"
#include "Material.h"

class Mesh{ //Class Mesh: To store VBO (Vertex and Color Buffer Obj) and IBO (Index Buffer Obj)
	public:
		enum class DRAW_MODE{
			DRAW_TRIANGLES, DRAW_TRIANGLE_STRIP, DRAW_LINES, DRAW_FAN, DRAW_MODE_LAST
		};
		Mesh();
		~Mesh();
		void Render();
		DRAW_MODE mode;
		unsigned vertexBuffer, colorBuffer, indexBuffer;
		Material material;
};