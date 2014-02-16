/** @file Instance.h
 * 
 * @brief Object that is an instance of another object
 * 
 * Copyright 2013, Stewart Hall
 *
 * Licensed under The MIT License
 * Redistributions of files must retain the above copyright notice.
 *
 * @author		Stewart Hall (www.stewartghall.com)
 * @date		1/2/2013
 * @copyright	Copyright 2013, Stewart Hall
 * @license		MIT License (http://www.opensource.org/licenses/mit-license.php)
 */

#ifndef _INSTANCE_
#define _INSTANCE_

#include "Geometry.h"

/**
 * @brief An instance of another object uses the object's geometry with a different transform
 * @details A pointer to another object. Does not write a mesh at all, just
 * an instance of an existing one.
 */
class Instance : public Geometry {
private:
	/**
	 * Pointer to the object that this is instancing
	 */
	Geometry *original;

public:
	Instance(Geometry *obj);		/**< Constructor for instance of an object. */

	~Instance();					/**< Destructor. */

	/**
	 * Does nothing because an instance has no geometry
	 * @param root Pointer to a pugixml node to write geometry in
	 * @return Returns 0 if no errors occur
	 */
	virtual int saveGeometry(pugi::xml_node root);

	/**
	 * Saves an instance node in COLLADA format to the scene
	 * @param root The scene element to add the node to
	 * @param id Unique suffix to place after the name of the node
	 * @return Returns 0 if no errors occur
	 */
	virtual int saveInstance(pugi::xml_node root, int *id, Transform *parent);

	/**
	 * An instance cannot be filtered, so this does nothing
	 */
	virtual void filter();

	/**
	 * An instance has to combine it's parent geometry
	 * @param g Geometry to combine into
	 * @param t Transform to apply to this geometry
	 */
	virtual void combineInto(Geometry *g, Matrix *parent_t);
};

#endif
