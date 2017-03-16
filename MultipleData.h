#ifndef MULTIPLEDATA_GXF_20040527
#define MULTIPLEDATA_GXF_20040527

struct CMulQuantitySequence
{//多态数据传输流
	int m_nSize;      //包大小
	int m_nStart;     //起始位置
	long m_lReference;//参考
	long m_lMax;      //最大
	long m_lMin;      //最小
	int m_nSeqs;      //m_ppValues数组大小
	long **m_ppValues;//数据流指针(数组指针)
};
#endif