'use strict'

var should = require('should');
var assert = require('assert');
var format = require('util').format;
var fs = require('fs');
var path = require('path');
var kernel = require('../lib/kvs').kernel;


var itShouldBeDefined = function (obj, prop) {
  var msg = format('should be defined `%s`', prop);
  it(msg, function () {
    should.exist(obj[prop]);
  });
};


describe('kernel', function () {
  describe('mmap', function () {
    var mmap = kernel.mmap;
    itShouldBeDefined(mmap, 'PROT_NONE');
    itShouldBeDefined(mmap, 'PROT_READ');
    itShouldBeDefined(mmap, 'PROT_WRITE');
    itShouldBeDefined(mmap, 'PROT_EXEC');
    itShouldBeDefined(mmap, 'MAP_SHARED');
    itShouldBeDefined(mmap, 'MAP_PRIVATE');
    itShouldBeDefined(mmap, 'MAP_NORESERVE');
    itShouldBeDefined(mmap, 'MAP_FIXED');
    // TODO: should be check per platform
    //itShouldBeDefined(mmap, 'MAP_GROWSDOWN');
    //itShouldBeDefined(mmap, 'MAP_32BIT');
    //itShouldBeDefined(mmap, 'MAP_ANONYMOUS');
    //itShouldBeDefined(mmap, 'MAP_HUGETLB');
    //itShouldBeDefined(mmap, 'MAP_LOCKED');
    //itShouldBeDefined(mmap, 'MAP_NONBLOCK');
    //itShouldBeDefined(mmap, 'MAP_POPULATE');
    //itShouldBeDefined(mmap, 'MAP_STACK');
    //itShouldBeDefined(mmap, 'MAP_UNINITIALIZED');
    itShouldBeDefined(mmap, 'PAGESIZE');

    describe('map', function () {
      describe('arguments', function () {
        var fd, size;
        before(function (done) {
          fd = fs.openSync(__filename, 'r');
          size = fs.fstatSync(fd).size;
          done();
        });

        // TODO: should be check argument pattern ...
        it('should be succeed with `all` arguments', function (done) {
          var buf = mmap.map(size, mmap.PROT_READ, mmap.MAP_SHARED, fd, 0);
          buf.length.should.eql(size);
          done();
        });

        it('should be succeed with `3` arguments', function (done) {
          (function () {
            var buf = mmap.map(size, mmap.PROT_READ, mmap.MAP_SHARED);
          }).should.throw(/Bad argument/);
          done();
        });

        it('should be succeed with `4` arguments', function (done) {
          var buf = mmap.map(size, mmap.PROT_READ, mmap.MAP_SHARED, fd);
          buf.length.should.eql(size);
          done();
        });

        it('should be succeed with arguments and callback', function (done) {
          mmap.map(size, mmap.PROT_READ, mmap.MAP_SHARED, fd, 0, function (err, buf) {
            if (err) { return done(err); }
            buf.length.should.eql(size);
            done();
          });
        });
      });

      var testMapSize = function (size) {
        var file = path.join(__dirname, format('map%d', size));
        var fd;
        describe('map size', function () {
          before(function (done) {
            fd = fs.openSync(file, 'a+');
            var buf = new Buffer(size);
            buf.fill(0xFF, 0, buf.length);
            fs.writeSync(fd, buf, 0, buf.length);
            done();
          });
          after(function (done) {
            fs.closeSync(fd);
            fs.unlink(file);
            done();
          });
          it(format('should be succedd with `%d` size', size), function (done) {
            var b = mmap.map(size, mmap.PROT_READ | mmap.PROT_WRITE, mmap.MAP_SHARED, fd);
            b.length.should.eql(size);
            for (var i = 0; i < b.length; i++) {
              b[i].should.eql(0xFF);
            }
            done();
          });
        });
      };

      testMapSize(mmap.PAGESIZE);
      testMapSize(mmap.PAGESIZE * 2);
      testMapSize(mmap.PAGESIZE * 3);
      testMapSize(mmap.PAGESIZE * 8);
      testMapSize(mmap.PAGESIZE * mmap.PAGESIZE);
      //testMapSize(0x3fffffff);
    });
  });
});
