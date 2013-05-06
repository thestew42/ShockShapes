/** @file CSource.cpp
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

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>

#include "CSource.h"

CSource::CSource()
{
	params = NULL;
	data_buffer = NULL;
}

//Construct from COLLADA
CSource::CSource(pugi::xml_node root)
{
	params = NULL;
	data_buffer = NULL;

	//Read the id attribute
	id = root.attribute("id").value();

	//Read the data array
	if(root.child("float_array")) {
		type = ST_FLOAT;
		readFloatArray(root.child("float_array"));
	} else if(root.child("int_array")) {
		type = ST_INT;
		readIntArray(root.child("int_array"));
	}

	//Read the technique for accessor information
	if(root.child("technique_common")) {
		pugi::xml_node technique_node = root.child("technique_common");
		
		if(technique_node.child("accessor")) {
			pugi::xml_node accessor_node = technique_node.child("accessor");

			//Get count and stride
			count = atoi(accessor_node.attribute("count").value());
			stride = atoi(accessor_node.attribute("stride").value());

			//Read each parameter
			params = new param_type[stride];
			int index = 0;
			for(pugi::xml_node param = accessor_node.first_child(); param; param = param.next_sibling()) {
				const char *param_name = param.attribute("name").value();

				if(!strcmp(param_name, "A")) {
					params[index] = PT_A;
				} else if(!strcmp(param_name, "ANGLE")) {
					params[index] = PT_ANGLE;
				} else if(!strcmp(param_name, "B")) {
					params[index] = PT_B;
				} else if(!strcmp(param_name, "DOUBLE_SIDED")) {
					params[index] = PT_DOUBLE_SIDED;
				} else if(!strcmp(param_name, "G")) {
					params[index] = PT_G;
				} else if(!strcmp(param_name, "P")) {
					params[index] = PT_P;
				} else if(!strcmp(param_name, "Q")) {
					params[index] = PT_Q;
				} else if(!strcmp(param_name, "R")) {
					params[index] = PT_R;
				} else if(!strcmp(param_name, "S")) {
					params[index] = PT_S;
				} else if(!strcmp(param_name, "T")) {
					params[index] = PT_T;
				} else if(!strcmp(param_name, "TIME")) {
					params[index] = PT_TIME;
				} else if(!strcmp(param_name, "U")) {
					params[index] = PT_U;
				} else if(!strcmp(param_name, "V")) {
					params[index] = PT_V;
				} else if(!strcmp(param_name, "W")) {
					params[index] = PT_W;
				} else if(!strcmp(param_name, "X")) {
					params[index] = PT_X;
				} else if(!strcmp(param_name, "Y")) {
					params[index] = PT_Y;
				} else if(!strcmp(param_name, "Z")) {
					params[index] = PT_Z;
				}

				index++;
			}
		}
	}
}

//Read a float array
void CSource::readFloatArray(pugi::xml_node root)
{
	float *buffer;

	//Allocate buffer
	buffer_size = atoi(root.attribute("count").value());
	buffer = new float[buffer_size];

	//Read each float from the array
	std::string text = root.text().get();
	std::istringstream iss(text);
	for(int i = 0; i < buffer_size; i++) {
		iss >> buffer[i];
	}

	data_buffer = (void*)buffer;

	return;
}

//Read integer array
void CSource::readIntArray(pugi::xml_node root)
{
	int *buffer;

	//Allocate buffer
	buffer_size = atoi(root.attribute("count").value());
	buffer = new int[buffer_size];

	//Read each float from the array
	std::string text = root.text().get();
	std::istringstream iss(text);
	for(int i = 0; i < buffer_size; i++) {
		iss >> buffer[i];
	}

	data_buffer = (void*)buffer;

	return;
}

//Returns the id
const char* CSource::getId()
{
	return id.c_str();
}

//Returns the type of data in the source
source_type CSource::getType()
{
	return type;
}

//Getter for a float value
float CSource::getFloat(unsigned int index)
{
	if(type == ST_FLOAT && index < buffer_size)
		return ((float*)data_buffer)[index];

	return 0.0f;
}

//Getter for an int value
int CSource::getInt(unsigned int index)
{
	if(type == ST_INT && index < buffer_size)
		return ((int*)data_buffer)[index];

	return 0.0f;
}

//Getter for a param
float CSource::accessFloatParameter(unsigned int element, param_type param_id)
{
	if(type == ST_FLOAT) {
		//Can only be used if a technique_common node was processed
		if(params) {
			if(element < count) {
				for(int i = 0; i < stride; i++) {
					if(params[i] == param_id)
						return ((float*)data_buffer)[element * stride + i];
				}
			}
		}
	}

	return 0.0f;
}

//Return the number of parameters
int CSource::getNumParameters()
{
	return stride;
}

//Return the type of parameter
param_type CSource::getParamType(int index)
{
	if(index < stride)
		return params[index];

	return PT_INVALID;
}

//Return the number of elements
int CSource::getNumElements()
{
	return count;
}
