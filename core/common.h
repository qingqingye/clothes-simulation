#pragma once
#ifndef _COMMONH_
#define _COMMONH_


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <iostream>
#pragma comment(lib,"3rdparty/opengl/glfw-3.2.1.bin.WIN64/lib-vc2015/glfw3.lib")
#pragma comment(lib,"3rdparty/opengl/glew-1.13.0/lib/Release/x64/glew32.lib")
#pragma comment(lib,"3rdparty/opengl/freeglut-MSVC-3.0.0-2.mp/freeglut/lib/x64/freeglut.lib")
#pragma comment(lib,"3rdparty/opengl/Simple OpenGL Image Library/lib/SOIL.lib")
#pragma comment(lib,"3rdparty/opengl/glutdlls36/glut32.lib")
#include "../3rdparty/opengl/glew-1.13.0/include/GLEW/glew.h"
#include "../3rdparty/opengl/freeglut-MSVC-3.0.0-2.mp/freeglut/include/GL/freeglut.h"
#include "../3rdparty/opengl/glfw-3.2.1.bin.WIN64/include/GLFW/glfw3.h"
#include "../3rdparty/stb_image.h"

#include "../3rdparty/opengl/glm/glm/glm.hpp"
#include "../3rdparty/opengl/glm/glm/gtc/matrix_transform.hpp"
#include "../3rdparty/opengl/glm/glm/gtc/type_ptr.hpp"
#include "../3rdparty/opengl/glutdlls36/glut.h"
#include "../3rdparty/opengl/Simple OpenGL Image Library/src/SOIL.h"


/*
Orchid source code is Copyright(c) 2018
Li Wei.
This file is part of Orchid.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
- Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifdef _WIN32
#include <windows.h> 
#endif

#include <math.h>
#include <vector>
#include <iostream>



/* Some physics constants */
#define DAMPING 0.01 // how much to damp the cloth simulation each frame  ==¡· we dont use this in our proj
#define TIME_STEPSIZE2 0.25 // how large time step each object takes each frame
#define COLLISION_ITERATIONS 5 // how many iterations of constraint   satisfaction each frame (more is rigid, less is soft)



#ifndef __FUNCTION_NAME__
#if defined(_WIN32) || defined(__WIN32__)
#define __FUNCTION_NAME__ __FUNCTION__
#else
#define __FUNCTION_NAME__ __func__
#endif
#endif

#define Assertion(PREDICATE, MSG) \
do {\
if (!(PREDICATE)) {	\
	std::cerr << "Asssertion \"" \
	<< #PREDICATE << "\" failed in " << __FILE__ \
	<< " line " << __LINE__ \
	<< " in function \"" << (__FUNCTION_NAME__) << "\"" \
	<< " : " << (MSG) << std::endl; \
	std::abort(); \
} \
} while (false)



#endif // !_COMMONH_