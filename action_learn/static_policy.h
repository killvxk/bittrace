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
 #ifndef __STATIC_POLICY_H__
#define __STATIC_POLICY_H__

NTSTATUS check_thrd_create( action_context *ctx, 
						   thrd_create *do_thrd_create, 
						   action_response_type *resp ); 

NTSTATUS init_static_policy_dispatch(); 


#endif //__STATIC_POLICY_H__