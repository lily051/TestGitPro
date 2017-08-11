#pragma once

// ����ļ�����.
#define MAX_FILE_NUM 3000


static size_t const SEG_HEAD_LEN = 1024;
typedef struct _seg_head
{
	INT32 cbSize; // �˽ṹ��С
	INT32 meidaType; // ¼�� or ¼��
	INT32 mediaFormat; // hikmp4 or dahuamp4 or wav or mp3
	INT32 segno; // ��ԭʼ���ݵĵ�n��Ƭ��
	INT32 begintime; // ԭʼ���ݵĿ�ʼʱ�䣬������ݿ�����ԭʼ���ݵ�һ��Ƭ��
	INT32 endtime; // ԭʼ���ݵĽ���ʱ��
	UINT32 headSize; // ý��ͷ��С
	BYTE headData[SEG_HEAD_LEN - 7 * sizeof(INT32)]; // ý��ͷ����
	/*_seg_head(const FileTrans_t &filetrans)
	{
		cbSize = sizeof(*this);
		meidaType = filetrans.mediaType;
		mediaFormat = filetrans.mediaFormat;
		segno = filetrans.segno;
		begintime = filetrans.oribegintime;
		endtime = filetrans.oriendtime;
		headSize = filetrans.headSize;
		memset(headData, 0, sizeof(headData));
	}*/
}seg_head_t;