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

#include "_stdafx.h"
#include "common_func.h"
#include "action_type.h"
#include "trace_log_api.h"
#include "bitsafe_common.h"
//#include "list_ex_common.h"
//#include "UIListSubElementEx.h"
//#include "list_log.h"
//#include "win_impl_base.hpp"
#include "sqlite3.h"
#include "filter_manage_dlg.h"
#include "trace_filter.h"
#include "event_list_item.h"
#include "msg_box.h"

CDialogBuilder builder; 

/*********************************************************************
过滤点类型：
1.前过滤：
	可以在行为发生前得到行为的信息，可以指定行为的结果。
2.后过滤：
	在行为发生后得行为的信息与结果，可以得到行为的结果。

跟踪器过滤点设计：
1.跟踪器需要得到行为的结果，所以要进行后过滤。
2.跟踪器可能需要对一些行进行控制，所要进行前过滤。

如何将前后过滤结合起来，同时具有很好的性能？

1.方案1：
前过滤提交行为信息与ID号
后过滤提交行为的ID号与结果

下一步，可能需要实现前过滤到后过滤的转换。

2.方案2：

*********************************************************************/

LPCWSTR all_filter_cond_desc[] = { L"可执行文件路径", 
L"进程名", 
L"命令行", 
L"进程ID", 
L"父进程ID", 
L"线程ID", 
L"主类型", 
L"事件类型", 
L"读/写", 
L"事件描述", 
L"事件细节", 
L"事件数据", 
L"结果", 
L"调用堆栈", 
L"时间区间", 
L"日期 时间", 
L"相对时间", 
L"会话", 
L"用户", 
L"身份ID号", 
L"序列号", 
L"公司", 
L"版本", 
L"虚拟化", 
L"CPU架构" }; 

LPCWSTR all_filter_cond_colume[] = { L"proc_name", 
L"proc_name", 
L"", //命令行
L"proc_id", 
L"", //父进程ID
L"thread_id", 
L"", //主类型
L"type", 
L"", //读/写
L"", //事件描述
L"", //事件细节
L"", //结果
L"", //时间区间
L"time", //日期 时间
L"", //相对时间
L"", //会话
L"", //用户
L"", //身份ID号
L"id", //序列号
L"", //公司
L"", //版本
L"", //虚拟化
L"" //CPU架构 
}; 

LRESULT get_filter_cond_colume_name( action_filter_cond cond, LPCWSTR *colume_name )
{
	LRESULT ret = ERROR_SUCCESS; 
	LPCWSTR _colume_name; 

	do 
	{
		if( colume_name == NULL )
		{
			ret = ERROR_INVALID_PARAMETER; 
			break; 
		}

		*colume_name = NULL; 

		if( ( LONG )cond >= ARRAYSIZE( all_filter_cond_colume ) )
		{
			ret = ERROR_INVALID_PARAMETER; 
			break; 
		}

		_colume_name = all_filter_cond_colume[ ( LONG )cond ]; 

		if( *_colume_name == L'\0' )
		{
			ret = ERROR_INVALID_PARAMETER; 
			break; 
		}

		*colume_name = _colume_name; 
	}while( FALSE );

	return ret; 
}

LRESULT WINAPI construct_sql_filter_cond( LPWSTR sql, 
										 ULONG cc_buf_len, 
										 ULONG *cc_ret_len )
{
	LRESULT ret = ERROR_SUCCESS; 
	LRESULT _ret; 
	LPCWSTR colume_name; 
	INT32 i; 

	do 
	{
		if( sql == NULL )
		{
			ret = ERROR_INVALID_PARAMETER; 
			break; 
		}

		if( cc_buf_len == 0 )
		{
			ret = ERROR_INVALID_PARAMETER; 
			break; 
		}

		for( i = 0; ( ULONG )i < filter_infos.filter_infos.size(); i ++ )
		{
			do 
			{
				_ret = get_filter_cond_colume_name( filter_infos.filter_infos[ i ]->cond, &colume_name ); 
				if( _ret != ERROR_SUCCESS )
				{
					ret = _ret; 
					continue; 
				}

			}while( FALSE );

		}

	}while( FALSE );

	return ret; 
}

LPCWSTR get_filter_cond_desc( action_filter_cond cond )
{
	LPCWSTR desc = L"未知"; 
	
	

	do 
	{
		if( ( ULONG )cond < ARRAYSIZE( all_filter_cond_desc ) )
		{
			desc = all_filter_cond_desc[ ( ULONG )cond ]; 
		}

		//switch( cond )
		//{
		//case FLT_PATH: 
		//	desc = "可执行文件路径"; 
		//	break; 
		//case FLT_PROCESS_NAME: 
		//	desc = "进程名"; 
		//case FLT_COMMAND: 
		//	desc = "命令行"; 
		//case FLT_PID: 
		//	desc = "进程ID"; 
		//		case FLT_PARENT_PID: 
		//			desc = "父进程ID"; 
		//case FLT_TID: 
		//	desc = "线程ID"; 
		//case FLT_MAIN_TYPE: 
		//	desc = 
		//case FLT_ACTION_TYPE: 
		//	desc = 
		//case FLT_READ_WRITE: 
		//	desc = 
		//case FLT_DESCRITION: 
		//	desc = 
		//case FLT_DESC_DETAIL: 
		//	desc = 
		//case FLT_ACTION_RESULT: 
		//	desc = 
		//case FLT_TIME_REGION: 
		//	desc = 
		//case FLT_DATE_TIME: 
		//	desc = 
		//case FLT_RELATIVE_TIME: 
		//	desc = 
		//case FLT_SESSION: 
		//	desc = 
		//case FLT_USER: 
		//	desc = 
		//case FLT_AUTH_ID: 
		//	desc = 
		//case FLT_ACTION_NO: 
		//	desc = 
		//case FLT_CORPORATION: 
		//	desc = 
		//case FLT_VERSION: 
		//	desc = 
		//case FLT_VIRTUAL_TECH: 
		//	desc = 
		//case FLT_CPU_ARCH: 
		//	desc = 
		//}

	} while ( FALSE );

	return desc; 
}

LPCWSTR all_filter_mode_desc[] = { L"包含", 
L"不包含", 
L"等于", 
L"不等于", 
L"小于", 
L"大于", 
L"开始", 
L"结束" }; 

LPCWSTR get_filter_mode_desc( action_compare_mode mode )
{
	LPCWSTR desc = L"未知"; 

	do 
	{
		if( ( ULONG )mode < ARRAYSIZE( all_filter_mode_desc ) )
		{
			desc = all_filter_mode_desc[ ( ULONG )mode ]; 
		}

	} while ( FALSE );

	return desc; 
}

LPCWSTR all_filter_result_desc[] = { L"加入", 
L"排除" }; 

LPCWSTR get_filter_result_desc( action_filter_mode result )
{
	LPCWSTR desc = L"未知"; 

	do 
	{
		if( ( ULONG )result < ARRAYSIZE( all_filter_result_desc ) )
		{
			desc = all_filter_result_desc[ ( ULONG )result ]; 
		}

	} while ( FALSE );

	return desc; 
}

LRESULT WINAPI input_default_filter_info( ULONG flags )
{
	LRESULT ret = ERROR_SUCCESS; 
	HRESULT hr; 
	ULONG _ret; 
	action_filter_info filter_info; 
	LPWSTR exe_file_name = NULL; 

	do 
	{
		exe_file_name = ( LPWSTR )malloc( sizeof( WCHAR ) * MAX_PATH ); 
		if( NULL == exe_file_name )
		{
			ret = ERROR_NOT_ENOUGH_MEMORY; 
			break; 
		}

		_ret = GetModuleFileNameW( NULL, exe_file_name, MAX_PATH ); 
		if( _ret <= 0 || 
			_ret >= MAX_PATH )
		{
			ret = ERROR_ERRORS_ENCOUNTERED; 
			break; 
		}


		init_filter_ui_info( &filter_info.ui_info ); 

		filter_info.cond = FLT_PROCESS_NAME; 

		filter_info.filter_mode = FLT_EXCLUDE; 

		filter_info.compare_mode = FLT_CONTAINS; 

		hr = StringCbPrintfW( filter_info.value.text.text_mode_value, 
			sizeof( filter_info.value.text.text_mode_value ), 
			L"%s", 
			exe_file_name ); 

		if( FAILED( hr ) )
		{
			ret = ERROR_ERRORS_ENCOUNTERED; 
			break; 
		}

		filter_info.value.text.cc_string_len = ( ULONG )wcslen( filter_info.value.text.text_mode_value ); 

		filter_info.value.type = STRING_VALUE_TYPE; 
		filter_info.value.text.text_is_ptr = FALSE; 

		ret = adjust_filter_cond_value( &filter_info ); 
		if( ret != ERROR_SUCCESS )
		{
			break; 
		}

		ret = check_action_filter_valid( &filter_info ); 
		if( ret != ERROR_SUCCESS )
		{
			break; 
		}

		if( INPUT_FILTER_INFORMATION_TO_LIST == flags )
		{
			ret = input_filter_condition( &filter_info ); 
		}
		else
		{
			ret = input_action_cond_to_db( &filter_info ); 
		}
		
		if( ret != ERROR_SUCCESS )
		{
			//break; 
		}
	
	}while( FALSE ); 

	if( NULL != exe_file_name )
	{
		free( exe_file_name ); 
	}

	return ret; 
}

NTSTATUS CALLBACK load_action_filter_cond( ULONG action_cond_id, 
										  action_filter_info *filter_cond, 
										  LPCWSTR time, 
										  PVOID work_context, 
										  ULONG *state_out )
{
	NTSTATUS ntstatus = STATUS_SUCCESS; 
	LRESULT ret = ERROR_SUCCESS; 
	bool _ret; 
	CListContainerElementExUI* pListElement = NULL;
	load_filter_ui_context *load_context; 
	CDialogBuilder *dlg_builder; 
	CListUI *list; 
	CPaintManagerUI *pm; 
	//action_filter_info_array *filters; 
	ULONG flags; 

	do 
	{
		if( work_context == NULL )
		{
			ntstatus = STATUS_INVALID_PARAMETER_1; 
			break; 
		}

		load_context = ( load_filter_ui_context* )work_context; 

		if( load_context->builder == NULL )
		{
			ret = ERROR_INVALID_PARAMETER; 
			break; 
		}

		if( load_context->list == NULL )
		{
			ret = ERROR_INVALID_PARAMETER; ; 
			break; 
		}

		if( load_context->pm == NULL )
		{
			ret = ERROR_INVALID_PARAMETER; ; 
			break; 
		}

		flags = load_context->flags; 
		list = load_context->list; 
		//filters = load_context->filters; 

		//if( filters != NULL )
		{
			if( filter_cond->value.text.text_is_ptr == TRUE )
			{
				LPCWSTR flt_value; 
				HRESULT hr; 

				flt_value = filter_cond->value.text.string_value; 

				hr = StringCbCopyW( filter_cond->value.text.string_value, 
					sizeof( filter_cond->value.text.string_value ), 
					flt_value ); 

				if( hr != NULL )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}

				filter_cond->value.text.text_is_ptr = FALSE; 
			}

			//filters->push_back( *filter_cond ); 
		}

		dlg_builder = load_context->builder; 

		pm = load_context->pm; 

		if( !dlg_builder->GetMarkup()->IsValid() ) 
		{
#define ACTION_FILTER_LIST_ITEM_XML_FILE _T( "filter_cond_item.xml" ) 
			pListElement = static_cast<CListContainerElementExUI*>( dlg_builder->Create(ACTION_FILTER_LIST_ITEM_XML_FILE, (UINT)0, NULL, pm ));
		}
		else 
		{
			pListElement = static_cast<CListContainerElementExUI*>( dlg_builder->Create( NULL, pm ));
		}

		if( pListElement == NULL )
		{
			ret = ERROR_NOT_ENOUGH_MEMORY; 
			break; 
		}

		pListElement->SetOwner( list ); 

		pListElement->SetManager( pm, list, false ); 

		pListElement->SetVisible( true ); 

		//CRect rcPadding = text_padding; 

		//pListElement->SetPadding( rcPadding );

		//pListElement->SetMaxWidth( FILTER_COND_LIST_ROW_WIDTH ); 
#define FILTER_COND_LIST_ROW_WIDTH 300
#define FILTER_COND_LIST_ROW_HEIGHT 20

#define FILTER_COND_INFO_LAYOUT_CONTAINER_NAME L"main_layout"

		pListElement->SetFixedWidth( FILTER_COND_LIST_ROW_WIDTH ); 

		CContainerUI* main_layout = static_cast< CContainerUI* >( pm->FindSubControlByName( pListElement, FILTER_COND_INFO_LAYOUT_CONTAINER_NAME ) ); 

		if( main_layout != NULL )
		{
			//#ifdef _UI_DEBUG
			//main_layout->SetMaxWidth( LIST_ROW_WIDTH ); 
			main_layout->SetFixedWidth( FILTER_COND_LIST_ROW_WIDTH ); 
			//#endif //_UI_DEBUG
			main_layout->SetVisible( true );
		}

		pListElement->SetFixedHeight( FILTER_COND_LIST_ROW_HEIGHT ); 

		CControlUI *ctrl = static_cast< CContainerUI* >( pm->FindSubControlByName( pListElement, L"cond" ) ); 
		LPCWSTR desc; 

		ASSERT( ctrl != NULL ); 

		desc = get_filter_cond_desc( filter_cond->cond ); 
		ASSERT( desc != NULL ); 

		ctrl->SetText( desc ); 

		ctrl = static_cast< CContainerUI* >( pm->FindSubControlByName( pListElement, L"mode" ) ); 

		ASSERT( ctrl != NULL ); 

		desc = get_filter_mode_desc( filter_cond->compare_mode ); 
		ASSERT( desc != NULL ); 

		ctrl->SetText( desc ); 

		ctrl = static_cast< CContainerUI* >( pm->FindSubControlByName( pListElement, L"value" ) ); 

		ASSERT( ctrl != NULL ); 

		if( filter_cond->value.text.text_is_ptr == TRUE )
		{
			if( filter_cond->value.text.string_value_ptr  == NULL )
			{
				ASSERT( FALSE ); 
				filter_cond->value.text.string_value_ptr = L""; 
			}
			ctrl->SetText( filter_cond->value.text.string_value_ptr ); 
		}
		else
		{
			ctrl->SetText( filter_cond->value.text.string_value ); 
		}

		ctrl = static_cast< CContainerUI* >( pm->FindSubControlByName( pListElement, L"result" ) ); 

		ASSERT( ctrl != NULL ); 

		desc = get_filter_result_desc( filter_cond->filter_mode ); 

		ASSERT( desc != NULL ); 

		ctrl->SetText( desc ); 

		_ret = list->AddSubItem( pListElement, UI_LIST_CONTAINER_ELEMENT_CLASS_ID ); 
		if( _ret == false )
		{
			ntstatus = STATUS_UNSUCCESSFUL; 
		}
	}while( FALSE );
	
	return ntstatus; 
}

//filter_dlg::filter_dlg( CPaintManagerUI* pManager) 
//{
//	ASSERT( pManager != NULL ); 
//
//	m_pManager = pManager; 
//}
filter_dlg::filter_dlg( CWindowWnd *wnd ) 
{
	main_wnd = wnd; 
}

NTSTATUS filter_dlg::load_log_filter( action_filter_info *filter_cond )
{
	NTSTATUS ntstatus = STATUS_SUCCESS; 
	LRESULT ret = ERROR_SUCCESS; 
	bool _ret; 
	CListContainerElementExUI* pListElement = NULL;
	CListUI *list; 
	//ULONG flags; 

	do 
	{
		ASSERT( filter_cond != NULL ); 

		//if( filter_cond->value.type == STRING_POINTER_VALUE_TYPE )
		//{
		//	LPCWSTR flt_value; 
		//	HRESULT hr; 

		//	flt_value = filter_cond->value.text.string_value_ptr; 

		//	hr = StringCbCopyW( filter_cond->value.text.string_value, 
		//		sizeof( filter_cond->value.text.string_value ), 
		//		flt_value ); 

		//	if( hr != NULL )
		//	{
		//		ret = ERROR_INVALID_PARAMETER; 
		//		break; 
		//	}
		//}

		list = ( CListUI* )m_pm.FindControl( L"flt_cond_list" ); 

		//FindSubControlByName( this, L"flt_cond_list" ); 
		ASSERT( list ); 

		if( !builder.GetMarkup()->IsValid() ) 
		{
#define ACTION_FILTER_LIST_ITEM_XML_FILE _T( "filter_cond_item.xml" ) 
			pListElement = static_cast<CListContainerElementExUI*>( builder.Create(ACTION_FILTER_LIST_ITEM_XML_FILE, (UINT)0, NULL, &m_pm ));
		}
		else 
		{
			pListElement = static_cast<CListContainerElementExUI*>( builder.Create( NULL, &m_pm ));
		}

		if( pListElement == NULL )
		{
			ret = ERROR_NOT_ENOUGH_MEMORY; 
			break; 
		}

		pListElement->SetOwner( list ); 

		pListElement->SetManager( &m_pm, list, false ); 

		pListElement->SetVisible( true ); 

		//CRect rcPadding = text_padding; 

		//pListElement->SetPadding( rcPadding );

		//pListElement->SetMaxWidth( FILTER_COND_LIST_ROW_WIDTH ); 
#define FILTER_COND_LIST_ROW_WIDTH 300
#define FILTER_COND_LIST_ROW_HEIGHT 20

#define FILTER_COND_INFO_LAYOUT_CONTAINER_NAME L"main_layout"

		pListElement->SetFixedWidth( FILTER_COND_LIST_ROW_WIDTH ); 

		CContainerUI* main_layout = static_cast< CContainerUI* >( m_pm.FindSubControlByName( pListElement, FILTER_COND_INFO_LAYOUT_CONTAINER_NAME ) ); 

		if( main_layout != NULL )
		{
			//#ifdef _UI_DEBUG
			//main_layout->SetMaxWidth( LIST_ROW_WIDTH ); 
			main_layout->SetFixedWidth( FILTER_COND_LIST_ROW_WIDTH ); 
			//#endif //_UI_DEBUG
			main_layout->SetVisible( true );
		}

		pListElement->SetFixedHeight( FILTER_COND_LIST_ROW_HEIGHT ); 

		pListElement->set_user_data( filter_cond ); 

		CControlUI *ctrl = static_cast< CControlUI* >( m_pm.FindSubControlByName( pListElement, L"cond" ) ); 
		LPCWSTR desc; 

		ASSERT( ctrl != NULL ); 

		desc = get_filter_cond_desc( filter_cond->cond ); 
		ASSERT( desc != NULL ); 

		ctrl->SetText( desc ); 

		ctrl = static_cast< CControlUI* >( m_pm.FindSubControlByName( pListElement, L"mode" ) ); 

		ASSERT( ctrl != NULL ); 

		desc = get_filter_mode_desc( filter_cond->compare_mode ); 
		ASSERT( desc != NULL ); 

		ctrl->SetText( desc ); 

		ctrl = static_cast< CControlUI* >( m_pm.FindSubControlByName( pListElement, L"value" ) ); 

		ASSERT( ctrl != NULL ); 

		if( filter_cond->value.text.text_is_ptr == TRUE )
		{
			if( filter_cond->value.text.string_value_ptr == NULL )
			{
				filter_cond->value.text.string_value_ptr = L""; 
			}
			ctrl->SetText( filter_cond->value.text.string_value_ptr ); 
		}
		else
		{
			ctrl->SetText( filter_cond->value.text.string_value ); 
		}

		ctrl = static_cast< CControlUI* >( m_pm.FindSubControlByName( pListElement, L"result" ) ); 

		ASSERT( ctrl != NULL ); 

		desc = get_filter_result_desc( filter_cond->filter_mode ); 

		ASSERT( desc != NULL ); 

		ctrl->SetText( desc ); 

		_ret = list->AddSubItem( pListElement, UI_LIST_CONTAINER_ELEMENT_CLASS_ID ); 
		if( _ret == false )
		{
			ntstatus = STATUS_UNSUCCESSFUL; 
		}
	}while( FALSE );

	return ntstatus; 
}

LRESULT filter_dlg::get_selected_filter_info( action_filter_info **filter_info, BOOLEAN reseting )
{
	LRESULT ret = ERROR_SUCCESS; 
	INT32 _ret; 
	INT32 item_index; 
	CListContainerElementExUI* pListElement = NULL;
	CListUI *list; 
	//CControlUI *ctrl; 
	//CStdString user_data; 
	
	do
	{
		*filter_info = NULL; 
		
		list = ( CListUI* )m_pm.FindControl( L"flt_cond_list" ); 

		//FindSubControlByName( this, L"flt_cond_list" ); 
		ASSERT( list ); 

		item_index = list->GetCurSel(); 

		pListElement = ( CListContainerElementExUI* )list->GetSubItemAt( item_index, UI_LIST_CONTAINER_ELEMENT_CLASS_ID ); 
		if( pListElement == NULL )
		{
			ret = ERROR_NOT_ENOUGH_MEMORY; 
			break; 
		}

		*filter_info = ( action_filter_info* )pListElement->get_user_data(); 

		if( reseting == TRUE )
		{
			pListElement->set_user_data( NULL ); 
		}

		if( *filter_info == NULL )
		{
#ifdef _DEBUG
			DBG_BP(); 
#endif //_DEBUG

			break; 
		}
	}while( FALSE );

	return ret; 
}

//LRESULT filter_dlg::get_selected_filter_info_from_ui( action_filter_info **filter_info )
//{
//	//pListElement->SetOwner( list ); 
//
//	//pListElement->SetManager( &m_pm, list, false ); 
//
//	//pListElement->SetVisible( true ); 
//
//	//CRect rcPadding = text_padding; 
//
//	//pListElement->SetPadding( rcPadding );
//
//	//pListElement->SetMaxWidth( FILTER_COND_LIST_ROW_WIDTH ); 
//	//#define FILTER_COND_LIST_ROW_WIDTH 300
//	//#define FILTER_COND_LIST_ROW_HEIGHT 20
//	//
//	//#define FILTER_COND_INFO_LAYOUT_CONTAINER_NAME L"main_layout"
//	//
//	//		pListElement->SetFixedWidth( FILTER_COND_LIST_ROW_WIDTH ); 
//	//
//	//		CContainerUI* main_layout = static_cast< CContainerUI* >( m_pm.FindSubControlByName( pListElement, FILTER_COND_INFO_LAYOUT_CONTAINER_NAME ) ); 
//	//
//	//		if( main_layout != NULL )
//	//		{
//	//			//#ifdef _UI_DEBUG
//	//			//main_layout->SetMaxWidth( LIST_ROW_WIDTH ); 
//	//			main_layout->SetFixedWidth( FILTER_COND_LIST_ROW_WIDTH ); 
//	//			//#endif //_UI_DEBUG
//	//			main_layout->SetVisible( true );
//	//		}
//	//
//	//		pListElement->SetFixedHeight( FILTER_COND_LIST_ROW_HEIGHT ); 
//
//	ctrl = static_cast< CContainerUI* >( m_pm.FindSubControlByName( pListElement, L"cond" ) ); 
//	LPCWSTR desc; 
//
//	ASSERT( ctrl != NULL ); 
//
//	ctrl->GetText() 
//		desc = get_filter_cond_desc( ); 
//	ASSERT( desc != NULL ); 
//
//	ctrl->SetText( desc ); 
//
//	ctrl = static_cast< CContainerUI* >( m_pm.FindSubControlByName( pListElement, L"mode" ) ); 
//
//	ASSERT( ctrl != NULL ); 
//
//	desc = get_filter_mode_desc( filter_cond->mode ); 
//	ASSERT( desc != NULL ); 
//
//	ctrl->SetText( desc ); 
//
//	ctrl = static_cast< CContainerUI* >( m_pm.FindSubControlByName( pListElement, L"value" ) ); 
//
//	ASSERT( ctrl != NULL ); 
//
//	if( filter_cond->value.type == STRING_POINTER_VALUE_TYPE )
//	{
//		if( filter_cond->value.text.string_value_ptr == NULL )
//		{
//			filter_cond->value.text.string_value_ptr = L""; 
//		}
//		ctrl->SetText( filter_cond->value.text.string_value_ptr ); 
//	}
//	else
//	{
//		ctrl->SetText( filter_cond->value.text.string_value ); 
//	}
//
//	ctrl = static_cast< CContainerUI* >( m_pm.FindSubControlByName( pListElement, L"result" ) ); 
//
//	ASSERT( ctrl != NULL ); 
//
//	desc = get_filter_result_desc( filter_cond->result ); 
//
//	ASSERT( desc != NULL ); 
//
//	ctrl->SetText( desc ); 
//
//	_ret = list->AddSubItem( pListElement, UI_LIST_CONTAINER_ELEMENT_CLASS_ID ); 
//	if( _ret == false )
//	{
//		ntstatus = STATUS_UNSUCCESSFUL; 
//	}
//}
//filter_dlg::filter_dlg( CPaintManagerUI* pManager) 
//{
//	CDialogBuilder builder;
//	CContainerUI* filter_ui; 
//	
//	do 
//	{
//		filter_ui = static_cast<CContainerUI*>(builder.Create(_T("filter_dlg.xml"), NULL, NULL, pManager )); 
//
//		if( filter_ui != NULL ) 
//		{
//			this->AddItem( filter_ui );
//		}
//		else
//		{
//			this->RemoveAllItem();
//			break; 	
//		}
//
//		//filter_infos = ( action_filter_info* )malloc( sizeof( action_filter_info ) * MAX_ACTION_FILTER_COND_COUNT ); 
//		//if( filter_infos == NULL )
//		//{
//		//	DBG_BP(); 
//		//	break; 
//		//}
//
//	}while( FALSE ); 
//}


LRESULT filter_dlg::reset_flt_cond()
{
	LRESULT ret = ERROR_SUCCESS; 

	do 
	{
		ret = delete_action_cond_from_db( NULL ); 
		
		if( ret != ERROR_SUCCESS )
		{

		}

		release_filter_conds(); 

		load_all_log_filters(); 
	}while( FALSE );

	return ret; 
}

action_filter_cond get_action_filter_cond( LPCWSTR cond )
{
	action_filter_cond _cond = FLT_PATH; 

	do 
	{
		//"可执行文件路径"
		//	"进程名"; 
		//	"命令行"; 
		//	"进程ID"; 
		//	"父进程ID"; 
		//	"线程ID"; 
		//	"主类型"; 
		//	"事件类型"; 
		//	"读/写"; 
		//	"事件描述"; 
		//	"事件细节"; 
		//	"结果"; 
		//	"时间区间"; 
		//	"日期 时间"; 
		//	"相对时间"; 
		//	"会话"; 
		//	"用户"; 
		//	"身份ID号"; 
		//	"序列号"; 
		//	"公司"; 
		//	"版本"; 
		//	"虚拟化"; 
		//	"CPU架构"; 
	}while( FALSE );

	return _cond; 
}

LRESULT WINAPI check_action_filter_valid( action_filter_info *flt_info )
{
	LRESULT ret = ERROR_SUCCESS; 

	do 
	{
		ASSERT( flt_info != NULL ); 
		//ASSERT( flt_info->value.type == STRING_VALUE_TYPE ); 

		switch( flt_info->cond )
		{
		case FLT_DURATION: 
			{
				LPWSTR tmp_text; 
				
				tmp_text = wcschr( flt_info->value.text.text_mode_value, L'-' ); 
				if( tmp_text == NULL 
					|| tmp_text == flt_info->value.text.text_mode_value
					|| tmp_text == flt_info->value.text.text_mode_value + ( flt_info->value.text.cc_string_len - 1 ) )
				{
					ret = ERROR_INVALID_PARAMETER; 
					break; 
				}
			}		
			break; 
		case FLT_PID: 
			{
				ULONG i; 
				for( i = 0; i < flt_info->value.text.cc_string_len; i ++ )
				{
					if( flt_info->value.text.text_mode_value[ i ] < L'0' 
						|| flt_info->value.text.text_mode_value[ i ] > L'9' )
					{
						ret = ERROR_INVALID_PARAMETER; 
						break; 
					}
				}
			}		
			break; 
		default: 
			//ASSERT( FALSE ); 
			break; 
		}

	}while( FALSE );

	return ret; 
}

LRESULT WINAPI delete_filter_condition( action_filter_info *filter_condition )
{
	LRESULT ret = ERROR_SUCCESS; 
	INT32 i; 

	do 
	{
		ASSERT( filter_condition != NULL ); 

		ret = remove_action_cond_from_db( filter_condition ); 
		if( ret != ERROR_SUCCESS )
		{
			//break; 
		}

		w_lock_filter_infos(); 

		for( i = 0; ( ULONG )i < filter_infos.filter_infos.size(); i ++ )
		{
			ret = is_same_filter_info( filter_infos.filter_infos[ i ], filter_condition ); 
			if( ret == TRUE )
			{
				filter_infos.filter_infos.erase( filter_infos.filter_infos.begin() + i ); 
				//break; 
			}
		}

		w_unlock_filter_infos(); 
	}while( FALSE ); 
		
	return ret; 
}

LRESULT WINAPI input_filter_condition( action_filter_info *filter_condition )
{
	LRESULT ret = ERROR_SUCCESS; 
	BOOLEAN found = FALSE; 
	INT32 i; 

	do 
	{
		ASSERT( filter_condition != NULL ); 

		ret = input_action_cond_to_db( filter_condition ); 
		if( ret != ERROR_SUCCESS )
		{
			//break; 
		}

		w_lock_filter_infos(); 

		for( i = 0; ( ULONG )i < filter_infos.filter_infos.size(); i ++ )
		{
			ret = is_same_filter_info( filter_infos.filter_infos[ i ], filter_condition ); 
			if( ret == TRUE )
			{
				found = TRUE; 
				//break; 
			}
		}

		w_unlock_filter_infos(); 

		if( TRUE == found )
		{
			break; 
		}

		{
			action_filter_info *_filter_info; 

			_filter_info = ( action_filter_info* )malloc( sizeof( action_filter_info ) ); 
			if( _filter_info == NULL )
			{
				ret = ERROR_NOT_ENOUGH_MEMORY; 
				break; 
			}

			memcpy( _filter_info, filter_condition, sizeof( *_filter_info ) ); 

			w_lock_filter_infos(); 
			filter_infos.filter_infos.push_back( _filter_info ); 
			w_unlock_filter_infos(); 
		}

	}while( FALSE );

	return ret; 
}

INT32 WINAPI is_same_filter_info( action_filter_info *src_flt_info, action_filter_info *dst_flt_info )
{
	INT32 ret = FALSE; 

	do 
	{
		if( src_flt_info->cond != dst_flt_info->cond )
		{
			break; 
		}

		if( src_flt_info->compare_mode != dst_flt_info->compare_mode )
		{
			break; 
		}

		if( src_flt_info->filter_mode != dst_flt_info->filter_mode )
		{
			break; 
		}

		if( src_flt_info->value.type != dst_flt_info->value.type )
		{
			break; 
		}

		switch( src_flt_info->value.type )
		{
		case ULONGLONG_VALUE_TYPE:
			if( src_flt_info->value.data.ulonglong_value == dst_flt_info->value.data.ulonglong_value )
			{
				ret = TRUE; 
				break; 
			}
			break; 
		case ULONG_VALUE_TYPE: 
			{
				if( src_flt_info->value.data.ulong_value == dst_flt_info->value.data.ulong_value )
				{
					ret = TRUE; 
					break; 
				}
			}
			break; 
		case INT32_VALUE_TYPE: 
			{
				if( src_flt_info->value.data.int32_value == dst_flt_info->value.data.int32_value )
				{
					ret = TRUE; 
					break; 
				}
			}
			break; 
		case TIME_VALUE_TYPE: 
			{
				if( src_flt_info->value.data.time_value.QuadPart == dst_flt_info->value.data.time_value.QuadPart )
				{
					ret = TRUE; 
					break; 
				}
			}
			break; 
		case DURATION_VALUE_TYPE: 
			{
				if( src_flt_info->value.data.duration.begin_time.QuadPart == src_flt_info->value.data.duration.begin_time.QuadPart
					&& src_flt_info->value.data.duration.end_time.QuadPart == src_flt_info->value.data.duration.end_time.QuadPart )
				{
					ret = TRUE; 
					break; 
				}
			}
			break; 
		case STRING_VALUE_TYPE: 
			{
				if( src_flt_info->value.text.text_is_ptr == FALSE )
				{
					if( 0 == wcsnicmp( src_flt_info->value.text.string_value, 
						dst_flt_info->value.text.string_value, 
						src_flt_info->value.text.cc_string_len ) )
					{
						ret = TRUE; 
						break; 
					}
				}
				else
				{
					if( 0 == wcsnicmp( src_flt_info->value.text.string_value_ptr, 
						dst_flt_info->value.text.string_value_ptr, 
						src_flt_info->value.text.cc_string_len ) )
					{
						ret = TRUE; 
						break; 
					}
				}
			}
			break; 
		default:
			break; 
		}
	}while( FALSE );

	return ret; 
}

LRESULT filter_dlg::set_flt_cond( BOOLEAN remove_item )
{
	LRESULT ret = ERROR_SUCCESS; 
	HRESULT hr; 

	CControlUI *ctrl; 
	action_filter_info flt_info; 

	CStdString flt_cond; 
	CStdString flt_comp; 
	CStdString flt_value; 
	CStdString flt_result; 

	action_filter_cond _flt_cond; 
	action_compare_mode _flt_comp; 
	//action_filter_value  _flt_value; 
	action_filter_mode _flt_result; 
	
	do 
	{
		if( remove_item == FALSE )
		{

			ctrl = m_pm.FindControl( _T( "flt_value" ) ); 

			//FindSubControlByName( this, _T( "flt_value" ) ); 
			if( ctrl == NULL )
			{
				ASSERT( FALSE ); 
				ret = ERROR_ERRORS_ENCOUNTERED; 
				break; 
			}

			flt_value = ctrl->GetText(); 

			if( flt_value.GetLength() == 0 )
			{
				show_msg( GetHWND(), _T( "请设置过滤条件值" ), NULL, _T( "输入不正确" ) ); 
				break; 
			}

			ctrl = m_pm.FindControl( _T( "flt_cond" ) ); 

			//FindSubControlByName( this, _T( "flt_cond" ) ); 
			if( ctrl == NULL )
			{
				ASSERT( FALSE ); 
				ret = ERROR_ERRORS_ENCOUNTERED; 
				break; 
			}

			flt_cond = ctrl->GetText(); 
			_flt_cond = ( action_filter_cond )( ( CComboUI*)ctrl )->GetCurSel(); 

			ctrl = m_pm.FindControl( _T( "flt_comp" ) ); 

			//FindSubControlByName( this, _T( "flt_comp" ) ); 
			if( ctrl == NULL )
			{
				ASSERT( FALSE ); 
				ret = ERROR_ERRORS_ENCOUNTERED; 
				break; 
			}

			flt_comp = ctrl->GetText(); 
			_flt_comp = ( action_compare_mode )( ( CComboUI*)ctrl )->GetCurSel(); 

			ctrl = m_pm.FindControl( _T( "flt_result" ) ); 
			//FindSubControlByName( this, _T( "flt_result" ) ); 
			if( ctrl == NULL )
			{
				ASSERT( FALSE ); 
				ret = ERROR_ERRORS_ENCOUNTERED; 
				break; 
			}

			flt_result = ctrl->GetText(); 
			_flt_result = ( action_filter_mode )( ( CComboUI*)ctrl )->GetCurSel(); 

			//_flt_result = FLT_EXCLUDE; 

			memset( &flt_info.value.data, 0, sizeof( flt_info.value.data ) ); 

			flt_info.cond = _flt_cond; 
			flt_info.compare_mode = _flt_comp; 
			flt_info.filter_mode = _flt_result; 

			hr = StringCbCopyW( flt_info.value.text.text_mode_value, 
				sizeof( flt_info.value.text.text_mode_value ), 
				flt_value.GetData() ); 
			if( FAILED( hr ) )
			{
				ret = ERROR_ERRORS_ENCOUNTERED; 
				break; 
			}

			flt_info.value.text.cc_string_len = ( ULONG )wcslen( flt_info.value.text.text_mode_value ); 
			flt_info.value.type = STRING_VALUE_TYPE; 

			ret = check_action_filter_valid( &flt_info ); 
			if( ret != ERROR_SUCCESS )
			{
				break; 
			}

			init_filter_ui_info( &flt_info.ui_info ); 

			ret = input_filter_condition( &flt_info ); 
		
			if( ret != ERROR_SUCCESS )
			{
				dbg_print( MSG_FATAL_ERROR, "input the filter condition error %u\n",ret ); 
			}
		}
		else
		{
			INT32 i; 
			action_filter_info *_filter_info; 

			ret = get_selected_filter_info( &_filter_info, TRUE ); 
			if( ret != ERROR_SUCCESS )
			{
				ASSERT( _filter_info == NULL ); 
				break; 
			}

			ASSERT( _filter_info != NULL ); 

			ret = remove_action_cond_from_db( _filter_info ); 
			if( ret != ERROR_SUCCESS )
			{
				//break; 
			}

			w_lock_filter_infos(); 

			for( i = 0; ( ULONG )i < filter_infos.filter_infos.size(); i ++ )
			{
				//ret = is_same_filter_info( &filter_infos.filter_infos[ i ], &flt_info ); 
				if( _filter_info == filter_infos.filter_infos[ i ] )
				{
					filter_infos.filter_infos.erase( filter_infos.filter_infos.begin() + i ); 

					free( _filter_info ); 
					break; 
				}
			}
			
			w_unlock_filter_infos(); 
		}

		load_all_log_filters(); 

		//{
		//	load_filter_ui_context context; 
		//	ULONG ret_state; 
		//	
		//	ctrl = m_pm.FindControl( _T( "flt_cond_list" ) ); 

		//		//FindSubControlByName( this, _T( "flt_cond_list" ) ); 
		//	ASSERT( ctrl != NULL ); 

		//	context.builder = &builder; 
		//	context.list = static_cast< CListUI* >( ctrl ); 
		//	context.pm = &m_pm; 
		//	context.flags = 0; 

		//	ret = load_action_filter_cond( 0, &flt_info, L"", ( PVOID )&context, &ret_state ); 
		//	if( ret != ERROR_SUCCESS )
		//	{

		//	}
		//}
	}while( FALSE );

	return ret; 
}

/******************************************************************************************
事件信息过滤方法:
1.将所有的信息直接输出到内存中。
优点是简单，缺点是如果输出的事件信息太多，那么对内存的占用率就会过高。
2.将所有的事件信息直接输出到一个数据库，或都其格式的中间文件中。
优点是不占用内存，如果文件格式简单，实现也比较简单。缺点是如果输出的事件太多，速度会慢。
3.内存输出与文件输出两种方式并用，当数量较小时，使用内存方式，当数量较大时，使用文件方式。

初步实现方案：
1.其它现成的开源项目
2.文件格式使用SQLITE
3.文件格式使用FOXBASE格式。


******************************************************************************************/
#define MAX_FILTER_SELECT_SQL 2048

LRESULT WINAPI construct_filter_sql( WCHAR *sql, ULONG cc_buf_len, ULONG *cc_ret_len )
{
	LRESULT ret = ERROR_SUCCESS; 

	do 
	{
		if( sql == NULL )
		{
			ret = ERROR_INVALID_PARAMETER; 
			break; 
		}

		if( cc_buf_len == 0 )
		{
			ret = ERROR_INVALID_PARAMETER; 
			break; 
		}

		if( cc_ret_len != NULL )
		{
			*cc_ret_len = 0; 
		}


	}while( FALSE );

	return ret; 
}

LRESULT filter_dlg::filter_events()
{
	LRESULT ret = ERROR_SUCCESS; 
	//WCHAR sql[ MAX_FILTER_SELECT_SQL ]; 

	do 
	{ 
		if( NULL == main_wnd )
		{
			ret = ERROR_INVALID_PARAMETER; 
			break; 
		}

		main_wnd->SendMessage( WM_ACTION_EVENT_FILTERING, NULL, NULL ); 
	}while( FALSE );

	return ret; 
}

LRESULT filter_dlg::load_all_log_filters()
{
	LRESULT ret = ERROR_SUCCESS; 
	LRESULT _ret; 
	INT32 i; 
	CListUI *list; 

	do 
	{
		list = ( CListUI* )m_pm.FindControl( L"flt_cond_list" ); 

		list->RemoveSubAllItem( UI_LIST_CONTAINER_ELEMENT_CLASS_ID ); ; 

		for( i = 0; ( ULONG )i < filter_infos.filter_infos.size(); i ++ )
		{
			_ret = load_log_filter( filter_infos.filter_infos[ i ] ); 
			if( _ret != ERROR_SUCCESS )
			{
				ret = _ret; 
			}
		}
	}while( FALSE );

	return ret; 
}

LRESULT filter_dlg::_Init()
{
	LRESULT ret = ERROR_SUCCESS; 
	//load_filter_ui_context context; 
	//CControlUI *ctrl; 

	do 
	{
		//ASSERT( m_pManager != NULL ); 

		ret = init_action_filter_list(); 
		if( ret != ERROR_SUCCESS )
		{

		}

		//ctrl = m_pm.FindControl( L"flt_cond_list" ); 

		//	//FindSubControlByName( this, L"flt_cond_list" ); 
		//ASSERT( ctrl ); 

		ret = load_all_log_filters(); 

		//context.builder = &builder; 
		//context.list = static_cast< CListUI* >( ctrl ); 
		//context.pm = &m_pm; 
		//context.flags = DATA_BY_POINTER; 
		//context.filters = &filter_infos; 

		//ret = load_all_action_cond( 0, 0, &context ); 
		//if( ret != ERROR_SUCCESS )
		//{

		//}

	}while( FALSE );

	return ret; 
}

//LRESULT WINAPI set_filter_event_cond()
//{
//	LRESULT ret = ERROR_SUCCESS; 
//	HRESULT hr; 
//
//	CControlUI *ctrl; 
//	action_filter_info flt_info; 
//
//	CStdString flt_cond; 
//	CStdString flt_comp; 
//	CStdString flt_value; 
//	CStdString flt_result; 
//
//	action_filter_cond _flt_cond; 
//	action_compare_mode _flt_comp; 
//	//action_filter_value  _flt_value; 
//	action_filter_mode _flt_result; 
//
//	do 
//	{
//		if( remove_item == FALSE )
//		{
//
//			ctrl = m_pm.FindControl( _T( "flt_value" ) ); 
//
//			//FindSubControlByName( this, _T( "flt_value" ) ); 
//			if( ctrl == NULL )
//			{
//				ASSERT( FALSE ); 
//				ret = ERROR_ERRORS_ENCOUNTERED; 
//				break; 
//			}
//
//			flt_value = ctrl->GetText(); 
//
//			if( flt_value.GetLength() == 0 )
//			{
//				show_msg( GetHWND(), _T( "请设置过滤条件值" ), NULL, _T( "输入不正确" ) ); 
//				break; 
//			}
//
//			ctrl = m_pm.FindControl( _T( "flt_cond" ) ); 
//
//			//FindSubControlByName( this, _T( "flt_cond" ) ); 
//			if( ctrl == NULL )
//			{
//				ASSERT( FALSE ); 
//				ret = ERROR_ERRORS_ENCOUNTERED; 
//				break; 
//			}
//
//			flt_cond = ctrl->GetText(); 
//			_flt_cond = ( action_filter_cond )( ( CComboUI*)ctrl )->GetCurSel(); 
//
//			ctrl = m_pm.FindControl( _T( "flt_comp" ) ); 
//
//			//FindSubControlByName( this, _T( "flt_comp" ) ); 
//			if( ctrl == NULL )
//			{
//				ASSERT( FALSE ); 
//				ret = ERROR_ERRORS_ENCOUNTERED; 
//				break; 
//			}
//
//			flt_comp = ctrl->GetText(); 
//			_flt_comp = ( action_compare_mode )( ( CComboUI*)ctrl )->GetCurSel(); 
//
//#if 0
//			ctrl = m_pm.FindControl( _T( "flt_result" ) ); 
//			//FindSubControlByName( this, _T( "flt_result" ) ); 
//			if( ctrl == NULL )
//			{
//				ASSERT( FALSE ); 
//				ret = ERROR_ERRORS_ENCOUNTERED; 
//				break; 
//			}
//
//			flt_result = ctrl->GetText(); 
//			_flt_result = ( action_filter_result )( ( CComboUI*)ctrl )->GetCurSel(); 
//#endif //0
//
//			_flt_result = FLT_EXCLUDE; 
//
//			memset( &flt_info.value.data, 0, sizeof( flt_info.value.data ) ); 
//
//			flt_info.cond = _flt_cond; 
//			flt_info.compare_mode = _flt_comp; 
//			flt_info.filter_mode = _flt_result; 
//
//			hr = StringCbCopyW( flt_info.value.text.text_mode_value, 
//				sizeof( flt_info.value.text.text_mode_value ), 
//				flt_value.GetData() ); 
//			if( FAILED( hr ) )
//			{
//				ret = ERROR_ERRORS_ENCOUNTERED; 
//				break; 
//			}
//
//			flt_info.value.text.cc_string_len = ( ULONG )wcslen( flt_info.value.text.text_mode_value ); 
//			flt_info.value.type = STRING_VALUE_TYPE; 
//
//			ret = check_action_filter_valid( &flt_info ); 
//			if( ret != ERROR_SUCCESS )
//			{
//				break; 
//			}
//
//			ret = input_action_cond_to_db( &flt_info ); 
//			if( ret != ERROR_SUCCESS )
//			{
//				//break; 
//			}
//
//			{
//				action_filter_info *_filter_info; 
//
//				_filter_info = ( action_filter_info* )malloc( sizeof( action_filter_info ) ); 
//				if( _filter_info == NULL )
//				{
//					ret = ERROR_NOT_ENOUGH_MEMORY; 
//					break; 
//				}
//
//				memcpy( _filter_info, &flt_info, sizeof( *_filter_info ) ); 
//
//				filter_infos.filter_infos.push_back( _filter_info ); 
//			}
//
//		}
//		else
//		{
//			INT32 i; 
//			action_filter_info *_filter_info; 
//
//			ret = get_selected_filter_info( &_filter_info ); 
//			if( ret != ERROR_SUCCESS )
//			{
//				ASSERT( _filter_info == NULL ); 
//				break; 
//			}
//
//			ASSERT( _filter_info != NULL ); 
//
//			ret = remove_action_cond_to_db( _filter_info ); 
//			if( ret != ERROR_SUCCESS )
//			{
//				//break; 
//			}
//
//			for( i = 0; ( ULONG )i < filter_infos.filter_infos.size(); i ++ )
//			{
//				//ret = is_same_filter_info( &filter_infos.filter_infos[ i ], &flt_info ); 
//				if( _filter_info == filter_infos.filter_infos[ i ] )
//				{
//					filter_infos.filter_infos.erase( filter_infos.filter_infos.begin() + i ); 
//
//					free( _filter_info ); 
//					break; 
//				}
//			}
//		}
//
//		load_all_log_filters(); 
//
//		//{
//		//	load_filter_ui_context context; 
//		//	ULONG ret_state; 
//		//	
//		//	ctrl = m_pm.FindControl( _T( "flt_cond_list" ) ); 
//
//		//		//FindSubControlByName( this, _T( "flt_cond_list" ) ); 
//		//	ASSERT( ctrl != NULL ); 
//
//		//	context.builder = &builder; 
//		//	context.list = static_cast< CListUI* >( ctrl ); 
//		//	context.pm = &m_pm; 
//		//	context.flags = 0; 
//
//		//	ret = load_action_filter_cond( 0, &flt_info, L"", ( PVOID )&context, &ret_state ); 
//		//	if( ret != ERROR_SUCCESS )
//		//	{
//
//		//	}
//		//}
//	}while( FALSE );
//
//	return ret; 
//}
LRESULT WINAPI show_filter_dlg( CWindowWnd *parent, ULONG flags )
{
	LRESULT ret = ERROR_SUCCESS; 
	INT32 _ret; 

	do
	{
		filter_dlg dlg( parent ); 
		POINT cursor_pos; 
		BOOLEAN pos_got = FALSE; 

		if( ( SHOW_WINDOW_CENTER & flags ) == 0 )
		{
			_ret = GetCursorPos( &cursor_pos ); 
			if( TRUE == _ret )
			{
				pos_got = TRUE; 
			}
		}

		dlg.Create( GetHWND(), _T( "filtering" ), UI_WNDSTYLE_DIALOG, 0L, 0, 0, 1024, 738);
		dlg.SetIcon( IDI_MAIN_ICON ); 
		//dlg.CenterWindow();

		if( TRUE == pos_got )
		{
			SetWindowPos( dlg.GetHWND(), NULL, cursor_pos.x, cursor_pos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER ); 
		}
		else
		{
			dlg.CenterWindow(); 
		}

		dlg.ShowModal(); 

	}while( FALSE ); 

	return ret; 
}
