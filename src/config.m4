PHP_ARG_ENABLE(sdhumming, whether to enable sdhumming, [ --enable-sdhumming    Enable sdhumming])
 
if test "$PHP_SDHUMMING" = "yes"; then
	AC_DEFINE(HAVE_SDHUMMING, 1, [Whether you have sdhumming])
	PHP_REQUIRE_CXX()
	PHP_SUBST(SDHUMMING_SHARED_LIBADD)
	PHP_ADD_LIBRARY(stdc++, 1, SDHUMMING_SHARED_LIBADD)
	PHP_NEW_EXTENSION(sdhumming, php_sdhumming.cpp SDHumming/SDFuzzySearch.cpp SDHumming/SFrameMatch.cpp SDHumming/SNoteMatch.cpp SDHumming/SSearchCommon.cpp SDHumming/SDSP.cpp SDHumming/SMelody.cpp SDHumming/SModel.cpp SDHumming/SUtil.cpp SDHBuildModel/SBuildModel.cpp SDHBuildModel/midifile.cpp, $ext_shared)
fi
