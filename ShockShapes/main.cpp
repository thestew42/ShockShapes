/** @file main.cpp
 * 
 * @brief Development driver to test the code
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

#include "Scene.h"
#include "Cube.h"
#include "Group.h"
#include "Subdivide.h"
#include "GBumpFilter.h"
#include "NormalFilter.h"
#include "Instance.h"
#include "TiledGroup.h"

int main(void)
{
	Scene *scene = new Scene("test_file");

	Cube *c = new Cube(4.0f);
	c->getTransform()->rotate(0.0f, 45.0f, 0.0f);

	Cube *c1 = new Cube(5.0f, 1.0f, 2.0f);
	c1->addFilter(new Subdivide(3));

	GBumpFilter *bump = new GBumpFilter(Parameter(-0.05f, 0.05f), true);
	Vector3D dir;
	dir.x = 0.0f; dir.y = 1.0f; dir.z = 0.0f;
	bump->setupDirectionConstraint(dir, 0.1f);
	c1->addFilter(bump);

	NormalFilter *norm_f = new NormalFilter(NM_SOFTEN);
	norm_f->enableSoftenThreshold(0.5f);
	c1->addFilter(norm_f);

	//Set up tiled group
	TiledGroup *wall = new TiledGroup("Wall");
	wall->setBaseObject(c1, 5.15f, 2.15f);
	wall->setTiledProperties(8, 40.0f, 40.0f, 2.0f, TEM_SCALE);
	scene->addObject(wall);
	
	scene->generate(0);
	if(scene->save("test.dae")) {
		printf("Error occured\n");
		getchar();
	}

	//Attempt to copy the file
	Scene *scene2 = new Scene();
	scene2->load("victoria.dae");

	//Flatten the scene to a single object
	scene2->consolidate();
	scene2->save("copy.dae");

	return 0;
}
