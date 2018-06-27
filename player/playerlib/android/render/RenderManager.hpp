#ifndef _RENDER_MANAGER_HPP_
#define _RENDER_MANAGER_HPP_

#include "VideoFormat.hpp"
#include "RenderInterface.hpp"
namespace android{
    //
    //  管理着支持的所有render
    //
    class RenderManager {
    public:
        RenderManager();
        ~RenderManager();
    public:
        //
        //  注册,反注册支持的所有render
        //
        void registerRenders();
        void unRegisterRenders();
        //
        //   根据输入的格式，返回一个可以使用的render
        //    format :需要的render格式
        //    arg  :参数，通过这个参数，找到一个最配置的render
        //
        RenderInterface *getRender(player::VideoFormat format, void *arg);
    };
}
#endif