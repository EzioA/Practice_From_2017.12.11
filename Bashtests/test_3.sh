#!/bin/bash

if test -e /root/bak && test -d /root/bak
then
	echo "path:/root/bak"
else
	mkdir /root/bak
	echo "folder created successfully!"
fi

echo "* * 1 * * /bin/tar -cvzf `date +%y%m%d`_etc.tar.gz -C /root/bak /etc" > tmpcron
crontab -u root tmpcron

rm tmpcron
