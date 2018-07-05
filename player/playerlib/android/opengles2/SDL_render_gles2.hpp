/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  SDL_render_gles2.hpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/07/06 
*  https://github.com/zhenfei2016/FFL-v2.git
*  移植修改的SDL2中的opengles相关部分
*
*/
#ifndef _SDL_RENDER_GLES_HPP_
#define _SDL_RENDER_GLES_HPP_


#include <GLES2/gl2.h>

typedef struct SDL_Point
{
	int x;
	int y;
} SDL_Point;

typedef struct SDL_Rect
{
	int x, y;
	int w, h;
} SDL_Rect;

typedef struct
{
	float x;
	float y;
	float w;
	float h;
} SDL_FRect;

typedef struct
{
	float x;
	float y;
} SDL_FPoint;

typedef enum
{
	SDL_FLIP_NONE = 0x00000000,     /**< Do not flip */
	SDL_FLIP_HORIZONTAL = 0x00000001,    /**< flip horizontally */
	SDL_FLIP_VERTICAL = 0x00000002     /**< flip vertically */
} SDL_RendererFlip;

typedef unsigned char Uint8;

//
//  顶点属性在sharder中的索引位置(句柄值)
//  定点位置，纹理坐标，旋转角度，旋转中心位置
//
typedef enum
{
	GLES2_ATTRIBUTE_POSITION = 0,
	GLES2_ATTRIBUTE_TEXCOORD = 1,
	GLES2_ATTRIBUTE_ANGLE = 2,
	GLES2_ATTRIBUTE_CENTER = 3,
} GLES2_Attribute;

//
//  sharder中的uniform存储于类实例mUniformLocations数组中的索引位置
//
typedef enum
{
	GLES2_UNIFORM_PROJECTION,
	GLES2_UNIFORM_TEXTURE,
	GLES2_UNIFORM_MODULATION,
	GLES2_UNIFORM_COLOR,
	GLES2_UNIFORM_TEXTURE_U,
	GLES2_UNIFORM_TEXTURE_V,
	GLES_UNIFORM_COUNT,
} GLES2_Uniform;

namespace player{
	class VideoTexture;
}
class GLES2Renderer {
public:
	GLES2Renderer();
	~GLES2Renderer();

	bool install();
	bool useProgram();
	void uninstall();

	//
	//  创建，更新，删除纹理
	//
	int  createTexture(player::VideoTexture* tex);
	bool uploadTexture(player::VideoTexture* tex,
					   const SDL_Rect *rect);
	void destroyTexture();


	//
	//  开始绘制
	//
	int render(player::VideoTexture* tex,
			   	     const SDL_Rect *srcrect,
					 const SDL_FRect *dstrect,
					 const double angle,
					 const SDL_FPoint *center,
					 const SDL_RendererFlip flip);

protected:
	//
	//  更新顶点相关信息
	//
	void updateVertexBuffer(GLES2_Attribute attr,
							const void *vertexData,
							size_t dataSizeInBytes);


	int texSubImage2D(GLenum target,
					  GLint xoffset, GLint yoffset,
					  GLsizei width, GLsizei height,
					  GLenum format, GLenum type,
					  const GLvoid *pixels, GLint pitch, GLint bpp);

private:
	//
	//  顶点，片段着色
	//
	GLuint mVertexShader;
	GLuint mFragmentShader;
	GLuint mProgram;
	GLuint mUniformLocations[GLES_UNIFORM_COUNT];

	//
	//  纹理
	//
	GLuint mTextureU;
	GLuint mTextureV;
	GLuint mTexture;

	//
	//
	//
	bool mIsyuv;
	bool mIsnv12;
};

#endif
