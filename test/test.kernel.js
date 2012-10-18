'use strict'

var should = require('should');
var assert = require('assert');
var format = require('util').format;
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
    itShouldBeDefined(mmap, 'MAP_32BIT');
    itShouldBeDefined(mmap, 'MAP_ANONYMOUS');
    itShouldBeDefined(mmap, 'MAP_FIXED');
    itShouldBeDefined(mmap, 'MAP_GROWSDOWN');
    itShouldBeDefined(mmap, 'MAP_HUGETLB');
    itShouldBeDefined(mmap, 'MAP_LOCKED');
    itShouldBeDefined(mmap, 'MAP_NONBLOCK');
    itShouldBeDefined(mmap, 'MAP_NORESERVE');
    itShouldBeDefined(mmap, 'MAP_POPULATE');
    itShouldBeDefined(mmap, 'MAP_STACK');
    itShouldBeDefined(mmap, 'MAP_UNINITIALIZED');
    itShouldBeDefined(mmap, 'PAGESIZE');

  });
});
