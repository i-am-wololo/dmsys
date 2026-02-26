subdir('./transport/')
subdir('./client/')


executable('dm',
  ['./main.c', './common.c'],
  dependencies: [
    modtransport_dep,
    modclient_dep
  ]
)
