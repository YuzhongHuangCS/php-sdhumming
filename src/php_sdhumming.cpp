#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#ifdef __cplusplus
extern "C"{
#endif
	#include "php.h"
	#include "php_ini.h"
#ifdef __cplusplus
}
#endif

#include <stdio.h>
#include <time.h>
#include "SDHumming/SModel.h"
#include "SDHumming/SDFuzzySearch.h"
#include "SDHumming/SUtil.h"
#include "SDHumming/SMelody.h"

#define PHP_SDHUMMING_EXTNAME "sdhumming"
#define PHP_SDHUMMING_VERSION "1.0"

extern zend_module_entry sdhumming_module_entry;
#define phpext_sdhumming_ptr &sdhumming_module_entry

// extension function declaration
PHP_FUNCTION(SDHummingSearch);
PHP_FUNCTION(SDHummingBuildModel);
PHP_FUNCTION(SDHummingLoadModel);

// list of custom PHP functions provided by this extension
// PHP_FE_END as the last record to mark the end of list
static zend_function_entry sdhumming_functions[] = {
	PHP_FE(SDHummingSearch, NULL)
	PHP_FE(SDHummingBuildModel, NULL)
	PHP_FE(SDHummingLoadModel, NULL)
	PHP_FE_END
};

/**
* PHP equivalent:
*
* class SDResultRow {
*    public $id;
*    public $info;
*    public $score;
* }
*/

// extension class declartion
zend_class_entry* sd_result_row_ce;
static zend_function_entry sd_result_row_functions[] = {
	PHP_FE_END
};

// MINIT implementation
SModel* SQBHModels = NULL;
char** szModelInfoStrs = NULL; 
int nModels = 0;
int nTotalModel = 0;
int nTotalSongs = 0;

int load_model(char* model = NULL, char* info = NULL) {
	if (model == NULL) {
		model = "model/QBHModel.dat";
	}
	if (info == NULL) {
		info = "model/QBHModel.info";
	}

	nTotalModel = SLoadModel(model, SQBHModels, nModels);
	if (nTotalModel <= 0) {
		php_printf("Error on loading model!\n");
		return FAILURE;
	}

	nTotalSongs = SReadMelodyDBInfoFile(info, szModelInfoStrs);
	if (nTotalSongs <= 0) {
		php_printf("Error on loading modelinfo!\n");
		return FAILURE;
	}

	return SUCCESS;
}

PHP_MINIT_FUNCTION(sdhumming) {
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "SDResultRow", sd_result_row_functions);
	sd_result_row_ce = zend_register_internal_class(&ce TSRMLS_CC);

	zend_declare_property_null(sd_result_row_ce, "id", sizeof("id") - 1, ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(sd_result_row_ce, "info", sizeof("info") - 1, ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(sd_result_row_ce, "score", sizeof("score") - 1, ZEND_ACC_PUBLIC TSRMLS_CC);

	return load_model();
}

// MSHUTDOWN implementation
PHP_MSHUTDOWN_FUNCTION(sdhumming) {
	for (int i = 0; i < nModels; i++) {
		delete[] SQBHModels[i].PhrasePos; 
		delete[] SQBHModels[i].sNotes;	
	}

	for (int i=0; i<nTotalSongs; i++){
		delete[] szModelInfoStrs[i];
	}

	delete[] SQBHModels;
	delete[] szModelInfoStrs;

	return SUCCESS;
}

// the following code creates an entry for the module and registers it with Zend.
zend_module_entry sdhumming_module_entry = {
	STANDARD_MODULE_HEADER,
	PHP_SDHUMMING_EXTNAME,
	sdhumming_functions,
	PHP_MINIT(sdhumming), // name of the MINIT function or NULL if not applicable
	PHP_MSHUTDOWN(sdhumming), // name of the MSHUTDOWN function or NULL if not applicable
	NULL, // name of the RINIT function or NULL if not applicable
	NULL, // name of the RSHUTDOWN function or NULL if not applicable
	NULL, // name of the MINFO function or NULL if not applicable
	PHP_SDHUMMING_VERSION,
	STANDARD_MODULE_PROPERTIES
};
 
ZEND_GET_MODULE(sdhumming)

/**
* SDHummingSearch implementation
*
* @param    string  $audio  Wav audio file
* @return   array           Array of SDResultRow

* PHP equivalent:
*
* function SDHummingSearch($audio);
*/
PHP_FUNCTION(SDHummingSearch) {
	char* audio = NULL;
	int audio_len;

	// parse arguments
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &audio, &audio_len) != SUCCESS) {
		return;
	}

	//1, Feature Extraction
	float* pFeaBuf = NULL;
	int nFeaLen = 0;
	SNote* QueryNotes = NULL;
	int nNoteLen=0;
	SMelodyFeatureExtraction(audio, pFeaBuf, nFeaLen, QueryNotes, nNoteLen);	
	
	if (!pFeaBuf || !QueryNotes) {
		return;
	}

	//2, Melody Search
	NoteBasedResStru* myEMDResStru = new NoteBasedResStru[nTotalModel];
	SNoteBasedMatch(SQBHModels, nModels, QueryNotes, nNoteLen,myEMDResStru,nFeaLen);

	FrameBasedResStru* myDTWResStru = new FrameBasedResStru[20];
	SFrameBasedMatch(SQBHModels, nModels, pFeaBuf, nFeaLen, myEMDResStru, 20, myDTWResStru);

	//3, Finalize and print the result
	zval *row, *id, *info, *score;
	array_init(return_value);

	for(int i=0;i<20;i++){
		MAKE_STD_ZVAL(row);
		object_init_ex(row, sd_result_row_ce);

		MAKE_STD_ZVAL(id);
		ZVAL_LONG(id, myDTWResStru[i].nModelID + 1);
		zend_update_property(sd_result_row_ce, row, "id", sizeof("id") - 1, id TSRMLS_DC);

		MAKE_STD_ZVAL(info);
		ZVAL_STRING(info, szModelInfoStrs[myDTWResStru[i].nModelID], 1);
		zend_update_property(sd_result_row_ce, row, "info", sizeof("info") - 1, info TSRMLS_DC);

		MAKE_STD_ZVAL(score);
		ZVAL_DOUBLE(score, myDTWResStru[i].fScore);
		zend_update_property(sd_result_row_ce, row, "score", sizeof("score") - 1, score TSRMLS_DC);

		add_next_index_zval(return_value, row);
	}

	delete[] pFeaBuf;
	delete[] QueryNotes;
	delete[] myEMDResStru;
	delete[] myDTWResStru;
}

/**
* SDHummingBuildModel implementation
*/
PHP_FUNCTION(SDHummingBuildModel) {
	RETURN_STRING("This is SDHummingBuildModel function\n", 1);
}

/**
* SDHummingLoadModel implementation
*/
PHP_FUNCTION(SDHummingLoadModel) {
	char* model = NULL;
	int model_len;
	char* info = NULL;
	int info_len;

	/* accepting arguments */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ss", &model, &model_len, &info, &info_len) != SUCCESS) {
		RETURN_BOOL(0);
	}

	RETURN_BOOL(load_model(model, info) + 1);
}
