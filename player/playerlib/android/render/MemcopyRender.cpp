/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  MemcopyRender.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/06/28
*  https://github.com/zhenfei2016/FFL-v2.git
*  android ANativewindow数据拷贝显示方式
*
*/
#include <android/native_window.h>
#include "MemcopyRender.hpp"

#include "VideoTexture.hpp"
#include "VideoSurface.hpp"
#include "FFMpeg.hpp"
namespace android{
    status_t ANativeWindowDisplay(ANativeWindow *window, player::VideoTexture* tex);

    MemcopyRender::MemcopyRender(){}
    MemcopyRender::~MemcopyRender(){}
    //
    //  创建这个render ，arg自定义的参数
    //
    status_t MemcopyRender::create(void* arg){

        return  FFL_OK;
    }
    //
    //  绘制图片tex到 surface上面
    //
    status_t MemcopyRender::draw(player::VideoSurface* surface,player::VideoTexture* tex){
        return  ANativeWindowDisplay(surface->getHandle(),tex);
    }
    //
    //  释放这个render，释放后这个render就不可用了
    //
    void MemcopyRender::destroy(){

    }

    static int renderRgb2Rgb(ANativeWindow_Buffer *out_buffer, const player::VideoTexture *tex, int bpp) {
        assert(tex->mPlanesCount == 1);

        int min_height = FFL_MIN(out_buffer->height, tex->mHeight);
        int dst_stride = out_buffer->stride;
        int src_line_size = tex->mPitches[0];
        int dst_line_size = dst_stride * bpp / 8;

        uint8_t *dst_pixels =(uint8_t*) out_buffer->bits;
        const uint8_t *src_pixels = tex->mPixels[0];

        if (dst_line_size == src_line_size) {
            int plane_size = src_line_size * min_height;
            memcpy(dst_pixels, src_pixels, plane_size);
        } else {
            int bytewidth = FFL_MIN(dst_line_size, src_line_size);
            av_image_copy_plane(dst_pixels, dst_line_size, src_pixels, src_line_size, bytewidth, min_height);
        }
        return 0;
    }

    status_t ANativeWindowDisplay(ANativeWindow *window, player::VideoTexture* tex){
        int retval;
        if (NULL==window || NULL==tex) {
            FFL_LOG_ERROR("ANativeWindow is Null or tex is NULL.");
            return FFL_FAILED;
        }

        if (tex->mWidth <= 0 || tex->mHeight <= 0){
            return FFL_FAILED;
        }

        int curr_w = ANativeWindow_getWidth(window);
        int curr_h = ANativeWindow_getHeight(window);
        int curr_format = ANativeWindow_getFormat(window);
        int buff_w = FFL_ALIGN(tex->mWidth, 2);
        int buff_h = FFL_ALIGN(tex->mHeight, 2);
//
//        /*  图片格式 */
//        AndroidHalFourccDescriptor *overlayDesc = native_window_get_desc(overlay->format);
//        if (!overlayDesc)
//        {
//            FFL_LOG_ERROR("SDL_Android_NativeWindow_display_l: unknown overlay format: %d", overlay->format);
//            return -1;
//        }
//
//        /*  输出屏幕格式 */
//        AndroidHalFourccDescriptor *voutDesc = native_window_get_desc(curr_format);
//        if (!voutDesc || voutDesc->hal_format != overlayDesc->hal_format)
//        {
//            FFL_LOG_ERROR("ANativeWindow_setBuffersGeometry: w=%d, h=%d, f=%.4s(0x%x) => w=%d, h=%d, f=%.4s(0x%x)",
//                      curr_w, curr_h, (char*) &curr_format, curr_format,
//                      buff_w, buff_h, (char*) &overlay->format, overlay->format);
//            retval = ANativeWindow_setBuffersGeometry(native_window, buff_w, buff_h, overlayDesc->hal_format);
//            if (retval < 0)
//            {
//                FFL_LOG_ERROR("SDL_Android_NativeWindow_display_l: ANativeWindow_setBuffersGeometry: failed %d", retval);
//                return retval;
//            }
//
//            if (!voutDesc)
//            {
//                FFL_LOG_ERROR("SDL_Android_NativeWindow_display_l: unknown hal format %d", curr_format);
//                return -1;
//            }
//        }
//
//        ANativeWindow_Buffer out_buffer;
//        retval = ANativeWindow_lock(native_window, &out_buffer, NULL);
//        if (retval < 0)
//        {
//            FFL_LOG_ERROR("SDL_Android_NativeWindow_display_l: ANativeWindow_lock: failed %d", retval);
//            return retval;
//        }
//
//        if (out_buffer.width != buff_w || out_buffer.height != buff_h)
//        {
//            FFL_LOG_ERROR("unexpected native window buffer (%p)(w:%d, h:%d, fmt:'%.4s'0x%x), expecting (w:%d, h:%d, fmt:'%.4s'0x%x)",
//                      native_window,
//                      out_buffer.width, out_buffer.height, (char*)&out_buffer.format, out_buffer.format,
//                      buff_w, buff_h, (char*)&overlay->format, overlay->format);
//            // TODO: 8 set all black
//            ANativeWindow_unlockAndPost(native_window);
//            ANativeWindow_setBuffersGeometry(native_window, buff_w, buff_h, overlayDesc->hal_format);
//            return -1;
//        }
//
//        /*  绘制 */
//        int render_ret = voutDesc->render(&out_buffer, overlay);
//        if (render_ret < 0)
//        {
//            // TODO: 8 set all black
//            // return after unlock image;
//        }
//
//        /*  提交 */
//        retval = ANativeWindow_unlockAndPost(native_window);
//        if (retval < 0)
//        {
//            FFL_LOG_ERROR("SDL_Android_NativeWindow_display_l: ANativeWindow_unlockAndPost: failed %d", retval);
//            return retval;
//        }
        return FFL_OK;
    }

}
