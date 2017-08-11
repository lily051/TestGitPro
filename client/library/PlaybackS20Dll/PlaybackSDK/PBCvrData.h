#pragma once

// 最大文件个数.
#define MAX_FILE_NUM 3000


static size_t const SEG_HEAD_LEN = 1024;
typedef struct _seg_head
{
	INT32 cbSize; // 此结构大小
	INT32 meidaType; // 录音 or 录像？
	INT32 mediaFormat; // hikmp4 or dahuamp4 or wav or mp3
	INT32 segno; // 是原始数据的第n个片段
	INT32 begintime; // 原始数据的开始时间，这段数据可能是原始数据的一个片段
	INT32 endtime; // 原始数据的结束时间
	UINT32 headSize; // 媒体头大小
	BYTE headData[SEG_HEAD_LEN - 7 * sizeof(INT32)]; // 媒体头数据
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