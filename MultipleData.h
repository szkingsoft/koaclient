#ifndef MULTIPLEDATA_GXF_20040527
#define MULTIPLEDATA_GXF_20040527

struct CMulQuantitySequence
{//��̬���ݴ�����
	int m_nSize;      //����С
	int m_nStart;     //��ʼλ��
	long m_lReference;//�ο�
	long m_lMax;      //���
	long m_lMin;      //��С
	int m_nSeqs;      //m_ppValues�����С
	long **m_ppValues;//������ָ��(����ָ��)
};
#endif