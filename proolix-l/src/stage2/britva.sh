#!/bin/sh
echo britva
#echo "jmp _main" > ctmp.s
cp prefix.s ctmp.s
cat $1 | grep -v "# " | grep -v "/APP" | grep -v "/NO_APP" | grep -v "__main" | grep -v ".file" | grep -v ".seh_" | grep -v ".section" | grep -v ".text" | grep -v ".code16gcc" >> ctmp.s
cat suffix.s >> ctmp.s

