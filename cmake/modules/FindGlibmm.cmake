find_package(PkgConfig)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4 giomm-2.4)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Glibmm DEFAULT_MSG GLIBMM_FOUND)
