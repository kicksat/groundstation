INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_SPRITE sprite)

FIND_PATH(
    SPRITE_INCLUDE_DIRS
    NAMES sprite/api.h
    HINTS $ENV{SPRITE_DIR}/include
        ${PC_SPRITE_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREEFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    SPRITE_LIBRARIES
    NAMES gnuradio-sprite
    HINTS $ENV{SPRITE_DIR}/lib
        ${PC_SPRITE_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SPRITE DEFAULT_MSG SPRITE_LIBRARIES SPRITE_INCLUDE_DIRS)
MARK_AS_ADVANCED(SPRITE_LIBRARIES SPRITE_INCLUDE_DIRS)

