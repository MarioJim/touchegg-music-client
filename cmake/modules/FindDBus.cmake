find_package(PkgConfig)
pkg_check_modules(DBUS REQUIRED dbus-1)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DBus DEFAULT_MSG DBUS_FOUND)
