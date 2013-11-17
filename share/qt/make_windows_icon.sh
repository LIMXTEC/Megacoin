#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/megacoin.png
ICON_DST=../../src/qt/res/icons/megacoin.ico
convert ${ICON_SRC} -resize 16x16 megacoin-16.png
convert ${ICON_SRC} -resize 32x32 megacoin-32.png
convert ${ICON_SRC} -resize 48x48 megacoin-48.png
convert megacoin-16.png megacoin-32.png megacoin-48.png ${ICON_DST}

