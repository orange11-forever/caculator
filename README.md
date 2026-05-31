# Calculator

一个基于 **Qt 6 / Qt 5** 的桌面计算器应用，采用 C++ 编写，使用 CMake 构建。

## 功能

- **基本四则运算**：加（+）、减（-）、乘（×）、除（÷）
- **一元运算**：平方（x²）、开方（Sqrt）、倒数（1/x）
- **正负号切换**（±）
- **小数点支持**
- **退格**（BackSpace）、清除（Clear）、全部清除（Clear All）
- 运算符优先级处理（乘除优先于加减）
- 除零及负数开方等错误提示（显示 `######`）

## 界面

![screenshot](screenshot.png)

## 项目结构

```
calculator/
├── main.cpp          # 应用入口
├── widget.h          # 主窗口头文件
├── widget.cpp        # 主窗口实现（含计算逻辑）
├── widget.ui         # UI 布局文件
├── button.h          # 自定义按钮头文件
├── button.cpp        # 自定义按钮实现
├── CMakeLists.txt    # CMake 构建配置
├── .gitignore
└── README.md
```

## 构建

### 依赖

- **Qt 5.15+** 或 **Qt 6.x**（Widgets 模块）
- **CMake 3.16+**
- 支持 C++17 的编译器（GCC、Clang、MSVC 或 MinGW）

### 编译步骤

```bash
# 在项目根目录下
mkdir build
cd build
cmake ..
cmake --build .
```

### 运行

```bash
# Windows
build\calculator.exe

# macOS
open build/calculator.app

# Linux
build/calculator
```

## 技术要点

- 基于 **QWidget** 构建 GUI，使用 `QGridLayout` 布局
- 自定义 `Button` 类继承自 `QToolButton`，统一按钮样式
- 使用 Qt 信号槽机制连接按钮点击与计算逻辑
- 运算符优先级通过分离加减操作数与乘除操作数实现
- 支持 **Qt 5** 和 **Qt 6** 双版本构建

## License

MIT
