#
# Copyright 2016 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

FIREBASE_DIR=../..
GTEST_DIR=googletest/googletest
ARDUINOJSON_DIR=../../src/third-party/arduino-json-5.6.7

FIREBASE_SRCS=${FIREBASE_DIR}/src/FirebaseObject.cpp
GTEST_SRCS=${GTEST_DIR}/src/gtest-all.cpp

SRCS=FirebaseArduino_test.cpp\
	${FIREBASE_SRCS}\
	${GTEST_SRCS}

OBJS=${SRCS:.cpp=.o}

CXXFLAGS=-I. -I${FIREBASE_DIR}/src -Igoogletest/googletest/include -Igoogletest/googletest -std=c++11 -g
LDFLAGS=-lpthread

all: check

firebase-test: ${OBJS}
	${CXX} -o $@ ${OBJS} ${LDFLAGS}

check: firebase-test
	./firebase-test

clean:
	rm -f ${OBJS} firebase-test
