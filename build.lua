module 'core' {
   lib {
      src {
         'src/*.cpp',
         'src/native/*.cpp',
         'src/native/$(toolchain)/*.cpp',
         exclude 'src/id.cpp',
         exclude 'src/namespaced_id.cpp',
         pch_src 'src/pch.cpp'
      },
      define 'BE_CORE_IMPL',
      link_project 'zlib-static'
   },
   lib '-id' {
      src { 'src/id.cpp', 'src/namespaced_id.cpp' },
      define 'BE_CORE_ID_IMPL',
      define 'BE_CORE_IMPL',
      link_project 'core'
   },
   lib '-id-with-names' {
      src { 'src/id.cpp', 'src/namespaced_id.cpp' },
      define {
         'BE_CORE_ID_IMPL',
         'BE_CORE_IMPL',
         'BE_ID_NAMES_ENABLED'
      },
      export_define 'BE_ID_NAMES_ENABLED',
      link_project 'core'
   },
   lib '-lua' {
      src 'src-lua/*.cpp',
      define 'BE_CORE_LUA_IMPL',
      link_project 'core',
      link_project 'belua'
   },
   app '-test' {
      icon 'icon/bengine-test-perf.ico',
      link_project 'testing'
   },
   app '-perf' {
      icon 'icon/bengine-test-perf.ico',
      link_project {
         'testing', 'perf'
      }
   }
}
