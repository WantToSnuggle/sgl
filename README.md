# SGL (Small Graphics Library)
# [中文](https://github.com/LiShanwenGit/sgl/blob/main/README_CN.md)

![SGL_LOGO](SGL_logo.png)

#### Minimum hardware requirements
| Flash Size | Ram Size |
| :---------:| :------: |
| 15kB       |    3kb   |

#### Introduction
SGL (Small Graphics Library) is a lightweight and fast graphics library designed to provide a beautiful and lightweight GUI (Graphics User Interface) for MCU-level processors.
Please refer to the `docs` directory for documentation.

## Quick Start (Based on SDL2 Simulation)
Steps are as follows:
1. Install gcc compiler
2. git clone https://github.com/sgl-org/sgl-port-windows.git    
3. cd sgl-port-windows && git submodule init    
4. git submodule update --remote    
3. cd demo && make config    
4. make run    

Note: Please use git terminal or VSCode's git bash terminal to enter the above commands

## 1. Install gcc Compiler
Download `gcc` from this address (https://github.com/niXman/mingw-builds-binaries/releases/download/13.2.0-rt_v11-rev0/x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev0.7z)
Then extract to any directory and add to environment variables

## 2. Pull sgl demo Source Code
git clone https://github.com/sgl-org/sgl-port-windows.git

## 3. Configure Project
cd sgl-port-windows && git submodule init    
git submodule update --remote    
cd demo && make config   

## 4. Run
Execute `make run` command to run

QQ Group: 544602724
or if you have any questions, please contact me: 1477153217@qq.com
