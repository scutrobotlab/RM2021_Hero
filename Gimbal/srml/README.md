## SRML: ~~SCUT Robotlab Middlewares Layer Library~~ 
## Shit Rubbish Mountain Library
<Font><font size="5">**机器人实验室嵌入式软件中间件层库**</Font>

![](https://img.shields.io/badge/当前版本-V2.2-blue.svg)

### Summary 简介 

SRML旨在将已有的代码解耦，模块化后封装成一个服务于组织内所有软件项目的中间件层，形成实验室内部软件开发标准接口。今后机器人的电控软件业务逻辑（跟比赛项目中机器人的具体功能流程逻辑相关的代码）的开发过程中统一基于这个接口，以解决实验室代码混乱的问题。同时机器人软件开发者需要维护、升级已有的模块，而无需为了同一个功能重复造轮子，提高软件开发效率和质量。如果你认为自己对某个模块有更好的代码方案，或者发现了现有模块不再满足当前使用需求，请仔细阅读完原有代码后向实验室其他队员收集需求意见然后再进行重构。重构时请务必谨慎考虑模块接口的设置！
- __功能强大__
  - 本库包含实验室所有稳定的底层代码，通信协议，设备驱动，功能模块以及终端调试系统，可提供完整的嵌入式控制系统开发工具链。
- __灵活方便__
  - 内核可裁剪（通过预编译宏决定参与编译的模块），使用时只需要包含一个头文件即可。
- __维护方便__
  - 本仓库将严格按照代码规范编写，各个功能模块由模块负责人专人维护，建立清晰的依赖关系。
  - 所有代码需要严格测试后才能发布。
  - 按照Git使用规范管理本仓库。

### Improvement 改进方向

后续可以参考软件设计模式中的发布订阅模型，典型的可以学习**ROS2**，**PX4**，**Apollo**，**DJI Robomaster2020 AI开源机器人**等已有产品的架构，鼓励有兴趣的同学去思考根据我们的应用场景来改进甚至重构我们的软件架构。

### How to Deploy 配置方法
- __获取 SRML 库__
  - 参见：[添加子模块的远程仓库](#添加子模块的远程仓库)
- __添加 SRML 库到崭新的 STM32 工程中__
  - 包含`srml.h`
  - 将`srml_config_template.h`拷贝到用户代码目录，并重命名为`srml_config.h`
  - 在`srml_config.h`中定义各模块是否参与编译
  - __单个STM32CubeIDE工程__
    - 将SRML目录加入源代码位置和包含路径
    - 编译
  - [__MDK工程__](https://github.com/scutrobotlab/21_Project_Template.git)
    - 按照文件目录结构将库文件添加到工程中
    - 编译

### How to Develop 维护方法
- __Git仓库管理__
  - 添加子模块的远程仓库

  ```bash
  # 以下两种方法选一种即可
  $ git submodule add https://github.com/scutrobotlab/srml.git # http方法
  $ git submodule add ssh://git@github.com/scutrobotlab/srml.git # ssh方法
  ```

  - clone已有的工程时拉取SRML的代码
  ```bash
  $ git submodule update --init --recursive
  ```

  - 更新 SRML 库

  ```bash
  $ cd srml/
  $ git pull origin master
  ```

  - 推送更改到 SRML 库
  
  ```bash
  $ cd srml/
  $ git push origin dev
  ```
