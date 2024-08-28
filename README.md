# Deepin Tool Kit Widget {#mainpage}

Deepin Tool Kit (Dtk) is the base devlopment tool of all C++/Qt Developer work on Deepin.

## Dependencies

### Build dependencies

* Qt >= 5.6

## Installation

### Build from source code

1. Make sure you have installed all dependencies.
````
sudo apt build-dep .
````
If you need to use the designer plugin, you should also install:
````
sudo apt install qttools5-dev
````
2. Build:

````
$ mkdir build
$ cd build
$ qmake ..
$ make
````

3. Install:

````
$ sudo make install
````

## Getting help

Any usage issues can ask for help via

* [Gitter](https://gitter.im/orgs/linuxdeepin/rooms)
* [IRC channel](https://webchat.freenode.net/?channels=deepin)
* [Forum](https://bbs.deepin.org)
* [WiKi](https://wiki.deepin.org/)

## Getting involved

We encourage you to report issues and contribute changes

* [Contribution guide for developers](https://github.com/linuxdeepin/developer-center/wiki/Contribution-Guidelines-for-Developers-en). (English)
* [开发者代码贡献指南](https://github.com/linuxdeepin/developer-center/wiki/Contribution-Guidelines-for-Developers) (中文)

## License

deepin-tool-kit is licensed under [GPLv3](LICENSE).
