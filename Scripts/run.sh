#!/bin/sh

buildType=`cat Scripts/buildType.txt`
runDirectory=""

case "$buildType" in
    "Debug")
        runDirectory="build/debug"
        ;;
    "Release")
        runDirectory="build/release"
        ;;
    *)
        echo "Unknown build type"
        exit 1
esac

cp -r "resources" $runDirectory
echo "Running in $buildType mode"
cd $runDirectory && ./Pong
