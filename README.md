# FFLV2

主要参考 ffmpeg,sdl等开源库，提取一些部分基础代码，并在此基础上进行了修改

计划的支持 android/mac/win/ios


文件介绍
ref:  c++ 引用计数 参考android源码，进行了修改
atomic :c      参考sdl
thread: c      参考了sdl
Pipeline：c++  流水线框架


工程介绍：
src: FFL基础库

player: 播放器，使用FFL基础库
player\third :     播放器用到的第三方库
player\playerlib : 播放器lib
player\player ：   简单的播放器， (mac/win)
player\player_vs2015 : mfc实现的播放器

