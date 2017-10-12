#!/bin/sh
echo britva 2
cat prefix.S $1 > ctmp.orig
cp prefix.S ctmp.s
# cat $1 | grep -v ".text" | grep -v "_GLOBAL_OFFSET_TABLE_" | grep -v ".code16gcc" | grep -v ".cfi_" | grep -v "pushl	\-4(%ecx)" | grep -v "andl	$\-16, %esp" | grep -v "leal	\-4(%ecx), %esp" | grep -v "leal	\-8(%ebp), %esp" | grep -v "/APP" | grep -v "/NO_APP" | grep -v "__main" | grep -v "\.file" | grep -v "\.seh_" | grep -v "\.section" | grep -v "__stack_chk_fail" | grep -v ".type" | grep -v ".ident" | grep -v ".size" >> ctmp.s
cat $1 | grep -v ".text" | grep -v "_GLOBAL_OFFSET_TABLE_" | grep -v ".code16gcc" | grep -v ".cfi_" | grep -v "/APP" | grep -v "/NO_APP" | grep -v "__main" | grep -v "\.file" | grep -v "\.seh_" | grep -v "\.section" | grep -v "__stack_chk_fail" | grep -v ".type" | grep -v ".ident" | grep -v ".size" >> ctmp.s

sed -i -e 's/@GOTOFF(%eax)//g' ctmp.s

sed -i -e 's/@GOTOFF(%ebx)//g' ctmp.s

sed -i -e 's/@PLT//g' ctmp.s

#	replace
# movl    test_global@GOT(%ebx), %eax
#	to
# movl    $test_global, %eax
#sed -iBAK -e 's/\([a-zA-Z0-9_]\{1,\}\)@GOT(%[a-zA-Z]\{1,\})/$\1/' ctmp.s

if grep @GOT ctmp.s
then
echo ERROR: Found @GOT modificator!!!
exit 1
fi
