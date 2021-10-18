#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

COMPONENT_ADD_INCLUDEDIRS := ../utils
CONFIG += c++11
CPPFLAGS += -D_GLIBCXX_USE_C99