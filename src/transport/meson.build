modtransport = library('modtransport', 
  [
    './core_transport.c',
    '../common.c',
    '../utils.c'
    ]
)

modtransport_dep = declare_dependency(
  link_with: modtransport,
  include_directories: include_directories('.')
  )
