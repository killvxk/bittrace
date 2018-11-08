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
#ifndef __MINI_PORT_DEV_H__
#define __MINI_PORT_DEV_H__


#ifdef __cplusplus
extern "C" 
{
#endif //__cplusplus

HRESULT switch_mini_port_func( HANDLE mini_port, ULONG sw_id, BYTE state ); 


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__MINI_PORT_DEV_H__