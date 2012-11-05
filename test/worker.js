var kernel = require('../lib/cabinet').kernel;
var mmap = kernel.mmap;
var fs = require('fs');

var shared_path = __dirname + '/shared';
var size = mmap.PAGESIZE;
var fd = fs.openSync(shared_path, 'a+');

process.on('message', function (msg, srv) {
  if (!msg || !msg.cmd) { return; }

  switch (msg.cmd) {
    case 'get':
      var buf = mmap.map(size, mmap.PROT_READ | mmap.PROT_WRITE, mmap.MAP_SHARED, fd);
      console.log(process.pid, buf[0]);
      process.send({
        cmd: 'get',
        value: buf[0]
      });
      break;
  }
});

process.on('exit', function () {
  fs.closeSync(fd);
});
