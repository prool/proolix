#!/bin/sh
echo britva
#echo "jmp _main" > ctmp.s
cp ../lib/prefix.s ctmp.s
cat $1 | grep -v "# " | grep -v "/APP" | grep -v "/NO_APP" | grep -v "__main" | grep -v ".file" | grep -v ".seh_" | grep -v ".section" | grep -v ".text" | grep -v ".code16gcc" | grep -v "__stack_chk_fail" | grep -v ".cfi" | grep -v "pushl	\-4(%ecx)" >> ctmp.s
cat ../lib/suffix.s >> ctmp.s
