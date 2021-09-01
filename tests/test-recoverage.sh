#!/bin/bash

BUILD_DIR=build
REPORT_DIR=report

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
TESTARGS="--gtest_output=xml:dde_test_report_dtkwidget.xml" make check -j$(nproc)

lcov -d ./ -c -o coverage_all.info
lcov --extract coverage_all.info $EXTRACT_ARGS --output-file coverage.info
filter_files=(
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
"*/dthememanager*"
"*/dmainwindow*"
"*/dapplication*"
"*/dblureffectwidget*"
"*/private/startupnotifications*"
"*/private/keyboardmonitor*"
)
lcov --remove coverage_all.info "*/tests/*" "*/usr/include*" "*build/src*" ${filter_files[*]} --output-file coverage.info
genhtml -o ../../tests/$REPORT_DIR coverage.info

cd ..
test -e ./build/asan.log* && mv ./build/asan.log* ./build/asan_dtkwidget.log || touch ./build/asan.log
