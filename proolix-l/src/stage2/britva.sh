#!/bin/sh
echo britva
#echo "jmp _main" > ctmp.s
cp prefix.s ctmp.s
cat $1 | grep -v "# " | grep -v "/APP" | grep -v "/NO_APP" | grep -v "__main" | grep -v "\.file" | grep -v "\.seh_" | grep -v "\.section" | grep -v "\.text" | grep -v "\.code16gcc" | grep -v "__stack_chk_fail" | grep -v "\.cfi" | grep -v "pushl	\-4(%ecx)" | grep -v GLOBAL_OFFSET_TABLE >> ctmp.s
cat suffix.s >> ctmp.s

sed -i -e 's/@GOTOFF(%eax)//g' ctmp.s

sed -i -e 's/@GOTOFF(%ebx)//g' ctmp.s

#	replace
# movl    test_global@GOT(%ebx), %eax
#	to
# movl    $test_global, %eax
sed -iBAK -e 's/\([a-zA-Z0-9_]\{1,\}\)@GOT(%[a-zA-Z]\{1,\})/$\1/' ctmp.s

if grep @GOT ctmp.s
then
echo ERROR: Found @GOT modificator!!!
exit 1
fi
