/****************************************************************************************

Copyright (C) 2015 Autodesk, Inc.
All rights reserved.

Use of this software is subject to the terms of the Autodesk license agreement
provided at the time of installation or download, or which otherwise accompanies
this software in either electronic or hard copy form.

****************************************************************************************/

#include "SceneCache.h"

namespace
{
    const float ANGLE_TO_RADIAN = 3.1415926f / 180.f;
    const GLfloat BLACK_COLOR[] = {0.0f, 0.0f, 0.0f, 1.0f};
    const GLfloat GREEN_COLOR[] = {0.0f, 1.0f, 0.0f, 1.0f};
    const GLfloat WHITE_COLOR[] = {1.0f, 1.0f, 1.0f, 1.0f};
    const GLfloat WIREFRAME_COLOR[] = {0.5f, 0.5f, 0.5f, 1.0f};

    const int TRIANGLE_VERTEX_COUNT = 3;

    // Four floats for every position.
    const int VERTEX_STRIDE = 4;
    // Three floats for every normal.
    const int NORMAL_STRIDE = 3;
    // Two floats for every UV.
    const int UV_STRIDE = 2;

    const GLfloat DEFAULT_LIGHT_POSITION[] = {0.0f, 0.0f, 0.0f, 1.0f};
    const GLfloat DEFAULT_DIRECTION_LIGHT_POSITION[] = {0.0f, 0.0f, 1.0f, 0.0f};
    const GLfloat DEFAULT_SPOT_LIGHT_DIRECTION[] = {0.0f, 0.0f, -1.0f};
    const GLfloat DEFAULT_LIGHT_COLOR[] = {1.0f, 1.0f, 1.0f, 1.0f};
    const GLfloat DEFAULT_LIGHT_SPOT_CUTOFF = 180.0f;
}

VBOMesh::VBOMesh() : mHasNormal(false), mHasUV(false), mAllByControlPoint(true)
{
    // Reset every VBO to zero, which means no buffer.
    for (int lVBOIndex = 0; lVBOIndex < VBO_COUNT; ++lVBOIndex)
    {
        mVBONames[lVBOIndex] = 0;
    }
}

VBOMesh::~VBOMesh()
{
    // Delete VBO objects, zeros are ignored automatically.
//    glDeleteBuffers(VBO_COUNT, mVBONames);
	
	//for(int i=0; i < mSubMeshes.GetCount(); i++)
	//{
	//	delete mSubMeshes[i];
	//}
	//
	//mSubMeshes.Clear();

}

MaterialCache::MaterialCache()
{

}

MaterialCache::~MaterialCache()
{

}

LightCache::LightCache() 
{
}

LightCache::~LightCache()
{
  //  glDisable(mLightIndex);
}

