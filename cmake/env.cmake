include(cmake/compiler.cmake)

function(set_toolchain_env)
    # TODO: setup toolchain options
endfunction()

function(set_win)
set_win_compiler()
endfunction()

function(set_unix)
set_unix_compiler()
endfunction()

function(set_apple)
set_apple_compiler()
endfunction()
