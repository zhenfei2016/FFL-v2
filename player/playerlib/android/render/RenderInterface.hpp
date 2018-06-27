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