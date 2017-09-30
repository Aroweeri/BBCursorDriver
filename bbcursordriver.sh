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
done
