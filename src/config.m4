PHP_ARG_ENABLE(sdhumming, whether to enable sdhumming,
[ --enable-sdhumming   Enable sdhumming])
 
if test "$PHP_SDHUMMING" = "yes"; then
  AC_DEFINE(HAVE_SDHUMMING, 1, [Whether you have sdhumming])
  PHP_NEW_EXTENSION(sdhumming, php_sdhumming.c, $ext_shared)
fi
