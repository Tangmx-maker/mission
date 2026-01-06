[readme.md](https://github.com/user-attachments/files/24444224/readme.md)
# DrawingBoard - MFC绘图板项目

一个使用Visual Studio 2026开发的MFC绘图板应用程序，实现了多种绘图工具和设计模式。

## 项目信息

- **项目名称**: DrawingBoard
- **开发工具**: Visual Studio 2026
- **项目类型**: MFC桌面应用程序
- **编程语言**: C++
- **包含测试项目**: DrawingBoardTests

## 项目结构

### 主项目 (DrawingBoard)

**头文件 (Header Files):**
- DoubleBufferStrategy.h     - 双缓冲策略类
- DrawingBoard.h             - 应用程序类
- DrawingBoardDoc.h          - 文档类
- DrawingBoardView.h         - 视图类
- framework.h                - MFC框架头文件
- iRenderStrategy.h          - 渲染策略接口
- LanguageManager.h          - 多语言管理类
- MainFrm.h                  - 主框架类
- pch.h                      - 预编译头文件
- Resource.h                 - 资源定义头文件
- targetver.h                - 目标版本定义

**源文件 (Source Files):**
- DoubleBufferStrategy.cpp   - 双缓冲策略实现
- DrawingBoard.cpp           - 应用程序实现
- DrawingBoardDoc.cpp        - 文档类实现
- DrawingBoardView.cpp       - 视图类实现
- LanguageManager.cpp        - 多语言管理实现
- MainFrm.cpp                - 主框架实现
- pch.cpp                    - 预编译头源文件

**资源文件 (Resource Files):**
- DrawingBoard.ico           - 应用程序图标
- DrawingBoard.rc            - 资源脚本文件
- DrawingBoard.rc2           - 额外资源文件
- DrawingBoardDoc.ico        - 文档图标
- Toolbar.bmp                - 工具栏位图(小)
- Toolbar256.bmp             - 工具栏位图(256色)
- userimages.bmp             - 用户图像位图
- DrawingBoard.reg           - 注册表文件

### 测试项目 (DrawingBoardTests)

**测试文件:**
- DrawCommandTests.cpp       - 绘制命令测试
- test.cpp                   - 其他测试
- pch.h                      - 预编译头文件
- pch.cpp                    - 预编译头源文件
- packages.config            - NuGet包配置

## 功能特性

基于项目结构分析，本绘图板包含以下功能：

### 核心技术
1. **双缓冲渲染** - 防止画面闪烁
2. **策略模式** - iRenderStrategy接口支持不同渲染策略
3. **多语言支持** - LanguageManager实现界面国际化
4. **MVC架构** - 标准的MFC文档/视图架构

### 预期功能
- 多种绘图工具（直线、矩形、椭圆等）
- 撤销/重做功能
- 工具栏和菜单
- 文件保存/加载
- 单元测试覆盖

## 编译和运行

### 系统要求
- Windows 10/11 操作系统
- Visual Studio 2026
- MFC组件（安装时需选择）

### 编译步骤
1. 双击打开 `DrawingBoard.sln` 解决方案文件
2. 等待Visual Studio加载项目
3. 选择编译配置（推荐：Debug x86 或 Release x86）
4. 按 `F5` 编译并调试运行
5. 或按 `Ctrl+F5` 直接运行（不调试）

### 生成文件位置
- Debug版本：`Debug\DrawingBoard.exe`
- Release版本：`Release\DrawingBoard.exe`

## 测试项目

项目包含独立的测试项目 `DrawingBoardTests`，用于单元测试。

### 运行测试
1. 在解决方案资源管理器右键点击 `DrawingBoardTests` 项目
2. 选择"设为启动项目"
3. 按 `Ctrl+R,A` 运行所有测试
4. 或使用测试资源管理器查看测试结果

## 项目配置说明

### 主要配置项
1. **字符集**：使用Unicode字符集
2. **MFC使用**：在共享DLL中使用MFC
3. **预编译头**：使用预编译头(pch.h/pch.cpp)
4. **C++标准**：C++17或更高版本

### 依赖项
- MFC框架库
- Windows SDK
- C++运行时库

## 开发注意事项

1. **资源管理**：所有图标和位图位于资源文件中
2. **双缓冲**：使用DoubleBufferStrategy防止绘图闪烁
3. **多语言**：通过LanguageManager实现界面文本切换
4. **设计模式**：采用策略模式(iRenderStrategy)实现灵活的渲染方式

## 故障排除

### 常见问题
1. **编译错误"找不到MFC库"**
   - 确保安装了MFC组件
   - 项目属性 → 常规 → MFC的使用 → 在共享DLL中使用MFC

2. **运行时缺少DLL**
   - 安装对应版本的Visual C++ Redistributable
   - 或将运行时库改为静态链接

3. **测试项目无法运行**
   - 确保测试框架已正确安装
   - 检查packages.config中的NuGet包

## 项目维护

### 添加新功能
1. 在相应目录添加头文件和源文件
2. 更新资源文件（如需添加图标或字符串）
3. 编写对应的单元测试
4. 更新文档说明

### 代码规范
- 使用有意义的变量和函数名
- 添加必要的注释
- 遵循MFC命名约定
- 使用预编译头提高编译速度

---
*项目创建时间：2025年*
*最后更新：2025年12月*
