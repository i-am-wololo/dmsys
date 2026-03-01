
modserver_deps = {}

foreach build: builds
  libmod = library('modserver'+build.to_lower(),
    [
        './server.c',
        './serverpipe.c',
        # 'servershm.c',
        # 'servermsg.c',
      '../common.c'
    ],
    c_args: ['-D'+build]
  )
  modserver_deps += {build: declare_dependency(
    link_with: libmod,
    include_directories: include_directories('.'),
  )}
endforeach



