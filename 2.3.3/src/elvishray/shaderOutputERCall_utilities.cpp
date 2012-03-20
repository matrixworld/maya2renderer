#include "shaderOutputERCall.h"
#include <liqlog.h>
#include "../common/mayacheck.h"
#include "../shadergraph/convertShadingNetwork.h"
#include "../shadergraph/shadermgr.h"

#include <eiAPI/ei.h>

namespace ERCall
{
/// General utilities ///
// @node	maya shader node name
void Visitor::visitArrayMapper(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitBump2d(const char* node)
{
	OutputHelper o;

	o.beginRSL(node);

	ei_shader_param_string("desc", "maya_bump2d");
	//input
	o.addRSLVariable("scalar",  "bumpValue",		"bumpValue",	node);
	o.addRSLVariable("scalar",  "bumpDepth",		"bumpDepth",	node);
	o.addRSLVariable("index",	"bumpInterp",		"bumpInterp",	node);
	o.addRSLVariable("scalar",  "bumpFilter",		"bumpFilter",	node);
	o.addRSLVariable("scalar",  "bumpFilterOffset",	"bumpFilterOffset",	node);
	o.addRSLVariable("normal",  "normalCamera",		"normalCamera",	node);
	if(doesPlugExist(node, "bumpNormal")){
		o.addRSLVariable("normal",  "bumpNormal",	"bumpNormal",	node);
	}
	//output
	o.addRSLVariable("normal",  "outNormal",		"outNormal",	node);

	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitBump3d(const char* node)
{
	OutputHelper o;

	o.beginRSL(node);

	ei_shader_param_string("desc", "maya_bump3d");
	//input
	o.addRSLVariable("scalar",  "bumpValue",		"bumpValue",	node);
	o.addRSLVariable("scalar",  "bumpDepth",		"bumpDepth",	node);
	o.addRSLVariable("scalar",  "bumpFilter",		"bumpFilter",	node);
	o.addRSLVariable("scalar",  "bumpFilterOffset",	"bumpFilterOffset",	node);
	o.addRSLVariable("normal",  "normalCamera",		"normalCamera",	node);
	//output
	o.addRSLVariable("normal",  "outNormal",		"outNormal",	node);
	
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitCondition(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitDistanceBetween(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitHeightField(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitLightInfo(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitMultiplyDivide(const char* node)
{

}
void Visitor::visitPlace2dTexture(const char* node)
{
	OutputHelper o;

	o.beginRSL(node);

	ei_shader_param_string("desc", "maya_place2dTexture");
	//Inputs
	o.addRSLVariable("vector",  "uvCoord",		"uvCoord",		node);
	o.addRSLVariable("float",	"coverageU",	"coverageU",	node);
	o.addRSLVariable("float",	"coverageV",	"coverageV",	node);
	o.addRSLVariable("bool",	"mirrorU",		"mirrorU",		node);
	o.addRSLVariable("bool",	"mirrorV",		"mirrorV",		node);
	o.addRSLVariable("float",	"noiseU",		"noiseU",		node);
	o.addRSLVariable("float",	"noiseV",		"noiseV",		node);
	o.addRSLVariable("float",	"offsetU",		"offsetU",		node);
	o.addRSLVariable("float",	"offsetV",		"offsetV",		node);
	o.addRSLVariable("float",	"repeatU",		"repeatU",		node);
	o.addRSLVariable("float",	"repeatV",		"repeatV",		node);
	o.addRSLVariable("float",	"rotateFrame",	"rotateFrame",	node);
	o.addRSLVariable("float",	"rotateUV",		"rotateUV",		node);
	o.addRSLVariable("bool",	"stagger",		"stagger",		node);
	o.addRSLVariable("float",	"translateFrameU",	"translateFrameU",	node);
	o.addRSLVariable("float",	"translateFrameV",	"translateFrameV",	node);
	o.addRSLVariable("float",	"wrapU",		"wrapU",		node);
	o.addRSLVariable("float",	"wrapV",		"wrapV",		node);
	//Outputs
	o.addRSLVariable("vector",  "outUV",		"outUV",		node);

	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitPlace3dTexture(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitPlusMinusAverage(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitProjection(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitReverse(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitSamplerInfo(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitSetRange(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitStencil(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitStudioClearCoat(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitUVChooser(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitVectorProduct(const char* node)
{

}
/// color utilities ///
// @node	maya shader node name
void Visitor::visitBlendColors(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitClamp(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitContrast(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitGammaCorrect(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitHsvToRgb(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitLuminance(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitRemapColor(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitrRemapHsv(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitRemapValue(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitRgbToHsv(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitSmear(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitSurfaceLuminance(const char* node)
{

}
/// switch utilities ///
// @node	maya shader node name
void Visitor::visitDoubleShadingSwitch(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitQuadShadingSwitch(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitSingleShadingSwitch(const char* node)
{

}
// @node	maya shader node name
void Visitor::visitTripleShadingSwitch(const char* node)
{

}
/// particle utilities ///	
// @node	maya shader node name
void Visitor::visitParticleSamplerInfo(const char* node)
{

}
/// image planes ///
// @node	maya shader node name
void Visitor::visitImagePlane(const char* node)
{

}
/// glow ///
// @node	maya shader node name
void Visitor::visitOpticalFX(const char* node)
{

}
}//namespace ERCall