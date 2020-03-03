#!/bin/bash

if [ -z "$*" ];then
    echo "arg is null"
    exit 
fi

pathP=projects/android-p/out/target/product/franklin/
pathQ=projects/android-q/out/target/product/newton/

root_path=/mnt/fileroot02/houren.wang/
project_path=$pathP  #default project path

for arg ;do
case $arg in
    'P')
        project_path=$pathP
        shift
        ;;

    'Q')
        project_path=$pathQ
        shift
        ;;

    '-r')
        project_path=
        shift
        ;;

esac; done 

#echo project_path=$project_path
#echo args=$*
path=$root_path$project_path
echo path=$path

if [ $# -eq 0 ];then
    echo "arg is null"
    exit 
fi

echo src_file=$path$*

exit 0

scp -r 10.18.11.213:$path/$* $PWD 


