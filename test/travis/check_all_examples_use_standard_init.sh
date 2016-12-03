#!/bin/bash
FBASE_SNIPPET=test/travis/firebase_init.cc.snip
FBASE_NOAUTH_SNIPPET=test/travis/no_firebase_init.cc.snip
FBASE_NOCONFIG_SNIPPET=test/travis/no_config_init.cc.snip
for example in `find examples/ -name *.ino`;
do
  echo $example;
  (xxd -p $example | tr -d '\n' | grep -q `xxd -p $FBASE_SNIPPET | tr -d '\n'`) ||
  (xxd -p $example | tr -d '\n' | grep -q `xxd -p $FBASE_NOAUTH_SNIPPET | tr -d '\n'`) ||
  (xxd -p $example | tr -d '\n' | grep -q `xxd -p $FBASE_NOCONFIG_SNIPPET | tr -d '\n'`) ||
  if [ $? -ne 0 ];
  then
    echo $example does not contain standard defined in test/travis/*_init.cc.snip.
    exit 1;
  fi;
done;
