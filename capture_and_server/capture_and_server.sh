#!/bin/bash
rm -rf picture
mkdir picture

rm LastPicture.txt
touch LastPicture.txt

echo "Server start"
./server &

echo "Capture pictures"
./capture 

