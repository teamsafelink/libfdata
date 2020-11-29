dnl Checks for required headers and functions
dnl
dnl Version: 20201128

dnl Function to detect if libfdata dependencies are available
AC_DEFUN([AX_LIBFDATA_CHECK_LOCAL],
  [dnl No additional checks.
  ])

dnl Function to check if DLL support is needed
AC_DEFUN([AX_LIBFDATA_CHECK_DLL_SUPPORT],
  [AS_IF(
    [test "x$enable_shared" = xyes],
    [AS_CASE(
      [$host],
      [*cygwin* | *mingw* | *msys*],
      [AC_DEFINE(
        [HAVE_DLLMAIN],
        [1],
        [Define to 1 to enable the DllMain function.])
      AC_SUBST(
        [HAVE_DLLMAIN],
        [1])

      AC_SUBST(
        [LIBFDATA_DLL_EXPORT],
        ["-DLIBFDATA_DLL_EXPORT"])

      AC_SUBST(
        [LIBFDATA_DLL_IMPORT],
        ["-DLIBFDATA_DLL_IMPORT"])
      ])
    ])
  ])

