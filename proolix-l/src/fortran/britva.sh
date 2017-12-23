#!/bin/sh
# britva ("razor") - foolish and dirty [hack] preprocessor for proolix gfortran utilities
echo britva
cat prefix.S $1 > ftmp.orig
cp prefix.S ftmp.s
cat $1 | grep -v "\.text" | grep -v "_GLOBAL_OFFSET_TABLE_" | grep -v "\.code16gcc" | grep -v "\.cfi_" | grep -v "/APP" | grep -v "/NO_APP" | grep -v "__main" | grep -v "\.file" | grep -v "\.seh_" | grep -v "\.section" | grep -v "__stack_chk_fail" | grep -v "\.type" | grep -v "\.ident" | grep -v "\.size" >> ftmp.s

sed -i -e 's/@GOTOFF(%eax)//g' ftmp.s

sed -i -e 's/@GOTOFF(%ebx)//g' ftmp.s

sed -i -e 's/@PLT//g' ftmp.s

#	replace
# movl    test_global@GOT(%ebx), %eax
#	to
# movl    $test_global, %eax
sed -iBAK -e 's/\([a-zA-Z0-9_]\{1,\}\)@GOT(%[a-zA-Z]\{1,\})/$\1/' ftmp.s

if grep @GOT ftmp.s
then
echo ERROR: Found @GOT modificator!!!
exit 1
fi
