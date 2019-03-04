#!/bin/bash

function fun1()
{
    #echo $1 $2 $3
    return 10
}

function fun2()
{
    echo 11111
    v1=$(fun1 a b c)
    echo aaaaa
    #echo $v1
}

fun2
#echo $v1

