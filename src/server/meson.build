
modserver_deps = {}

foreach build: builds
  libmod = library('modserver'+build.to_lower(),
    [
        './serverpipe.c',
        './servershm.c',
        './servermsgq.c',
        '../common.c',
        '../utils.c'
    ],
    c_args: ['-D'+build]
  )
  modserver_deps += {build: declare_dependency(
    link_with: libmod,
    include_directories: include_directories('.'),
  )}
endforeach



