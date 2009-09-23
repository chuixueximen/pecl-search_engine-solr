/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2009 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Israel Ekpo <iekpo@php.net>                                  |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#include "php_solr.h"

/* {{{ Macro for extracting property values using string constants */
#define solr_read_response_object_property(objptr, name, silent) zend_read_property(Z_OBJCE_P(objptr), objptr, name, sizeof(name)-1, silent TSRMLS_CC)
/* }}} */

/* {{{ proto int SolrResponse::getHttpStatus(void)
   Returns the value of the http_status property. */
PHP_METHOD(SolrResponse, getHttpStatus)
{
	zend_bool silent = 1;

	zval *objptr = getThis();

	zval *http_status = solr_read_response_object_property(objptr, "http_status", silent);

	RETURN_LONG(Z_LVAL_P(http_status));
}
/* }}} */

/* {{{ proto string SolrResponse::getHttpStatusMessage(void)
   Returns the http_status_message property. */
PHP_METHOD(SolrResponse, getHttpStatusMessage)
{
	zend_bool silent = 1;

	zval *objptr = getThis();

	zval *http_status_message = solr_read_response_object_property(objptr, "http_status_message", silent);

	RETURN_STRINGL(Z_STRVAL_P(http_status_message), Z_STRLEN_P(http_status_message), 1);
}
/* }}} */

/* {{{ proto bool SolrResponse::success(void)
   Returns whether the request was successful or not. */
PHP_METHOD(SolrResponse, success)
{
	zend_bool silent = 1;

	zval *objptr = getThis();

	zval *success = solr_read_response_object_property(objptr, "success", silent);

	RETURN_BOOL(Z_BVAL_P(success));
}
/* }}} */

/* {{{ proto string SolrResponse::getRequestUrl(void)
   Returns the URL used for the request. */
PHP_METHOD(SolrResponse, getRequestUrl)
{
	zend_bool silent = 1;

	zval *objptr = getThis();

	zval *prop = solr_read_response_object_property(objptr, "http_request_url", silent);

	RETURN_STRINGL(Z_STRVAL_P(prop), Z_STRLEN_P(prop), 1);
}
/* }}} */

/* {{{ proto string SolrResponse::getRawRequestHeaders(void)
   Returns the raw http request headers sent to the server. */
PHP_METHOD(SolrResponse, getRawRequestHeaders)
{
	zend_bool silent = 1;

	zval *objptr = getThis();

	zval *prop = solr_read_response_object_property(objptr, "http_raw_request_headers", silent);

	RETURN_STRINGL(Z_STRVAL_P(prop), Z_STRLEN_P(prop), 1);
}
/* }}} */

/* {{{ proto string SolrResponse::getRawRequest(void)
   Returns the raw http request sent to the server. */
PHP_METHOD(SolrResponse, getRawRequest)
{
	zend_bool silent = 1;

	zval *objptr = getThis();

	zval *prop = solr_read_response_object_property(objptr, "http_raw_request", silent);

	RETURN_STRINGL(Z_STRVAL_P(prop), Z_STRLEN_P(prop), 1);
}
/* }}} */

/* {{{ proto string SolrResponse::getRawResponseHeaders(void)
   Returns the raw http response headers from the server. */
PHP_METHOD(SolrResponse, getRawResponseHeaders)
{
	zend_bool silent = 1;

	zval *objptr = getThis();

	zval *prop = solr_read_response_object_property(objptr, "http_raw_response_headers", silent);

	RETURN_STRINGL(Z_STRVAL_P(prop), Z_STRLEN_P(prop), 1);
}
/* }}} */

/* {{{ proto string SolrResponse::getRawResponse(void)
   Returns the raw http response from the server. */
PHP_METHOD(SolrResponse, getRawResponse)
{
	zend_bool silent = 1;

	zval *objptr = getThis();

	zval *prop = solr_read_response_object_property(objptr, "http_raw_response", silent);

	if (Z_STRLEN_P(prop))
	{
		RETURN_STRINGL(Z_STRVAL_P(prop), Z_STRLEN_P(prop), 1);
	}

	RETURN_NULL();
}
/* }}} */

/* {{{ proto string SolrResponse::getDigestedResponse(void)
   Returns the serialized object string derived from the XML response. */
PHP_METHOD(SolrResponse, getDigestedResponse)
{
	zend_bool silent = 0;

	zval *objptr = getThis();

	zval *prop = solr_read_response_object_property(objptr, "http_digested_response", silent);

	if (Z_STRLEN_P(prop))
	{
		RETURN_STRINGL(Z_STRVAL_P(prop), Z_STRLEN_P(prop), 1);
	}

	RETURN_NULL();
}
/* }}} */

/* {{{ proto string SolrReponse::setParseMode([bool parse_mode])
   Sets the parsing mode. This determines whether documents will be parsed as SolrObjects or SolrDocuments. */
PHP_METHOD(SolrResponse, setParseMode)
{
	long int parse_mode = 0L;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &parse_mode) == FAILURE) {

		RETURN_FALSE;
	}

	parse_mode = ((parse_mode < 0L) ? 0L : ((parse_mode > 1L) ? 1L : parse_mode));

	zval *objptr = getThis();

	zend_update_property_long(Z_OBJCE_P(objptr), objptr, "parser_mode", sizeof("parser_mode")-1, parse_mode TSRMLS_CC);

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto SolrObject SolrResponse::getResponse(void)
   Returns the response object from the server. */
PHP_METHOD(SolrResponse, getResponse)
{
	zend_bool silent = 0;

	zval *objptr = getThis();

	if (return_value_used)
	{
		zval *raw_response = solr_read_response_object_property(objptr, "http_raw_response", silent);

		zval *success = solr_read_response_object_property(objptr, "success", silent);

		zval *parser_mode = solr_read_response_object_property(objptr, "parser_mode", silent);

		if (Z_BVAL_P(success) && Z_STRLEN_P(raw_response))
		{
			solr_string_t buffer;

			memset(&buffer, 0, sizeof(solr_string_t));

			/* Convert from XML serialization to PHP serialization format */
			solr_encode_generic_xml_response(&buffer, Z_STRVAL_P(raw_response), Z_STRLEN_P(raw_response), Z_LVAL_P(parser_mode) TSRMLS_CC);

			if (buffer.len)
			{
				zend_update_property_stringl(Z_OBJCE_P(objptr), objptr, "http_digested_response", sizeof("http_digested_response")-1, buffer.str, buffer.len TSRMLS_CC);
			}

			php_unserialize_data_t var_hash;

			memset(&var_hash, 0, sizeof(php_unserialize_data_t));

			PHP_VAR_UNSERIALIZE_INIT(var_hash);

			const unsigned char *raw_resp = (unsigned char *) buffer.str;
			size_t raw_res_length         = buffer.len;
			const unsigned char  *str_end = raw_resp + raw_res_length;

			int successful = 1;

			if (!php_var_unserialize(&return_value, &raw_resp, str_end, &var_hash TSRMLS_CC))
			{
				successful = 0;

				zend_throw_exception_ex(solr_ce_SolrException, SOLR_ERROR_1000 TSRMLS_CC, SOLR_ERROR_1000_MSG);

				php_error_docref(NULL TSRMLS_CC, E_WARNING, "Error unserializing raw response.");
			}

			PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

			solr_string_free(&buffer);

			if (successful)
			{
				/* Overriding the default object handlers */
				Z_OBJ_HT_P(return_value) = &solr_object_handlers;
			}

			return ;
		}

		RETURN_NULL();

	} else {

		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Return value requested without processing output.");
	}
}
/* }}} */

/* {{{ proto string SolrPingResponse::getResponse(void)
  Ping responses are always empty. Returns null. */
PHP_METHOD(SolrPingResponse, getResponse)
{
	/* Ping responses are not serialized */
	if (return_value_used)
	{
		RETURN_NULL();
	}
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 * vim600: fdm=marker
 * vim: noet sw=4 ts=4
 */
