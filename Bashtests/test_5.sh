#!/bin/bash

grep "class1" /etc/group > /dev/null
if test $? -ne 0
then
	groupadd class1
else
	echo "group exists!"
fi


i=1
while test $i -le 30 
do
	grep "`printf "std%02d" $i`" /etc/passwd > /dev/null
	if test $? -ne 0
	then
		useradd -g class1 -s /bin/bash `printf "std%02d" $i`
	else
		echo "`printf "std%02d" $i` exists!"
	fi
	let "i++"
done
