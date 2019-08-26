#
# @Author: Jxtopher
# @License: *
# @Date: 2018-10
# @Version: 1
# @Description: Makefile
#
.PHONY: all test check
MAKEFLAGS += --no-print-directory

all : check
	@cd build && cmake ..
	@cd build && make

test: check
	@cd build && cmake ..
	@cd build && make
	@cd build && make test

check:
	#build-essential
	@hash cmake 2> /dev/null || sudo apt-get install -y cmake 2> /dev/null || ( echo >&2 "Please install cmake" ; exit -1)