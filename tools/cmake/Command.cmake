################################################################################
# Command overrides
################################################################################

#! add_library : Overrides add_library command.
# \argn: See add_library command.
# \param:TARGET TARGET specify the target
function(add_library TARGET)
    _add_library(${TARGET} ${ARGN})
endfunction(add_library)

#! add_library : Overrides add_executable command.
# \argn: See add_library command.
# \param:TARGET TARGET specify the target
function(add_executable TARGET)
    _add_executable(${TARGET} ${ARGN})
endfunction(add_executable)
