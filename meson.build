project('chess-engine', 'c',
        default_options : ['c_std=c11'])
add_project_arguments('-Wno-unused-function', language : 'c')

incdir = include_directories('include/')

libengine = library('chess-engine',
           'src/load_fen.c',
           'src/move_gen.c',
           'src/pawn_move_gen.c',
           'src/board.c',
           include_directories : incdir,
          )

libengine_dep = declare_dependency(include_directories : incdir,
                                   link_with : libengine)

executable('chess',
           'src/main.c',
           include_directories : incdir,
           link_with : libengine)
