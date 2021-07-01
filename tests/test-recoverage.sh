#!/bin/bash

BUILD_DIR=build
REPORT_DIR=report

#cd ../
#rm -rf $BUILD_DIR
#mkdir $BUILD_DIR
#cd $BUILD_DIR
#qmake .. CONFIG+=debug
#make -j$(nproc)
#cd ../tests/

rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR
qmake ../ CONFIG+=debug
export ASAN_OPTIONS=halt_on_error=0
make check -j$(nproc)

lcov -d ./ -c -o coverage_all.info
lcov --extract coverage_all.info $EXTRACT_ARGS --output-file coverage.info
lcov --remove coverage_all.info "*/tests/*" "*/usr/include*" "*build/src*" --output-file coverage.info
genhtml -o ../../tests/$REPORT_DIR coverage.info

cd ..
genhtml -o $REPORT_DIR $BUILD_DIR/coverage.info

test -e ./build/asan.log* && mv ./build/asan.log* ./build/asan_dtkwidget.log || echo "Not detected any memory leak."
