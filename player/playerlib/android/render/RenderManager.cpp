#include "RenderManager.hpp"
#include "MemcopyRender.hpp"

namespace android {
    MemcopyRender* gMemcopyRender=new MemcopyRender();

    RenderManager::RenderManager(){
    }
    RenderManager::~RenderManager(){
    }
    //
    //  注册,反注册支持的所有render
    //
    void RenderManager::registerRenders(){
    }

    void RenderManager::unRegisterRenders(){

    }
    //
    //   根据输入的格式，返回一个可以使用的render
    //    format :需要的render格式
    //    arg  :参数，通过这个参数，找到一个最配置的render
    //
    RenderInterface* RenderManager::getRender(player::VideoFormat format, void *arg){
        return gMemcopyRender;
    }
}