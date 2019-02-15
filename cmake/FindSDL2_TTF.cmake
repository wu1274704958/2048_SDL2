message( "finding SDL2_ttf!"  )

if(WIN32)

    message("Is Windows")
    set(SDL2_TTF_PATH $ENV{SDL2_TTF_PATH})
    if( SDL2_TTF_PATH )

        message("Find SDL2_TTF_PATH env!")
        message(${SDL2_TTF_PATH})

        find_path( SDL2_TTF_INCLUDE_DIR SDL_ttf.h "${SDL2_TTF_PATH}/include" )
        find_library( SDL2_TTF_LIBRARY SDL2_ttf.lib "${SDL2_TTF_PATH}/lib" "${SDL2_TTF_PATH}/lib/x64")

        if( SDL2_TTF_INCLUDE_DIR AND SDL2_TTF_LIBRARY )

            set( SDL2_TTF_FOUND TRUE )

        else()

            set( SDL2_TTF_FOUND FALSE )

        endif()

    else()

        set( SDL2_TTF_FOUND FALSE )
        message("Not Find SDL2_TTF_PATH env!")

    endif()

else()

    message("Not Windows!")

endif()

message("................................................................")