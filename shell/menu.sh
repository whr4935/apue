#!/bin/bash

echo "
    1) amffmpeg
    2) ffmpeg40-iptv
    3) ffmpeg40-upstream
"
echo -n "input your choice:"
read -rn3 inpKey 

echo youer choice $inpKey
case $inpKey in
    1):
        target="amffmpeg"
        ;;
    2):
        target="ffmpeg40-iptv"
        ;;
    3):
        target="ffmpeg40-upstream"
        ;;
esac

if [ -n "$target" ];then
    if [ -h ffmpeg40 ];then
        rm ffmpeg40
    fi

    ln -sf $target ffmpeg40
fi
