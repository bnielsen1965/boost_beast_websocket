# boost beast websocket server

An websocket server that accepts connections and echoes messages.

# requirements

Requires the boost library (~1.71.0) with compiled binary libraries. See the [Makefile](Makefile) for
include and library path settings.

> IBOOST_PATH The include path to the boost includes.

> LBOOST_PATH The library path to the boost binary libraries.

## rpath

A relative rpath is specified as .lib in case the boost libraries are not installed
system wide. In this case you must copy the libboost_thread so file from your boost
projects directory into the wsserver's lib folder so it can be dynamically loaded
by the executable.

I.E. Assuming the boost project was downloaded and compiled into the user's home path
at Projects/. Copy ~/Projects/boost_1_71_0/state/lib/libboost_thread.so.1.71.0 ./lib/
