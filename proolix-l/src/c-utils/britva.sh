#!/bin/sh
echo britva 2
cp prefix.S ctmp.s
cat $1 | grep -v ".text" | grep -v "_GLOBAL_OFFSET_TABLE_" | grep -v ".code16gcc" | grep -v ".cfi_" | grep -v "pushl	\-4(%ecx)" | grep -v "/APP" | grep -v "/NO_APP" | grep -v "__main" | grep -v "\.file" | grep -v "\.seh_" | grep -v "\.section" | grep -v "__stack_chk_fail" | grep -v ".type" | grep -v ".ident" | grep -v ".size" | grep -v "__x86" >> ctmp.s
sed -i.bak -e 's/@PLT//g' ctmp.s
