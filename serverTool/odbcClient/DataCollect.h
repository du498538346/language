#ifndef _INCLUDE_GAME_H_
#define _INCLUDE_GAME_H_
#include "CppODBC.h"
#pragma pack(1)
#include "CaiPiaoData.h"
struct ResHead
{
	unsigned int rows;    //����
	unsigned int columns; //����
	unsigned int ilength; //�������ݵĳ���
};
#define PARSE_D(tag)	if(!DataParser.GetNext(tag)){return false;}
// ��Ӧ���ݽ�����
class CResDataParser
{
private:
	// ���ݻ���
	void *m_pBuf;
	// ���ݳ���
	const size_t m_iDataLength;
	// ��Ӧͷ
	ResHead *m_pResHead;
	// ��Ӧ����
	char *m_pRespData;
	// ��ǰȡ����λ��
	size_t m_iCurrentGetDataPos;
	size_t m_iCurrentRestLen;
	// ��ǰ��
	size_t m_iCurrentRow;
	// ��ǰ��
	size_t m_iCurrentColumn;

public:
	CResDataParser(void *BufData, const size_t length)
		: m_pBuf(BufData),
		m_iDataLength(length),
		m_pResHead(NULL),
		m_pRespData(NULL),
		m_iCurrentGetDataPos(0),
		m_iCurrentRestLen(0),
		m_iCurrentRow(0),
		m_iCurrentColumn(0)
	{
	}
	//����ѹ����
	static char *ProcessSYN(int sData,int& noffset)
	{
		static char sBYTE[6]={0};
		memset(sBYTE,0,sizeof(sBYTE));
		noffset=0;
		for (int i=4;i>=0&&sData>0;i--)
		{
			noffset++;
			sBYTE[i] = sData&0x7F;//�õ���7λ
			if (i==4)
			{
				sBYTE[i]&=0x7F;  //��λ��0
			}
			else
			{
				sBYTE[i]|=0x80;  //��λ��1
			}
			sData >>= 7;  //����7λ
		}
		return sBYTE+5-noffset;
	}

	unsigned int ReadLenField(const char* szString, size_t nDataLen, size_t &nOffset)
	{
		unsigned char value;
		unsigned int len = 0;

		for(nOffset = 0; nOffset < nDataLen && nOffset < 5; nOffset++)
		{
			value = *(unsigned char*)(szString + nOffset);
			if(value < 128)
			{
				len = (len<<7)|value;
				nOffset++;
				break;
			}
			else
			{
				len = (len<<7)|(value&0x7F);
			}
		}

		return len;
	}

	bool CheckValid()
	{
		if (m_pBuf&&m_iDataLength>sizeof(ResHead))
		{
			m_pResHead = (ResHead*)m_pBuf;
			m_pRespData = (char *)(m_pResHead+1);
			m_iCurrentGetDataPos = 0;
			m_iCurrentRestLen = m_pResHead->ilength;
			m_iCurrentRow = 0;
			m_iCurrentColumn;
			return true;
		}
		return false;
	}

	bool GetNext(std::string &Column)
	{
		if (NULL == m_pResHead || NULL == m_pRespData)
		{
			return false;
		}

		size_t iRow = m_pResHead->rows;
		size_t iColumn = m_pResHead->columns;
		size_t len = 0;
		size_t offset = 0;
		char pOutBuf[128] = {0};

		if (m_iCurrentRow < iRow && m_iCurrentColumn < iColumn && m_iCurrentRestLen > 0)
		{
			len = ReadLenField(m_pRespData + m_iCurrentGetDataPos, m_iCurrentRestLen, offset);

			if(offset && len <= m_iCurrentRestLen - offset)
			{
				if(len > 0)
				{
					memcpy(pOutBuf, m_pRespData + m_iCurrentGetDataPos + offset, len);
				}
				pOutBuf[len] = '\0';
				Column = string(pOutBuf);

				m_iCurrentGetDataPos += (offset + len);
				m_iCurrentRestLen -= (offset + len);

				// ����������Ϣ
				m_iCurrentColumn++;
				if (m_iCurrentColumn == iColumn)
				{
					m_iCurrentRow++;
					m_iCurrentColumn = 0;
				}

				return true;
			}
			else 
			{
				return false;
			}
		}
		return false;
	}

	const size_t GetRecordRow()const
	{
		if (NULL != m_pResHead)
		{
			return m_pResHead->rows;
		}
		return 0;
	}

	const size_t GetRecordColumn()const
	{
		if (NULL != m_pResHead)
		{
			return m_pResHead->columns;
		}
		return 0;
	}

	bool IsEnd()
	{
		if (NULL != m_pResHead)
		{
			return true;
		}
		return GetRecordRow() == m_iCurrentRow && GetRecordColumn() == m_iCurrentColumn;
	}
};
#pragma pack()

//������ҵ��  
class CDataCollect
{
protected:
	CDataCollect();
	~CDataCollect();
public:
	//��ѯ���ݿ�  ����true��ѯ�ɹ� ����ʧ��   szSqlString��ѯ��� iLength���ݵĳ���
	bool ReadDataFromDB(char *szSqlString,unsigned int&iLength);
	unsigned int StrToUl(const string& data);
	bool StrNCpy(char *buf, const int StartPos, const int MaxSize, const string & data);
	bool StrToTime(const string& data,DateTime& date);
	double StrToDouble(const string& data);
	int StrToInt(const string& data);
	static CDataCollect* GetInstance(); 
public:
	char m_pDBDataMemory[16*1024*1024];//������ݵĻ���
	CppODBC m_ODBCHand; 
};
#endif
