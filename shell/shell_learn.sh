#!/bin/bash

#if [ ! 2 -eq 2 -a 3 -eq 3 ]; then
    #echo yes
#else
    #echo no
#fi


#echo \$*
#for arg in "$*"
#do
    #echo $arg
#done


#for arg in "$@"
#do
    #echo $1
    #shift
#done


#exit 0


while getopts "a:bc" arg
do
    case $arg in
        a)
            echo "$OPTIND a's arg:$OPTARG"
            ;;

        b)
            echo "$OPTIND arg b"
            ;;

        c)
            echo "$OPTIND arg c"
            ;;
        ?)
            echo "unknow argument"
            exit 1
            ;;
    esac
done
shift $(($OPTIND-1))
echo extra args: $*


