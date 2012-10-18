{
  'target_defaults': {
    'configurations': {
      'Debug': {
        'defines': [ 'DEBUG', '_DEBUG' ]
      },
      'Release': {
        'defines': [ 'NDEBUG' ]
      }
    }
  },
  'targets': [
    {
      'target_name': 'kvs',
      'sources': [
        'src/kernel_wrap.cc',
        'src/kvs.cc'
      ]
    }
  ]
}
