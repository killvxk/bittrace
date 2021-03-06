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
 
 #pragma once 

//#include "menu_ui.h"
#include "conf_file.h"

extern vector< fw_log > fw_logs; 

#define FW_EVENT_APP _T( "应用程序" )

INT32 CALLBACK load_fw_log( fw_log *log, PVOID param ); 
LRESULT CALLBACK alloc_fw_log( PVOID *log, PVOID param ); 

class fw_event_dlg : public CWindowWnd, public INotifyUI, public IListCallbackUI
{
public:
	fw_event_dlg() : log_list( NULL )
	{
    };

    LPCTSTR GetWindowClassName() const 
    { 
        return _T("FwEventDlg"); 
    };

    UINT GetClassStyle() const
    { 
        return CS_DBLCLKS; 
    };

    void OnFinalMessage(HWND /*hWnd*/) 
    { 
    };

	void add_arp_info()
	{
	}

    void Init() 
    {
		INT32 ret; 
		INT32 i; 

        m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("close_btn")));
		ASSERT( m_pCloseBtn != NULL ); 
        m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("max_btn")));
        ASSERT( m_pMaxBtn != NULL ); 
		m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("nor_btn")));
        ASSERT( m_pRestoreBtn != NULL ); 
		m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("min_btn")));
		ASSERT( m_pMinBtn != NULL ); 

		log_list = static_cast<CListUI*>( m_pm.FindControl( _T( "fw_log_list" ) ) );
		ASSERT( log_list != NULL ); 
		log_list->RemoveSubAllItem( UI_LIST_CONTAINER_ELEMENT_CLASS_ID );
		log_list->SetTextCallback( this ); 

		ret = get_fw_log( load_fw_log, alloc_fw_log, &fw_logs ); 
		if( ret != 0 )
		{
			__Trace( _T( "load the firewall log failed \n" ) ); 
		}

		for( i = 0; i < fw_logs.size(); i ++ )
		{
			add_list_item( log_list, ( ULONG )i ); 
		}

	}

    void OnPrepare(TNotifyUI& msg) 
    {

    }

    /*
    * 关键的回调函数，IListCallbackUI 中的一个虚函数，渲染时候会调用,在[1]中设置了回调对象
    */
    LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
	{
		TCHAR szBuf[ ITEM_TEXT_BUF_LEN ] = {0};

		switch (iSubItem)
		{
		case 0:
			{
				_sntprintf( szBuf, ITEM_TEXT_BUF_LEN, fw_logs[iIndex].proc_name );
			}
			break;
		case 1:
			{
				_sntprintf( szBuf, ITEM_TEXT_BUF_LEN, _T( "%s" ), get_sys_action_desc( fw_logs[iIndex].type ) );
			}
			break; 
		case 2:
			{
				_sntprintf( szBuf, ITEM_TEXT_BUF_LEN, _T( "%s" ), get_protocol_text( fw_logs[iIndex].prot ) );
			}
			break; 
		case 3:
			{
				ip_addr_2_str( fw_logs[iIndex].dest_ip, szBuf, ITEM_TEXT_BUF_LEN );
			}
			break; 
		case 4:
			{
				port_2_str(  fw_logs[iIndex].dest_port, szBuf, ITEM_TEXT_BUF_LEN ); 
			}
			break; 
		case 5:
			{
				ip_addr_2_str( fw_logs[iIndex].src_ip, szBuf, ITEM_TEXT_BUF_LEN );
			}
			break; 
		case 6:
			{
				port_2_str(  fw_logs[iIndex].src_port, szBuf, ITEM_TEXT_BUF_LEN ); 
			}
			break; 
		case 7:
			{
				_sntprintf( szBuf, ITEM_TEXT_BUF_LEN, _T( "%s" ), get_response_desc( fw_logs[iIndex].action ) );
			}
			break; 
		case 8:
			{
				_sntprintf( szBuf, ITEM_TEXT_BUF_LEN, _T( "%s" ), fw_logs[iIndex].desc );
			}
			break; 
		case 9:
			{
				_sntprintf( szBuf, ITEM_TEXT_BUF_LEN, _T( "%s" ), _wctime( &fw_logs[iIndex].time ) );
			}
			break; 
		default:
			ASSERT( FALSE && "invalid list column" ); 
			break; 
		}

        pControl->SetUserData(szBuf);
        return pControl->GetUserData();
    }

	VOID remove_list_item( INT32 index )
	{
		ASSERT( index >= 0 ); 
		fw_logs.erase( fw_logs.begin() + index ); 
	}

//	VOID set_list_item( 
//		INT32 index, 
//		LPCTSTR proc_info, 
//		ACTION_TYPE action, 
//		PROT_TYPE protocol, 
//		LPCTSTR dest_desc, 
//		USHORT dest_port, 
//		LPCTSTR src_desc, 
//		USHORT src_port, 
//		time_t time
//		)
//	{
//#define TMP_BUF_LEN 512
//		TCHAR tmp_buf[ TMP_BUF_LEN ]; 
//
//		ASSERT( index >= 0 ); 
//		ASSERT( action < MAX_ACTION ); 
//		ASSERT( protocol < MAX_PROT_TYPE ); 
//
//		if( ( ULONG )index >= fw_logs.fw_src_desc.size() )
//		{
//			fw_logs.fw_proc_info.push_back( proc_info );  
//			fw_logs.fw_action.push_back( action ); 
//			fw_logs.fw_protocol.push_back( protocol ); 
//			fw_logs.fw_dest_desc.push_back( dest_desc ); 
//			_sntprintf( tmp_buf, TMP_BUF_LEN, _T( "%d" ), dest_port ); 
//			fw_logs.fw_dest_port.push_back( tmp_buf );  
//			fw_logs.fw_src_desc.push_back( src_desc ); 
//			_sntprintf( tmp_buf, TMP_BUF_LEN, _T( "%d" ), src_port ); 
//			fw_logs.fw_src_port.push_back( tmp_buf );  ; 
//			fw_logs.fw_event_time.push_back( time ); 
//		}
//		else
//		{
//			fw_logs.fw_proc_info[ index ] = proc_info;  
//			fw_logs.fw_action[ index ] = action_descs[ action ]; 
//			fw_logs.fw_protocol[ index ] = prot_descs[ protocol ]; 
//			fw_logs.fw_dest_desc[ index ] = dest_desc; 
//			_sntprintf( tmp_buf, TMP_BUF_LEN, _T( "%d" ), dest_port ); 
//			fw_logs.fw_dest_port.push_back( tmp_buf );  
//			fw_logs.fw_src_desc[ index ] = src_desc; 
//			_sntprintf( tmp_buf, TMP_BUF_LEN, _T( "%d" ), src_port ); 
//			fw_logs.fw_src_port.push_back( tmp_buf );  ; 
//			fw_logs.fw_event_time[ index ] = time; 
//		}
//	}

    void Notify(TNotifyUI& msg)
    {
        if( msg.sType == _T("windowinit") ) 
            OnPrepare(msg);
        else if( msg.sType == _T("click") ) 
        {
            if( msg.pSender == m_pCloseBtn ) 
            {
				Close(); 
                return; 
            }
            else if( msg.pSender == m_pMinBtn ) 
            { 
                SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
                return; 
            }
            else if( msg.pSender == m_pMaxBtn ) 
            { 
                //SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; 
            }
            else if( msg.pSender == m_pRestoreBtn ) 
            { 
                SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; 
            }
        }
        else if(msg.sType==_T("setfocus"))
        {
        }
        else if( msg.sType == _T("itemclick") ) 
        {
        }
        else if( msg.sType == _T("itemactivate") ) 
        {
        }
  //      else if(msg.sType == _T("menu")) 
  //      {
  //          if( msg.pSender->GetName() != _T("domainlist") ) return;
  //          menu_ui* pMenu = new menu_ui();
  //          if( pMenu == NULL ) { return; }
  //          POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
  //          ::ClientToScreen(*this, &pt);
  //          pMenu->Init(msg.pSender, pt);
  //      }
  //      else if( msg.sType == _T("menu_Delete") ) {
  //          CListUI* pList = static_cast<CListUI*>(msg.pSender);
  //          int nSel = pList->GetCurSel();
  //          if( nSel < 0 ) return;
  //          pList->RemoveAt(nSel);
		//	
		//	remove_list_item( nSel ); 
		//}
    }

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
        styleValue &= ~WS_CAPTION;
        ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
        m_pm.Init(m_hWnd);
        //m_pm.SetTransparent(100);
        CDialogBuilder builder;
        CControlUI* pRoot = builder.Create(_T("fw_event.xml"), (UINT)0, NULL, &m_pm);
        ASSERT(pRoot && "Failed to parse XML");
        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);
		set_ui_style( &m_pm ); 

		Init(); 
        return 0;
    }

    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if( ::IsIconic(*this) ) bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
    }

    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
        ::ScreenToClient(*this, &pt);

        RECT rcClient;
        ::GetClientRect(*this, &rcClient);

        if( !::IsZoomed(*this) ) {
            RECT rcSizeBox = m_pm.GetSizeBox();
            if( pt.y < rcClient.top + rcSizeBox.top ) {
                if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
                if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
                return HTTOP;
            }
            else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
                if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
                if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
                return HTBOTTOM;
            }
            if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
            if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
        }

        RECT rcCaption = m_pm.GetCaptionRect();
        if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
            && pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
                if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
                    _tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
                    _tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
                    return HTCAPTION;
        }

        return HTCLIENT;
    }

    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        SIZE szRoundCorner = m_pm.GetRoundCorner();
        if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
            CRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++; rcWnd.bottom++;
            RECT rc = { rcWnd.left, rcWnd.top + szRoundCorner.cx, rcWnd.right, rcWnd.bottom };
            HRGN hRgn1 = ::CreateRectRgnIndirect( &rc );
            HRGN hRgn2 = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom - szRoundCorner.cx, szRoundCorner.cx, szRoundCorner.cy);
            ::CombineRgn( hRgn1, hRgn1, hRgn2, RGN_OR );
            ::SetWindowRgn(*this, hRgn1, TRUE);
            ::DeleteObject(hRgn1);
            ::DeleteObject(hRgn2);
        }

        bHandled = FALSE;
        return 0;
    }

    LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
        CRect rcWork = oMonitor.rcWork;
        rcWork.Offset(-rcWork.left, -rcWork.top);

        LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
        lpMMI->ptMaxPosition.x	= rcWork.left;
        lpMMI->ptMaxPosition.y	= rcWork.top;
        lpMMI->ptMaxSize.x		= rcWork.right;
        lpMMI->ptMaxSize.y		= rcWork.bottom;

        bHandled = FALSE;
        return 0;
    }

    LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        // 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
        if( wParam == SC_CLOSE ) {
            ::PostQuitMessage(0L);
            bHandled = TRUE;
            return 0;
        }
        BOOL bZoomed = ::IsZoomed(*this);
        LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
        if( ::IsZoomed(*this) != bZoomed ) {
            if( !bZoomed ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("max_btn")));
                if( pControl ) pControl->SetVisible(false);
                pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("nor_btn")));
                if( pControl ) pControl->SetVisible(true);
            }
            else {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("max_btn")));
                if( pControl ) pControl->SetVisible(true);
                pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("nor_btn")));
                if( pControl ) pControl->SetVisible(false);
            }
        }
        return lRes;
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch( uMsg ) {
            case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
            case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
            case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
            case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
            case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
            case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
            case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
            case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
            case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
            case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
            default:
                bHandled = FALSE;
        }
        if( bHandled ) return lRes;
        if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }
public:
    CPaintManagerUI m_pm;

private:
    CButtonUI* m_pCloseBtn;
    CButtonUI* m_pMaxBtn;
    CButtonUI* m_pRestoreBtn;
    CButtonUI* m_pMinBtn;

	CListUI *log_list; 
};