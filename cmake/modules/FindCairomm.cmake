find_package(PkgConfig)
pkg_check_modules(CAIROMM REQUIRED cairomm-1.0)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Cairomm DEFAULT_MSG CAIROMM_FOUND)
