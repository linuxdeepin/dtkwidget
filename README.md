# Deepin Tool Kit Widget {#mainpage}

Deepin Tool Kit Widget(DtkWidget) provides the base widgets on Deepin.

## Dependencies

### Build dependencies

* Qt >= 5.6

## Installation

### Build from source code

1. Make sure you have installed all dependencies.
```bash
$ sudo apt build-dep ./
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
$ cmake -B build
$ cmake --build build -j$(nproc)
```

3. Install:

```bash
$ cd build
$ sudo make install
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

## License

deepin-tool-kit is licensed under [LGPL-3.0-or-later](LICENSE).
