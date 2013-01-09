/** @file GBumpFilter.h
 * 
 * @brief Geometry filter makes the surface of an object rough
 * 
 * Copyright 2013, Stewart Hall
 *
 * Licensed under The MIT License
 * Redistributions of files must retain the above copyright notice.
 *
 * @author		Stewart Hall (www.stewartghall.com)
 * @date		1/1/2013
 * @copyright	Copyright 2013, Stewart Hall
 * @license		MIT License (http://www.opensource.org/licenses/mit-license.php)
 */

#ifndef _GBUMPFILTER_
#define _GBUMPFILTER_

#include "GeometryFilter.h"
#include "Geometry.h"

/**
 * @brief Makes the surface of an object rough
 * @details TODO
 */
class GBumpFilter : public GeometryFilter {
private:
	Parameter magnitude;		/**< The amount to perturb vertices. */
	
	bool perturb_once;			/**< When set, each vertex is only perturbed once. */
	
	bool direction_constrain;	/**< When set, only triangles facing in the constrained direction are perturbed. */
	
	Vector3D direction;			/**< Direction of constraint. Normal is dotted with this and compared against the tolerance. */
	
	float tolerance;			/**< How much the vertex normal can vary from the constraint direction. */

public:
	/**
	 * Constructs a new bump filter
	 * @param imagnitude The amount to perturb a vertex
	 * @param iperturb_once If true, each vertex is only perturbed once
	 */
	GBumpFilter(Parameter imagnitude, bool iperturb_once);

	~GBumpFilter();				/**< Destructor. */

	/**
	 * Sets up a direction constraint
	 * @param idirection The direction of normals on faces to be perturbed
	 * @param itolerance The max allowable variance from that direction
	 */
	void setupDirectionConstraint(Vector3D idirection, float itolerance);

	/**
	 * Disables a direction constraint
	 */
	void disableDirectionConstraint();

	/**
	 * Runs the bump filter
	 * @param g The object to apply the filter to
	 */
	virtual void run(Geometry *g);
};

#endif
