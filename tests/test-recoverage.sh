#!/bin/bash

BUILD_DIR=`pwd`/../build-ut
HTML_DIR=${BUILD_DIR}/html
XML_DIR=${BUILD_DIR}/report

cd ../
#rm -rf $BUILD_DIR
#mkdir $BUILD_DIR
#cd $BUILD_DIR
#qmake .. CONFIG+=debug
#make -j$(nproc)
#cd ../tests/

rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR

qmake .. CONFIG+=debug && make qmake_all
#make -j$(nproc)
cd ../tests/

rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR
qmake ../ CONFIG+=debug
export ASAN_OPTIONS=halt_on_error=0
TESTARGS="--gtest_output=xml:${XML_DIR}/report_dtkwidget.xml" make check -j$(nproc)

lcov -d ./ -c -o coverage_all.info
lcov --extract coverage_all.info $EXTRACT_ARGS --output-file coverage.info
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
lcov --remove coverage_all.info "*/tests/*" "*/usr/include*" "*build-ut/src*" ${filter_files[*]} --output-file coverage.info
genhtml -o $HTML_DIR $BUILD_DIR/coverage.info && mv ${BUILD_DIR}/html/index.html ${BUILD_DIR}/html/cov_dtkwidget.html

test -e ${BUILD_DIR}/asan.log* && mv ${BUILD_DIR}/asan.log* ${BUILD_DIR}/asan_dtkwidget.log || touch ${BUILD_DIR}/asan_dtkwidget.log
