## Deepin Tool Kit Widget

Deepin Tool Kit Widget(DtkWidget) 提供各种UOS风格dtk基础控件.

## 文档

中文文档：[dtkwidget文档](https://linuxdeepin.github.io/dtkwidget/index.html)

## 依赖

### 编译依赖

* Qt >= 5.6

## 安装

### 从源代码构建

1. 确保已经安装了所有的编译依赖.

```bash
sudo apt build-dep ./
```

如果需要使用 `qtcreator` 的设计功能，可以 ：

```bash
$ sudo apt install qttools5-dev

# build
$ cmake ./plugin/dtkuiplugin -B build -DINSTALL_PLUGIN=ON -DCMAKE_INSTALL_PREFIX=/usr
$ cmake --build build -j$(nproc)

# install
$ cd build
$ sudo make install
```

2. 构建 `dtkwidget` :

```bash
cmake -B build
cmake --build build -j$(nproc)
```

3. 安装:

```bash
cd build
sudo make install
```

## 帮助

任何使用问题都可以通过以下方式寻求帮助:

* [Gitter](https://gitter.im/orgs/linuxdeepin/rooms)
* [IRC channel](https://webchat.freenode.net/?channels=deepin)
* [Forum](https://bbs.deepin.org)
* [WiKi](https://wiki.deepin.org/)

## 参与贡献

我们鼓励您报告问题并作出更改

* [开发者代码贡献指南](https://github.com/linuxdeepin/developer-center/wiki/Contribution-Guidelines-for-Developers)

## 使用dtkwidget的项目

下面是使用dtkwidget的开源项目:(如果您想添加属于自己的开源项目请给我们提交PR)

* **[DMarked](https://github.com/DMarked/DMarked)**: 使用Dtk构建的Markdown编辑器
* **[DtkTimer](https://github.com/gfdgd-xi/timer)**: 使用DtkWidget构建的时钟
* **[simple-image-filter](https://github.com/dependon/simple-image-filter)**: 使用DtkWidget构建的图像处理软件
* **[SparkStore](https://github.com/Spark-Store/Spark-Store)**: 星火商店
* **[WingHexExplorer](https://github.com/Wing-summer/WingHexExplorer)**: 使用DtkWidget构建的强大的十六进制编辑器
* **[WingTool](https://github.com/Wing-summer/WingTool)**: 使用DtkWidget构建的基于插件的工具箱
* **[ScreenLight](https://github.com/Wing-summer/ScreenLight)**: 使用DtkWidget构建的用于调节屏幕亮度的小工具

## 协议

DTK工具包遵循协议 [LGPL-3.0-or-later](LICENSE).
