#!/bin/bash

tr '\n' ' ' < $1 | sed -e "s/#include//g" | sed -e "s/\"//g"
