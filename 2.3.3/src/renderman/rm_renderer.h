#ifndef RM_Renderer_H
#define RM_Renderer_H

#include "../rendererinterface.h"


namespace renderman
{
	class Renderer : public liquid::RendererInterface
	{
	public:
		Renderer();
		virtual ~Renderer();
		virtual void test();

		virtual void setRenderScriptFormatAndCompress(const bool bbinary, const bool bcompress);
		
//		virtual MStatus ribPrologue(const structJob& currentJob);
		virtual MStatus ribPrologue_begin(const structJob& currentJob);
		virtual void ribPrologue_comment(const char* liqversion, 
			const char* scenename, const char* user, const time_t &time);
		virtual MStatus ribPrologue_options(const structJob& currentJob);
		virtual void ribPrologue_writeSearthPath();
//		virtual MStatus framePrologue(const long scanTime,const structJob& currentJob);
		virtual MStatus worldPrologue(const structJob& currentJob);
//		virtual MStatus lightBlock(const structJob& currentJob);
//		virtual MStatus coordSysBlock(const structJob& currentJob);
//		virtual MStatus objectBlock(const structJob& currentJob);
		virtual MStatus worldEpilogue();
//		virtual MStatus frameEpilogue(const long scanTime);
		virtual MStatus ribPrologue_end(const structJob& currentJob);
//		virtual MStatus ribEpilogue();



		virtual void openLog();
		virtual void closeLog();

		//
		// Light
		//
		virtual MStatus exportLight(const liqRibNodePtr& light, const structJob &currentJob);
		virtual MStatus liqRibLightData_write(const liqRibLightData *lightdata, const structJob &currentJob);
		virtual liqLightHandle exportShadowPassLight(
			const std::string &shadertype, 
			const std::string &shaderinstance, 
			const liqString & shadowname,
			const liqMatrix &transform);
		virtual liqLightHandle exportAmbientLight(
			const std::string &shadertype, 
			const std::string &shaderinstance, 
			const liqFloat & intensity,
			const liqColor & color,
			const liqMatrix &transform);
		virtual liqLightHandle exportDistantLight(
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
			const liqMatrix &transform);
		virtual liqLightHandle exportPointLight(
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
			const liqMatrix &transform);
		virtual liqLightHandle exportSpotLight(
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
			const liqMatrix &transform);
		virtual liqLightHandle exportAreaLight(
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
			const liqMatrix &transform);
		virtual void transform_(const liqMatrix& transform);
		//
		//
		//
		virtual void exportOneObject(
			const liqRibNodePtr *lightdata, 
			const structJob &currentJob
			);
		virtual void exportOneGeometry_Mesh(
			const liqRibMeshData *mesh, 
			const structJob &currentJob
			);

	protected:
		Renderer(const Renderer&);
		Renderer& operator=(const Renderer&);

		void _RiOption_format_compress(bool bBinary, bool bCompress);
	
	private:

	};
}



#endif//RM_Renderer_H