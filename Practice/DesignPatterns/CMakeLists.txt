# CMakeLists.txt

# Create executable
add_executable(DesignPatterns
    main.cpp
    visitor.cpp
    # add more .cpp files here as needed
 "specificationfilter.h"
 "specificationfilter.cpp"
 "VariadicBuilder.h"
 strategy.h strategy.cpp
 builder.h builder.cpp
 factories.h factories.cpp
 singletondb.h singletondb.cpp
 person.h person.cpp
 pimpl.h pimpl.cpp
 "wrapper.h")

# Enable warnings
target_compile_options(DesignPatterns PRIVATE -Wall -pedantic)
