#!/bin/bash

# SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: LGPL-3.0-or-later

BUILD_DIR=`pwd`/../build/tests/
HTML_DIR=${BUILD_DIR}/html
XML_DIR=${BUILD_DIR}/report

export ASAN_OPTIONS="halt_on_error=0"

# back to project directroy
cd ..

cmake -Bbuild -DCMAKE_BUILD_TYPE=Debug -GNinja

cmake --build build --target ut-dtkwidget

cd $BUILD_DIR

./ut-dtkwidget
lcov -d ./ -c -o coverage_all.info

filter_files=(
# deprecated
"*/src/widgets/dimagebutton*"
"*/src/widgets/dbaseexpand*"
"*/src/widgets/dexpandgroup*"
"*/src/widgets/dsegmentedcontrol*"
"*/src/widgets/dshortcutedit*"
"*/src/widgets/dtoast*"
"*/src/widgets/dswitchlineexpand*"
"*/src/widgets/darrowlineexpand*"
"*/src/util/dregionmonitor*"
"*/src/util/dtrashmanager*"
"*/src/util/dthumbnailprovider*"
# depends xcb, etc...
"*/dthememanager*"
"*/dsimplelistview*"
"*/dmainwindow*"
"*/dapplication*"
"*/xutil*"
"*/private/startupnotifications*"
"*/private/keyboardmonitor*"
# dbus
"*/dbusmpris.*"
"*/dbusinterface.*"
"*/ddesktopservices_linux.cpp"
)
lcov --remove coverage_all.info "*/tests/*" "*/usr/include*" "*build/src*" ${filter_files[*]} --output-file coverage.info
genhtml -o $HTML_DIR $BUILD_DIR/coverage.info && mv ${BUILD_DIR}/html/index.html ${BUILD_DIR}/html/cov_dtkwidget.html

test -e ${BUILD_DIR}/asan.log* && mv ${BUILD_DIR}/asan.log* ${BUILD_DIR}/asan_dtkwidget.log || touch ${BUILD_DIR}/asan_dtkwidget.log

