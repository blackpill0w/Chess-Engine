project('chess-engine', 'cpp',
        default_options : ['cpp_std=c++17'])

incdir = include_directories('include/')

libengine = library('chess-engine',
                    'src/bitboard.cpp',
                    'src/load_fen.cpp',
                    'src/move_gen.cpp',
                    'src/pawn_move_gen.cpp',
                    'src/board.cpp',
                    'src/zobrist.cpp',
                    include_directories : incdir,
                   )

libengine_dep = declare_dependency(include_directories : incdir,
                                   link_with : libengine)

raylib_cpp_dep = dependency('raylib-cpp')
fmt_dep = dependency('fmt')

executable('chess',
           'src/main.cpp',
           include_directories : incdir,
           link_with : libengine)

executable('tests',
           'src/tests.cpp',
           include_directories : incdir,
           link_with : libengine,
           dependencies : fmt_dep)

executable('chess-gui',
           'src/main2.cpp',
           include_directories : incdir,
           link_with : libengine,
           dependencies : raylib_cpp_dep)
