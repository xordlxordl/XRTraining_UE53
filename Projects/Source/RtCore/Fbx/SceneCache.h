/****************************************************************************************

Copyright (C) 2015 Autodesk, Inc.
All rights reserved.

Use of this software is subject to the terms of the Autodesk license agreement
provided at the time of installation or download, or which otherwise accompanies
this software in either electronic or hard copy form.

****************************************************************************************/

#ifndef _SCENE_CACHE_H
#define _SCENE_CACHE_H

#include "fbxsdk/core/base/fbxarray.h"
//#include "GlFunctions.h"
typedef unsigned int GLuint;
typedef float GLfloat;

// Save mesh vertices, normals, UVs and indices in GPU with OpenGL Vertex Buffer Objects
class VBOMesh
{
public:
    VBOMesh();
    ~VBOMesh();

private:
    enum
    {
        VERTEX_VBO,
        NORMAL_VBO,
        UV_VBO,
        INDEX_VBO,
        VBO_COUNT,
    };

    // For every material, record the offsets in every VBO and triangle counts
    struct SubMesh
    {
        SubMesh() : IndexOffset(0), TriangleCount(0) {}

        int IndexOffset;
        int TriangleCount;
    };

    GLuint mVBONames[VBO_COUNT];
//    FbxArray<SubMesh*> mSubMeshes;
    bool mHasNormal;
    bool mHasUV;
    bool mAllByControlPoint; // Save data in VBO by control point or by polygon vertex.
};

// Cache for FBX material
class MaterialCache
{
public:
    MaterialCache();
    ~MaterialCache();

private:
    struct ColorChannel
    {
        ColorChannel() : mTextureName(0)
        {
            mColor[0] = 0.0f;
            mColor[1] = 0.0f;
            mColor[2] = 0.0f;
            mColor[3] = 1.0f;
        }

        GLuint mTextureName;
        GLfloat mColor[4];
    };
    ColorChannel mEmissive;
    ColorChannel mAmbient;
    ColorChannel mDiffuse;
    ColorChannel mSpecular;
    GLfloat mShinness;
};

// Property cache, value and animation curve.
struct PropertyChannel
{
    PropertyChannel(){}
    char * mAnimCurve;
    GLfloat mValue;
};

// Cache for FBX lights
class LightCache
{
public:
    LightCache();
    ~LightCache();
	enum EType
	{
		ePoint,
		eDirectional,
		eSpot,
		eArea,
		eVolume
	};
private:
    static int sLightCount;         // How many lights in this scene.

    GLuint mLightIndex;
    EType mType;
    PropertyChannel mColorRed;
    PropertyChannel mColorGreen;
    PropertyChannel mColorBlue;
    PropertyChannel mConeAngle;
};

#endif // _SCENE_CACHE_H
