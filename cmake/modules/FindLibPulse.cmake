find_package(PkgConfig)
pkg_check_modules(LIBPULSE REQUIRED libpulse)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibPulse DEFAULT_MSG LIBPULSE_FOUND)