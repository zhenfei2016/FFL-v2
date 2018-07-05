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
#include <FFL.h>
#include "SDL_render_gles2.hpp"
extern "C" {
#include "SDL_shaders_gles2.h"
}

#include "VideoTexture.hpp"

void GLES2_checkError(const char* op) {
	for (GLint error = glGetError(); error; error = glGetError()) {
		FFL_LOG_ERROR("[GLES2] after %s() glError (0x%x)\n", op, error);
	}
}

void GLES2_printString(const char *name, GLenum s) {
	const char *v = (const char *)glGetString(s);
	FFL_LOG_INFO("[GLES2] %s = %s\n", name, v);
}

GLES2Renderer::GLES2Renderer(){

}
GLES2Renderer::~GLES2Renderer(){

}
bool GLES2Renderer::install(){
	const GLES2_Shader* shader = GLES2_GetShader(GLES2_SHADER_FRAGMENT_SOLID_SRC);
	const GLES2_ShaderInstance* instance = shader->instances[0];

	GLint compileSuccessful;
	GLuint shaderId = glCreateShader(instance->type);
	if (instance->format == (GLenum)-1) {
		glShaderSource(shaderId, 1, (const char **)(char *)&instance->data, NULL);
		glCompileShader(shaderId);
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileSuccessful);
	}
	else {
		glShaderBinary(1, &shaderId, instance->format, instance->data, instance->length);
		compileSuccessful = GL_TRUE;
	}

	mProgram = glCreateProgram();  
    GLES2_checkError("glCreateProgram");
	if (!mProgram) {
		return false;
	}


	glAttachShader(mProgram, mVertexShader);
    GLES2_checkError("glAttachShader(vertex)");
	glAttachShader(mProgram, mFragmentShader);
    GLES2_checkError("glAttachShader(fragment)");

	glBindAttribLocation(mProgram, GLES2_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(mProgram, GLES2_ATTRIBUTE_TEXCOORD, "a_texCoord");
	glBindAttribLocation(mProgram, GLES2_ATTRIBUTE_ANGLE, "a_angle");
	glBindAttribLocation(mProgram, GLES2_ATTRIBUTE_CENTER, "a_center");

	glLinkProgram(mProgram);
    GLES2_checkError("glLinkProgram");

	GLint link_status = GL_FALSE;
	glGetProgramiv(mProgram, GL_LINK_STATUS, &link_status);
	if (!link_status) {
		return false;
	}

	/* Predetermine locations of uniform variables */
	mUniformLocations[GLES2_UNIFORM_PROJECTION] =
		glGetUniformLocation(mProgram, "u_projection");
	mUniformLocations[GLES2_UNIFORM_TEXTURE_V] =
		glGetUniformLocation(mProgram, "u_texture_v");
	mUniformLocations[GLES2_UNIFORM_TEXTURE_U] =
		glGetUniformLocation(mProgram, "u_texture_u");
	mUniformLocations[GLES2_UNIFORM_TEXTURE] =
		glGetUniformLocation(mProgram, "u_texture");
	mUniformLocations[GLES2_UNIFORM_MODULATION] =
		glGetUniformLocation(mProgram, "u_modulation");
	mUniformLocations[GLES2_UNIFORM_COLOR] =
		glGetUniformLocation(mProgram, "u_color");

	glUseProgram(mProgram);
	glUniform1i(mUniformLocations[GLES2_UNIFORM_TEXTURE_V], 2);  /* always texture unit 2. */
	glUniform1i(mUniformLocations[GLES2_UNIFORM_TEXTURE_U], 1);  /* always texture unit 1. */
	glUniform1i(mUniformLocations[GLES2_UNIFORM_TEXTURE], 0);  /* always texture unit 0. */
	//glUniformMatrix4fv(mUniformLocations[GLES2_UNIFORM_PROJECTION], 1, GL_FALSE, (GLfloat *)projection);
	glUniform4f(mUniformLocations[GLES2_UNIFORM_MODULATION], 1.0f, 1.0f, 1.0f, 1.0f);
	glUniform4f(mUniformLocations[GLES2_UNIFORM_COLOR], 1.0f, 1.0f, 1.0f, 1.0f);
	return true;
}
bool GLES2Renderer::useProgram(){
	FFL_LOG_INFO("use render yuv420p\n");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glUseProgram(mProgram);

	//            if (0 == renderer->plane_textures[0])
	//                glGenTextures(3, renderer->plane_textures);
	//
	//            for (int i = 0; i < 3; ++i) {
	//                glActiveTexture(GL_TEXTURE0 + i);
	//                glBindTexture(GL_TEXTURE_2D, renderer->plane_textures[i]);
	//
	//                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//
	//                glUniform1i(renderer->us2_sampler[i], i);
	//            }
	//
	//            glUniformMatrix3fv(renderer->um3_color_conversion, 1, GL_FALSE, IJK_GLES2_getColorMatrix_bt709());
	return false;
}
void GLES2Renderer::uninstall() {

}
//
//  创建，更新，删除纹理
//
int  GLES2Renderer::createTexture(player::VideoTexture* tex) {
	GLenum format;
	GLenum type;
	GLenum scaleMode;


//	/* Determine the corresponding GLES texture format params */
//	switch (texture->format)
//	{
//		case SDL_PIXELFORMAT_ARGB8888:
//		case SDL_PIXELFORMAT_ABGR8888:
//		case SDL_PIXELFORMAT_RGB888:
//		case SDL_PIXELFORMAT_BGR888:
//			format = GL_RGBA;
//			type = GL_UNSIGNED_BYTE;
//			break;
//		case SDL_PIXELFORMAT_IYUV:
//		case SDL_PIXELFORMAT_YV12:
//		case SDL_PIXELFORMAT_NV12:
//		case SDL_PIXELFORMAT_NV21:
//			format = GL_LUMINANCE;
//			type = GL_UNSIGNED_BYTE;
//			break;
//		default:
//			return SDL_SetError("Texture format not supported");
//	}
//
//	texture = 0;
//	texture_type = GL_TEXTURE_2D;
//	pixel_format = format;
//	pixel_type = type;
//	yuv = ((texture->format == SDL_PIXELFORMAT_IYUV) || (texture->format == SDL_PIXELFORMAT_YV12));
//	nv12 = ((texture->format == SDL_PIXELFORMAT_NV12) || (texture->format == SDL_PIXELFORMAT_NV21));
//	texture_u = 0;
//	texture_v = 0;
//	scaleMode = GetScaleQuality();
//
//	/* Allocate a blob for image renderdata */
//	if (texture->access == SDL_TEXTUREACCESS_STREAMING) {
//		size_t size;
//		pitch = texture->w * SDL_BYTESPERPIXEL(texture->format);
//		size = texture->h * pitch;
//		if (yuv) {
//			/* Need to add size for the U and V planes */
//			size += (2 * (texture->h * pitch) / 4);
//		}
//		if (nv12) {
//			/* Need to add size for the U/V plane */
//			size += ((texture->h * pitch) / 2);
//		}
//		pixel_data = SDL_calloc(1, size);
//		if (!pixel_data) {
//			SDL_free(data);
//			return SDL_OutOfMemory();
//		}
//	}
//
//	/* Allocate the texture */
//	GL_CheckError("", renderer);
//
//	if (yuv) {
//		glGenTextures(1, &texture_v);
//		if (GL_CheckError("glGenTexures()", renderer) < 0) {
//			return -1;
//		}
//		glActiveTexture(GL_TEXTURE2);
//		glBindTexture(texture_type, texture_v);
//		glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, scaleMode);
//		glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, scaleMode);
//		glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		glTexImage2D(texture_type, 0, format, texture->w / 2, texture->h / 2, 0, format, type, NULL);
//
//		glGenTextures(1, &texture_u);
//		if (GL_CheckError("glGenTexures()", renderer) < 0) {
//			return -1;
//		}
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(texture_type, texture_u);
//		glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, scaleMode);
//		glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, scaleMode);
//		glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		glTexImage2D(texture_type, 0, format, texture->w / 2, texture->h / 2, 0, format, type, NULL);
//		if (GL_CheckError("glTexImage2D()", renderer) < 0) {
//			return -1;
//		}
//	}
//
//	if (nv12) {
//		glGenTextures(1, &texture_u);
//		if (GL_CheckError("glGenTexures()", renderer) < 0) {
//			return -1;
//		}
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(texture_type, texture_u);
//		glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, scaleMode);
//		glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, scaleMode);
//		glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		glTexImage2D(texture_type, 0, GL_LUMINANCE_ALPHA, texture->w / 2, texture->h / 2, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, NULL);
//		if (GL_CheckError("glTexImage2D()", renderer) < 0) {
//			return -1;
//		}
//	}
//
//	glGenTextures(1, &mTexture);
//	if (GL_CheckError("glGenTexures()", renderer) < 0) {
//		return -1;
//	}
//	texture->driverdata = data;
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(texture_type, texture);
//	glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, scaleMode);
//	glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, scaleMode);
//	glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexImage2D(texture_type, 0, format, texture->w, texture->h, 0, format, type, NULL);
//	if (GL_CheckError("glTexImage2D()", renderer) < 0) {
//		return -1;
//	}
//
//	if (texture->access == SDL_TEXTUREACCESS_TARGET) {
//		fbo = GLES2_GetFBO(renderer->driverdata, texture->w, texture->h);
//	}
//	else {
//		fbo = NULL;
//	}
//
//	return GL_CheckError("", renderer);
    return 0;
}

bool GLES2Renderer::uploadTexture(player::VideoTexture* tex,
				   const SDL_Rect *rect){

//	GLuint ttexture_type = GL_TEXTURE_2D;
//	glBindTexture(GL_TEXTURE_2D, mTextureV);
//	GLES2_TexSubImage2D(GL_TEXTURE_2D,
//						rect->x / 2,
//						rect->y / 2,
//						rect->w / 2,
//						rect->h / 2,
//						tpixel_format,
//						tpixel_type,
//						Vplane, Vpitch, 1);
//
//	glBindTexture(GL_TEXTURE_2D, ttexture_u);
//	GLES2_TexSubImage2D(ttexture_type,
//						rect->x / 2,
//						rect->y / 2,
//						rect->w / 2,
//						rect->h / 2,
//						tpixel_format,
//						tpixel_type,
//						Uplane, Upitch, 1);
//
//	glBindTexture(GL_TEXTURE_2D, ttexture);
//	GLES2_TexSubImage2D(ttexture_type,
//						rect->x,
//						rect->y,
//						rect->w,
//						rect->h,
//						tpixel_format,
//						tpixel_type,
//						Yplane, Ypitch, 1);
	return true;
}
void GLES2Renderer::destroyTexture(){

}

//
//  更新顶点相关信息
//
void GLES2Renderer::updateVertexBuffer(GLES2_Attribute attr,
						const void *vertexData,
						size_t dataSizeInBytes){
	glVertexAttribPointer(attr, attr == GLES2_ATTRIBUTE_ANGLE ? 1 : 2, GL_FLOAT, GL_FALSE, 0, vertexData);
}

//
//  开始绘制
//
int GLES2Renderer::render(player::VideoTexture* tex,
		   const SDL_Rect *srcrect,
		   const SDL_FRect *dstrect,
		   const double angle,
		   const SDL_FPoint *center,
		   const SDL_RendererFlip flip){
	GLfloat vertices[8];
	GLfloat texCoords[8];
	GLfloat translate[8];
	GLfloat fAngle[4];
	GLfloat tmp;

	glEnableVertexAttribArray(GLES2_ATTRIBUTE_CENTER);
	glEnableVertexAttribArray(GLES2_ATTRIBUTE_ANGLE);
	fAngle[0] = fAngle[1] = fAngle[2] = fAngle[3] = (GLfloat)(360.0f - angle);
	/* Calculate the center of rotation */
	translate[0] = translate[2] = translate[4] = translate[6] = (center->x + dstrect->x);
	translate[1] = translate[3] = translate[5] = translate[7] = (center->y + dstrect->y);

	/* Emit the textured quad */
	vertices[0] = dstrect->x;
	vertices[1] = dstrect->y;
	vertices[2] = (dstrect->x + dstrect->w);
	vertices[3] = dstrect->y;
	vertices[4] = dstrect->x;
	vertices[5] = (dstrect->y + dstrect->h);
	vertices[6] = (dstrect->x + dstrect->w);
	vertices[7] = (dstrect->y + dstrect->h);
	if (flip & SDL_FLIP_HORIZONTAL) {
		tmp = vertices[0];
		vertices[0] = vertices[4] = vertices[2];
		vertices[2] = vertices[6] = tmp;
	}
	if (flip & SDL_FLIP_VERTICAL) {
		tmp = vertices[1];
		vertices[1] = vertices[3] = vertices[5];
		vertices[5] = vertices[7] = tmp;
	}

	/*glVertexAttribPointer(GLES2_ATTRIBUTE_ANGLE, 1, GL_FLOAT, GL_FALSE, 0, &fAngle);
    glVertexAttribPointer(GLES2_ATTRIBUTE_CENTER, 2, GL_FLOAT, GL_FALSE, 0, translate);
    glVertexAttribPointer(GLES2_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);*/

	updateVertexBuffer(GLES2_ATTRIBUTE_ANGLE, fAngle, 4 * sizeof(GLfloat));
	updateVertexBuffer(GLES2_ATTRIBUTE_CENTER, translate, 8 * sizeof(GLfloat));
	updateVertexBuffer(GLES2_ATTRIBUTE_POSITION, vertices, 8 * sizeof(GLfloat));

	texCoords[0] = srcrect->x / (GLfloat)tex->mWidth;
	texCoords[1] = srcrect->y / (GLfloat)tex->mHeight;
	texCoords[2] = (srcrect->x + srcrect->w) / (GLfloat)tex->mWidth;
	texCoords[3] = srcrect->y / (GLfloat)tex->mHeight;
	texCoords[4] = srcrect->x / (GLfloat)tex->mWidth;
	texCoords[5] = (srcrect->y + srcrect->h) / (GLfloat)tex->mHeight;
	texCoords[6] = (srcrect->x + srcrect->w) / (GLfloat)tex->mWidth;
	texCoords[7] = (srcrect->y + srcrect->h) / (GLfloat)tex->mHeight;
	/*glVertexAttribPointer(GLES2_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, texCoords);*/
	updateVertexBuffer(GLES2_ATTRIBUTE_TEXCOORD, texCoords, 8 * sizeof(GLfloat));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(GLES2_ATTRIBUTE_CENTER);
	glDisableVertexAttribArray(GLES2_ATTRIBUTE_ANGLE);
	return 1;
}


int GLES2Renderer::texSubImage2D(GLenum target,
				  GLint xoffset, GLint yoffset,
				  GLsizei width, GLsizei height,
				  GLenum format, GLenum type,
				  const GLvoid *pixels, GLint pitch, GLint bpp) {
	Uint8 *blob = NULL;
	Uint8 *src;
	int src_pitch;
	int y;

	/* Reformat the texture data into a tightly packed array */
	src_pitch = width * bpp;
	src = (Uint8 *)pixels;
	if (pitch != src_pitch) {
		blob = (Uint8 *)FFL_malloc(src_pitch * height);
		if (!blob) {
			return 0;
		}
		src = blob;
		for (y = 0; y < height; ++y)
		{
			memcpy(src, pixels, src_pitch);
			src += src_pitch;
			pixels = (Uint8 *)pixels + pitch;
		}
		src = blob;
	}


	glTexSubImage2D(target, 0, xoffset, yoffset, width, height, format, type, src);
	if (blob) {
		FFL_free(blob);
	}
	return 0;
}