BUILDTYPE ?= Release

ifeq (${BUILDTYPE},Debug)
GYP_BUILD_TYPE = --d
else
GYP_BUILD_TYPE = --r
endif

TARGET = build/${BUILDTYPE}/kvs.node


all: $(TARGET)

$(TARGET): src/kvs.cc src/kernel_wrap.cc src/debug.h
	node-gyp rebuild ${GYP_BUILD_TYPE}

clean:
	node-gyp clean
	rm -rf build

distclean: clean
	rm -rf node_modules

node_modules: package.json
	npm install

test: node_modules $(TARGET)
	./node_modules/.bin/mocha

.PHONY: test
