modclients = library('modclients', 
  [
    './client.c',
    '../utils.c'
  ]
)

modclient_dep = declare_dependency(
  link_with: modclients,
  include_directories: include_directories('.')
  )
