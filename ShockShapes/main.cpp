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

int main(void)
{
	Scene *scene = new Scene("test_file");

	Cube *c = new Cube(4.0f);
	c->getTransform()->rotate(0.0f, 45.0f, 0.0f);

	Cube *c1 = new Cube(2.0f, 5.0f, 1.0f);
	c1->addFilter(new Subdivide(3));

	GBumpFilter *bump = new GBumpFilter(Parameter(-0.05f, 0.05f), true);
	Vector3D dir;
	dir.x = 0.0f; dir.y = 0.0f; dir.z = -1.0f;
	bump->setupDirectionConstraint(dir, 0.1f);
	c1->addFilter(bump);

	NormalFilter *norm_f = new NormalFilter(NM_SOFTEN);
	norm_f->enableSoftenThreshold(0.5f);
	c1->addFilter(norm_f);

	c1->getTransform()->rotate(100.0f, 0.0f, 0.0f);
	c1->getTransform()->translate(6.0f, 1.0f, 0.0f);

	Group *g = new Group();
	g->addObject(c);
	g->addObject(c1);

	g->getTransform()->translate(0.0f, 3.0f, 0.0f);

	scene->addObject(g);

	Instance *c1_inst = new Instance(c1);
	c1_inst->getTransform()->translate(0.0f, 5.0f, 0.0f);
	scene->addObject(c1_inst);
	
	scene->generate(0);
	if(scene->save("test.dae")) {
		printf("Error occured\n");
		getchar();
	}

	return 0;
}
