/** @file Transform.h
 * 
 * @brief Defines class used for 3D transforms
 *
 * This implementation stores transforms in their scale, translate,
 * rotate components and can bake them to a single matrix
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

#ifndef _TRANSFORM_
#define _TRANSFORM_

#include <math.h>
#include <sstream>
#include <iomanip>
#include <string>

#include "pugixml.hpp"
#include "CommonDefs.h"

#define DOT(x,y) (((x[0]) * (y[0])) + ((x[1]) * (y[1])) + ((x[2]) * (y[2])) + ((x[3]) * (y[3])))

class Matrix
{
public:
	/**
	 * Matrices are expressed as rows for convenient indexing
	 */
	float r0[4];
	float r1[4];
	float r2[4];
	float r3[4];

	Matrix();					/**< Constructs an identity matrix. */
	Matrix(Matrix &c);			/**< Copy constructor. */

	~Matrix();					/**< Destructor */

	/**
	 * Resets the matrix to identity
	 */
	void identity();

	/**
	 * Adds a translation to the transform
	 * @param x Distance to translate along the x axis
	 * @param y Distance to translate along the y axis
	 * @param z Distance to translate along the z axis
	 */
	void translate(float x, float y, float z);

	/**
	 * Adds a scale to the transform
	 * @param x Amount to scale in the x direction
	 * @param y Amount to scale in the y direction
	 * @param z Amount to scale in the z direction
	 */
	void scale(float x, float y, float z);

	/**
	 * Adds a rotation to the transform
	 * @param x X coordinate of the vector specifying rotation axis
	 * @param y Y coordinate of the vector specifying rotation axis
	 * @param z Z coordinate of the vector specifying rotation axis
	 * @param angle The angle in degrees to rotate around this axis
	 */
	void rotate(float x, float y, float z, float angle);

	/**
	 * Rotate around the X axis
	 * @param angle Amount to rotate around x axis
	 */
	void rotateX(float angle);

	/**
	 * Rotate around the Y axis
	 * @param angle Amount to rotate around y axis
	 */
	void rotateY(float angle);

	/**
	 * Rotate around the Z axis
	 * @param angle Amount to rotate around z axis
	 */
	void rotateZ(float angle);

	/**
	 * Multiplies this matrix by another matrix
	 * @param tr0 Column 0 of the other matrix
	 * @param tr1 Column 1 of the other matrix
	 * @param tr2 Column 2 of the other matrix
	 * @param tr3 Column 3 of the other matrix
	 */
	void multiply(float *c0, float *c1, float *c2, float *c3);

	/**
	 * Multiplies this matrix by another transform
	 * @param t the transform to multiply with
	 */
	void multiply(Matrix *t);

	/**
	 * Saves this transform into a matrix COLLADA node
	 * @param root The node to use as a parent for the matrix
	 * @return Returns 0 if successful
	 */
	int save(pugi::xml_node root);
};

/**
 * Class that a user interacts with to transform nodes
 */
class Transform {
public:
	/**
	 * Translation coordinates
	 */
	float xt, yt, zt;

	/**
	 * Degrees rotation about each axis
	 */
	float xr, yr, zr;

	/**
	 * Scaling on each axis
	 */
	float xs, ys, zs;

	Transform();				/**< Creates empty transform. */
	Transform(Transform &c);	/**< Copy constructor. */

	~Transform();				/**< Destructor. */

	/**
	 * Change the translation by given amounts
	 * @param x Amount to translate on x axis
	 * @param y Amount to translate on y axis
	 * @param z Amount to translate on z axis
	 */
	void translate(float x, float y, float z);

	/**
	 * Change the scale by given amounts
	 * @param x Amount to scale on x axis
	 * @param y Amount to scale on y axis
	 * @param z Amount to scale on z axis
	 */
	void scale(float x, float y, float z);

	/**
	 * Change the rotation by given amounts
	 * @param x Amount to rotate on x axis
	 * @param y Amount to rotate on y axis
	 * @param z Amount to rotate on z axis
	 */
	void rotate(float x, float y, float z);

	/**
	 * Set the translation
	 * @param x Amount to translate on x axis
	 * @param y Amount to translate on y axis
	 * @param z Amount to translate on z axis
	 */
	void setTranslation(float x, float y, float z);

	/**
	 * Set the rotation
	 * @param x Amount to rotate on x axis
	 * @param y Amount to rotate on y axis
	 * @param z Amount to rotate on z axis
	 */
	void setRotation(float x, float y, float z);

	/**
	 * Set the scale
	 * @param x Amount to scale on x axis
	 * @param y Amount to scale on y axis
	 * @param z Amount to scale on z axis
	 */
	void setScale(float x, float y, float z);

	/**
	 * Combine another transform with this one
	 * @param t The other transform to apply to this one
	 */
	void combine(Transform *t);

	/**
	 * Resets the transform
	 */
	void reset();

	/**
	 * Bake to a matrix and save to XML
	 * @param root The node to use as a parent for the matrix
	 * @return Returns 0 if successful
	 */
	int save(pugi::xml_node root);
};

#endif
