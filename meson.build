project('yugioh-pic-dl', 'cpp',
  version : '0.1',
  default_options : ['buildtype=release','warning_level=3', 'cpp_std=c++17'])


sources = [
            'dbquery.h',
            'dbquery.cpp',
            'yugioh_pic_dl.h',
            'yugioh_pic_dl.cpp'
          ]

deps = [
  dependency('sqlite3', version: '>=3.34.0'),
  dependency('libcurl', version: '>=8.0.0')
]

executable('yugioh-pic-dl',sources,
           dependencies: deps,
           install : false
           )
