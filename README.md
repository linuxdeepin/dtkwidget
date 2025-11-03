# Deepin Tool Kit Widget

Deepin Tool Kit Widget(DtkWidget) provides the base widgets on Deepin.

中文说明：[README_zh_CN.md](./README.zh_CN.md)

## Documentation

中文文档：[dtkwidget文档](https://linuxdeepin.github.io/dtkwidget/index.html)

## Dependencies

### Build dependencies

* Qt >= 5.6

## Installation

### Build from source code

1. Make sure you have installed all dependencies.

```bash
sudo apt build-dep ./
```

If you need to use the designer plugin, you can:

```bash
$ sudo apt install qttools5-dev

# build
$ cmake ./plugin/dtkuiplugin -B build -DINSTALL_PLUGIN=ON -DCMAKE_INSTALL_PREFIX=/usr
$ cmake --build build -j$(nproc)

# install
$ cd build
$ sudo make install
```

2. Build:

```bash
cmake -B build
cmake --build build -j$(nproc)
```

3. Install:

```bash
cd build
sudo make install
```

## Getting help

Any usage issues can ask for help via

* [Gitter](https://gitter.im/orgs/linuxdeepin/rooms)
* [IRC channel](https://webchat.freenode.net/?channels=deepin)
* [Forum](https://bbs.deepin.org)
* [WiKi](https://wiki.deepin.org/)

## Getting involved

We encourage you to report issues and contribute changes

* [Contribution guide for developers](https://github.com/linuxdeepin/developer-center/wiki/Contribution-Guidelines-for-Developers-en). (English)

## Made with dtkwidget

List of some open source projects using dtkwidget: (Contact us or open a pull request to add yours)

* **[DMarked](https://github.com/DMarked/DMarked)**: Markdown Editor with dtkwidget
* **[DtkTimer](https://github.com/gfdgd-xi/timer)**: Clock with dtkwidget
* **[simple-image-filter](https://github.com/dependon/simple-image-filter)**: Image Process with dtkwidget
* **[SparkStore](https://github.com/Spark-Store/Spark-Store)**: SparkStore
* **[WingHexExplorer](https://github.com/Wing-summer/WingHexExplorer)**: Powerful Hexadecimal Editor with dtkwidget
* **[WingTool](https://github.com/Wing-summer/WingTool)**: A Productivity Plugin-based Toolbox with dtkwidget
* **[ScreenLight](https://github.com/Wing-summer/ScreenLight)**: A small tool to adjust the brightness of the screen with dtkwidget

## License

deepin-tool-kit is licensed under [LGPL-3.0-or-later](LICENSE).
