
modtransport_deps = {}

foreach build: builds
  libmod = library('modtransport'+build.to_lower(),
    [
      './core_transport.c',
      'pipe.c',
      'shm.c',
      'msgq.c',
      '../common.c',
      '../utils.c'
      ],
    c_args: ['-D'+build]
    )
  modtransport_deps += {build: declare_dependency(
        link_with: libmod,
        include_directories: include_directories('.'),
      )
    }
endforeach

