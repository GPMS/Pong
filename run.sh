#!/bin/sh

buildType=`cat config.txt`
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

echo "Running in $buildType mode"
./$runDirectory/Pong

