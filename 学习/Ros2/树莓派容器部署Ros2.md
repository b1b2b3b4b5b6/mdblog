# 树莓派容器部署Ros2
## 要求
- 树莓派要接入相机模块
- 树莓派要接入Ros2

## 方案
硬件：树莓派4B
系统：Raspberry Pi OS 64 bit
Ros版本: Humble
容器: [docker-ros2-desktop-vnc](https://github.com/Tiryoh/docker-ros2-desktop-vnc)

- 要接入相机, 需要raspberry系统, ubuntu系统不好弄
- 64位的raspberry系统 , 能安装Ros2的容器
- 使用容器方便部署, x64端也使用相同的容器
- 容器要带桌面, 方便查看可视化



## 部署
- python3.11通过pip3安装docker-compose有难度, 直接下载可执行程序[docker-compose](https://github.com/docker/compose/)
- Ros2各个节点通过组播来通讯, 映射容器端口不现实, 选择直接使用host模式, docker-compose中配置`network_mode=host`
    - 使用host模式后, 容器内的novnc服务启动失败, 原因是绑定80端口失败
    ![](https://raw.githubusercontent.com/b1b2b3b4b5b6/pic/main/ros2/树莓派容器部署ros2.md/543322916246545.png)
    - 原因是1024以下的端口要root权限才能打开, 而ubuntu用户没有此权限, 在容器内修改80到6080, 打包容器并上传dockerhub
    ![](https://raw.githubusercontent.com/b1b2b3b4b5b6/pic/main/ros2/树莓派容器部署ros2.md/154513716266711.png)
    - vncserver启动失败, 原因是/tmp/.X1_XX类似文件占用, 删除即可
 
## 使用
网页打开`<宿主机ip>:6080`访问即可