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
      'target_name': 'cabinet',
      'sources': [
        'src/kernel_wrap.cc',
        'src/cabinet.cc'
      ]
    }
  ]
}
