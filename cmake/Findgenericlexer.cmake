# Usage:
# FIND_PACKAGE(lexer REQUIRED)
# [...]
# TARGET_LINK_LIBRARIES(target_name lexer)

CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

if (NOT lexer_FOUND)
  INCLUDE(FetchContent)
  set(GENLEXER_VERSION 1.0 CACHE STRING "Generic List version")

  FetchContent_Declare(lexer
    GIT_REPOSITORY "https://github.com/smbss1/GenericLexer.git"
    GIT_TAG release-${GENLEXER_VERSION}
  )
  FetchContent_GetProperties(lexer)

  if (NOT lexer_POPULATED)
    SET(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(lexer)
    SET(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    ADD_SUBDIRECTORY(${lexer_SOURCE_DIR} ${lexer_BINARY_DIR})
    SET(lexer_FOUND TRUE)
  endif()
endif()
