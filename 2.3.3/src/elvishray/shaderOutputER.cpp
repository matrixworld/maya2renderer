#include "shaderOutputER.h"
#include <liqlog.h>
#include "../common/mayacheck.h"
#include "../shadergraph/convertShadingNetwork.h"
#include "../shadergraph/shadermgr.h"

namespace ER
{

OutputHelper::OutputHelper(std::ofstream& file_)
:file(file_)
{
	assert(file.is_open());
}
//
OutputHelper::~OutputHelper()
{

}
//
void OutputHelper::addRSLVariable(MString rslType, const MString& rslName,
					const MString& mayaName, const MString& mayaNode)
{
	MString cmd;

	// If the user specified that the type was an array of floats
	// (eg "float2"), extract the size and set the type to float.
	int rslTypeSize = 1;

	MString matchedStr;
	IfMErrorWarn(MGlobal::executeCommand("match(\"float[0-9]+$\",\""+rslType+"\")", matchedStr));
	if(matchedStr != "")
	{
		IfMErrorWarn(MGlobal::executeCommand("match(\"[0-9]+$\",\""+rslType+"\")", matchedStr));
		rslTypeSize = matchedStr.asInt();
		rslType = "float";
	}

	// Create the plug's name, and check for convertible connections.
	MString plug(mayaNode+"."+mayaName);
	int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(plug.asChar());

	// If there are no convertible connections, then we have to
	// write out the variable into the shader's body.
	if( connected == 0 )
	{
		//rslTypeSize(int) --> rslTypeSizeStr(string)
		MString rslTypeSizeStr;
		rslTypeSizeStr.set(rslTypeSize);
		// Write out the description of the variable.
		rslShaderBody += (" " + rslType + " " + rslName);
		rslShaderBody += ( rslTypeSize != 1 )?
							 ( "[" + rslTypeSizeStr + "] = " )
							:( " = " + rslType + " " );

		// Write out the value of the variable.
		if(   rslType=="color"
			||rslType=="point"
			||rslType=="normal"
			||rslType=="vector")
		{
			MDoubleArray val; val.setLength(3);
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
			//val(double) --> valStr(string)
			MStringArray valStr; valStr.setLength(3);
			valStr[0].set(val[0]);
			valStr[1].set(val[1]);
			valStr[2].set(val[2]);
			rslShaderBody +="("+valStr[0]+","+valStr[1]+","+valStr[2]+")";
			file<<"ei_shader_param_vector(\""<< rslName<<"\", "<<val[0]<<", "<<val[1]<<", "<<val[2]<<");"<<endl;
		}else if(rslType=="string"){
			MString val;
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
			rslShaderBody +="\""+val+"\"";
			file<<"ei_shader_param_string(\""<<rslName<<"\", \""<<val<<"\");"<<endl;
		}else if(rslType=="texture"){
			MString val;
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
			rslShaderBody +="\""+val+"\"";
			file<<"ei_shader_param_texture(\""<<rslName<<"\", \""<<val<<"\");"<<endl;
		}else if(rslType=="float"){
			if(rslTypeSize == 1){
				double val;
				IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
				//val(double) --> valStr(string)
				MString valStr;
				valStr.set(val);
				rslShaderBody += valStr;
				file<<"ei_shader_param_scalar(\""<<rslName<<"\", "<<val<<");"<<endl;
			}else{
				rslShaderBody += "{ ";
				MDoubleArray val; val.setLength(rslTypeSize);
				IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
				for(int i=0; i<rslTypeSize; ++i){
					if( i != 0 ){
						rslShaderBody += ", ";
					}
					//val[i](double) --> valStr(string)
					MString valStr;
					valStr.set(val[i]);
					rslShaderBody += valStr;
				}
				rslShaderBody += " }";
			}
		}
		rslShaderBody += ";\n";
	}//if( $connected == 0 )
	// Otherwise, we have a convertible connection, so we'll be
	// adding the variable to the block's header.
	else{
		rslShaderHeader += " ";

		// Note if it's connected as an output.
		if(connected == 2){
			rslShaderHeader += "output ";
			if( rslType=="vector")
			{
				MDoubleArray val; val.setLength(3);
				IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
				//val(double) --> valStr(string)
				MStringArray valStr; valStr.setLength(3);
				valStr[0].set(val[0]);
				valStr[1].set(val[1]);
				valStr[2].set(val[2]);
				rslShaderBody +="("+valStr[0]+","+valStr[1]+","+valStr[2]+")";
				file<<"ei_shader_param_vector(\""<< rslName<<"\", "<<val[0]<<", "<<val[1]<<", "<<val[2]<<");"<<endl;
			}
		}

		// Write out the description.
		rslShaderHeader += ( rslType + " " + rslName );
		if( rslTypeSize != 1 )
		{
			rslShaderHeader += "[]";
		}
		rslShaderHeader += ";\n";

		//
		if(connected == 1)
		{
			MStringArray srcPlug;
			IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+plug+"\"", srcPlug));
			assert(srcPlug.length()==1);
			MStringArray src;
			srcPlug[0].split('.',src);
			MString srcNode(src[0]);
			MString srcAttr(src[1]);
			rslShaderBody +="//"+plug+" <-- "+srcPlug[0]+"\n";

			// if the srcNode is a texture
			if( is2DTexture(srcNode) || is3DTexture(srcNode) ){
				if( is2DFileTexture(srcNode) ){
					MString fileTextureName;
					IfMErrorWarn(MGlobal::executeCommand("getAttr \""+srcNode+".fileTextureName\"", fileTextureName));
					file<<"ei_shader_param_texture(\""<<rslName<<"_tex\", \""<<fileTextureName<<"\");"<<std::endl;
				}else{
					//file<<"ei_shader_param_texture(\""<<rslName<<"_tex\", \""<<srcNode<<"\");"<<std::endl;
					file<<"ei_shader_link_param(\""<<rslName<<"\", \""<<srcNode<<"\", \""<<srcAttr<<"\");"<<std::endl;
				}
			}
			//the srcNode is NOT a texture
			else{
				file<<"ei_shader_link_param(\""<<rslName<<"\", \""<<srcNode<<"\", \""<<srcAttr<<"\");"<<std::endl;
			}
		}

	}//else
}
//
void OutputHelper::addToRSL(const MString& code)
{
	file<<code<<std::endl;
}
//
void OutputHelper::beginRSL (const MString& name)
{
	file<< "ei_shader(\""<<name<<"\");"<<std::endl;
}
//
void OutputHelper::endRSL ()
{
	file<< "ei_end_shader();"<< std::endl;
	file<<std::endl;
}
//////////////////////////////////////////////////////////////////////////
Visitor::Visitor()
{

}
//
Visitor::~Visitor()
{

}
//
void Visitor::preOutput(const char* shaderNodeName)
{
}
void Visitor::outputBegin(const char* startingNode)
{
	file.open( (getShaderDirectory()+startingNode+".erapi").asChar() );
}
void Visitor::outputUpstreamShader(const char* shaderNodeName)
{
	MString nodetype;
	IfMErrorWarn(MGlobal::executeCommand( ("nodeType \""+MString(shaderNodeName)+"\""), nodetype));

	_outputUpstreamShader(shaderNodeName, nodetype.asChar());
}
void Visitor::outputShaderMethod(const char* shaderName,
						const char* shaderMethodVariavles,const char* shaderMethodBody)
{
// 	file << "void " << shaderName << "()\n{\n";
// 	file << shaderMethodVariavles;
// 	file << "\n";
// 	file << shaderMethodBody;
// 	file << "}\n";
}
void Visitor::outputEnd()
{
	file.close();
}
void Visitor::postOutput()
{
}
void Visitor::outputShadingGroup(const char* shadingGroupNode)
{
	MString cmd;

	MStringArray surfaceShaders;
	MStringArray volumeShaders;
	MStringArray displacementShaders;
	MStringArray shadowShaders;
	MStringArray environmentShaders;
	MStringArray photonShaders;
	{
		cmd = "listConnections (\""+MString(shadingGroupNode)+"\" + \".surfaceShader\")";
		IfMErrorWarn(MGlobal::executeCommand( cmd, surfaceShaders));

		cmd = "listConnections (\""+MString(shadingGroupNode)+"\" + \".volumeShader\")";
		IfMErrorWarn(MGlobal::executeCommand( cmd, volumeShaders));

		cmd = "listConnections (\""+MString(shadingGroupNode)+"\" + \".displacementShader\")";
		IfMErrorWarn(MGlobal::executeCommand( cmd, displacementShaders));

		cmd = "listConnections (\""+MString(shadingGroupNode)+"\" + \".liqShadowShader\")";
		IfMErrorWarn(MGlobal::executeCommand( cmd, shadowShaders));
		
		cmd = "listConnections (\""+MString(shadingGroupNode)+"\" + \".liqEnvironmentShader\")";
		IfMErrorWarn(MGlobal::executeCommand( cmd, environmentShaders));

		cmd = "listConnections (\""+MString(shadingGroupNode)+"\" + \".liqPhotonShader\")";
		IfMErrorWarn(MGlobal::executeCommand( cmd, photonShaders));
	}

	// Work out where to put it & make sure the directory exists
	MString shadingGroupFileName(getShaderDirectory()+shadingGroupNode);

	std::ofstream shadingGroupFile;
	shadingGroupFile.open((shadingGroupFileName+".erapi").asChar());
	shadingGroupFile<<"ei_material(\""<<shadingGroupNode<<"\");"<<std::endl;
	if( surfaceShaders[0].length() != 0 ){
		shadingGroupFile<<"ei_add_surface(\""<<surfaceShaders[0].asChar()<<"\");"<<std::endl;
	}
	if( volumeShaders[0].length() != 0 ){
		shadingGroupFile<<"ei_add_volume(\""<<volumeShaders[0].asChar()<<"\");"<<std::endl;
	}
	if( displacementShaders[0].length() != 0 ){
		shadingGroupFile<<"ei_add_displace(\""<<displacementShaders[0].asChar()<<"\");"<<std::endl;
	}
	if( shadowShaders[0].length() != 0 ){
		shadingGroupFile<<"ei_add_shadow(\""<<shadowShaders[0].asChar()<<"\");"<<std::endl;
	}
	if( environmentShaders[0].length() != 0 ){
		shadingGroupFile<<"ei_add_environment(\""<<environmentShaders[0].asChar()<<"\");"<<std::endl;
	}
	if( photonShaders[0].length() != 0 ){
		shadingGroupFile<<"ei_add_photon(\""<<photonShaders[0].asChar()<<"\");"<<std::endl;
	}
	shadingGroupFile<<"ei_end_material();"<<std::endl;
	shadingGroupFile.close();

	
}
//
void Visitor::visitLambert(const char* node)
{
	OutputHelper o(file);

	o.beginRSL(node);

	o.addToRSL("ei_shader_param_string(\"desc\", \"plastic\");");
	o.addRSLVariable("vector", "Cs",		"color",		node);
	o.addRSLVariable("vector", "transparency", "transparency", node);
	o.addRSLVariable("vector", "ambientColor",	"ambientColor", node);
	o.addRSLVariable("vector", "incandescence","incandescence",node);
	o.addRSLVariable("float",  "diffusion",	"diffuse",		node);
	o.addRSLVariable("vector", "outColor",		"outColor",		node);
	o.addRSLVariable("vector", "outTransparency","outTransparency",node);

	o.endRSL();
}
void Visitor::visitBlinn(const char* node)
{
	OutputHelper o(file);

	o.beginRSL(node);

	o.addToRSL("ei_shader_param_string(\"desc\", \"plastic\");");
	o.addRSLVariable("vector", "inColor",		"color",		node);
	o.addRSLVariable("vector", "transparency", "transparency", node);
	o.addRSLVariable("vector", "ambColor",		"ambientColor", node);
	o.addRSLVariable("vector", "incandescence","incandescence",node);
	o.addRSLVariable("float",  "diffusion",	"diffuse",		node);
	o.addRSLVariable("float",  "eccentricity", "eccentricity", node);
	o.addRSLVariable("float",  "specRollOff",	"specularRollOff",node);
	o.addRSLVariable("vector", "specColor",	"specularColor",node);
	o.addRSLVariable("vector", "outColor",		"outColor",		node);

//	MStringArray con;
//	IfMErrorWarn(MGlobal::executeCommand( ("listConnections(\""+MString(node)+"\" + \".reflectedColor\")"), con));
//	if( con.length() != 0 )
//	{
//		o.addRSLVariable( "float", "reflectivity", "reflectivity", node);
//		o.addRSLVariable( "vector", "refColor", "reflectedColor", node);
//		o.addToRSL( "Cspecular += ( reflectivity * refColor );");
//	}

	o.endRSL();
}
//
void Visitor::visitFile(const char* node)
{
	OutputHelper o(file);

	//generate texture and construct texture node
	{
		MString fileImageName;
		IfMErrorWarn(MGlobal::executeCommand("getAttr \""+MString(node)+".fileTextureName\"", fileImageName));
		//test "fileImageName" exist or not.
		if( access(fileImageName.asChar(), 0) != 0){
			liquidMessage2(messageError,"%s not exist!", fileImageName.asChar());
			assert(0&&"image not exist.");
		}

		bool isERTex;//whether fileImageName is ER texture
		{
			std::string fileImageName_(fileImageName.asChar());
			std::size_t i_last_dot = fileImageName_.find_last_of('.');
			if( i_last_dot == std::string::npos ){
				liquidMessage2(messageWarning,"%s has no extention!", fileImageName_.c_str());
				assert(0&&"warrning: texture name has not extention.");
			}
			std::string imgext(fileImageName_.substr(i_last_dot+1));//imgext=tex
			std::transform(imgext.begin(),imgext.end(),imgext.begin(),tolower);
		
			isERTex = (imgext == "tex");
		}

		MString fileTextureName = (isERTex)? fileImageName : (fileImageName+".tex");

		//generate texture
		if ( access(fileTextureName.asChar(), 0) != 0 )//not exist
		{
			o.addToRSL( "ei_make_texture(\""+fileImageName+"\",\""+fileTextureName+"\","+
				"EI_TEX_WRAP_CLAMP, EI_TEX_WRAP_CLAMP, EI_FILTER_BOX, 1.0f, 1.0f)" );
		}
		//construct texture node
		//if (ei_file_exists(fileTextureName))
		{
			o.addToRSL( "ei_texture(\""+fileImageName+"\");" );
			o.addToRSL( "ei_file_texture(\""+fileTextureName+"\", eiFALSE);" );
			o.addToRSL( "ei_end_texture();" );
		}
	}


	o.beginRSL(node);

	o.addToRSL("ei_shader_param_string(\"desc\", \"maya_file\");");
	o.addRSLVariable("vector",  "uvCoord",	"uvCoord",	node);
	o.addRSLVariable("texture", "fileTextureName",	"fileTextureName",	node);
//	o.addRSLVariable("vector", "outColor",	"outColor",	node);
//	o.addToRSL("ei_shader_param_texture(\"fileTextureName\", texturename1)");
	o.endRSL();
}
void Visitor::visitPlace2dTexture(const char* node)
{
	OutputHelper o(file);

	o.beginRSL(node);

	o.addToRSL("ei_shader_param_string(\"desc\", \"maya_place2dTexture\");");
	o.addRSLVariable("vector",  "repeatUV",	"repeatUV",	node);
	o.addRSLVariable("vector",  "outUV",	"outUV",	node);

	o.endRSL();
}
void Visitor::visitChecker(const char* node)
{
	OutputHelper o(file);

	o.beginRSL(node);

	o.addToRSL("ei_shader_param_string(\"desc\", \"maya_checker_uv\");");
	o.addRSLVariable("color",  "color1",	"color1",	node);
	o.addRSLVariable("color",  "color2",	"color2",	node);
	o.addRSLVariable("vector", "uvCoord",	"uvCoord",	node);
	o.addRSLVariable("color",  "outColor",	"outColor",	node);

	o.endRSL();
}

}//namespace ER