# libdui

libdui is a UI library for Deepin.

## Dependencies

### Build dependencies

* qmake >= 5.3

### Runtime dependencies

* Qt >= 5.3
  * Qt5-core
  * Qt5-gui
  * Qt5-widget
  * Qt5-multimedia
  * Qt5-multimediawidgets
  * Qt5-x11extras
* xext

## Installation

### Build from source code

1. Make sure you have installed all dependencies.

2. Build:
```
$ cd libdui
$ mkdir build
$ cd build
$ qmake ..
$ make
```

3. Install:
```
$ sudo make install
```

When install complete, the binary file is placed into `/usr/lib/`, c++ header file is placed into `/usr/include/libdui/`.

## Getting help

Any usage issues can ask for help via

* [Gitter](https://gitter.im/orgs/linuxdeepin/rooms)
* [IRC channel](https://webchat.freenode.net/?channels=deepin)
* [Forum](https://bbs.deepin.org)
* [WiKi](http://wiki.deepin.org/)

## Getting involved

We encourage you to report issues and contribute changes

* [Contribution guide for users](http://wiki.deepin.org/index.php?title=Contribution_Guidelines_for_Users)
* [Contribution guide for developers](http://wiki.deepin.org/index.php?title=Contribution_Guidelines_for_Developers).

## License

libdui is licensed under [GPLv3](LICENSE).
