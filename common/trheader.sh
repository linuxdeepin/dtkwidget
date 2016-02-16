#!/bin/bash

public_header=$1
headers=`tr '\n' ' ' < $public_header | sed -e "s/#include//g" | sed -e "s/\"//g"`
file_dir=$(dirname "${public_header}")
for f in $headers
do
    echo $file_dir/$f
done
