#include "Mesh.h"
#include "GL\glew.h"

Mesh::Mesh(): mode(DRAW_MODE::DRAW_TRIANGLES){ //Generate buffer objs
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &colorBuffer);
	glGenBuffers(1, &indexBuffer);
}

Mesh::~Mesh(){ //Delete buffer objs
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteBuffers(1, &indexBuffer);
}

void Mesh::Render(){
	glEnableVertexAttribArray(0); //1st attribute buffer: vertices
	glEnableVertexAttribArray(1); //2nd attribute buffer: colors
	glEnableVertexAttribArray(2); //3rd attribute buffer: normals
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	switch(mode){
		case DRAW_MODE::DRAW_TRIANGLE_STRIP: glDrawElements(GL_TRIANGLE_STRIP, size/sizeof(GLuint), GL_UNSIGNED_INT, 0); break;
		case DRAW_MODE::DRAW_LINES: glDrawElements(GL_LINES, size / sizeof(GLuint), GL_UNSIGNED_INT, 0); break;
		case DRAW_MODE::DRAW_FAN: glDrawElements(GL_TRIANGLE_FAN, size / sizeof(GLuint), GL_UNSIGNED_INT, 0);
		default: glDrawElements(GL_TRIANGLES, size / sizeof(GLuint), GL_UNSIGNED_INT, 0);
	}
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}