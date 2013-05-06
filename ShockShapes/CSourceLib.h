/** @file CSource.h
 * 
 * @brief Helper for storing and accessing multiple collada source nodes
 * 
 * Copyright 2013, Stewart Hall
 *
 * Licensed under The MIT License
 * Redistributions of files must retain the above copyright notice.
 *
 * @author		Stewart Hall (www.stewartghall.com)
 * @date		5/5/2013
 * @copyright	Copyright 2013, Stewart Hall
 * @license		MIT License (http://www.opensource.org/licenses/mit-license.php)
 */

#ifndef _CSOURCELIB_
#define _CSOURCELIB_

#include <string>
#include <vector>
#include <stdio.h>

#include "pugixml.hpp"
#include "CSource.h"

/**
 * @brief Holds multiple CSource objects
 */
class CSourceLib {
private:
	/**
	 * Collection of CSource objects
	 */
	std::vector<CSource*> sources;

public:
	CSourceLib();
	~CSourceLib();

	void addSource(pugi::xml_node root);		/**< Loads a new source and adds to the collection. */

	CSource* getSource(const char* name);		/**< Returns a source by name. */
};

#endif
