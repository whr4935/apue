#!/bin/bash

function getdir() {
    for dir in `ls $1`
    do
        if [ -d $1"/"$dir ];then
            # echo $1"/"$dir
            touch $1"/"$dir/"readme.md"
        fi
    done
}

ROOT=$PWD
CUR_DIR=${ROOT##*/}
# echo $CUR_DIR

if [ $CUR_DIR = "scripts" ];then
    ROOT=..
elif [ -f "Makefile" ];then
    ROOT=.
else
    echo "exit,do nothing."
    exit
fi

if [ -n "$ROOT" ];then
    echo root_dir=$ROOT
    getdir $ROOT 
fi
