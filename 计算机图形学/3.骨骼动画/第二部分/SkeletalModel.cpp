#include "SkeletalModel.h"

#include <FL/Fl.H>
#include <cmath>

using namespace std;

void SkeletalModel::load(const char *skeletonFile, const char *meshFile, const char *attachmentsFile)
{
	loadSkeleton(skeletonFile);

	m_mesh.load(meshFile);
	m_mesh.loadAttachments(attachmentsFile, m_joints.size());

	computeBindWorldToJointTransforms();
	updateCurrentJointToWorldTransforms();
}

void SkeletalModel::draw(Matrix4f cameraMatrix, bool skeletonVisible)
{
	// draw() gets called whenever a redraw is required
	// (after an update() occurs, when the camera moves, the window is resized, etc)

	m_matrixStack.clear();
	m_matrixStack.push(cameraMatrix);

	drawJoints();

	drawSkeleton();

	// Clear out any weird matrix we may have been using for drawing the bones and revert to the camera matrix.
	glLoadMatrixf(m_matrixStack.top());
	// Tell the mesh to draw itself.
	m_mesh.draw();
}

void SkeletalModel::loadSkeleton(const char* filename)
{
	// Load the skeleton from file here.
	ifstream infile;
	infile.open(filename);
	int father;
	float locate[3];
	if (infile.is_open())
	{
		while (!infile.eof()) {
			infile >> locate[0] >> locate[1] >> locate[2] >> father;
			Joint* joint = new Joint;
			joint->transform = Matrix4f(1, 0, 0, locate[0], 0, 1, 0, locate[1], 0, 0, 1, locate[2], 0, 0, 0, 1);
			joint->children = std::vector<Joint*>();
			this->m_joints.push_back(joint);
			if (father >= 0) {
				Joint* f = this->m_joints[father];
				f->children.push_back(joint);
			}
		}
		infile.close();
		this->m_rootJoint = this->m_joints[0];
	}
}

void SkeletalModel::drawJoints()
{
	// Draw a sphere at each joint. You will need to add a recursive helper function to traverse the joint hierarchy.
	//
	// We recommend using glutSolidSphere( 0.025f, 12, 12 )
	// to draw a sphere of reasonable size.
	//
	// You are *not* permitted to use the OpenGL matrix stack commands
	// (glPushMatrix, glPopMatrix, glMultMatrix).
	// You should use your MatrixStack class
	// and use glLoadMatrix() before your drawing call.
	this->drawOneJoint(this->m_rootJoint);
}

void SkeletalModel::drawOneJoint(Joint* joint) {
	this->m_matrixStack.push(joint->transform);
	glLoadMatrixf(this->m_matrixStack.top());
	glutSolidSphere(0.025f, 12, 12);
	for (int i = 0; i < joint->children.size(); i++) {
		this->drawOneJoint(joint->children[i]);
	}
	this->m_matrixStack.pop();
}

void SkeletalModel::drawSkeleton()
{
	// Draw boxes between the joints. You will need to add a recursive helper function to traverse the joint hierarchy.
	this->drawBouns(this->m_rootJoint);
}

void SkeletalModel::drawBouns(Joint* joint) {

	this->m_matrixStack.push(joint->transform);

	for (int i = 0; i < joint->children.size(); i++) {
		glLoadMatrixf(this->m_matrixStack.top());
		this->drawOneBouns(joint->children[i]);
	}

	for (int i = 0; i < joint->children.size(); i++) {
		if (joint->children[i]->children.size() > 0) {
			this->drawBouns(joint->children[i]);
		}
	}

	this->m_matrixStack.pop();
}

void SkeletalModel::drawOneBouns(Joint* son) {
	GLfloat x, y, z, l1, l2, l3;
	x = son->transform[12] / 2;
	y = son->transform[13] / 2;
	z = son->transform[14] / 2;
	glTranslatef(x, y, z);
	GLfloat length = sqrt(pow(x + x, 2) + pow(y + y, 2) + pow(z + z, 2));
	glRotatef(atan(z / y) * 180 / M_PI, 1, 0, 0);
	glRotatef(atan(y / x) * 180 / M_PI, 0, 0, 1);
	glScalef(length / 0.025f, 1.0f, 1.0f);
	glutSolidCube(0.025f);
}

void SkeletalModel::setJointTransform(int jointIndex, float rX, float rY, float rZ)
{
	// Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
	rX *= 10; rY *= 10; rZ *= 10;
	Joint* joint = this->m_joints[jointIndex];
	if (rX == joint->rX&&rY == joint->rY&&rZ == joint->rZ) {
		return;
	}
	if (joint->rX > rX) {
		rX -= joint->rX;
		joint->rX += rX;
	}
	else {
		rX -= joint->rX;
		joint->rX += rX;
	}
	if (joint->rY > rY) {
		rY -= joint->rY;
		joint->rY += rY;
	}
	else {
		rY -= joint->rY;
		joint->rY += rY;
	}
	if (joint->rZ > rZ) {
		rZ -= joint->rZ;
		joint->rZ += rZ;
	}
	else {
		rZ -= joint->rZ;
		joint->rZ += rZ;
	}
	glLoadMatrixf(joint->transform);
	glRotatef(rX, 1.0f, 0.0f, 0.0f);
	glRotatef(rY, 0.0f, 1.0f, 0.0f);
	glRotatef(rZ, 0.0f, 0.0f, 1.0f);
	GLfloat* m = new GLfloat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	for (int i = 0; i < 16; i++) {
		joint->transform[i] = m[i];
	}
	delete[] m;
	cout << jointIndex << endl;
	joint->transform.print();
}


void SkeletalModel::computeBindWorldToJointTransforms()
{
	// 2.3.1. Implement this method to compute a per-joint transform from
	// world-space to joint space in the BIND POSE.
	//
	// Note that this needs to be computed only once since there is only
	// a single bind pose.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
	MatrixStack ms;
	this->computeOneJoint(this->m_rootJoint, ms);
}

void SkeletalModel::computeOneJoint(Joint* joint, MatrixStack& ms) {
	ms.push(joint->transform);
	joint->bindWorldToJointTransform = ms.top().inverse();
	for (int i = 0; i < joint->children.size(); i++) {
		this->computeOneJoint(joint->children[i], ms);
	}
	ms.pop();
}

void SkeletalModel::updateOneJoint(Joint* joint, MatrixStack& ms) {
	ms.push(joint->transform);
	joint->currentJointToWorldTransform = ms.top();
	for (int i = 0; i < joint->children.size(); i++) {
		this->updateOneJoint(joint->children[i], ms);
	}
	ms.pop();
}

void SkeletalModel::updateCurrentJointToWorldTransforms()
{
	// 2.3.2. Implement this method to compute a per-joint transform from
	// joint space to world space in the CURRENT POSE.
	//
	// The current pose is defined by the rotations you've applied to the
	// joints and hence needs to be *updated* every time the joint angles change.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
	MatrixStack ms;
	updateOneJoint(this->m_rootJoint, ms);
}

void addRight(Matrix4f& m, float right) {
	for (int i = 0; i < 16; ++i) {
		m[i] *= right;
	}
}

void addMatrix(Matrix4f& m1, const Matrix4f& m2) {
	for (int i = 0; i < 16; i++) {
		m1[i] += m2[i];
	}
}

Matrix4f computeLocation(Matrix4f& m1, Matrix4f& m2, Vector3f& m3) {
	Matrix4f m(0, 0, 0, m3[0], 0, 0, 0, m3[1], 0, 0, 0, m3[2], 0, 0, 0, 1);
	return m1 * m2 * m;
}

void SkeletalModel::updateMesh()
{
	// 2.3.2. This is the core of SSD.
	// Implement this method to update the vertices of the mesh
	// given the current state of the skeleton.
	// You will need both the bind pose world --> joint transforms.
	// and the current joint --> world transforms.
	for (int i = 0; i < this->m_mesh.bindVertices.size(); i++) {
		vector<Matrix4f>vec;
		Matrix4f res;
		for (int j = 0; j < this->m_mesh.attachments[i].size(); j++) {
			Matrix4f temp(computeLocation(this->m_joints[j]->currentJointToWorldTransform, this->m_joints[j]->bindWorldToJointTransform, this->m_mesh.bindVertices[i]));
			addRight(temp, this->m_mesh.attachments[i][j]);
			vec.push_back(temp);
		}
		for (int j = 0; j < vec.size(); j++) {
			addMatrix(res, vec[j]);
		}
		this->m_mesh.currentVertices[i] = Vector3f(res[12], res[13], res[14]);
	}
}

