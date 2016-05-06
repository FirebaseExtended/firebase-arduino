#!/bin/bash
STANDARD_SNIPPET=test/travis/example_standard_init.cc.snip
for example in `find examples/ -name *.ino`;
do
  echo $example;
  xxd -p $example | tr -d '\n' | grep `xxd -p $STANDARD_SNIPPET | tr -d '\n'`;
  if [ $? -ne 0 ];
  then
    echo $example does not contain standard defined in $STANDARD_SNIPPET.
    exit 1;
  fi;
done;
