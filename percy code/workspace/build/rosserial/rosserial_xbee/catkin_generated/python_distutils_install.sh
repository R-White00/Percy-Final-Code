#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/gdp31/GDP31/workspace/src/rosserial/rosserial_xbee"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/gdp31/GDP31/workspace/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/gdp31/GDP31/workspace/install/lib/python2.7/dist-packages:/home/gdp31/GDP31/workspace/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/gdp31/GDP31/workspace/build" \
    "/usr/bin/python2" \
    "/home/gdp31/GDP31/workspace/src/rosserial/rosserial_xbee/setup.py" \
     \
    build --build-base "/home/gdp31/GDP31/workspace/build/rosserial/rosserial_xbee" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/gdp31/GDP31/workspace/install" --install-scripts="/home/gdp31/GDP31/workspace/install/bin"
