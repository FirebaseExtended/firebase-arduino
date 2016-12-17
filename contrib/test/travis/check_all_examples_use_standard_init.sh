#!/bin/bash
FBASE_SNIPPET=contrib/test/travis/firebase_init.cc.snip
FBASE_NOAUTH_SNIPPET=contrib/test/travis/no_firebase_init.cc.snip
FBASE_NOCONFIG_SNIPPET=contrib/test/travis/no_config_init.cc.snip
for example in `find examples/ contrib/examples/ -name *.ino`;
do
  echo $example;
  (xxd -p $example | tr -d '\n' | grep -q `xxd -p $FBASE_SNIPPET | tr -d '\n'`) ||
  (xxd -p $example | tr -d '\n' | grep -q `xxd -p $FBASE_NOAUTH_SNIPPET | tr -d '\n'`) ||
  (xxd -p $example | tr -d '\n' | grep -q `xxd -p $FBASE_NOCONFIG_SNIPPET | tr -d '\n'`) ||
  if [ $? -ne 0 ];
  then
    echo $example does not contain standard defined in contrib/test/travis/*_init.cc.snip.
    exit 1;
  fi;
done;
