/*---------------------------------------------------------------------------------------------
This file is part of elvish ray Renderer.
Copyright (C) 2007-2009  Enning Liang(Len3d)

elvish ray Renderer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

elvish ray Renderer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
---------------------------------------------------------------------------------------------*/
#include <iostream>
#include "eiAPI/ei.h"

using namespace std;

/*---------------------------------------------------------------------------------------------

Function	:	test_example_scene
Inputs		:	
Outputs		:	
Description	:	test the basic rendering functionalities of the renderer with a very 
simple example scene.
Comments	:	
History		:	

---------------------------------------------------------------------------------------------*/

extern void ei_face_color_unittest(const eiApprox *approx, const char *suffix);
extern void ei_shader_graph_unittest(const eiBool texturing, const char *suffix);
extern void ei_shader_link_test(const eiBool texturing, const char *suffix);
//
void test_liquid2er()
{
//#include"cornellbox.esa"
//#include "d:/script.er"
//#include"ei_scene_unittest.esa"//<----- use your own elvishender api script file here.
	
	//ei_face_color_unittest(NULL, "suffix");
	//ei_shader_graph_unittest(true, "suffix");
	ei_shader_link_test(false, "suffix");
}
//
int main()
{
	cout << "Launching elvish ray Renderer..." << endl;

 
	test_liquid2er();


	cout << "all tests done." << endl;
 

	return 0;
}
