/*
 *
 * Copyright 2010 JiJie Shi
 *
 * This file is part of bittrace.
 *
 * bittrace is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * bittrace is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with bittrace.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 
 #ifdef _DEBUG_MEM_LEAKS
#undef _DEBUG_MEM_LEAKS
#endif //_DEBUG_MEM_LEAKS

#include "common_func.h"
#include "acl_define.h"
#include "action_type_parse.h"
#include "filter_config_file.h"
#include "sqlite_supp.h"
#include "action_analyze_define.h"
//#include "stack_dump.h"
#include "action_display.h"
#include "r3_rw_lock.h"
#import "msxml.tlb"

#pragma comment(lib, "Delayimp.lib")
#pragma comment(linker, "/DelayLoad:d3d9.dll")

CRITICAL_SECTION filter_info_lock; 
//CReadWriteLock filter_info_lock; 
action_filter_infos filter_infos; 

LRESULT WINAPI adjust_filter_cond_value( action_filter_info *filter_info )
{
	LRESULT ret = ERROR_SUCCESS; 
	INT32 _ret; 
	LPCWSTR text_value; 
	ULONG ulong_value; 
	INT32 int32_value; 
	time_region duration; 
	LPWSTR tmp_text; 

	do 
	{
		ASSERT( NULL != filter_info ); 

		if( filter_info->value.text.text_is_ptr == TRUE ) 
		{
			text_value = filter_info->value.text.text_mode_value_ptr; 
		}
		else
		{
			//ASSERT( FALSE ); 
			text_value = filter_info->value.text.text_mode_value; 
		}

		ASSERT( filter_info->value.text.cc_string_len > 0 
			&& text_value[ 0 ] != L'\0' ); 

		switch( filter_info->cond )
		{
		case FLT_PATH: 
		case FLT_PROCESS_NAME: 
		case FLT_COMMAND: 
		case FLT_DESCRITION: 
		case FLT_DESC_DETAIL: 
		case FLT_USER: 
		case FLT_AUTH_ID: 
		case FLT_CORPORATION: 
			{
				//if( STRING_POINTER_VALUE_TYPE == filter_info->value.type )
				//{
				//	HRESULT hr; 

				//	text_value = filter_info->value.text.text_mode_value_ptr; 

				//	hr = StringCbCopyW( filter_info->value.text.string_value, 
				//		sizeof( filter_info->value.text.string_value ), 
				//		text_value ); 

				//	if( hr != NULL )
				//	{
				//		ret = ERROR_INVALID_PARAMETER; 
				//		break; 
				//	}
				//}
			}
			break; 
		case FLT_MAIN_TYPE: 
		case FLT_ACTION_TYPE:  
		case FLT_PID: 
		case FLT_PARENT_PID: 
		case FLT_TID: 
		case FLT_ACTION_RESULT: 
		case FLT_SESSION: 
			{
				ulong_value = wcstoul( text_value, 
					&tmp_text, 
					10 ); 

				filter_info->value.data.ulong_value = ulong_value; 
				filter_info->value.type = ULONG_VALUE_TYPE; 
			}
			break; 
		case FLT_READ_WRITE: 
		case FLT_CPU_ARCH: 
		case FLT_VIRTUAL_TECH: 
			{
				int32_value = wcstol( text_value, 
					&tmp_text, 
					10 ); 

				filter_info->value.data.ulong_value = int32_value; 
				filter_info->value.type = INT32_VALUE_TYPE; 
			}
			break; 
		case FLT_DURATION: 
			{
				_ret = wscanf_s( text_value, 
					L"%I64-%I64", 
					&duration.begin_time.QuadPart, 
					&duration.end_time.QuadPart ); 

				filter_info->value.data.duration.begin_time.QuadPart = duration.begin_time.QuadPart; 
				filter_info->value.data.duration.end_time.QuadPart = duration.end_time.QuadPart; 

				filter_info->value.type = DURATION_VALUE_TYPE; 
			}
			break; 
		case FLT_DATE_TIME: 
		case FLT_RELATIVE_TIME: 
			{
				//duration.begin_time.QuadPart = _wcstoui64( text_value, 
				//	&tmp_text, 
				//	10 ); 

				_ret = string_to_local_time( text_value, &filter_info->value.data.time_value ); 
				if( _ret != ERROR_SUCCESS )
				{}

				filter_info->value.type = TIME_VALUE_TYPE; 
			}
			break; 
		case FLT_ACTION_NO: 
		case FLT_VERSION: 
			{
				duration.begin_time.QuadPart = _wcstoui64( text_value, 
					&tmp_text, 
					10 ); 

				filter_info->value.data.ulonglong_value = duration.begin_time.QuadPart; 

				filter_info->value.type = ULONGLONG_VALUE_TYPE; 
			}
			break; 
		default: 
			break; 
		}
	}while( FALSE );

	return ret; 
}

LRESULT WINAPI load_filer_info_from_xml( action_filter_info *filter_info, 
										MSXML::IXMLDOMNode *xml_node )
{
	LRESULT ret = ERROR_SUCCESS; 
	HRESULT hr; 
	MSXML::IXMLDOMNamedNodeMapPtr attrs = NULL; 
	MSXML::IXMLDOMNodePtr attr; 
	_bstr_t temp_text; 
	_variant_t value; 
	LPCWSTR _temp_text; 

	do 
	{
		ASSERT( NULL != filter_info ); 
		ASSERT( NULL != xml_node ); 

		hr = xml_node->get_attributes( &attrs ); 
		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		temp_text = L"type"; 
		hr = attrs->raw_getNamedItem( temp_text.GetBSTR(), &attr ); 
		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		hr = attr->get_nodeValue( &value ); 
		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		switch( value.vt )
		{
		case VT_INT: 
			{
				if( FALSE == is_valid_filter_condition( value.intVal ) )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				filter_info->cond = ( action_filter_cond )value.intVal; 
			}
			break; 
		case VT_BSTR: 
			{
				LPWSTR end_text; 
				LONG _value; 

				temp_text = value.bstrVal; 

				_temp_text = temp_text.operator const wchar_t*(); 
				if( NULL == _temp_text )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				_value = wcstol( _temp_text, &end_text, 10 ); 

				if( FALSE == is_valid_filter_condition( _value ) )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				filter_info->cond = ( action_filter_cond )_value; 

			}
			break; 
		default: 
			{
				ret = ERROR_INVALID_PARAMETER; 
				break; 
			}
			break; 
		}

		if( ret != ERROR_SUCCESS )
		{
			break; 
		}

		temp_text = L"compare"; 
		hr = attrs->raw_getNamedItem( temp_text.GetBSTR(), &attr ); 
		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		hr = attr->get_nodeValue( &value ); 
		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		switch( value.vt )
		{
		case VT_INT: 
			{
				if( FALSE == is_valid_filter_compare_mode( value.intVal ) )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}
				
				filter_info->compare_mode = ( action_compare_mode )value.intVal; 
			}
			break; 
		case VT_BSTR: 
			{
				LPWSTR end_text; 
				LONG _value; 

				temp_text = value.bstrVal; 

				_temp_text = temp_text.operator const wchar_t*(); 
				if( NULL == _temp_text )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				_value = wcstoul( _temp_text, &end_text, 10 ); 
			
				if( FALSE == is_valid_filter_compare_mode( _value ) )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				filter_info->compare_mode = ( action_compare_mode )_value; 
			}
			break; 
		default: 
			{
				ret = ERROR_INVALID_PARAMETER; 
				break; 
			}
			break; 
		}

		if( ret != ERROR_SUCCESS )
		{
			break; 
		}

		temp_text = L"mode"; 
		hr = attrs->raw_getNamedItem( temp_text.GetBSTR(), &attr ); 
		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		hr = attr->get_nodeValue( &value ); 
		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		switch( value.vt )
		{
		case VT_INT: 
			{
				if( FALSE == is_valid_filtering_mode( value.intVal ) )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				filter_info->filter_mode = ( action_filter_mode )value.intVal; 
			}
			break; 
		case VT_BSTR: 
			{
				LPWSTR end_text; 
				LONG _value; 

				temp_text = value.bstrVal; 

				_temp_text = temp_text.operator const wchar_t*(); 
				if( NULL == _temp_text )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				_value = wcstoul( _temp_text, &end_text, 10 ); 

				if( FALSE == is_valid_filtering_mode( _value ) )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				filter_info->filter_mode = ( action_filter_mode )_value; 
			}
			break; 
		default: 
			{
				ret = ERROR_INVALID_PARAMETER; 
				break; 
			}
			break; 
		}

		if( ret != ERROR_SUCCESS )
		{
			break; 
		}

		temp_text = L"value_type"; 
		hr = attrs->raw_getNamedItem( temp_text.GetBSTR(), &attr ); 
		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		hr = attr->get_nodeValue( &value ); 
		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		switch( value.vt )
		{
		case VT_INT: 
			{
				if( FALSE == is_valid_filter_value_type( value.intVal ) )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				filter_info->value.type = ( action_filter_value_type )value.intVal; 
			}
			break; 
		case VT_BSTR: 
			{
				LPWSTR end_text; 
				LONG _value; 

				temp_text = value.bstrVal; 

				_temp_text = temp_text.operator const wchar_t*(); 
				if( NULL == _temp_text )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				_value = wcstoul( _temp_text, &end_text, 10 ); 
			
				if( FALSE == is_valid_filter_value_type( _value ) )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				filter_info->value.type = ( action_filter_value_type )_value; 
			}
			break; 
		default: 
			{
				ret = ERROR_INVALID_PARAMETER; 
				break; 
			}
			break; 
		}

		if( ret != ERROR_SUCCESS )
		{
			break; 
		}

		temp_text = L"value"; 
		hr = attrs->raw_getNamedItem( temp_text.GetBSTR(), &attr ); 
		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		hr = attr->get_nodeValue( &value ); 
		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		memset( &filter_info->value, 
			0, 
			sizeof( filter_info->value ) ); 
		
		switch( value.vt )
		{
		case VT_INT: 
			{
				if( filter_info->value.type != INT32_VALUE_TYPE )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				filter_info->value.data.int32_value = value.intVal; 
			}
			break; 
		case VT_UINT: 
			{
				if( filter_info->value.type != ULONG_VALUE_TYPE )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				filter_info->value.data.ulong_value = value.ulVal; 
			}
			break; 
		case VT_UI8:
			{
				if( filter_info->value.type != ULONGLONG_VALUE_TYPE )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				filter_info->value.data.ulonglong_value = value.ullVal; 
			}
			break; 
		case VT_BSTR: 
			{
				LPWSTR end_text; 

				temp_text = value.bstrVal; 

				_temp_text = temp_text.operator const wchar_t*(); 
				if( NULL == _temp_text )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				hr = StringCchCopyW( filter_info->value.text.text_mode_value, 
					ARRAYSIZE( filter_info->value.text.text_mode_value ), 
					_temp_text ); 

				if( FAILED( hr ) )
				{
					ret = hr; 
					break; 
				}

				filter_info->value.text.cc_string_len = ( ULONG )wcslen( filter_info->value.text.text_mode_value ); 
				filter_info->value.text.text_is_ptr = FALSE; 

				ret = adjust_filter_cond_value( filter_info ); 
				if( ret != ERROR_SUCCESS )
				{
					break; 
				}

			}
			break; 
		default: 
			{
				ret = ERROR_INVALID_PARAMETER; 
				break; 
			}
			break; 
		}

		if( ret != ERROR_SUCCESS )
		{
			break; 
		}

		temp_text = L"bk_clr"; 

		init_filter_ui_info( &filter_info->ui_info ); 

		hr = attrs->raw_getNamedItem( temp_text.GetBSTR(), &attr ); 
		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		hr = attr->get_nodeValue( &value ); 
		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		switch( value.vt )
		{
		case VT_INT: 
			{
				filter_info->ui_info.bk_clr = value.intVal; 
			}
			break; 
		case VT_BSTR: 
			{
				LPWSTR end_text; 

				temp_text = value.bstrVal; 

				_temp_text = temp_text.operator const wchar_t*(); 
				if( NULL == _temp_text )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				filter_info->ui_info.bk_clr = wcstoul( _temp_text, &end_text, 10 ); 
			}
			break; 
		default: 
			{
				ret = ERROR_INVALID_PARAMETER; 
				break; 
			}
			break; 
		}

		if( ret != ERROR_SUCCESS )
		{
			break; 
		}

	}while( FALSE ); 

	return ret; 
}

LRESULT WINAPI save_filer_info_to_xml( action_filter_info *filter_info, 
										MSXML::IXMLDOMNode *xml_node )
{
	LRESULT ret = ERROR_SUCCESS; 
	HRESULT hr; 
	MSXML::IXMLDOMElementPtr xml_element; 
	MSXML::IXMLDOMNamedNodeMapPtr attrs = NULL; 
	MSXML::IXMLDOMNodePtr attr; 
	_bstr_t temp_text; 
	_variant_t value; 
	LPCWSTR _temp_text; 

	do 
	{
		ASSERT( NULL != filter_info ); 
		ASSERT( NULL != xml_node ); 

		//查找是否已经存在这个过滤策略
		hr = xml_node->QueryInterface( uuid( MSXML::IXMLDOMElement ), &xml_element ); 
		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		temp_text = L"type"; 

		value = ( INT32 )filter_info->cond; 
		hr = xml_element->setAttribute( temp_text, value ); 

		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		temp_text = L"compare"; 

		value = ( INT32 )filter_info->compare_mode; 

		hr = xml_element->setAttribute( temp_text, value ); 

		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		temp_text = L"mode"; 

		value = ( INT32 )filter_info->filter_mode; 
		hr = xml_element->setAttribute( temp_text, value ); 

		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		temp_text = L"value_type"; 

		value = ( INT32 )filter_info->value.type; 
		hr = xml_element->setAttribute( temp_text, value ); 

		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		temp_text = L"value"; 

		value = ( LPCWSTR )filter_info->value.text; 
		hr = xml_element->setAttribute( temp_text, value ); 

		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

		temp_text = L"bk_clr"; 

		value = ( INT32 )filter_info->ui_info.bk_clr; 
		hr = xml_element->setAttribute( temp_text, value ); 

		if( FAILED( hr ) )
		{
			ret = hr; 
			break; 
		}

	}while( FALSE ); 

	return ret; 
}

LRESULT WINAPI load_filter_infos_from_conf_file( LPCWSTR file_name, 
									ULONG offset, 
									ULONG limit, 
									PVOID context, 
									action_filter_cond_callback filter_cond_func )
{
	LRESULT ret = ERROR_SUCCESS; 
	LRESULT _ret; 
	HRESULT hr; 

	MSXML::IXMLDOMDocumentPtr xml_doc = NULL; 
	MSXML::IXMLDOMElementPtr xml_element = NULL; 
	MSXML::IXMLDOMNodeListPtr xml_node_list = NULL; 
	MSXML::IXMLDOMNodePtr xml_node = NULL; 
	VARIANT_BOOL __ret; 
	//MSXML::IXMLDOMNamedNodeMapPtr xml_attrs = NULL; 
	//MSXML::IXMLDOMNodePtr xml_attr = NULL; 

	LONG i; 
	LONG node_count; 
	LONG original_node_count; 
	WCHAR *temp_text; 
	action_filter_info filter_info; 

	do 
	{
		ASSERT( file_name != NULL ); 
		ASSERT( filter_cond_func != NULL ); 

		/**************************************************
		<?xml version="1.0" encoding="UTF-8" ?>
		<FILTERS>
		<FILTER >
		</FILTER>
		</FILTERS>
		**************************************************/

#ifdef PERFORMANCE_DEBUG
		ULONG tick_begin; 
		ULONG tick_end; 
#endif //PERFORMANCE_DEBUG

#ifdef PERFORMANCE_DEBUG
		tick_begin = GetTickCount(); 
#endif //PERFORMANCE_DEBUG

		if( file_name == NULL )
		{
			ret = ERROR_INVALID_PARAMETER; 
			break; 
		}

		hr = CoCreateInstance( __uuidof( MSXML::DOMDocument ), 
			NULL, 
			CLSCTX_INPROC_SERVER, 
			__uuidof( MSXML::IXMLDOMDocument ), 
			( PVOID* )&xml_doc ); 

		if( hr != S_OK ) 
		{
			dbg_print( DBG_MSG_AND_ERROR_OUT, "CoCreateInstance error\n" ); 
			break; 
		}

		__ret = xml_doc->load( ( WCHAR* )file_name ); 

		if( __ret != VARIANT_TRUE )
		{
			MSXML::IXMLDOMParseErrorPtr spParseError;
			_bstr_t bstrReason;

			spParseError = xml_doc->parseError;

			bstrReason = spParseError->reason;

			temp_text = bstrReason.operator wchar_t*(); 

			if( NULL != temp_text )
			{
				dbg_print( DBG_MSG_AND_ERROR_OUT, "load xml error %ws\n", temp_text ); 
			}

			break; 		
		}

		xml_element = xml_doc->documentElement; 
		if( xml_element == NULL ) 
		{
			ret = ERROR_INVALID_PARAMETER; 
			log_trace_ex( MSG_IMPORTANT, " documentElement invalid\n" );
			break; 
		}

		xml_node_list = xml_element->selectNodes( L"//FILTER" ); 
		if( xml_node_list == NULL )
		{
			ret = ERROR_INVALID_PARAMETER; 
			log_trace_ex( MSG_IMPORTANT, "%s: //page/element is not existed", __FUNCTION__ ); 
			break; 
		}

		hr = xml_node_list->get_length( &node_count ); 
		if( FAILED( hr ) )
		{
			ret = ERROR_INVALID_PARAMETER; 
			log_trace_ex( MSG_IMPORTANT, "%s: //page/element is not existed", __FUNCTION__ ); 
			break; 
		}

		//node_count = xml_node_list->Getlength(); 

		for( i = 0; i < node_count; i ++ )
		{
			do 
			{
				hr = xml_node_list->get_item( i, &xml_node ); 
				if( FAILED( hr ) )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				ret = load_filer_info_from_xml( &filter_info, xml_node ); 
				if( ret != ERROR_SUCCESS )
				{
					break; 
				}

			}while( FALSE ); 
		}

#ifdef PERFORMANCE_DEBUG
		tick_end = GetTickCount(); 
		dbg_print( MSG_IMPORTANT, "time usage: %u", tick_end - tick_begin ); 
#endif //PERFORMANCE_DEBUG
	}while( FALSE );

	return ret; 
}

LRESULT WINAPI save_filter_infos_to_conf_file( action_filter_info_array *filter_infos, 
											  LPCWSTR file_name, 
											  ULONG offset, 
											  ULONG limit, 
											  PVOID context )
{
	LRESULT ret = ERROR_SUCCESS; 
	LRESULT _ret; 
	HRESULT hr; 

	MSXML::IXMLDOMDocumentPtr xml_doc = NULL; 
	MSXML::IXMLDOMElementPtr xml_element = NULL; 
	MSXML::IXMLDOMNodeListPtr xml_node_list = NULL; 
	MSXML::IXMLDOMNodePtr xml_node = NULL; 
	VARIANT_BOOL __ret; 
	//MSXML::IXMLDOMNamedNodeMapPtr xml_attrs = NULL; 
	//MSXML::IXMLDOMNodePtr xml_attr = NULL; 

	LONG i; 
	LONG node_count; 
	LONG original_node_count; 
	WCHAR *temp_text; 
	action_filter_info filter_info; 
	BOOLEAN filter_info_found = FALSE; 
	action_filter_info_array::iterator filter_info_it; 

	do 
	{
		ASSERT( file_name != NULL ); 
		ASSERT( filter_cond_func != NULL ); 

		/**************************************************
		<?xml version="1.0" encoding="UTF-8" ?>
		<FILTERS>
		<FILTER >
		</FILTER>
		</FILTERS>
		**************************************************/

#ifdef PERFORMANCE_DEBUG
		ULONG tick_begin; 
		ULONG tick_end; 
#endif //PERFORMANCE_DEBUG

#ifdef PERFORMANCE_DEBUG
		tick_begin = GetTickCount(); 
#endif //PERFORMANCE_DEBUG

		if( file_name == NULL )
		{
			ret = ERROR_INVALID_PARAMETER; 
			break; 
		}

		hr = CoCreateInstance( __uuidof( MSXML::DOMDocument ), 
			NULL, 
			CLSCTX_INPROC_SERVER, 
			__uuidof( MSXML::IXMLDOMDocument ), 
			( PVOID* )&xml_doc ); 

		if( hr != S_OK ) 
		{
			dbg_print( DBG_MSG_AND_ERROR_OUT, "CoCreateInstance error\n" ); 
			break; 
		}

		__ret = xml_doc->load( ( WCHAR* )file_name ); 

		if( __ret != VARIANT_TRUE )
		{
			MSXML::IXMLDOMParseErrorPtr spParseError;
			_bstr_t bstrReason;

			spParseError = xml_doc->parseError;

			bstrReason = spParseError->reason;

			temp_text = bstrReason.operator wchar_t*(); 

			if( NULL != temp_text )
			{
				dbg_print( DBG_MSG_AND_ERROR_OUT, "load xml error %ws\n", temp_text ); 
			}

			break; 		
		}

		xml_element = xml_doc->documentElement; 
		if( xml_element == NULL ) 
		{
			ret = ERROR_INVALID_PARAMETER; 
			log_trace_ex( MSG_IMPORTANT, " documentElement invalid\n" );
			break; 
		}

		xml_node_list = xml_element->selectNodes( L"//FILTER" ); 
		if( xml_node_list == NULL )
		{
			ret = ERROR_INVALID_PARAMETER; 
			log_trace_ex( MSG_IMPORTANT, "%s: //page/element is not existed", __FUNCTION__ ); 
			break; 
		}

		hr = xml_node_list->get_length( &node_count ); 
		if( FAILED( hr ) )
		{
			ret = ERROR_INVALID_PARAMETER; 
			log_trace_ex( MSG_IMPORTANT, "%s: //page/element is not existed", __FUNCTION__ ); 
			break; 
		}

		//node_count = xml_node_list->Getlength(); 

		for( filter_info_it = filter_infos->begin(); 
			filter_info_it != filter_infos->end(); 
			filter_info_it ++ )
		{
			for( i = 0; i < node_count; i ++ )
			{
				do 
				{
					hr = xml_node_list->get_item( i, &xml_node ); 
					if( FAILED( hr ) )
					{
						ret = ERROR_INVALID_PARAMETER; 
						break; 
					}

					ret = load_filer_info_from_xml( &filter_info, xml_node ); 
					if( ret != ERROR_SUCCESS )
					{
						break; 
					}

				}while( FALSE ); 
			}

			if( TRUE == filter_info_found )
			{
				break; 
			}

#define FILTER_ELEMENT_TAG L"FILTER"
			do
			{
				MSXML::IXMLDOMElement _xml_element; 
				_xml_element = xml_doc->createElement( FILTER_ELEMENT_TAG ); 

				if( NULL == _xml_element )
				{
					break; 
				}

				ret = save_filer_info_to_xml( ( *filter_info_it ), 
					_xml_element ); 
				if( ret != ERROR_SUCCESS )
				{
					break; 
				}

				hr = xml_element->appendChild( _xml_element ); 

				if( FAILED( hr ) )
				{
					ret = hr; 
					break; 
				}
			}while( FALSE ); 

#ifdef PERFORMANCE_DEBUG
			tick_end = GetTickCount(); 
			dbg_print( MSG_IMPORTANT, "time usage: %u", tick_end - tick_begin ); 
#endif //PERFORMANCE_DEBUG
		}
	}while( FALSE );

	return ret; 
}

LRESULT WINAPI make_filter_infos_conf_file_exist( LPCWSTR file_name )
{
	LRESULT ret = ERROR_SUCCESS; 
	HRESULT hr; 
	MSXML::IXMLDOMDocumentPtr xml_doc; 
	MSXML::IXMLDOMProcessingInstructionPtr xml_instruction; 
	MSXML::IXMLDOMElementPtr xml_element; 
	_bstr_t text; 

	do
	{
#define HTML_CONFIG_ROOT_ELEMENT L"HTML_SITES"
		hr = CoCreateInstance(__uuidof( MSXML::DOMDocument ), 
			NULL, 
			CLSCTX_INPROC_SERVER, 
			__uuidof( MSXML::IXMLDOMDocument ), 
			( void** )&xml_doc ); 
		if( hr != S_OK)
		{
			log_trace_ex( MSG_IMPORTANT, "create xml document error %08x", hr);
			ret = hr; 
			break; 
		}

		// 创建Xml声明
		hr = xml_doc->raw_createProcessingInstruction( L"xml", L"version=\"1.0\" encoding=\"utf-8\"", &xml_instruction );

		if( S_OK != hr )
		{
			log_trace_ex( MSG_IMPORTANT, "构造KEY XML:创建XML声明失败" );
			ret = hr; 
			break; 
		}

		hr = xml_doc->raw_appendChild( xml_instruction, NULL );
		if( S_OK != hr )
		{
			ret = hr; 
			break; 
		}

#define FILTERS_ROOT_ELEMENT L"FILTERS"
		text = FILTERS_ROOT_ELEMENT; 
		xml_element = xml_doc->createElement( text ); 

		if( NULL == xml_element )
		{
			ret = ERROR_ERRORS_ENCOUNTERED; 
			break; 
		}

		hr = xml_doc->appendChild( xml_element ); 
		if( FAILED( hr ) )
		{
			ret = hr; 
			//break; 
		}

		{

			_variant_t save_file_name; 

			text = file_name; 
			save_file_name = text; 			
			hr = xml_doc->save( save_file_name ); 

			if( hr != S_OK )
			{
				ret = hr; 
			}
		}

	}while( FALSE ); 

	//// 释放xml声明
	//if (xml_instruction != NULL)
	//{
	//	xml_instruction.Release();
	//}

	//// 释放xml文档
	//if (xml_doc != NULL)
	//{
	//	xml_doc.Release ();
	//}

	//CoUninitialize ();

	return ret; 
}

LRESULT WINAPI r_lock_filter_infos()
{
	LRESULT ret = ERROR_SUCCESS; 
	
	do 
	{
		lock_cs( filter_info_lock ); 
		//filter_info_lock.LockReader(); 
	}while( FALSE );

	return ret; 
}

LRESULT WINAPI r_unlock_filter_infos()
{
	LRESULT ret = ERROR_SUCCESS; 

	do 
	{
		unlock_cs( filter_info_lock ); 
		//filter_info_lock.UnlockReader(); 
	}while( FALSE );

	return ret; 
}

LRESULT WINAPI w_lock_filter_infos()
{
	LRESULT ret = ERROR_SUCCESS; 

	do 
	{
		lock_cs( filter_info_lock ); 
		//filter_info_lock.LockWriter(); 
	}while( FALSE );

	return ret; 
}

LRESULT WINAPI w_unlock_filter_infos()
{
	LRESULT ret = ERROR_SUCCESS; 

	do 
	{
		unlock_cs( filter_info_lock ); 
		//filter_info_lock.UnlockWriter(); 
	}while( FALSE );

	return ret; 
}

LRESULT WINAPI w_try_lock_filter_infos()
{
	LRESULT ret = ERROR_SUCCESS; 
	INT32 _ret; 

	do 
	{
		_ret = TryEnterCriticalSection( &filter_info_lock ); 

		//try_unlock_cs( filter_info_lock ); 
		//_ret = filter_info_lock.TryLockWriter(); 
		if( FALSE == _ret )
		{
			log_trace( ( DBG_MSG_AND_ERROR_OUT, "try enter cs error" ) ); 

			SAFE_SET_ERROR_CODE( ret ); 
			break; 
		}

	}while( FALSE );

	return ret; 
}

//NTSTATUS CALLBACK get_action_filter_cond( ULONG action_cond_id, 
//										  action_filter_info *filter_cond, 
//										  LPCWSTR time, 
//										  PVOID work_context, 
//										  ULONG *state_out )
//{
//	NTSTATUS ntstatus = STATUS_SUCCESS; 
//	LRESULT ret = ERROR_SUCCESS; 
//	ULONG cc_ret_len; 
//
//	do 
//	{
//#define MAX_COMPARE_OP_LEN 64
//#define MAX_COLUME_NAME_LEN 260
//#define MAX_FILTER_VALUES_LEN 1024
//		WCHAR compare_op[ MAX_COMPARE_OP_LEN ]; 
//		WCHAR colume_name[ MAX_COLUME_NAME_LEN ]; 
//		WCHAR filter_values[ MAX_FILTER_VALUES_LEN ]; 
//
//		ret = get_filter_values( filter_cond, filter_values, ARRAYSIZE( filter_values ), &cc_ret_len ); 
//		if( ret != ERROR_SUCCESS )
//		{
//			break; 
//		}
//
//		ret = get_compare_op( filter_cond, compare_op, ARRAYSIZE( compare_op ), &cc_ret_len ); 
//		if( ret != ERROR_SUCCESS )
//		{
//			break; 
//		}
//
//		ret = get_colume_name( filter_cond, colume_name, ARRAYSIZE( colume_name ), &cc_ret_len ); 
//		if( ret != ERROR_SUCCESS )
//		{
//			break; 
//		}
//
//		ret = insert_condition_sql( colume_name, compare_op, filter_values ); 
//		if( ret != ERROR_SUCCESS )
//		{
//			break; 
//		}
//
//	}while( FALSE ); 
//
//
//	return ntstatus; 
//}

/*****************************************************************************
will make more database for caching events which is filtered.so need a function 
set to manage the data structure for caching.
caching database id:
1.timestamp
2.base database information.
3.filtered target database and table.
4.filtered sql

is when remove the temporary database for filtering? 
1.change the filtered condition.( accumulated filtering is best )
*****************************************************************************/

//ULONG filter_conds_count = 0; 
//action_filter_info *filters = NULL; 

//typedef struct _filter_value_header
//{
//	BOOLEAN valid; 
//} filter_value_header, *pfilter_value_header; 
//
//typedef struct _time_filter_value
//{
//	LARGE_INTEGER begin; 
//	LARGE_INTEGER end; 
//} time_filter_value, *ptime_filter_value; 
//
//typedef struct _text_filter_value
//{
//#define TEXT_FILTER_VALUE_LEN 1024
//	ULONG text_len; 
//	WCHAR text[ TEXT_FILTER_VALUE_LEN ]; 
//} text_filter_value, *ptext_filter_value; 
//
//typedef struct _data_filter_value
//{
//#define MAX_DATA_SIZE 200
//	ULONG data_size; 
//	BYTE data[ MAX_DATA_SIZE ]
//} data_filter_value, *pdata_filter_value; 
//
//typedef ULONG integer_value; 
//typedef struct _filter_values
//{
//	ULONG mask; 
//	text_filter_value image_path; 
//	text_filter_value proc_name; 
//	text_filter_value command_line; 
//	text_filter_value path; 
//	integer_value pid; 
//	integer_value tid; 
//	integer_value parent_pid; 
//	integer_value session; 
//	integer_value sequence; 
//	time_filter_value time; 
//	time_filter_value relative_time; 
//	time_filter_value duration; 
//	integer_value main_type; 
//	integer_value event_type; 
//	integer_value io_category; 
//	integer_value result; 
//	text_filter_value desc; 
//	text_filter_value detail; 
//	text_filter_value company; 
//	text_filter_value version; 
//	integer_value integrity; 
//	text_filter_value auth_id; 
//	text_filter_value user; 
//	text_filter_value arch_name; 
//	integer_value virtualized; 
//} filter_values, *pfilter_values; 

/*************************************************************
filter value type: 
1. value from ring 3. 
2. value from ring 0. 
*************************************************************/

/*************************************************************
time
main type
*************************************************************/
HANDLE WINAPI get_parent_pid( HANDLE pid )
{
	HANDLE parent_pid = INVALID_PROCESS_ID; 

	return parent_pid; 
} 

ULONG WINAPI get_action_main_type( sys_action_type action_type )
{
	ULONG main_type = 0; 

	switch( action_type )
	{

		//MT_execmon
	case EXEC_create:   //进程启动 进程路径名 （执行监控） 
	case EXEC_destroy:  //进程退出 进程路径名 
	case EXEC_module_load:  //模块加载 模块路径名 
		main_type = MT_execmon; 
		break; 
		//MT_filemon:  
	case FILE_touch:  //创建文件 文件全路径 （文件监控） 
	case FILE_open:  //打开文件 文件全路径 
	case FILE_read:  //读取文件 文件全路径 
	case FILE_write:  //写入文件 文件全路径 
	case FILE_modified:  //文件被修改 文件全路径 
	case FILE_readdir:  //遍历目录 目录全路径 
	case FILE_remove:  //删除文件 文件全路径 
	case FILE_rename:  //重命名文件 文件全路径 
	case FILE_truncate:  //截断文件 文件全路径 
	case FILE_mklink:  //建立文件硬链接 文件全路径 
	case FILE_chmod:  //设置文件属性 文件全路径 
	case FILE_setsec:  //设置文件安全属性 文件全路径 
	case FILE_getinfo:
	case FILE_setinfo:
	case FILE_close:
		main_type = MT_filemon; 
		break; 
		//MT_regmon:  
	case REG_openkey:  //打开注册表键 注册表键路径  （注册表监控） 
	case REG_mkkey:  //创建注册表键 注册表键路径 
	case REG_rmkey:  //删除注册表键 注册表键路径 
	case REG_mvkey:  //重命名注册表键 注册表键路径 
	case REG_rmval:  //删除注册表键 注册表键路径 
	case REG_getval:  //获取注册表值 注册表值路径 
	case REG_setval:  //设置注册表值 注册表值路径 
	case REG_loadkey:  //挂载注册表Hive文件 注册表键路径 
	case REG_replkey:  //替换注册表键 注册表键路径 
	case REG_rstrkey:  //导入注册表Hive文件 注册表键路径 
	case REG_setsec:  //设置注册表键安全属性 注册表键路径 
	case REG_closekey:
		main_type = MT_regmon; 
		break; 

		//MT_procmon:  
	case PROC_exec:  //创建进程 目标进程路径名  （进程监控）
	case PROC_open:  //打开进程 目标进程路径名 
	case PROC_debug:  //调试进程 目标进程路径名 
	case PROC_suspend:  //挂起进程 目标进程路径名 
	case PROC_resume:  //恢复进程 目标进程路径名 
	case PROC_kill:  
	case PROC_exit:  //结束进程 目标进程路径名 
	case PROC_job:  //将进程加入工作集 目标进程路径名 
	case PROC_pgprot:  //跨进程修改内存属性 目标进程路径名 
	case PROC_freevm:  //跨进程释放内存 目标进程路径名 
	case PROC_writevm:  //跨进程写内存 目标进程路径名 
	case PROC_readvm:  //跨进程读内存 目标进程路径名 
	case THRD_remote:  //创建远程线程 目标进程路径名 
	case THRD_create:  //创建线程
	case THRD_setctxt:  //跨进程设置线程上下文 目标进程路径名 
	case THRD_suspend:  //跨进程挂起线程 目标进程路径名 
	case THRD_resume:  //跨进程恢复线程 目标进程路径名 
	case THRD_kill:  //跨进程结束线程 目标进程路径名 
	case THRD_exit:  
	case THRD_queue_apc:  //跨进程排队APC 目标进程路径名 
		main_type = MT_procmon; 
		break; 

		//MT_common
	case COM_access:
	case PORT_read:
	case PORT_write:
	case PORT_urb: 
		main_type = MT_common; 
		break; 

		//MT_sysmon
	case SYS_settime:  //设置系统时间 无 
	case SYS_link_knowndll:  //建立KnownDlls链接 链接文件名 
	case SYS_open_physmm:  //打开物理内存设备 无 
	case SYS_read_physmm:  //读物理内存 无 
	case SYS_write_physmm:  //写物理内存 无 
	case SYS_load_kmod:  //加载内核模块 内核模块全路径 
	case SYS_load_mod:  //加载模块 内核模块全路径 
	case SYS_unload_mod:  //卸载模块 内核模块全路径 
	case SYS_enumproc:  //枚举进程 无 
	case SYS_regsrv:  //注册服务 服务进程全路径 
	case SYS_opendev:  //打开设备 设备名 
		main_type = MT_sysmon; 
		break; 

		//MT_w32mon
	case W32_postmsg:  //发送窗口消息（Post） 目标进程路径名 
	case W32_sendmsg:  //发送窗口消息（Send） 目标进程路径名 
	case W32_findwnd:  //查找窗口 无 
	case W32_msghook:  //设置消息钩子 无 
	case W32_lib_inject:  //DLL注入 注入DLL路径名 
		main_type = MT_w32mon; 
		break; 

		//MT_netmon:  
	case NET_create:  
	case NET_connect:  //网络连接 远程地址（格式：IP：端口号） （网络监控） 
	case NET_listen:  //监听端口 本机地址（格式：IP：端口号） 
	case NET_send:  //发送数据包 远程地址（格式：IP：端口号） 
	case NET_recv:  
	case NET_accept:  

	case NET_dns:  
	case NET_http:  //HTTP请求 HTTP请求路径（格式：域名/URL） 
	case NET_icmp_send:  
	case NET_icmp_recv:  
		main_type = MT_netmon; 
		break; 

		//MT_behavior:  
	case BA_extract_hidden:  //释放隐藏文件 释放文件路径名 （行为监控） 
	case BA_extract_pe:  //释放PE文件 释放文件路径名 
	case BA_self_copy:  //自我复制 复制目标文件路径名 
	case BA_self_delete:  //自我删除 删除文件路径名 
	case BA_ulterior_exec:  //隐秘执行 被执行映像路径名 
	case BA_invade_process:  //入侵进程 目标进程路径名 
	case BA_infect_pe:  //感染PE文件 目标文件路径名 
	case BA_overwrite_pe:  //覆写PE文件 目标文件路径名 
	case BA_register_autorun:  //注册自启动项 自启动文件路径名 
	case BA_other:  
		main_type = MT_behavior; 
	default:
		main_type = MT_unknown; 
		break; 
	}

	return main_type; 
}

ULONG WINAPI get_action_io_type( sys_action_type action_type )
{
	ULONG io_type = 0; 

	return io_type; 
}

/**********************************************************************************************************************
速度慢的解决方案：
大量的使用内存，通过内存来实现实时显示日志。
如果需要进行分析，则是立足于数据库，一定将所有能加载的日志全部加载，最大量由内存容量决定。

内存加载法，高速显示所有的信息。

多级缓存法：
第1级缓存是数据库
第2级缓存是内存，直接加载所有数据库中的内容。全部放在内存中。
第3级缓存是过滤后的数据，加载内存中经过过滤的信息。

速度的重要性：


显示一个完整的描述信息好，还是将操作与对像，细节分别显示好：

**********************************************************************************************************************/
BOOLEAN WINAPI filter_action_time_value( LARGE_INTEGER *event_time, time_region *duration, action_compare_mode flt_mode )
{
	BOOLEAN filtered = FALSE; 

	do 
	{
		switch( flt_mode )
		{
		case FLT_CONTAINS: 
			if( event_time->QuadPart >=
				duration->begin_time.QuadPart 
				&& event_time->QuadPart <= duration->end_time.QuadPart )
			{
				filtered = TRUE; 
			} 
			break; 
		case FLT_EXCLUDES: 
			if( event_time->QuadPart <
				duration->begin_time.QuadPart 
				&& event_time->QuadPart > duration->end_time.QuadPart )
			{
				filtered = TRUE; 
			} 
			break; 
		case FLT_EQUALS: 
			if( event_time->QuadPart >=
				duration->begin_time.QuadPart 
				&& event_time->QuadPart <= duration->end_time.QuadPart )
			{
				filtered = TRUE; 
			} 
			break; 

		case FLT_NOT_EQUALS: 
			if( event_time->QuadPart <
				duration->begin_time.QuadPart 
				&& event_time->QuadPart > duration->end_time.QuadPart )
			{
				filtered = TRUE; 
			} 
			break; 
		default: 
			ASSERT( FALSE ); 
			break; 
		}
	}while( FALSE );

	return filtered; 
}

BOOLEAN WINAPI _filter_action_time_value( LARGE_INTEGER *event_time, LARGE_INTEGER *dst_time, action_compare_mode flt_mode )
{
	BOOLEAN filtered = FALSE; 

	do 
	{
		event_time->QuadPart = event_time->QuadPart - ( event_time->QuadPart % ( 10000000 ) ); 

		switch( flt_mode )
		{
		case FLT_CONTAINS: 
			if( event_time->QuadPart == dst_time->QuadPart )
			{
				filtered = TRUE; 
			} 
			break; 
		case FLT_EXCLUDES: 
			if( event_time->QuadPart != dst_time->QuadPart )
			{
				filtered = TRUE; 
			} 
			break; 
		case FLT_EQUALS: 
			if( event_time->QuadPart == dst_time->QuadPart )
			{
				filtered = TRUE; 
			} 
			break; 

		case FLT_NOT_EQUALS: 
			if( event_time->QuadPart != dst_time->QuadPart )
			{
				filtered = TRUE; 
			} 
			break; 
		case FLT_GREATER_THAN: 
			if( event_time->QuadPart > dst_time->QuadPart )
			{
				filtered = TRUE; 
			} 
			break; 
		case FLT_LOWER_THAN: 
			if( event_time->QuadPart < dst_time->QuadPart )
			{
				filtered = TRUE; 
			} 
			break; 
		case FLT_START_WITH: 
			if( event_time->QuadPart >= dst_time->QuadPart )
			{
				filtered = TRUE; 
			} 
			break; 
		case FLT_END_WITH: 
			if( event_time->QuadPart <= dst_time->QuadPart )
			{
				filtered = TRUE; 
			} 
			break; 

		default: 
			ASSERT( FALSE ); 
			break; 
		}
	}while( FALSE );

	return filtered; 
}

BOOLEAN WINAPI filter_action_ulong_value( ULONG event_type, ULONG policy_type, action_compare_mode flt_mode )
{
	BOOLEAN filtered = FALSE; 

	switch( flt_mode )
	{
	case FLT_CONTAINS: 

		if( event_type == ( event_type & policy_type ) )
		{
			filtered = TRUE; 
			break; 
		}

		break; 
	case FLT_EXCLUDES: 
		if( 0 == ( event_type & policy_type ) )
		{
			filtered = TRUE; 
			break; 
		}
		break; 
	case FLT_EQUALS: 
		if( event_type == policy_type )
		{
			filtered = TRUE; 
			break; 
		}
		break; 

	case FLT_NOT_EQUALS: 
		if( event_type != policy_type )
		{
			filtered = TRUE; 
			break; 
		}
		break; 
	default: 
		ASSERT( FALSE ); 
		break; 
	}

	return filtered; 
}

BOOLEAN WINAPI filter_action_ulonglong_value( ULONGLONG source_value, ULONGLONG dest_value, action_compare_mode flt_mode )
{
	BOOLEAN filtered = FALSE; 

	switch( flt_mode )
	{
	case FLT_CONTAINS: 

		if( source_value == dest_value )
		{
			filtered = TRUE; 
			break; 
		}

		break; 
	case FLT_EXCLUDES: 
		if( source_value != dest_value )
		{
			filtered = TRUE; 
			break; 
		}
		break; 
	case FLT_EQUALS: 
		if( source_value == dest_value )
		{
			filtered = TRUE; 
			break; 
		}
		break; 

	case FLT_NOT_EQUALS: 
		if( source_value != dest_value )
		{
			filtered = TRUE; 
			break; 
		}
		break; 
	default: 
		ASSERT( FALSE ); 
		break; 
	}

	return filtered; 
}

BOOLEAN WINAPI filter_action_string_value( LPCWSTR source_value, ULONG cc_src_len, LPCWSTR dest_value, ULONG cc_dest_len, action_compare_mode flt_mode )
{
	BOOLEAN filtered = FALSE; 
	INT32 _ret; 
	LPWSTR text_value; 

	switch( flt_mode )
	{
	case FLT_CONTAINS: 
		text_value = wcsstr( ( LPWSTR )source_value, 
			dest_value ); //ctx->proc_name_len  
		if( text_value != NULL )
		{
			filtered = TRUE; 
		}
		break; 
	case FLT_EXCLUDES: 
		text_value = wcsstr( ( LPWSTR )source_value, 
			dest_value ); //ctx->proc_name_len  
		if( text_value == NULL )
		{
			filtered = TRUE; 
		}
		break; 
	case FLT_EQUALS: 
		_ret = wcsnicmp( source_value, 
			dest_value, 
			cc_dest_len ); 

		if( _ret == 0 )
		{
			filtered = TRUE; 
		}
		break; 
	case FLT_NOT_EQUALS: 
		_ret = wcsnicmp( source_value, 
			dest_value, 
			cc_dest_len ); 

		if( _ret != 0 )
		{
			filtered = TRUE; 
		}
		break; 
	default: 
		ASSERT( FALSE ); 
		break; 
	}

	return filtered; 
}

LRESULT WINAPI filter_action( action_filter_info *filter_info, 
										  sys_action_record *action, 
										  action_context *ctx, 
										  PVOID data, 
										  ULONG data_size )
{
	LRESULT ret = ERROR_NOT_FOUND; 
	LRESULT _ret; 
	BOOLEAN found = FALSE; 

	do 
	{
		switch( filter_info->cond )
		{
		case FLT_PATH:
			{
				WCHAR obj_path[ _MAX_REG_PATH_LEN ]; 
				LPCWSTR _obj_path; 
				ULONG obj_path_len; 
				
				_ret = get_object_path( action, 
					ctx, 
					obj_path, 
					ARRAYSIZE( obj_path ), 
					&_obj_path, 
					0, 
					&obj_path_len ); 
				
				if( _ret != ERROR_SUCCESS )
				{
					break; 
				}

				found = filter_action_string_value( _obj_path, 
					obj_path_len, 
					filter_info->value.text.string_value, 
					filter_info->value.text.cc_string_len, 
					filter_info->compare_mode ); 
				break; 			
			}
			break; 
		case FLT_PROCESS_NAME: 
			{
				LPCWSTR proc_name; 
				ULONG proc_name_len; 

				proc_name = wcsrchr( ctx->proc_name, PATH_DELIM ); 
				if( proc_name == NULL )
				{
					proc_name = ctx->proc_name; 
					proc_name_len = ( ULONG )wcslen( ctx->proc_name ); 

					ASSERT( wcsicmp( proc_name, L"SYSTEM" ) == 0 ); 
				}
				else
				{
					proc_name += 1;  
					proc_name_len = ( ULONG )( ULONG_PTR )( ( ctx->proc_name + ctx->proc_name_len ) - proc_name ); 
				
#ifdef _DEBUG
					if( proc_name[ proc_name_len - 1 ] != L'e' 
						&& proc_name[ proc_name_len - 1 ] != L'E' ) 
					{
						ASSERT( FALSE ); 
					}
#endif //_DEBUG
				}

				found = filter_action_string_value( proc_name, 
					proc_name_len, 
					filter_info->value.text.string_value, 
					filter_info->value.text.cc_string_len, 
					filter_info->compare_mode ); 

				break; 
			}
			break; 
		case FLT_COMMAND: 
			{
				//WCHAR *cmd_line; 

				ret = ERROR_INVALID_PARAMETER; 

				break; 
			}
			break; 
		case FLT_PID: 
			{
				found = filter_action_ulong_value( ctx->proc_id, 
					filter_info->value.data.ulong_value, 
					filter_info->compare_mode ); 
			}
			break; 
		case FLT_PARENT_PID: 
			{
				ULONG parent_pid; 

				ret = ERROR_NOT_SUPPORTED; 
				break; 
				parent_pid = ( ULONG )( ULONG_PTR )get_parent_pid( ( HANDLE )( ULONG_PTR )ctx->proc_id ); 
				
				found = filter_action_ulong_value( parent_pid, 
					filter_info->value.data.ulong_value, 
					filter_info->compare_mode ); 
			}
			break; 
		case FLT_TID: 
			{
				ULONG tid; 

				tid = ctx->thread_id; 

				found = filter_action_ulong_value( tid, 
					filter_info->value.data.ulong_value, 
					filter_info->compare_mode ); 
			}
			break; 
		case FLT_MAIN_TYPE: 
			{
				ULONG main_type; 

				main_type = get_action_main_type( action->type ); 

//#ifdef _DEBUG
//				if( main_type == MT_filemon )
//				{
//					__asm int 3; 
//				}
//#endif //_DEBUG

				found = filter_action_ulong_value( main_type, 
					filter_info->value.data.ulong_value, 
					filter_info->compare_mode ); 
			}
			break; 
		case FLT_ACTION_TYPE: 
			{
				found = filter_action_ulong_value( action->type, 
					filter_info->value.data.ulong_value, 
					filter_info->compare_mode ); 
			}
			break; 
		case FLT_READ_WRITE: 
			{
				ULONG io_type; 
				
				io_type = get_action_io_type( action->type ); 

				found = filter_action_ulong_value( io_type, 
					filter_info->value.data.ulong_value, 
					filter_info->compare_mode ); 
			}
			break; 
		case FLT_DESCRITION: 
			{
				//LPCWSTR desc; 

				ret = ERROR_NOT_SUPPORTED; 
				
				break;
				//found = filter_action_ulong_value( io_type, 
				//	filter_info->value.data.ulong_value, 
				//	filter_info->mode ); 
			}
			break; 
		case FLT_DESC_DETAIL: 
			break; 
		case FLT_ACTION_RESULT: 
			{
				found = filter_action_ulong_value( ctx->last_result, 
					filter_info->value.data.ulong_value, 
					filter_info->compare_mode ); 
			}
			break; 
		case FLT_DURATION: 
			{
				//ASSERT( filter_info->value.size >= sizeof( time_region ) ); 

				found = filter_action_time_value( &ctx->time, &filter_info->value.data.duration, filter_info->compare_mode ); 
			}
			break; 
		case FLT_DATE_TIME:  
			{
				found = _filter_action_time_value( &ctx->time, &filter_info->value.data.time_value, filter_info->compare_mode ); 
			}
			break; 
		case FLT_RELATIVE_TIME: 
			//{
			//	found = filter_action_time_value( &ctx->time, &filter_info->value.data.time_value ); 
			//}
			break; 
		case FLT_SESSION:  
			break; 
		case FLT_USER:  
			break; 
		case FLT_AUTH_ID:  
			break; 
		case FLT_ACTION_NO: 
			break; 
		case FLT_CORPORATION:  
			break; 
		case FLT_VERSION: 
			{
				LARGE_INTEGER file_version; 
				
				_ret = get_file_version( ctx->proc_name, &file_version ); 
				if( _ret != ERROR_SUCCESS )
				{
					break; 
				}

				found = filter_action_ulonglong_value( ( ULONGLONG )file_version.QuadPart, filter_info->value.data.ulonglong_value, filter_info->compare_mode ); 
			}
			break; 
		case FLT_VIRTUAL_TECH: 
			break; 
		case FLT_CPU_ARCH: 
			break; 
		default: 
			ret = ERROR_INVALID_PARAMETER; 
			ASSERT( FALSE ); 
			break; 
		}

		if( found != TRUE )
		{
			break; 
		}

		//if( filter_info->result != FLT_INCLUDE )
		//{
		ret = ERROR_SUCCESS; 
		//}

	}while( FALSE ); 

	return ret; 
}

//typedef struct _bittrace_base_work_info
//{
//	WCHAR exec_file_name[ MAX_PATH ]; 
//	WCHAR dest_file_name[ MAX_PATH ]; 
//} bittrace_base_work_info, *pbittrace_base_work_info; 
//
//bittrace_base_work_info log_base_work_info = { 0 }; 

/******************************************************************************************
当前的过滤的方法的问题：
1.过滤速度比较慢，尤其过滤出量大的时候。
处理此问题有两种方法：
1.大量使用内存缓存，像PROCMON一样的工作方式。
2.控制分析的时域。
******************************************************************************************/
LRESULT WINAPI init_filter_infos()
{
	LRESULT ret = ERROR_SUCCESS; 

	init_cs_lock( filter_info_lock ); 
	//init_cs_lock( hilight_filter_lock ); 

	return ret; 
}

LRESULT WINAPI uninit_filter_infos()
{
	LRESULT ret = ERROR_SUCCESS; 

	uninit_cs_lock( filter_info_lock ); 
	//uninit_cs_lock( hilight_filter_lock ); 

	return ret; 
}

#ifdef _DEBUG
ULONG dbg_thread_id = ( ULONG )( ULONG_PTR )INVALID_THREAD_ID - 1; 
ULONG dbg_proc_id = ( ULONG )( ULONG_PTR )INVALID_PROCESS_ID; 
ULONG test_proc_id = ( ULONG )( ULONG_PTR )INVALID_PROCESS_ID; 
#endif //_DEBUG

LRESULT WINAPI filter_action_log( sys_action_record *action, 
								 action_context *ctx,  
								 PVOID data, 
								 ULONG data_size )
{
	LRESULT ret; 
	LRESULT _ret; 
	BOOLEAN lock_held = FALSE; 
	BOOLEAN include_filter = FALSE; 
	ULONG i; 

	do 
	{
		ret = ERROR_SUCCESS; 

		_ret = w_try_lock_filter_infos(); 

		if( ERROR_SUCCESS != _ret )
		{
			break; 
		}

		lock_held = TRUE; 

#ifdef _DEBUG
		if( dbg_thread_id == ctx->thread_id )
		{
			DBG_BP(); 
		}

		if( dbg_proc_id == ctx->proc_id )
		{
			DBG_BP(); 
		}

#endif //_DEBUG

		_ret = ERROR_NOT_FOUND; 

		for( i = 0; i < filter_infos.filter_infos.size(); i ++ )
		{
			do 
			{
				if( filter_infos.filter_infos[ i ]->filter_mode != FLT_INCLUDE )
				{
					break; 
				}

				if( include_filter == FALSE )
				{
					include_filter = TRUE; 
				}

				_ret = filter_action( filter_infos.filter_infos[ i ], 
					action, 
					ctx, 
					data, 
					data_size ); 

				if( _ret == ERROR_NOT_FOUND )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

			}while( FALSE ); 
		}

		if( FALSE != include_filter )
		{
			break; 
		}

		ret = ERROR_SUCCESS; 
		_ret = ERROR_NOT_FOUND; 
		for( i = 0; i < filter_infos.filter_infos.size(); i ++ )
		{
			do 
			{
				if( filter_infos.filter_infos[ i ]->filter_mode != FLT_EXCLUDE )
				{
					break; 
				}

				_ret = filter_action( filter_infos.filter_infos[ i ], 
					action, 
					ctx, 
					data, 
					data_size ); 

			}while( FALSE );

			if( _ret != ERROR_NOT_FOUND )
			{
				ret = ERROR_INVALID_PARAMETER; 
				break; 
			}
		}

	}while( FALSE );

#ifdef _DEBUG
	if( test_proc_id != ( ULONG )INVALID_PROCESS_ID 
		&& test_proc_id != ctx->proc_id 
		&& ret == ERROR_SUCCESS )
	{
		DBG_BP(); 
	}
#endif //_DEBUG

	if( TRUE == lock_held )
	{
		w_unlock_filter_infos(); 
	}

	return ret; 
}

LRESULT WINAPI filter_log_for_ui( sys_action_record *action, 
								 action_context *ctx,  
								 PVOID data, 
								 ULONG data_size, 
								 filter_ui_info *ui_info )
{
	LRESULT ret; 
	LRESULT _ret; 
	BOOLEAN lock_held = FALSE; 
	ULONG i; 

	do 
	{
		ret = ERROR_INVALID_PARAMETER; 

		if( ui_info == NULL )
		{
			//ret = ERROR_INVALID_PARAMETER; 
			break; 
		}

		//ui_info->bk_clr = INVALID_UI_FILTERED_BK_COLOR; 
		//ui_info->cond = FLT_TYPE_INVALID; 

		_ret = w_try_lock_filter_infos(); 

		if( ERROR_SUCCESS != _ret )
		{
			break; 
		}

		lock_held = TRUE; 

		_ret = ERROR_NOT_FOUND; 

		for( i = 0; i < filter_infos.filter_infos.size(); i ++ )
		{
			do 
			{
				if( filter_infos.filter_infos[ i ]->ui_info.bk_clr == INVALID_UI_FILTERED_BK_COLOR )
				{
					break; 
				}

				_ret = filter_action( filter_infos.filter_infos[ i ], 
					action, 
					ctx, 
					data, 
					data_size ); 

			}while( FALSE );

			if( _ret != ERROR_NOT_FOUND )
			{
				ui_info->bk_clr = filter_infos.filter_infos[ i ]->ui_info.bk_clr; 
				ui_info->cond = filter_infos.filter_infos[ i ]->cond; 

				ret = ERROR_SUCCESS; 
				break; 
			}
		}
		
	}while( FALSE );

	if( TRUE == lock_held )
	{
		w_unlock_filter_infos(); 
	}

	if( ret != ERROR_SUCCESS )
	{
		ui_info->bk_clr = INVALID_UI_FILTERED_BK_COLOR; 
		ui_info->cond = FLT_TYPE_INVALID; 
	}

	return ret; 
}

