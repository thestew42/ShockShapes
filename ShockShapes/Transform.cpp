/** @file Transform.cpp
 * 
 * @brief Defines class used for 3D transforms
 * 
 * Copyright 2012-2013, Stewart Hall
 *
 * Licensed under The MIT License
 * Redistributions of files must retain the above copyright notice.
 *
 * @author		Stewart Hall (www.stewartghall.com)
 * @date		12/31/2012
 * @copyright	Copyright 2012-2013, Stewart Hall
 * @license		MIT License (http://www.opensource.org/licenses/mit-license.php)
 */

#include "Transform.h"

//Constructor
Transform::Transform()
{
	xt = yt = zt = 0.0f;
	xs = ys = zs = 1.0f;
	xr = yr = zr = 0.0f;
}

//Copy constructor
Transform::Transform(Transform &c)
{
	xt = c.xt; yt = c.yt; zt = c.zt;
	xs = c.xs; ys = c.ys; zs = c.zs;
	xr = c.xr; yr = c.yr; zr = c.zr;
}

//Destructor
Transform::~Transform()
{

}

//Modifiers for each type of transform
void Transform::translate(float x, float y, float z)
{
	xt += x;
	yt += y;
	zt += z;
}

void Transform::scale(float x, float y, float z)
{
	xs *= x;
	ys *= y;
	zs *= z;
}

void Transform::rotate(float x, float y, float z)
{
	xr += x;
	yr += y;
	zr += z;
}

//Setters for each type of transform
void Transform::setTranslation(float x, float y, float z)
{
	xt = x;
	yt = y;
	zt = z;
}

void Transform::setScale(float x, float y, float z)
{
	xs = x;
	ys = y;
	zs = z;
}

void Transform::setRotation(float x, float y, float z)
{
	xr = x;
	yr = y;
	zr = z;
}

//Reset the transform
void Transform::reset()
{
	xt = yt = zt = 0.0f;
	xs = ys = zs = 1.0f;
	xr = yr = zr = 0.0f;
}

//Combine two transforms
void Transform::combine(Transform *t)
{
	xt += t->xt; yt += t->yt; zt += t->zt;
	xr += t->xr; yr += t->yr; zr += t->zr;
	xs *= t->xs; ys *= t->ys; zs *= t->zs;
}

//Bake and save transform
int Transform::save(pugi::xml_node root)
{
	//Build matrix with rotate, scale, translate order
	Matrix m;

	m.translate(xt, yt, zt);
	
	m.rotateX(xr);
	m.rotateY(yr);
	m.rotateZ(zr);
	
	m.scale(xs, ys, zs);

	return m.save(root);
}

//----------------------Matrix Implementation----------------------------------

//Constructor
Matrix::Matrix()
{
	//Use identity matrix
	identity();
}

//Destructor
Matrix::~Matrix()
{
	
}

//Copy constructor
Matrix::Matrix(Matrix &c)
{
	r0[0] = c.r0[0]; r0[1] = c.r0[1]; r0[2] = c.r0[2]; r0[3] = c.r0[3];
	r1[0] = c.r1[0]; r1[1] = c.r1[1]; r1[2] = c.r1[2]; r1[3] = c.r1[3];
	r2[0] = c.r2[0]; r2[1] = c.r2[1]; r2[2] = c.r2[2]; r2[3] = c.r2[3];
	r3[0] = c.r3[0]; r3[1] = c.r3[1]; r3[2] = c.r3[2]; r3[3] = c.r3[3];
}

//Reset matrix to identity
void Matrix::identity()
{
	r0[0] = 1.0f;
	r1[1] = 1.0f;
	r2[2] = 1.0f;
	r3[3] = 1.0f;
	
	r0[1] = 0.0f;
	r0[2] = 0.0f;
	r0[3] = 0.0f;
	
	r1[0] = 0.0f;
	r1[2] = 0.0f;
	r1[3] = 0.0f;

	r2[0] = 0.0f;
	r2[1] = 0.0f;
	r2[3] = 0.0f;

	r3[0] = 0.0f;
	r3[1] = 0.0f;
	r3[2] = 0.0f;
}

//Add a translation matrix
void Matrix::translate(float x, float y, float z)
{
	float c0[4] = {1.0f, 0.0f, 0.0f, 0.0f};
	float c1[4] = {0.0f, 1.0f, 0.0f, 0.0f};
	float c2[4] = {0.0f, 0.0f, 1.0f, 0.0f};
	float c3[4] = {x, y, z, 1.0f};

	multiply(c0, c1, c2, c3);
}

//Add a scaling matrix
void Matrix::scale(float x, float y, float z)
{
	float c0[4] = {x, 0.0f, 0.0f, 0.0f};
	float c1[4] = {0.0f, y, 0.0f, 0.0f};
	float c2[4] = {0.0f, 0.0f, z, 0.0f};
	float c3[4] = {0.0f, 0.0f, 0.0f, 1.0f};

	multiply(c0, c1, c2, c3);
}

//Add a rotation matrix
void Matrix::rotate(float x, float y, float z, float angle)
{
	angle = angle * PI / 180.0f;
	float s = sinf(angle);
	float c = cosf(angle);
	float omc = 1.0f - c;

	float c0[4] = {(x * x * omc) + c, (x * y * omc) + (z * s), (x * z * omc) - (y * s), 0.0f};
	float c1[4] = {(y * x * omc) - (z * s), (y * y * omc) + c, (y * z * omc) + (x * s), 0.0f};
	float c2[4] = {(z * x * omc) + (y * s), (z * y * omc) - (x * s), (z * z * omc) + c, 0.0f};
	float c3[4] = {0.0f, 0.0f, 0.0f, 1.0f};

	multiply(c0, c1, c2, c3);
}

//Rotate with axis aligned angles
void Matrix::rotateX(float angle)
{
	angle = angle * PI / 180.0f;
	float s = sinf(angle);
	float c = cosf(angle);

	float c0[4] = {1.0f, 0.0f, 0.0f, 0.0f};
	float c1[4] = {0.0f, c, s, 0.0f};
	float c2[4] = {0.0f, -s, c, 0.0f};
	float c3[4] = {0.0f, 0.0f, 0.0f, 1.0f};

	multiply(c0, c1, c2, c3);
}

void Matrix::rotateY(float angle)
{
	angle = angle * PI / 180.0f;
	float s = sinf(angle);
	float c = cosf(angle);

	float c0[4] = {c, 0.0f, -s, 0.0f};
	float c1[4] = {0.0f, 1.0f, 0.0f, 0.0f};
	float c2[4] = {s, 0.0f, c, 0.0f};
	float c3[4] = {0.0f, 0.0f, 0.0f, 1.0f};

	multiply(c0, c1, c2, c3);
}

void Matrix::rotateZ(float angle)
{
	angle = angle * PI / 180.0f;
	float s = sinf(angle);
	float c = cosf(angle);

	float c0[4] = {c, s, 0.0f, 0.0f};
	float c1[4] = {-s, c, 0.0f, 0.0f};
	float c2[4] = {0.0f, 0.0f, 1.0f, 0.0f};
	float c3[4] = {0.0f, 0.0f, 0.0f, 1.0f};

	multiply(c0, c1, c2, c3);
}

//Multiply by another matrix
void Matrix::multiply(float *c0, float *c1, float *c2, float *c3)
{
	//Copy current matrix into a temporary matrix
	float tr0[4] = {r0[0], r0[1], r0[2], r0[3]};
	float tr1[4] = {r1[0], r1[1], r1[2], r1[3]};
	float tr2[4] = {r2[0], r2[1], r2[2], r2[3]};
	float tr3[4] = {r3[0], r3[1], r3[2], r3[3]};

	//Multipy the matrices into this matrix
	r0[0] = DOT(tr0,c0);
	r0[1] = DOT(tr0,c1);
	r0[2] = DOT(tr0,c2);
	r0[3] = DOT(tr0,c3);

	r1[0] = DOT(tr1,c0);
	r1[1] = DOT(tr1,c1);
	r1[2] = DOT(tr1,c2);
	r1[3] = DOT(tr1,c3);

	r2[0] = DOT(tr2,c0);
	r2[1] = DOT(tr2,c1);
	r2[2] = DOT(tr2,c2);
	r2[3] = DOT(tr2,c3);

	r3[0] = DOT(tr3,c0);
	r3[1] = DOT(tr3,c1);
	r3[2] = DOT(tr3,c2);
	r3[3] = DOT(tr3,c3);
}

//Multiply by another transform
void Matrix::multiply(Matrix *t)
{
	//Change rows to columns
	float tc0[4] = {t->r0[0], t->r1[0], t->r2[0], t->r3[0]};
	float tc1[4] = {t->r0[1], t->r1[1], t->r2[1], t->r3[1]};
	float tc2[4] = {t->r0[2], t->r1[2], t->r2[2], t->r3[2]};
	float tc3[4] = {t->r0[3], t->r1[3], t->r2[3], t->r3[3]};

	multiply(tc0, tc1, tc2, tc3);
}

//Save the matrix to a COLLADA node
int Matrix::save(pugi::xml_node root)
{
	if(!root)
		return 1;

	//Add the matrix node
	pugi::xml_node matrix_node = root.append_child("matrix");

	//Determine indent
	char indent[20];
	int depth = matrix_node.depth() - 1;
	int idx;
	for(idx = 0; idx < depth; idx++)
		indent[idx] = '\t';
	indent[idx] = '\0';

	//Build a formated string with the matrix values
	std::ostringstream mat_stream;
	mat_stream << std::setiosflags(std::ios::showpoint) << std::endl << indent;
	mat_stream << DEC_FORMAT << r0[0] << " ";
	mat_stream << DEC_FORMAT << r0[1] << " ";
	mat_stream << DEC_FORMAT << r0[2] << " ";
	mat_stream << DEC_FORMAT << r0[3] << std::endl << indent;

	mat_stream << DEC_FORMAT << r1[0] << " ";
	mat_stream << DEC_FORMAT << r1[1] << " ";
	mat_stream << DEC_FORMAT << r1[2] << " ";
	mat_stream << DEC_FORMAT << r1[3] << std::endl << indent;

	mat_stream << DEC_FORMAT << r2[0] << " ";
	mat_stream << DEC_FORMAT << r2[1] << " ";
	mat_stream << DEC_FORMAT << r2[2] << " ";
	mat_stream << DEC_FORMAT << r2[3] << std::endl << indent;

	mat_stream << DEC_FORMAT << r3[0] << " ";
	mat_stream << DEC_FORMAT << r3[1] << " ";
	mat_stream << DEC_FORMAT << r3[2] << " ";
	mat_stream << DEC_FORMAT << r3[3] << std::endl;

	indent[depth - 1] = '\0';
	mat_stream << indent;

	//Create string and add to node
	std::string matrix = mat_stream.str();
	matrix_node.text() = matrix.c_str();

	return 0;
}
