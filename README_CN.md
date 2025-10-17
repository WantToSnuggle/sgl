# SGL (Small Graphics Library)
![SGL_LOGO](SGL_logo.png)

#### 硬件最小要求
| Flash Size | Ram Size |
| :---------:| :------: |
| 15kB       |    3kb   |

#### 介绍
SGL (Small Graphics Library)是一种轻量快速的图形库，该库旨在为MCU级别的处理器提供一美观轻量的GUI（Graphics User Interface).
使用文档请参考`docs`目录

## 快速开始 (基于SDL2模拟)
步骤如下：
1. Install gcc compiler
2. git clone https://github.com/sgl-org/sgl-port-windows.git    
3. cd sgl-port-windows && git submodule init    
4. git submodule update --remote    
3. cd demo && make config    
4. make run    

注意: 请使用git终端或者VSCode下面的git bash终端输入上面命令

## 1. 安装gcc编译器
在此地址处下载`gcc`(https://github.com/niXman/mingw-builds-binaries/releases/download/13.2.0-rt_v11-rev0/x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev0.7z)
然后解压到任意目录下，并添加到环境变量中

## 2. 拉取sgl demo源代码
git clone https://github.com/sgl-org/sgl-port-windows.git

## 3. 配置项目
cd sgl-port-windows && git submodule init    
git submodule update --remote    
cd demo && make config   

## 4. 运行
执行`make run`命令即可运行

QQ群：544602724
