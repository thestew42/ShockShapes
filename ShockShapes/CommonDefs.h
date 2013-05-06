/** @file CommonDefs.h
 * 
 * @brief Defines common values for use throughout the code
 * 
 * Copyright 2012-2013, Stewart Hall
 *
 * Licensed under The MIT License
 * Redistributions of files must retain the above copyright notice.
 *
 * @author		Stewart Hall (www.stewartghall.com)
 * @date		12/30/2012
 * @copyright	Copyright 2012-2013, Stewart Hall
 * @license		MIT License (http://www.opensource.org/licenses/mit-license.php)
 */

#ifndef _COMMONDEFS_
#define _COMMONDEFS_

#ifndef NULL
#define NULL 0
#endif

#ifndef PI
#define PI 3.14159f
#endif

#define OPRECISION 7

#define DEC_FORMAT std::setw(OPRECISION) << std::setprecision(OPRECISION - 2)

#define VERSION_STRING "ShockShapes DEV"

/**
 * @brief Vector structure for storing vertices, normals, etc
 */
typedef struct {
	float x, y, z;
} Vector3D;

/**
 * @brief Vector structure for storing texture coordinates
 */
typedef struct {
	float u, v;
} Vector2D;

#endif
