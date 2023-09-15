
set( PROJECT_NAME BL )
project( ${PROJECT_NAME} CXX )

add_library_( ${PROJECT_NAME} STATIC 
    DEPENDS
        NetUI
        base
)

target_sources( ${PROJECT_NAME} 
    PRIVATE
        IDAFacade.cpp
        IFactory.h
        IDAFacade.h
        structs/User.cpp
        structs/User.h
)