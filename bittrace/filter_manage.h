/*
 *
 * Copyright 2010 JiJie Shi(weixin:AIChangeLife)
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

#ifndef __FILTER_MANAGE_H__
#define __FILTER_MANAGE_H__

#include "filter_cond_db.h"

#ifdef new
#undef new
#include <vector>
#else
#include <vector>
#endif //new

#define MAX_ACTION_FILTER_COND_COUNT 300
#define DATA_BY_POINTER 0x00000001

#define DROP_FILTERED_EVENTS 0x00000001

extern ULONG event_filtering_flags; 

ULONG set_event_filter_flags( ULONG flags ); 
ULONG get_event_filter_flags();

typedef std::vector<action_filter_info*> action_filter_info_array; 

typedef struct _action_filter_infos
{
	action_filter_info_array filter_infos; 
} action_filter_infos, *paction_filter_infos; 

extern action_filter_infos filter_infos; 


LRESULT WINAPI r_lock_filter_infos(); 
LRESULT WINAPI r_unlock_filter_infos(); 
LRESULT WINAPI w_lock_filter_infos(); 
LRESULT WINAPI w_unlock_filter_infos(); 
LRESULT WINAPI w_try_lock_filter_infos(); 

LRESULT WINAPI init_action_filter_sql( WCHAR *sql, 
									  ULONG cc_buf_len, 
									  ULONG *cc_ret_len ); 


LRESULT insert_condition_sql( WCHAR *colume_name, 
							 WCHAR *compare_op, 
							 WCHAR *filter_values  ); 

LRESULT WINAPI end_action_filter_sql( WCHAR *sql, ULONG cc_buf_len, ULONG *cc_ret_len ); 
LRESULT WINAPI do_filtered_events_cache(); 


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


LRESULT WINAPI filter_action( action_filter_info *filter_info, 
							 sys_action_record *action, 
							 action_context *ctx, 
							 PVOID data, 
							 ULONG data_size ); 

typedef struct _filter_ui_info
{
	ULONG bk_clr; 
	action_filter_cond cond; 
} filter_ui_info, *pfilter_ui_info; 

INLINE VOID WINAPI init_filter_ui_info( event_filter_ui_info *ui_info )
{
	ASSERT( ui_info != NULL ); 
	ui_info->bk_clr = INVALID_UI_FILTERED_BK_COLOR; 
}

LRESULT WINAPI filter_log_for_ui( sys_action_record *action, 
								 action_context *ctx,  
								 PVOID data, 
								 ULONG data_size, 
								 filter_ui_info *ui_info ); 

LRESULT WINAPI filter_action_log( sys_action_record *action, 
								 action_context *ctx, 
								 PVOID data, 
								 ULONG data_size ); 

LRESULT WINAPI init_filter_infos(); 
LRESULT WINAPI uninit_filter_infos(); 


typedef struct _filter_sql_context
{
	WCHAR *sql; 
	ULONG cc_buf_len; 
	ULONG *cc_ret_len; 
} filter_sql_context, *pfilter_sql_context;

#endif //__FILTER_MANAGE_H__