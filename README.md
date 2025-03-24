# [DuaEngine] 🎮

一个基于 **C++** 和 **OpenGL** 的图一乐 游戏/渲染 引擎\
架构基本来自Cherno大佬的Hazel

---

## 🛠️ 技术栈
- **语言**: C++17  
- **图形API**: OpenGL3，以后可能加上Vulkan？
- **UI框架**: ImGui Docking  
- **窗口管理**: GLFW  
- **数学库**: GLM（OpenGL Mathematics）  
- **日志库**：等我感觉麻烦了再补  
- **构建工具**: 以后补，应该是CMake  

---

## 🗓️ 里程
可能写的比较糊
- **（持续更新中。。。）** 🔨
- **2025/03/24** 2D渲染器，批处理渲染
- **2025/03/17** 着色器库，wasd鼠标控制摄像机
- **2025/03/05** 加载shader文本文件
- **2025/03/03** 加载纹理
- **2025/03/02** 加入了Transform，写了Shader抽象
- **2025/03/01** 把测试代码从引擎库中分离，加入了Timestep
- **2025/02/28** 加入了正交相机
- **2025/02/27** 完成了顶点缓冲区、顶点数组等抽象
- **2025/02/24** 输入事件竣工（大概），准备开始写渲染的抽象

---

## 📥 依赖项
- [Visual Studio](https://visualstudio.microsoft.com/) - Microsoft官方IDE
- [GLFW](https://www.glfw.org) - 跨平台窗口/输入管理  
- [GLAD](https://glad.dav1d.de/) - OpenGL扩展加载器
- [GLM](https://glm.g-truc.net) - 数学库
- [ImGui](https://github.com/ocornut/imgui/tree/docking) - 即时模式调试UI框架  