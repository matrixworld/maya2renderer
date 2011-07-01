#include "er_renderer.h"

#include "boost/format.hpp"

// Maya headers
#include "../log/prerequest_maya.h"
// Liquid headers
#include <liquid.h>
#include <liqRibHT.h>
#include <liqRenderer.h>
#include <liqRibLightData.h>
#include <liqGlobalHelpers.h>
#include <liqProcessLauncher.h>
#include <liqCustomNode.h>
#include <liqShaderFactory.h>
#include <liqExpression.h>
#include <liqRenderScript.h>
#include <liqGlobalVariable.h>
#include <liqJobScriptMgr.h>
#include <liqFrameScriptMgr.h>
#include <liqLightMgr.h>
#include <liqLocatorMgr.h>
#include <liqShadowRibWriterMgr.h>
#include <liqHeroRibWriterMgr.h>
#include <liqRibCamera.h>
#include <liqRibTranslator.h>

#include "../renderermgr.h"
#include "../log/mayacheck.h"
#include "ercall.h"
namespace elvishray
{
#define _s( _log_ ) dummy.get()<< _log_ <<std::endl;

#define _t( _log_or_call_ )  \
	if(0) dummy.get()<< _log_or_call_ <<std::endl; \
	else  dummy.get()<< #_log_or_call_ <<";"<<std::endl; _log_or_call_ ; 

// #define _S(_call_)  \
// 	dummy.get()<<#_call_<<std::endl; my_##_call_ ; 
#define _S(_call_)  my_##_call_ ; 

	//static const char *LogName="d:/script.er";
	/*static const*/ Renderer dummy; 
	// the only goal to define a Renderer variable is 
	// to run the constructor to register this renderer.
	static const char *RendererName="elvishray";

	Renderer::Renderer()
	{

		liquid::RendererMgr::getInstancePtr()->registerRenderer(
			RendererName, this
			);
	}
	//
	Renderer::~Renderer()
	{

	}
	//
	void Renderer::test()
	{
		liquidMessage2(messageInfo, "this is %s.", RendererName );

	}
	//
	void Renderer::setRenderScriptFormatAndCompress(const bool bbinary, const bool bcompress)
	{
		//todo...
	}
	//
	MStatus Renderer::worldPrologue(const structJob& currentJob)
	{

		_s("\n//worldPrologue");



		return MS::kSuccess;
	}
	//
	MStatus Renderer::worldEpilogue()
	{
		_s("\n//Renderer::worldEpilogue()");

		return MS::kSuccess;
	}
	//
	MStatus Renderer::exportLight(const liqRibNodePtr& ribNode, const structJob &currentJob)
	{
		RtString RibNodeName = getLiquidRibName( ribNode->name.asChar() );
		_s("\n// Renderer::exportLight(\""+std::string(RibNodeName)+"\");");
		
		ribNode->object(0)->writeObject("", currentJob);//call liqRibLightData::_write(...)
		ribNode->object(0)->written = 1;
// 		if( liqglo.m_illuminateByDefault )
// 			RiIlluminate( ribNode->object(0)->lightHandle(), 1 );
// 		else
// 			RiIlluminate( ribNode->object(0)->lightHandle(), 0 );
		return MS::kSuccess;
	}
	void Renderer::openLog()
	{
		m_log.open("d:/script.er");
	}
	void Renderer::closeLog()
	{
		m_log.close();
	}

	liqLightHandle Renderer::exportShadowPassLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqString & shadowname,
		const liqMatrix &t)
	{
		_s("\n// Renderer::exportShadowPassLight()");
		_s("ei_shader( \"shadowlight\", \"intensity\", 10.0)");

		_s("ei_light("<<shadertype<<")");
		_s("ei_lightsource(	\"shadowlight\", ei_end )");
		_S( ei_transform( t[0][0],t[0][1],t[0][2],t[0][3],  t[1][0],t[1][1],t[1][2],t[1][3],  t[2][0],t[2][1],t[2][2],t[2][3],  t[3][0],t[3][1],t[3][2],t[3][3]) );
		_s("ei_end_light()");

		_s("ei_instance( "<<shaderinstance<<" );");
		_s("ei_element(	"<<shadertype<<" );");
		_s("ei_end_instance();");

		return (liqLightHandle)(0);
	}
	liqLightHandle Renderer::exportAmbientLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqFloat & intensity,
		const liqColor & color,
		const liqMatrix &t)
	{
		_s("\n// Renderer::exportAmbientLight()");
		_s("ei_shader( \"ambientlight\", ");
		_s("           \"intensity\", ToString<liqFloat>(intensity),");
		_s("           \"lightcolor\", ToString<liqColor>(color)");
		_s(");");

		_s("ei_light( "<<shadertype<<" );");
		_s("ei_lightsource(	ambientlight, ei_end );");
		_S( ei_transform( t[0][0],t[0][1],t[0][2],t[0][3],  t[1][0],t[1][1],t[1][2],t[1][3],  t[2][0],t[2][1],t[2][2],t[2][3],  t[3][0],t[3][1],t[3][2],t[3][3]) );

		_s("ei_end_light();");

		_s("ei_instance( "<<shaderinstance<<" );");
		_s("ei_element(	"<<shadertype<<" );");
		_s("ei_end_instance();");

		return (liqLightHandle)(0);
	}

	void Renderer::transform_(const liqMatrix& transform)
	{

	}
	MStatus Renderer::liqRibLightData_write(const liqRibLightData *lightdata, const structJob &currentJob)
	{
		return MS::kSuccess;
	}
	liqLightHandle Renderer::exportDistantLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqFloat &i_intensity,
		const liqColor &i_lightcolor,
		const liqString &i_shadowname,
		const liqFloat &i_shadowbias,
		const liqFloat &i_shadowblur,
		const liqFloat &i_shadowsamples,
		const liqFloat &i_shadowfiltersize,
		const liqColor &i_shadowcolor,
		const liqFloat  &i_lightID,
		const liqString &o_category,
		const liqFloat &o_shadowF,
		const liqColor &o_shadowC,
		const liqColor &o_unshadowed_Cl,
		const liqFloat &o_nondiffuse,
		const liqFloat &o_nonspecular,
		const liqMatrix &t)
	{
		_s("// Renderer::exportDistantLight()");
		_s("ei_shader( \"distantlight\", ");
		_s("           \"intensity\", ToString<liqFloat>(intensity),");
		_s("           \"lightcolor\", ToString<liqColor>(color)");
		_s(");");

		_s("ei_light( "<<shadertype<<" );");
		_s("ei_lightsource(	distantlight, ei_end );");
		_S( ei_transform( t[0][0],t[0][1],t[0][2],t[0][3],  t[1][0],t[1][1],t[1][2],t[1][3],  t[2][0],t[2][1],t[2][2],t[2][3],  t[3][0],t[3][1],t[3][2],t[3][3]) );
		_s("ei_end_light();");

		_s("ei_instance( "<<shaderinstance<<" );");
		_s("ei_element(	"<<shadertype<<" );");
		_s("ei_end_instance();");

		return (liqLightHandle)(0);
	}
	//
	liqLightHandle Renderer::exportPointLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqFloat &i_intensity,
		const liqColor &i_lightcolor,
		const liqFloat &i_decay,
		const liqString &i_shadownamepx,
		const liqString &i_shadownamenx,
		const liqString &i_shadownamepy,
		const liqString &i_shadownameny,
		const liqString &i_shadownamepz,
		const liqString &i_shadownamenz,
		const liqFloat &i_shadowbias,
		const liqFloat &i_shadowblur,
		const liqFloat &i_shadowsamples,
		const liqFloat &i_shadowfiltersize,
		const liqColor &i_shadowcolor,
		const liqFloat  &i_lightID,
		const liqString &o_category,
		const liqFloat &o_shadowF,
		const liqColor &o_shadowC,
		const liqColor &o_unshadowed_Cl,
		const liqFloat &o_nondiffuse,
		const liqFloat &o_nonspecular,
		const liqMatrix &t)
	{
		_s("\n// Renderer::exportPointLight()");
		_s("ei_shader( \"pointlight\", ");
		_s("           \"intensity\", ToString<liqFloat>(intensity),");
		_s("           \"lightcolor\", ToString<liqColor>(color)");
		_s(");");

		_s("ei_light( "<<shadertype<<" );");
		_s("ei_lightsource(	pointlight, ei_end );");
		_S( ei_transform( t[0][0],t[0][1],t[0][2],t[0][3],  t[1][0],t[1][1],t[1][2],t[1][3],  t[2][0],t[2][1],t[2][2],t[2][3],  t[3][0],t[3][1],t[3][2],t[3][3]) );
		_s("ei_end_light();");

		_s("ei_instance( "<<shaderinstance<<" );");
		_s("ei_element(	"<<shadertype<<" );");
		_s("ei_end_instance();");

		return (liqLightHandle)(0);
	}
	//
	liqLightHandle Renderer::exportSpotLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqFloat &i_intensity,
		const liqColor &i_lightcolor,
		const liqFloat &i_coneangle,
		const liqFloat &i_penumbraangle,
		const liqFloat &i_dropoff,
		const liqFloat &i_decay,

		const liqFloat &i_barndoors,
		const liqFloat &i_leftbarndoor            ,
		const liqFloat &i_rightbarndoor           ,
		const liqFloat &i_topbarndoor             ,
		const liqFloat &i_bottombarndoor          ,

		const liqFloat &i_decayRegions            ,
		const liqFloat &i_startDistance1          ,
		const liqFloat &i_endDistance1            ,
		const liqFloat &i_startDistance2          ,
		const liqFloat &i_endDistance2            ,
		const liqFloat &i_startDistance3          ,
		const liqFloat &i_endDistance3            ,
		const liqFloat &i_startDistanceIntensity1,
		const liqFloat &i_endDistanceIntensity1,
		const liqFloat &i_startDistanceIntensity2,
		const liqFloat &i_endDistanceIntensity2,
		const liqFloat &i_startDistanceIntensity3,
		const liqFloat &i_endDistanceIntensity3,

		const liqString &i_shadowname,
		const liqFloat &i_shadowbias,
		const liqFloat &i_shadowblur,
		const liqFloat &i_shadowsamples,
		const liqFloat &i_shadowfiltersize,
		const liqColor &i_shadowcolor,
		const liqColor &i_shadowcolorsurf,
		const liqFloat &i_shadowcolormix,

		const liqFloat  &i_lightID,
		const liqString &i_category,
		const liqColor &o_shadowC,
		const liqFloat &o_shadowF,
		const liqFloat &o_barn,
		const liqColor &o_unshadowed_Cl,
		const liqFloat &o_nondiffuse,
		const liqFloat &o_nonspecular,
		const liqMatrix &t)
	{
		_s("\n// Renderer::exportSpotLight()");
		_s("ei_shader( \"spotlight\", ");
		_s("           \"intensity\", ToString<liqFloat>(intensity),");
		_s("           \"lightcolor\", ToString<liqColor>(color)");
		_s(");");

		_s("ei_light( "<<shadertype<<" );");
		_s("ei_lightsource(	spotlight, ei_end );");
		_S( ei_transform( t[0][0],t[0][1],t[0][2],t[0][3],  t[1][0],t[1][1],t[1][2],t[1][3],  t[2][0],t[2][1],t[2][2],t[2][3],  t[3][0],t[3][1],t[3][2],t[3][3]) );
		_s("ei_end_light();");

		_s("ei_instance( "<<shaderinstance<<" );");
		_s("ei_element(	"<<shadertype<<" );");
		_s("ei_end_instance();");

		return (liqLightHandle)(0);
	}
	//
	liqLightHandle Renderer::exportAreaLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqFloat &i_intensity,
		const liqColor &i_lightcolor,
		const liqFloat &i_decay,
		const liqString  &i_coordsys,
		const liqFloat   &i_lightsamples,
		const liqFloat &i_doublesided,
		const liqString &i_shadowname,
		const liqColor &i_shadowcolor,
		const liqString &i_hitmode,
		const liqString &i_lightmap,
		const liqFloat &i_lightmapsaturation,
		const liqFloat  &i_lightID,
		const liqString &i_category,
		const liqFloat &o_nonspecular,
		const liqFloat &o_shadowF,
		const liqColor &o_shadowC,
		const liqColor &o_unshadowed_Cl,
		const liqFloat &o_arealightIntensity,
		const liqColor &o_arealightColor,
		const liqMatrix &t)
	{
		_s("\n// Renderer::exportAreaLight()");
		_s( "ei_shader( \"arealight\", ");
		_s("           \"intensity\", ToString<liqFloat>(intensity),");
		_s("           \"lightcolor\", ToString<liqColor>(color)");
		_s(");");

		_s("ei_light( "<<shadertype<<" );");
		_s("ei_lightsource(	arealight, ei_end );");
		_S( ei_transform( t[0][0],t[0][1],t[0][2],t[0][3],  t[1][0],t[1][1],t[1][2],t[1][3],  t[2][0],t[2][1],t[2][2],t[2][3],  t[3][0],t[3][1],t[3][2],t[3][3]) );
		_s("ei_end_light();");

		_s("ei_instance( "+shaderinstance+" );");
		_s("ei_element(	"+shadertype+" );");
		_s("ei_end_instance();");

		return (liqLightHandle)(0);
	}
	//
	void Renderer::exportOneObject(
		const liqRibNodePtr *lightdata, 
		const structJob &currentJob
		)
	{

	}
	void Renderer::exportOneGeometry_Mesh(
		const liqRibMeshData *mesh, 
		const structJob &currentJob
		)
	{
		_s("\n// Renderer::exportOneGeometry_Mesh("<<mesh->getName()<<")");
		MStatus status;
		MFnMesh fnMesh(mesh->objDagPath, &status);
		IfMErrorWarn(status);

		MFloatPointArray position;
		IfMErrorWarn(fnMesh.getPoints (position, MSpace::kWorld));

		MIntArray triangleCounts,triangleVertices;
		IfMErrorWarn(fnMesh.getTriangles(triangleCounts, triangleVertices));

		// geometry data (shape)
		_s("\n");
		_s("ei_object(\""<< mesh->getName()<<"\")");
		_s("//### vertex positions");
		for(size_t i=0; i<position.length(); ++i)
		{
			_s("ei_vector("<<position[i][0]<<","<<position[i][1]<<","<<position[i][2]<<")");
		}
		_s("//### vertex color");
		for(int i=0; i<position.length(); i++)
		{
			_s("ei_vertex("<<i<<")");
			_s("ei_variable( et_color, \"Cs\", color( 0.0f, 0.0f, 0.0f ) )");
		}
		_s("//### triangles");
		for(size_t i=0; i<triangleVertices.length(); i=i+3)
		{
			_s("ei_triangle("
				<<0<<","
				<<triangleVertices[i  ]<<","
				<<triangleVertices[i+1]<<"," 
				<<triangleVertices[i+2]<<")");
		}
		_s("ei_end_object()");

		_s("ei_instance("<<"dummy"<<")");//transform
// 		ei_visible( on );
// 		ei_shadow( on );
// 		ei_trace( on );
//		ei_set_material( "mtl", ei_end );
		_s("ei_element(\""<<mesh->getName()<<"\")");
// 		if( motion_transform )
// 		{
// 			ei_transform( 1.0f, 0.0f, 0.0f, 0.0f, 
// 				0.0f, 1.0f, 0.0f, 0.0f, 
// 				0.0f, 0.0f, 1.0f, 0.0f, 
// 				0.0f, 0.0f, 0.0f, 1.0f );
// 			// 			ei_motion_transform( 1.0f, 0.0f, 0.0f, 0.0f, 
// 			// 				0.0f, 1.0f, 0.0f, 0.0f, 
// 			// 				0.0f, 0.0f, 1.0f, 0.0f, 
// 			// 				2.3f, 2.0f, 3.1f, 1.0f );
// 		}
// 		if( motion_transform || motion_deform )
// 		{
// 			_s("ei_motion( on )");
// 		}
		_s("ei_end_instance();");
	}
	//
	void Renderer::ribPrologue_comment(const char* liqversion, 
		const char* scenename, const char* user, const time_t &now)
	{
		// general info for traceability
		//
		_s("//    Generated by Liquid v"<<liqversion );
		_s("//    Scene : "<< scenename );

		if( user )
			_s("//    User  : "<< user );

		_s("//    Time  : "<< ctime(&now) );

	}
	//
	MStatus Renderer::ribPrologue_begin(const structJob& currentJob)
	{
		if( true )
		{
			_s("ei_make_texture("<<currentJob.imageName<<","<<currentJob.texName<<", EI_TEX_WRAP_CLAMP, EI_TEX_WRAP_CLAMP, EI_FILTER_BOX, 1.0f, 1.0f );");
		}	
		_s("//### SCENE BEGIN ###");
		_S( ei_scene() );

		_s("ei_set_connection(MayaConnection::getInstance());");

		//ei_verbose(	EI_VERBOSE_ALL );
		_s("ei_link( \"dso\" );");
		return MS::kSuccess;
	}
	MStatus Renderer::ribPrologue_end(const structJob& currentJob)
	{ 
		_s("ei_end_scene();");
		return MS::kSuccess;
	}
	//
	MStatus Renderer::ribPrologue_options(const structJob& currentJob)
	{
		_s("\n//###option");
		_s("ei_options( \"opt\" );");
		//ei_incremental_options( const char *name );

		//	Sampling Quality:
		_s("ei_contrast(  0.05f, 0.05f, 0.05f, 0.05f );");
		if( currentJob.isShadow ){
			_s("ei_samples( "<<currentJob.shadowPixelSamples<<","<<currentJob.shadowPixelSamples<<" );");
			_s("ei_filter( EI_FILTER_BOX, 1 );");
			_s("ei_shading_rate( "<<currentJob.shadingRateFactor<<" );");

		}else{
			_s("ei_samples(0,2);");//_s("ei_samples("<< liqglo.pixelSamples<<","<<liqglo.pixelSamples<<");");//4,4
			_s("ei_filter( EI_FILTER_GAUSSIAN, 3.0f );");
			_s("ei_shading_rate( "<<liqglo.shadingRate <<");");
		}
//		_s("ei_bucket_size( int size );");
		

		//	Tessellation Quality:
// 		_s("ei_shading_rate( float rate );");
// 		_s("ei_motion_factor( float factor );");
// 		_s("ei_max_displace( float dist );");

		//	Motion Blur:
//		_s("ei_shutter( float open, float close );");
		_s("ei_motion("<<(liqglo.liqglo_doMotion? 1:0)<<");");
//		_s("ei_motion_segments( int num );");

		//	Trace Depth:
		_s("ei_trace_depth(	2, 2, 2  );");

		//	Shadow:
//		_s("ei_shadow( int type );");

		//	Rendering Algorithms:
// 		_s("ei_trace( int type );");
// 		_s("ei_scanline( int type );");
// 		_s("ei_hardware( int type );");
// 		_s("ei_acceleration( int type );");
// 		_s("ei_simd( int type );");
// 		_s("ei_bsp_size( int size );");
// 		_s("ei_bsp_depth( int depth );");
// 		_s("ei_bsp_memory( int size );");

		//	Feature Disabling:
// 		_s("ei_lens( int type );");
// 		_s("ei_volume( int type );");
// 		_s("ei_geometry( int type );");
// 		_s("ei_displace( int type );");
// 		_s("ei_imager( int type );");

		//	Caustics:
// 		_s("ei_caustic( int type );");
// 		_s("ei_caustic_photons( int photons );");
// 		_s("ei_caustic_accuracy( int samples, float radius );");
// 		_s("ei_caustic_scale( float r, float g, float b );");
// 		_s("ei_caustic_filter( int filter, float filter_const );");
// 		_s("ei_photon_trace_depth( int reflect, int refract, int sum );");
// 		_s("ei_photon_decay( float decay );");
// 
		//	Global Illumination:
// 		_s("ei_globillum( int type );");
// 		_s("ei_globillum_photons( int photons );");
// 		_s("ei_globillum_accuracy( int samples, float radius );");
// 		_s("ei_globillum_scale( float r, float g, float b );");
// 		_s("ei_photonvol_accuracy( int samples, float radius );");
// 		_s("ei_finalgather( int type );");
// 		_s("ei_finalgather_precompute( int type );");
// 		_s("ei_finalgather_accuracy( int rays, int samples, float density, float radius );");
// 		_s("ei_finalgather_falloff( int type );");
// 		_s("ei_finalgather_falloff( float start, float stop );");
// 		_s("ei_finalgather_filter( float size );");
// 		_s("ei_finalgather_trace_depth( int reflect, int refract, int diffuse, int sum );");
// 		_s("ei_finalgather_scale( float r, float g, float b );");

		//	Frame Buffer Control:
// 		_s("ei_exposure( float gain, float gamma );");
// 		_s("ei_quantize( float one, float _min, float _max, float dither_amplitude );");
// 		_s("ei_frame_buffer( const char *name, int datatype, int interpolation );");

		//	Miscellaneous:
		_s("ei_face( EI_FACE_BOTH );");
//		_s("ei_memory( int size );");
		_s("ei_ambient( 0.12f, 0.13f, 0.05f );");

		_s("ei_end_options();");

		return MStatus::kSuccess;
	}
}//namespace
