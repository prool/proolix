#!/bin/sh
echo britva 2
cp prefix.S ctmp.s
cat $1 | grep -v ".text" | grep -v "_GLOBAL_OFFSET_TABLE_" | grep -v ".code16gcc" >> ctmp.s
