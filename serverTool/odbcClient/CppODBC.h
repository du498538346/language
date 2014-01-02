
#ifndef	_CPP_ODBC_H_
#define	_CPP_ODBC_H_

#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#include <string>
using namespace std;

//��ѯ������ֶ�����
#define FIELD_NUM 1024
#define FIELD_MAX_LEN 36

class CppODBC
{
public:
	CppODBC( );
	virtual ~CppODBC( );
//�����ӿ�
public:
	bool		Open( );
	bool		Close( );
	//�������������ݿ�
	bool 		Connect( const char* pszDSN, const char* pszUName, const char* pszUPasswd );
	bool 		DisConnect( );
	bool 		Clear( );
	int			SetFieldMaxLen( int nMaxLen );
	unsigned int 	SQLQuery( const char* pszSQL );
	unsigned int	SQLExec( const char* pszSQL );
	bool 	SQLExecAutoID( const char *pszSQL, const char* pIdField, long &lIdentity );
	bool	IsOpen( );
	//��ѯ�Ľ������������ʱ���ظ��µļ�¼������ɾ��ʱ����ɾ��������
	unsigned int  	GetCount( );
	//���ز�ѯ���������
	unsigned int  	GetColumns( );
	int  			GetIntValue( unsigned int uiIndex );
	double 			GetDoubleValue( unsigned int uiIndex );
	char *   		GetStrValue( unsigned int uiIndex );
	//ȡ������
   bool     		Cancel( );
	//��ȡ�������
	unsigned int  	GetError( );
	//��һ��
	bool     		Next( );
	bool			Eof( );
	
	void 			Lock();
	void 			UnLock();

private:
	SQLHENV			 	V_OD_Env_;				// Handle ODBC environment ��Ż�������
	SQLHDBC			 	V_OD_hdbc_;				// Handle connection ���Ӿ��
	SQLHSTMT 		 	V_OD_hstmt_;			// SQL���ľ��
	SQLINTEGER		 	V_OD_rowanz_;			// ����Ӱ��ļ�¼����
	SQLSMALLINT			V_OD_colanz_;			// ����Ӱ��ļ�¼�������ֶ�����
	char*				pszField_[FIELD_NUM];	// ���һ����ѯ�����,���������ݲ�ѯ�������
	int				    nMaxFiledLen_;				//�ֶε����ֵ
	bool 				bOpened_;
	bool				bConnected_;
	bool 				bEof_;
	int				errorNO;		//������
};

#endif


