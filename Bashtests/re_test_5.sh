#!/bin/bash

i=1
group=class1

while test $i -le 30 
do
	grep "`printf "std%02d" $i`" /etc/passwd > /dev/null
	if test $? -eq 0
	then
		userdel `printf "std%02d" $i`
	else
		echo "`printf "std%02d" $i` doesn/t exist!"
	fi
	let "i++"
done

grep "^class1" /etc/group > /dev/null
if test $? -eq 0
then
	groupdel class1
else
	echo "class1 doesn't exist!"
fi

