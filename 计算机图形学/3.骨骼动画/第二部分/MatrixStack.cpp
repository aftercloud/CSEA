#include "MatrixStack.h"

MatrixStack::MatrixStack()
{
	// Initialize the matrix stack with the identity matrix.
	this->m_matrices = std::vector<Matrix4f>();
}

void MatrixStack::clear()
{
	// Revert to just containing the identity matrix.
	this->m_matrices.clear();
}

Matrix4f MatrixStack::top()
{
	// Return the top of the stack
	return this->m_matrices[this->m_matrices.size()-1];
}

void MatrixStack::push( const Matrix4f& m )
{
	// Push m onto the stack.
	// Your stack should have OpenGL semantics:
	// the new top should be the old top multiplied by m
	if (this->m_matrices.size() == 0) {
		this->m_matrices.push_back(m);
	}
	else {
		Matrix4f n = this->top() * m;
		this->m_matrices.push_back(n);
	}
}

void MatrixStack::pop()
{
	// Remove the top element from the stack
	this->m_matrices.pop_back();
}
