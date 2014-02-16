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

#include "CSourceLib.h"

//Constructor
CSourceLib::CSourceLib()
{

}

//Destructor
CSourceLib::~CSourceLib()
{
	for(unsigned int i = 0; i < sources.size(); i++)
	{
		delete sources[i];
		sources[i] = NULL;
	}
}

//Add a new source to the collection
void CSourceLib::addSource(pugi::xml_node root)
{
	sources.push_back(new CSource(root));
}

//Get a source by name
CSource* CSourceLib::getSource(const char *name)
{
	//Remove # if present
	if(name[0] == '#')
		name = &(name[1]);

	for(unsigned int i = 0; i < sources.size(); i++)
	{
		if(!strcmp(sources[i]->getId(), name))
			return sources[i];
	}

	return NULL;
}
