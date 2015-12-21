#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif
#include "php.h"
 
#define PHP_SDHUMMING_VERSION "1.0"
#define PHP_SDHUMMING_EXTNAME "sdhumming"

extern zend_module_entry sdhumming_module_entry;
#define phpext_sdhumming_ptr &sdhumming_module_entry
 
// extension function declaration
PHP_FUNCTION(SDHummingSearch);
PHP_FUNCTION(SDHummingBuildModel);

// list of custom PHP functions provided by this extension
// set {NULL, NULL, NULL} as the last record to mark the end of list
static zend_function_entry sdhumming_functions[] = {
	PHP_FE(SDHummingSearch, NULL)
	PHP_FE(SDHummingBuildModel, NULL)
	{NULL, NULL, NULL}
};
 
// the following code creates an entry for the module and registers it with Zend.
zend_module_entry sdhumming_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_SDHUMMING_EXTNAME,
	sdhumming_functions,
	NULL, // name of the MINIT function or NULL if not applicable
	NULL, // name of the MSHUTDOWN function or NULL if not applicable
	NULL, // name of the RINIT function or NULL if not applicable
	NULL, // name of the RSHUTDOWN function or NULL if not applicable
	NULL, // name of the MINFO function or NULL if not applicable
#if ZEND_MODULE_API_NO >= 20010901
	PHP_SDHUMMING_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
 
ZEND_GET_MODULE(sdhumming)

/**
* SDHummingSearch implementation
*
*
* @param    string  $audio  Wav audio file
* @param    string  $model  QBHModel.dat
* @param    string  $info   QBHModel.info
* @return   array           Array of SDSearchResultRow

* PHP equivalent:
*
* function SDHummingSearch($audio, $model, $info);
*
* class SDSearchResultRow {
*    public $id;
*    public $info;
*    public $score;
* }
*
*/
PHP_FUNCTION(SDHummingSearch) {
	char* audio;
	int audio_len;
	char* info;
	int info_len;
	char* score;
	int score_len;
	  
	/* accepting arguments */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sss", &audio, &audio_len, &info, &info_len, &score, &score_len) != SUCCESS) {
		return;
	}
	 
	php_printf("audio: %s\n", audio);
	php_printf("info: %s\n", info);
	php_printf("score: %s\n", score);
}

/**
* SDHummingBuildModel implementation
*
*/
PHP_FUNCTION(SDHummingBuildModel) {
	RETURN_STRING("This is SDHummingBuildModel function\n", 1);
}
