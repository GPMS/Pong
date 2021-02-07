#!/bin/sh

buildType=`cat Scripts/buildType.txt`
buildDir=""

case "$buildType"  in
    "Debug")
        buildDir="build/debug"
        ;;
    "Release")
        buildDir="build/release"
        ;;
    *)
        echo "Unknown build type"
        exit 1
esac

mkdir -p $buildDir
cp -R "resources" $buildDir
echo "Building in $buildType mode"
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$buildType -B $buildDir
cd $buildDir && make

