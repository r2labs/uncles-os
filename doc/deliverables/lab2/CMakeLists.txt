cmake_minimum_required (VERSION 2.8)

project (ee445m-doc-lab2)
include (../UseLATEX.cmake)

set (doc-staging-area "./doc-staging")

add_custom_target (copy)
add_custom_command (
  TARGET copy
  COMMAND ${CMAKE_COMMAND} -E copy ../../../board-progs/test-os/test-os.c ${doc-staging-area}/test-os.c
  COMMAND ${CMAKE_COMMAND} -E copy ../../../lib/libos/os.c ${doc-staging-area}/os.c
  COMMAND ${CMAKE_COMMAND} -E copy ../../../lib/libos/os.h ${doc-staging-area}/os.h
)

add_latex_document (writeup.tex
  IMAGE_DIRS img
  DEPENDS copy
  CONFIGURE writeup.tex
  USE_GLOSSARY
  DEFAULT_PDF
)