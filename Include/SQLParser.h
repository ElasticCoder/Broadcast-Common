#ifndef __SQL_PARSER_H__
#define __SQL_PARSER_H__

#include <afxtempl.h>
#include <SchepTC.h>
//
// SQLParser.h
//

class CSQLParser
{
public:
	enum		SQL_STATEMENTS {SQLSelect,SQLUpdate,SQLDelete,SQLInsert,SQLCount,SQLSchUpdate,SQLSeqNext,SQLSeqCurr,SQLDeleteList, SQLDeleteTimeRange};
	enum		COMPARISON     {EQU, LT, LTE, GT, GTE, NE};
	enum		TIMECODE_TYPE  {TDB_Timecode, DB_DateTime};
public:
	void Join(const CString& Table1, const CString& Field1, const CString& Table2, const CString& Field2);
	void ClearComplexFilter();
	void		SetChannelName(CString sChan);
	void		SetEndTime(CSchepTC TimeVal);
	void		SetStartTime(CSchepTC TimeVal);
	void		ResetInsertValues();
	void		SimpleFilter(CString sSimpleFilter);
	void		AddSelectClause(CString sMoreSelect);
	void		FromTable(CString sTableName);
	void		InsertValue(CString sField, int iVal);
	void		InsertValue(CString sField, CSchepTC TimeValue, TIMECODE_TYPE p_Type = TDB_Timecode);
	void		InsertValue(CString sField, CTime TimeValue);
	void		InsertValue(CString, CString,bool bQuoted = true, bool bParse = true);
	void		SequenceName(CString);
	void		AddUpdateField(CString sField, CSchepTC TC, TIMECODE_TYPE p_Type = TDB_Timecode);
	void		AddUpdateField(CString sField, CTime sValue);
	void		AddUpdateField(CString sField, int iValue);
	void		AddUpdateField(CString sField, CString sValue);
	void		Filter(CString sField, int iCriteria);
	void		AddComplexFilter(CString );
	CString		DecorateField(CString sField);
	void		InitSelect();
	void		SelectFields(CString sField);
	void		Sort(CString sSortField);
	void		UpdateField(CString sUpdField, CString sValue);
	CString		GetSQL( SQL_STATEMENTS SQLType );
	void		Filter(CString sField, CString sCriteria);
	void		TableName(CString sTableName);
	

				CSQLParser( CString sDBName);
				~CSQLParser( );
private:
	CString			ParseForQuotes(const CString sValue);
	CString			FormatSort();
	CString			FormatFilter(bool bInclusive = true, COMPARISON = EQU);

	
	CString			m_sInsertValue;
	CString			m_sInsertField;
	CString			m_sSequenceName;
	CString			m_sSortField;
	CString			m_sDBName;
	CString			m_sTableName;
	CString			m_sFromTables;
	CString			m_sWhereClause;
	CString			m_sField;
	CString			m_sCriteria;
	CString			m_sUpdField;
	CString			m_sValue;
	CString			m_sSelectFields;
	CString			m_sSimpleFilter;
	CString			m_sStartRange;
	CString			m_sEndRange;
	CString			m_sInclusionRange;
	CString			m_sChanName;
	CString			m_sSimpleJoin;

	int				m_iStartDOY;
	int				m_iEndDOY;

	CArray<CString,CString&>m_FilterFields;
	CArray<CString,CString&>m_FilterCriteria;
	CArray<CString,CString&>m_SortCriteria;
	

	CArray<CString,CString&>m_UpdateClause;
};



#endif


