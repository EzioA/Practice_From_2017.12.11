#!/bin/bash

if test -b $1 || test -c $1
then
	cp $1 /dev
else
	echo "It's a wrong type!"
fi
