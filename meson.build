project('chess-engine', 'c',
        default_options : ['c_std=c11'])
add_global_arguments('-Wno-unused-function', language : 'c')

incdir = include_directories('include/')

executable('chess',
           'src/load_fen.c',
           'src/move_gen.c',
           'src/pawn_move_gen.c',
           'src/board.c',
           'src/main.c',
           include_directories : incdir,
          )
