subdir('./transport/')
subdir('./client/')
subdir('./server/')


executable('dm',
  ['./main.c', './common.c'],
  dependencies: [
    modtransport_dep,
    modclient_dep,
    modserver_dep
  ]
)
