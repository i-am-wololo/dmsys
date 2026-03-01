builds = ['SHMBUILD', 'PIPEBUILD', 'MSGQUEUEBUILD']

subdir('./transport/')
subdir('./client/')
subdir('./server/')


foreach build: builds
  executable(
    'dm'+build.to_lower(),
    ['./main.c', './common.c'],
    dependencies: [
      modtransport_deps[build],
      modserver_deps[build],
      modclient_dep
      ],
    c_args: ['-D'+build]
  )
endforeach

