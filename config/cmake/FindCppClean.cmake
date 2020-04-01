################################################################################
# cppclean
################################################################################

# cppclean is enabled by default.
option(cppclean "Enable cppclean." ON)

if(cppclean)

    # Check if cppclean is installed with find_program.
    find_program(CPPCLEAN_PROGRAM cppclean)

    if(CPPCLEAN_PROGRAM)

        # Define a function for enabling cppclean per target.
        function(enable_cppclean TARGET)

            # A horrible hack to get a 'space' between the generator expression
            # parameters, see: https://public.kitware.com/Bug/view.php?id=14353
            set(SPACE \t)

            # Execute ccpclean on the target's source files by taking the source
            # files from the target properties. Include directories are also set
            # from target properties.
            add_custom_command(
                TARGET ${TARGET}
                PRE_BUILD COMMAND
                ${CPPCLEAN_PROGRAM}
                -I$<JOIN:$<TARGET_PROPERTY:${TARGET},INCLUDE_DIRECTORIES>,$<1:${SPACE}>-I>
                 $<JOIN:$<TARGET_PROPERTY:${TARGET},SOURCES>,$<1:${SPACE}>>)

        endfunction(enable_cppclean)

    else(CPPCLEAN_PROGRAM)
        message(WARNING "Program 'cppclean' not found, unable to run 'cppclean'.")
        function(enable_cppclean TARGET)
        endfunction(enable_cppclean)
    endif(CPPCLEAN_PROGRAM)
endif(cppclean)
