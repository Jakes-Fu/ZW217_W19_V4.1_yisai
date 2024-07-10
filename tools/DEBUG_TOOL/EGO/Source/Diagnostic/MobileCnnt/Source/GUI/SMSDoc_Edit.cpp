#include "stdafx.h"
#include "MobileCnnt.h"

#include "SMSDoc.h"
#include "InfoListView.h"

#include "GlobalDef.h"


__inline CView * CSMSDoc::GetInfoListView(void)
{
	POSITION pos   = GetFirstViewPosition();
	CView  * pView = GetNextView(pos);
	for( ; pView != NULL; pView = GetNextView(pos))
	{
		if( pView->IsKindOf(RUNTIME_CLASS(CInfoListView)) )
		{
			return pView;
		}
	}

	return NULL;
}

void CSMSDoc::OnEditNew(UINT nID) 
{
	// TODO: Add your command handler code here

	CInfoListView * pView = (CInfoListView *)GetInfoListView();
	_ASSERTE( pView != NULL );

	pView->OnCommand(nID);
}

void CSMSDoc::OnEditEdit() 
{
	// TODO: Add your command handler code here
	CInfoListView * pView = (CInfoListView *)GetInfoListView();
	_ASSERTE( pView != NULL );

	pView->OnCommand(ID_EDIT_EDIT);
}

void CSMSDoc::OnUpdateEditEdit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CInfoListView * pView = (CInfoListView *)GetInfoListView();
	_ASSERTE( pView != NULL );
	

	pCmdUI->Enable( pView->IsSingleItemSelected() && (HIWORD(m_dwCateg) != ID_CATEG_PBGM));
}

void CSMSDoc::OnEditCopy() 
{
	// TODO: Add your command handler code here
	CInfoListView * pView = (CInfoListView *)GetInfoListView();
	_ASSERTE( pView != NULL );

	pView->OnCommand(ID_EDIT_COPY);
}

void CSMSDoc::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CInfoListView * pView = (CInfoListView *)GetInfoListView();
	_ASSERTE( pView != NULL );

	pCmdUI->Enable( pView->IsItemSelected() );
}

void CSMSDoc::OnEditDelete() 
{
	// TODO: Add your command handler code here
	CInfoListView * pView = (CInfoListView *)GetInfoListView();
	_ASSERTE( pView != NULL );

	pView->OnCommand(ID_EDIT_DELETE);
}

void CSMSDoc::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CInfoListView * pView = (CInfoListView *)GetInfoListView();
	_ASSERTE( pView != NULL );

	pCmdUI->Enable( pView->IsItemSelected()  &&
		(HIWORD(m_dwCateg) != ID_CATEG_PBGM &&
		HIWORD(m_dwCateg) != ID_CATEG_PBG));
}

void CSMSDoc::OnEditSelectAll() 
{
	// TODO: Add your command handler code here
	CInfoListView * pView = (CInfoListView *)GetInfoListView();
	_ASSERTE( pView != NULL );

	pView->OnCommand(ID_EDIT_SELECT_ALL);
}

void CSMSDoc::OnUpdateEditSelectAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CInfoListView * pView = (CInfoListView *)GetInfoListView();
	_ASSERTE( pView != NULL );

	pCmdUI->Enable( pView->GetItemCount() > 0 );
}

void CSMSDoc::OnEditNew() 
{
	// TODO: Add your command handler code here
	CInfoListView * pView = (CInfoListView *)GetInfoListView();
	_ASSERTE( pView != NULL );

	pView->OnNew();
}

void CSMSDoc::OnUpdateEditNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable( m_dwCateg != ID_CATEG_PBG && m_dwCateg != ID_CATEG_PBGM);
}