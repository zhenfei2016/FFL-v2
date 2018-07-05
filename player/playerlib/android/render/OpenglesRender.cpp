/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  OpenglesRender.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/07/06
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  opengl方式绘制图片
*/
#include <VideoTexture.hpp>
#include "OpenglesRender.hpp"
#include "android/opengles2/SDL_render_gles2.hpp"

namespace  render{
    OpenglEsRender::OpenglEsRender(){

    }
    OpenglEsRender::~OpenglEsRender(){

    }

    //  获取支持的格式
    //  wanted: 如果为nUll则返回所有支持的格式
    //           非null 返回跟他匹配的
    //  fmtList: 返回支持的格式list
    //
    void OpenglEsRender::getSupportFormat(player::VideoSurface* surface,const player::VideoFormat* wanted,FFL::List<player::VideoFormat>& fmtList){

    }
    bool OpenglEsRender::getOptimalFormat(player::VideoSurface* surface,const player::VideoFormat* wanted,player::VideoFormat* optinal){
        return false;
    }
    bool OpenglEsRender::isSupportFormat(player::VideoSurface* surface,const player::VideoFormat* wanted){
        return false;
    }
    //
    //  创建这个render ，arg自定义的参数
    //
    status_t OpenglEsRender::create(const player::VideoFormat*  arg){
        return FFL_FAILED;
    }
    //
    //  绘制图片tex到 surface上面
    //
    status_t OpenglEsRender::draw(player::VideoSurface* surface,player::VideoTexture* tex){
        return FFL_FAILED;
    }
    //
    //  释放这个render，释放后这个render就不可用了
    //
    void OpenglEsRender::destroy(){

    }
}