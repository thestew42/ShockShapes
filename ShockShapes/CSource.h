/** @file CSource.h
 * 
 * @brief Helper for loading and managing COLLADA source nodes
 * 
 * Copyright 2012-2013, Stewart Hall
 *
 * Licensed under The MIT License
 * Redistributions of files must retain the above copyright notice.
 *
 * @author		Stewart Hall (www.stewartghall.com)
 * @date		3/10/2013
 * @copyright	Copyright 2012-2013, Stewart Hall
 * @license		MIT License (http://www.opensource.org/licenses/mit-license.php)
 */

#ifndef _CSOURCE_
#define _CSOURCE_

#include <string>
#include <stdio.h>

#include "pugixml.hpp"
#include "CommonDefs.h"

/**
 * @brief Defines the different types of source data
 */
enum source_type {
	ST_FLOAT,
	ST_INT
};

/**
 * @brief Defines the different types of parameters that can be stored in the source
 */
enum param_type {
	PT_A,
	PT_ANGLE,
	PT_B,
	PT_DOUBLE_SIDED,
	PT_G,
	PT_P,
	PT_Q,
	PT_R,
	PT_S,
	PT_T,
	PT_TIME,
	PT_U,
	PT_V,
	PT_W,
	PT_X,
	PT_Y,
	PT_Z,
	PT_INVALID
};

/**
 * @brief Loads and stores data from COLLADA source nodes
 * @details Stores an array of the data and provides accessors
 * to retrieve it.
 */
class CSource {
private:
	/**
	 * ID of this source
	 */
	std::string id;

	/**
	 * The type of data in the buffer
	 */
	source_type type;

	/**
	 * Buffer containing source data
	 */
	void *data_buffer;
	int buffer_size;

	/**
	 * Array of params for accessor information
	 */
	param_type *params;
	int stride;
	int count;

	/**
	 * Reads a float array
	 * @param root pugixml node to read source from
	 */
	void readFloatArray(pugi::xml_node root);

	/**
	 * Reads an integer array
	 * @param root pugixml node to read source from
	 */
	void readIntArray(pugi::xml_node root);

public:
	CSource();								/**< Constructs an empty source. */
	CSource(pugi::xml_node root);			/**< Constructs a source from COLLADA data. */

	const char* getId();					/**< Returns the id attribute in C string format. */

	source_type getType();					/**< Returns the type of source. */

	float getFloat(unsigned int index);		/**< Returns a float attribute. */
	int getInt(unsigned int index);			/**< Returns an integer attribute. */

	/**
	 * Returns a parameter of an element in the source
	 * @param element The element id. Element can be coordinates, colors, etc
	 * @param param_id The parameter of the element to return
	 * @return The selected parameter from the selected element. Returns 0 if not available
	 */
	float accessFloatParameter(unsigned int element, param_type param_id);

	/**
	 * Returns the number of parameters per element
	 * @return Number of parameters
	 */
	int getNumParameters();

	/**
	 * Returns the number of elements
	 * @return Number of elements in the source
	 */
	int getNumElements();

	/**
	 * Returns the type of the parameter
	 * @param index The parameter index. Max value is getNumParameters() - 1
	 * @return The type of parameter
	 */
	param_type getParamType(int index);
};

#endif
