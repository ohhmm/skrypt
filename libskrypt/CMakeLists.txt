
set(deplist math)

# Using Boost components from openmind
set(deplist ${deplist} Boost::filesystem Boost::program_options)

if(TARGET Boost::dll)
	set(deplist ${deplist} Boost::dll)
elseif(Boost_DLL_LIBRARY)
	set(deplist ${deplist} ${Boost_DLL_LIBRARY})
endif()

lib(${deplist})
