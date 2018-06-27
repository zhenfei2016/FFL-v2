/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  RenderInterface.hpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/06/28 
*  https://github.com/zhenfei2016/FFL-v2.git
*  android渲染接口
*
*/
#ifndef _RENDER_INTERFACE_HPP_
#define _RENDER_INTERFACE_HPP_

#include <FFL.h>
namespace player{
    class VideoSurface;
    class VideoTexture;
}

namespace  android{
    class RenderInterface{
    public:
        //
        //  创建这个render ，arg自定义的参数
        //
        virtual status_t create(void* arg)=0;
        //
        //  绘制图片tex到 surface上面
        //
        virtual status_t draw(player::VideoSurface* surface,player::VideoTexture* tex)=0;
        //
        //  释放这个render，释放后这个render就不可用了
        //
        virtual void destroy()=0;
    };
};
#endif