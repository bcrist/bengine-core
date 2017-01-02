module { name = 'core',
   projects = {
      lib { suffix = 'id',
         src = {
            'src/id.cpp',
            'src/namespaced_id.cpp'
         },
         preprocessor = {
            'BE_CORE_ID_IMPL',
            'BE_CORE_IMPL'
         }
      },
      lib { suffix = 'id-with-names',
         src = {
            'src/id.cpp',
            'src/namespaced_id.cpp'
         },
         preprocessor = {
            'BE_CORE_ID_IMPL',
            'BE_CORE_IMPL',
            'BE_ID_NAMES_ENABLED'
         },
         exported_preprocessor = {
            'BE_ID_NAMES_ENABLED'
         }
      },
      lib {
         limp = {
            'include/log_attrib_ids.hpp',
            'include/service_ids.hpp'
         },
         src = {
            'src/*.cpp',
            'src/native/*.cpp',
            'src/native/' .. toolchain .. '/*.cpp',
            pch = 'src/pch.cpp',
            exclude = {
               'src/id.cpp',
               'src/namespaced_id.cpp'
            }
         },
         preprocessor = {
            'BE_CORE_IMPL'
         }
      },
      app { suffix = 'test',
         icon = 'icon/bengine-test-perf.ico',
         src = {
            'test/*.cpp'
         },
         libs = {
            'testing',
            'core',
            'core-id'
         }
      },
      app { suffix = 'perf',
         icon = 'icon/bengine-test-perf.ico',
         src = {
            'perf/*.cpp'
         },
         libs = {
            'testing',
            'core',
            'core-id'
         }
      }
   }
}
