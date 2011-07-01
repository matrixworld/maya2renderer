/*---------------------------------------------------------------------------------------------
    This file is part of Luna - the elvish ray renderer plug-in for Maya.
    Copyright (C) 2010  yaoyansi

    Luna is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Luna is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
---------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------

File		:	MayaConnection.cpp
Classes		:	
Description	:	
Comments	:	
History		:	Created by yaoyansi, 2010.04.16.

---------------------------------------------------------------------------------------------*/
#include "MayaConnection.h"
#include "../log/prerequest_std.h"
//#include "../log/prerequest_local.h"
#include "../log/mayacheck.h"

typedef float ChannelType;

void color128to64( Vector3f & color, ChannelType &r, ChannelType &g, ChannelType &b, ChannelType &a )
{
	clamp( color.r, 0.0f, 1.0f );
	clamp( color.g, 0.0f, 1.0f );
	clamp( color.b, 0.0f, 1.0f );
	r = ( 255.0f * color.r );
	g = ( 255.0f * color.g );
	b = ( 255.0f * color.b );
	a = 255.0f;
}

void MayaConnection::Print( const int code, const int severity, const char *message )
{
	//_LogFunctionCall("MayaConnection::Print()");

	static const int	NUM_SEVERITY_LEVELS = 5;
	static const char	*severity_strings[ NUM_SEVERITY_LEVELS ] = { "FATAL",
		"ERROR",
		"WARNING",
		"PROGRESS",
		"DEBUG" };

	if( severity < 1 || severity > NUM_SEVERITY_LEVELS )
		return;

	std::cout << severity_strings[severity - 1] << " : " << code << " : " << message << std::endl;
}
//
bool MayaConnection::Progress( const float percent )
{
	//_LogFunctionCall("MayaConnection::Progress(" << percent << " %)");

	return true;
}
//
void MayaConnection::ClearTile( const int left, const int right, 
									const int top, const int bottom )
{
	//_LogFunctionCall("MayaConnection::ClearTile()");
	if( !isInteractiveRenderingMode() )
		return;

}
//
void setPixel(RV_PIXEL*pixels, 
			const unsigned int tile_width, const unsigned int tile_height, 
			const int ix, const int iy, 
			const ChannelType r, const ChannelType g, const ChannelType b, const ChannelType a)
{
	unsigned int offset = iy*tile_width + ix;
	RV_PIXEL *target = pixels + offset;
	target->r = r;
	target->g = g;
	target->b = b;
	target->a = a;
}
void setPixel(RV_PIXEL*pixels, 
			  const unsigned int tile_width, 
			  const unsigned int tile_height, 
			  const int ix, const int iy, 
			  const RV_PIXEL &color )
{
	setPixel(pixels, tile_width, tile_height, ix, iy, color.r, color.g, color.b, color.a );
}

// Note:
// the tile of elvishray range from [left right) to [top bottom)
void MayaConnection::UpdateTile( e_FrameBufferCache *colorFrameBuffer, 
									 e_FrameBufferCache *opacityFrameBuffer, 
									 std::vector< e_FrameBufferCache * > *frameBuffers, 
									 const int left, const int right, 
									 const int top, const int bottom )
{
	//_LogFunctionCall("MayaConnection::UpdateTile("<<left<<","<<right<<","<<top<<","<<bottom<<")");
	MStatus status;

	if( !isInteractiveRenderingMode() )
		return;

	unsigned int tile_width  = right - left;
	unsigned int tile_height = bottom - top;
	
	assert( tile_width>=1 && tile_height>=1 );
	//_LogDebug("tile<"<<tile_width <<", "<< tile_height<<">, w*h="<< tile_width*tile_height);

	
	RV_PIXEL* pixels = new RV_PIXEL[(tile_width)*(tile_height)];

	unsigned int index = 0;
	for( int j = 0; j < colorFrameBuffer->get_height(); ++j )
	{
		Vector3f	*color_scanline = (Vector3f *) colorFrameBuffer->get_scanline( j );
		//Vector3f	*alpha_scanline = (Vector3f *) opacityFrameBuffer->get_scanline( j );

		for( int i = 0; i < colorFrameBuffer->get_width(); ++i )
		{
			ChannelType r,g,b,a;
			color128to64(color_scanline[ i], r,g,b,a);
			
			setPixel(pixels, tile_width, tile_height,
				i, tile_height-j-1,
				r,g,b,a);
			index++;
		}
	}
// 	_LogDebug("frame buffer<"<<colorFrameBuffer->get_width()<<","<<colorFrameBuffer->get_height()<<">" );
// 	_LogDebug("width*height = "<<colorFrameBuffer->get_width() * colorFrameBuffer->get_height() );
// 	_LogDebug("index = "<< index);

	unsigned int min_x, min_y, max_x, max_y;
#if ELIMINATE_OFFSET
	//motivation:
	// image A : the scene rendered by elvishray;
	// image B : the scene renderer by Maya software;
	// A has a translate offset (0,1) relative to B under Maya window coordinate.
	// to eliminate this offset, we set:
	// ( see ermaya\test\discontinuesPixel2_er.jpg and discontinuesPixel2_maya.jpg)
	// int _min_x = left;
	// int _min_y = height - (bottom - 1)-1;
	// to translate image A by (0, -1),
	
	// but, the first column will not be updated correctly, because their _min_x and _min_y are -1.
	// So, we clamp min_x and min_y greater than 0:
	// min_x = _min_x>=0 ? _min_x : 0;
	// min_y = _min_y>=0 ? _min_y : 0;
	
	// But we fail again, because the pixels on the edge of column 0 and column 1 are not continuous any more.
	// (see test\discontinuesPixel.jpg for details)
	int _min_x = left-1;
	int _min_y = height - (bottom - 1)-1;

	min_x = _min_x>=0 ? _min_x : 0;
	min_y = _min_y>=0 ? _min_y : 0;
#else
	//Note:
	// image A : the scene rendered by elvishray;
	// image B : the scene renderer by Maya software;
	// A has a translate offset (0,1) relative to B under Maya window coordinate.
	// see ermaya\test\discontinuesPixel2_er.jpg and discontinuesPixel2_maya.jpg
	min_x = left;
	min_y = height - (bottom - 1);
#endif

	max_x = min_x + tile_width;
	max_y = min_y + tile_height;

	// Send the data to the render view.
	if ( (status = MRenderView::updatePixels(min_x, max_x-1, min_y, max_y-1, pixels)) != MS::kSuccess)
	{
		IfErrorWarn(status);
		//_LogError( "MayaConnection: error occured in updatePixels." );
		delete [] pixels;
		return ;
	}
	delete [] pixels;
	// Force the Render View to refresh the display of the affected region.
	if ( (status = MRenderView::refresh(min_x, max_x-1, min_y, max_y-1)) != MS::kSuccess)
	{
		IfErrorWarn(status);
		//_LogError( "MayaConnection: error occured in refresh." );
		return ;
	}
}
//
void MayaConnection::DrawPixel( const int x, const int y, const Vector3f & color )
{	
	//_LogFunctionCall("MayaConnection::DrawPixel()");

	if( !isInteractiveRenderingMode() )
		return;
}
//
void MayaConnection::UpdateSubWindow( const int left, const int right, 
										  const int top, const int bottom )
{
	//_LogFunctionCall("MayaConnection::UpdateSubWindow()");

	if( !isInteractiveRenderingMode() )
		return;
}

MStatus MayaConnection::startRender( unsigned int w, unsigned int h,
					bool doNotClearBackground ,
					bool immediateFeedback)
{
	//_LogFunctionCall("MayaConnection::startRender()");
	width  = w;
	height = h;
	return MRenderView::startRender( width, height, doNotClearBackground, immediateFeedback);
}
//
MStatus MayaConnection::endRender()
{
	//_LogFunctionCall("MayaConnection::endRender()");
	width  = 0;
	height = 0;
	return MRenderView::endRender();
}
//
//
MayaConnection* MayaConnection::m_instance = NULL;
MayaConnection::MayaConnection()
{
	//_LogFunctionCall("MayaConnection::MayaConnection()");
}
//
MayaConnection::~MayaConnection()
{
	//_LogFunctionCall("MayaConnection::~MayaConnection()");
}
//
MayaConnection* MayaConnection::getInstance()
{
	//_LogFunctionCall("MayaConnection::getInstance()");
	if(!m_instance){
		m_instance = new MayaConnection();
	}
	return m_instance;
}
//
void MayaConnection::delInstance()
{
	//_LogFunctionCall("MayaConnection::delInstance()");
	if(m_instance){
		delete m_instance;
		m_instance = NULL;
	}
}
//
bool MayaConnection::isInteractiveRenderingMode()
{
	return MRenderView::doesRenderEditorExist();
}