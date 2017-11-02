

#include "OpenGL_Includes.h"

#include "glm/gtc/type_ptr.hpp"

#include "Mesh.h"

Mesh::Mesh() { }

Mesh::~Mesh() {
	if (meshUploaded) {
		GLint curp;
		glGetIntegerv(GL_CURRENT_PROGRAM, &curp);

		// Delete.
		glUseProgram(program);
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);

		// Reset.
		glUseProgram(curp);
	}
}
