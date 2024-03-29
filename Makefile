BUILDTYPE ?= Release

ifeq (${BUILDTYPE},Debug)
GYP_BUILD_TYPE = --d
else
GYP_BUILD_TYPE = --r
BUILDTYPE = Release
endif

TARGET = build/${BUILDTYPE}/cabinet.node


all: $(TARGET)

$(TARGET): src/cabinet.cc src/kernel_wrap.cc src/debug.h
	node-gyp rebuild ${GYP_BUILD_TYPE}

clean:
	node-gyp clean
	rm -rf build

distclean: clean
	rm -rf node_modules

node_modules: package.json
	npm install

test: node_modules $(TARGET)
	./node_modules/.bin/mocha -R spec

.PHONY: test
