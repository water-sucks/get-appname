project('get-appname', 'cpp',
  version : '0.1.0',
  default_options : ['warning_level=3', 'cpp_std=c++11'])

giounix = dependency('gio-unix-2.0')

executable('get-appname',
           './main.cpp',
           dependencies: [
             giounix,
           ],
           install : true)
