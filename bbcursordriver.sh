#!/bin/bash
if ! [ -f bbcursordriver ] ; then
	echo "No file bbcursordriver found."
	exit 1
fi
if ! [ -x bbcursordriver ] ; then
	echo "bbcursordriver is not executable."
	exit 1
fi

while [ 1 ] ; do
	timeout --foreground 1 ./bbcursordriver $1
	if [ $? -ne 124 ] ; then
		echo -e "\033[0;31mMust be root.\033[0;m"
		exit 1
	fi
done
