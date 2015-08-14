#!/bin/sh
echo britva
cat $1 | grep -v "# " | grep -v "/APP" | grep -v "/NO_APP" | grep -v "__main" | grep -v ".file" | grep -v ".seh_" > ctmp.s
