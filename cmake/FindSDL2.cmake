message( "finding SDL2!"  )

if(WIN32)

    message("Is Windows")
    set(SDL2_PATH $ENV{SDL2_PATH})
    if( SDL2_PATH )

        message("Find LIB_SDL2 env!")
        message(${SDL2_PATH})

        find_path( SDL2_INCLUDE_DIR SDL.h "${SDL2_PATH}/include" )
        find_library( SDL2_LIBRARY SDL2.lib "${SDL2_PATH}/lib" "${SDL2_PATH}/lib/x64")
        find_library( SDL2_MAIN_LIBRARY SDL2main.lib "${SDL2_PATH}/lib" "${SDL2_PATH}/lib/x64")

        if( SDL2_INCLUDE_DIR AND SDL2_LIBRARY AND SDL2_MAIN_LIBRARY)

            set( SDL2_FOUND TRUE )

        else()

            set( SDL2_FOUND FALSE )

        endif()

    else()

        set( SDL2_FOUND FALSE )
        message("Not Find SDL2_PATH env!")

    endif()

else()

    message("Not Windows!")

endif()

message("................................................................")