//////////////////////////////////////////////////////////////////////

// CameraDS.cpp: implementation of the CCameraDS class.
//
//////////////////////////////////////////////////////////////////////

#include "CameraDS.h"
#include "OSD.h"
#include "hlogConfig.h"
#include<time.h>
#include<iostream>
#include<stdexcept>

using namespace std;

CCameraDS* CCameraDS::m_instance = NULL;

unsigned int audiosamplerate = 44100;    //音频采样率
unsigned int videoframetype =  0;    //视频帧类型
unsigned int framerate = 30;

typedef struct _HIK_MEDIAINFO_    //海康数据头
{
    unsigned int    media_fourcc;   // "HKMI": 0x484B4D49 Hikvision Media Information
    unsigned short  media_version;   // 版本号：指本信息结构版本号，目前为0x0101,即1.01版本，01：主版本号；01：子版本号。
    unsigned short  device_id;    // 设备ID，便于跟踪/分析   

    unsigned short  system_format;          // 系统封装层 0表示没有封装，0x0002 ps封装  0x0004 rtp封装
                                            //0x0007AVI封装0x0005MP4文件层，兼容3GP文件层 0x0003 TS封装
    unsigned short  video_format;           // 视频编码类型 0x0100 标准H.264 0x0003 标准MPEG4; 

    unsigned short  audio_format;           // 音频编码类型
    unsigned char   audio_channels;         // 通道数，1：单通道；2：双通道  
    unsigned char   audio_bits_per_sample;  // 样位率
    unsigned int    audio_samplesrate;      // 采样率 
    unsigned int    audio_bitrate;          // 压缩音频码率,单位：bit
    unsigned int    reserved[4];            // 保留
}HIK_MEDIAINFO;
//#define AUDIO_G711_U		0x7110
//#define AUDIO_G711_A		0x7111
//#define AUDIO_G722_1		0x7221
//#define AUDIO_G723_1        0x7231
//#define AUDIO_G726_32       0x7260  //默认G726-32
//#define AUDIO_G726_16       0x7262      
//#define AUDIO_AAC               0x2001 // MPEG2-AAC-ADTS
//#define AUDIO_MPEG               0x2000 // MPEG2-AAC-ADTS
//#define AUDIO_RAW_UDATA16(pcm) 0x7001      //采样率为16k的原始数据，即L16

enum TransMode
{
    TD_H264 = 0,		//H264编码
    TD_MPEG4 =1,	//MPEG4编码
};
#define TS_PACKET_HEADER               4
#define TS_PACKET_SIZE                 188
#define TS_SYNC_BYTE                   0x47
#define TS_PAT_PID                     0x00
#define TS_PMT_PID                     0xFFF
#define TS_H264_PID                    0x100
#define TS_AAC_PID                     0x101
#define TS_H264_STREAM_ID              0xE0
#define TS_AAC_STREAM_ID               0xC0
#define PMT_STREAM_TYPE_VIDEO          0x1B
#define PMT_STREAM_TYPE_AUDIO          0x0F
#define MAX_ONE_FRAME_SIZE             1024 * 1024

//ts 包头
typedef struct Tag_PacketHeader
{
    unsigned char sync_byte         :8  ;         //同步字节, 固定为0x47,表示后面的是一个TS分组
    unsigned char tras_error        :1  ;         //传输误码指示符   
    unsigned char play_init         :1  ;         //有效荷载单元起始指示符
    unsigned char tras_prio	        :1  ;         //传输优先, 1表示高优先级,传输机制可能用到，解码用不着
    unsigned int  PID               :13 ;         //PID
    unsigned char tras_scramb       :2  ;         //传输加扰控制
    unsigned char ada_field_C       :2  ;         //自适应控制 01仅含有效负载，10仅含调整字段，11含有调整字段和有效负载，先调整字段然后有效负载。为00解码器不进行处理 
    unsigned char conti_cter        :4  ;         //连续计数器 一个4bit的计数器，范围0-15
}TsPacketHeader; 

//PAT结构体：节目相关表
typedef struct Tag_TsPat
{
    unsigned char table_id :8 ;                  //固定为0x00 ，标志是该表是PAT
    unsigned char section_syntax_indicator: 1;   //段语法标志位，固定为1
    unsigned char zero : 1;                      //0 
    unsigned char reserved_1 : 2;                //保留位
    unsigned int section_length : 12 ;           //表示这个字节后面有用的字节数，包括CRC32
    unsigned int transport_stream_id : 16 ;      //该传输流的ID，区别于一个网络中其它多路复用的流
    unsigned char reserved_2 : 2;                //保留位
    unsigned char version_number : 5 ;           //范围0-31，表示PAT的版本号
    unsigned char current_next_indicator : 1 ;   //发送的PAT是当前有效还是下一个PAT有效
    unsigned char section_number : 8 ;           //分段的号码。PAT可能分为多段传输，第一段为00，以后每个分段加1，最多可能有256个分段
    unsigned char last_section_number : 8 ;      //最后一个分段的号码
    unsigned int program_number  : 16 ;          //节目号
    unsigned char reserved_3  : 3  ;             //保留位
    unsigned int network_PID :13 ;               //网络信息表（NIT）的PID,节目号为0时对应的PID为network_PID,本例中不含有 networke_pid
    unsigned int program_map_PID : 13;           //节目映射表的PID，节目号大于0时对应的PID，每个节目对应一个
    unsigned int CRC_32  : 32;             //CRC32校验码
}TsPat; 

//PMT结构体：节目映射表
typedef struct Tag_TsPmt
{
    unsigned char table_id  : 8;                 //固定为0x02, 表示PMT表
    unsigned char section_syntax_indicator : 1;  //固定为0x01
    unsigned char zero: 1;                       //0x00
    unsigned char reserved_1 : 2;                //0x03
    unsigned int section_length: 12;             //首先两位bit置为00，它指示段的byte数，由段长度域开始，包含CRC。
    unsigned int program_number : 16;            // 指出该节目对应于可应用的Program map PID
    unsigned char reserved_2: 2;                 //0x03
    unsigned char version_number: 5;             //指出TS流中Program map section的版本号
    unsigned char current_next_indicator: 1;     //当该位置1时，当前传送的Program map section可用；当该位置0时，指示当前传送的Program map section不可用，下一个TS流的Program map section有效。
    unsigned char section_number : 8;            //固定为0x00
    unsigned char last_section_number: 8;        //固定为0x00
    unsigned char reserved_3 : 3;                //0x07
    unsigned int PCR_PID : 13;                   //指明TS包的PID值，该TS包含有PCR域，该PCR值对应于由节目号指定的对应节目。如果对于私有数据流的节目定义与PCR无关，这个域的值将为0x1FFF。
    unsigned char reserved_4 : 4;                //预留为0x0F
    unsigned int program_info_length : 12;       //前两位bit为00。该域指出跟随其后对节目信息的描述的byte数。
    unsigned char stream_type_video : 8;         //指示特定PID的节目元素包的类型。该处PID由elementary PID指定
    unsigned char reserved_5_video: 3;           //0x07
    unsigned int elementary_PID_video: 13;       //该域指示TS包的PID值。这些TS包含有相关的节目元素
    unsigned char reserved_6_video : 4;          //0x0F
    unsigned int ES_info_length_video : 12;      //前两位bit为00。该域指示跟随其后的描述相关节目元素的byte数
    unsigned char stream_type_audio : 8;         //指示特定PID的节目元素包的类型。该处PID由elementary PID指定
    unsigned char reserved_5_audio: 3;           //0x07
    unsigned int elementary_PID_audio: 13;       //该域指示TS包的PID值。这些TS包含有相关的节目元素
    unsigned char reserved_6_audio : 4;          //0x0F
    unsigned int ES_info_length_audio : 12;      //前两位bit为00。该域指示跟随其后的描述相关节目元素的byte数
    unsigned long long CRC_32: 32;               //CRC32校验码
}TsPmt; 

//连续性计数器,也就是说 有多少个 pat包，几个pmt包 ，几个MP3 包，几个 h264包，0x00 - 0x0f ，然后折回到0x00 用于查看丢包                            
typedef struct Tag_Continuity_Counter
{
    unsigned char continuity_counter_pat;
    unsigned char continuity_counter_pmt;
    unsigned char continuity_counter_video;
    unsigned char continuity_counter_audio;
}Continuity_Counter;

//自适应段标志
typedef struct Tag_Ts_Adaptation_field
{
    unsigned char discontinuty_indicator:1;                //1表明当前传送流分组的不连续状态为真
    unsigned char random_access_indicator:1;               //表明下一个有相同PID的PES分组应该含有PTS字段和一个原始流访问点
    unsigned char elementary_stream_priority_indicator:1;  //优先级
    unsigned char PCR_flag:1;                              //包含pcr字段
    unsigned char OPCR_flag:1;                             //包含opcr字段
    unsigned char splicing_point_flag:1;                   //拼接点标志       
    unsigned char transport_private_data_flag:1;           //私用字节
    unsigned char adaptation_field_extension_flag:1;       //调整字段有扩展

    unsigned char adaptation_field_length;                 //自适应段长度
    unsigned long long  pcr;                               //自适应段中用到的的pcr
    unsigned long long  opcr;                              //自适应段中用到的的opcr
    unsigned char splice_countdown;
    unsigned char private_data_len;
    unsigned char private_data [256];
}Ts_Adaptation_field;

//PTS_DTS结构体：本程序设置都有 “11”
typedef struct Tag_TsPtsDts
{
    unsigned char reserved_1 : 4;
    unsigned char pts_32_30  : 3;                //显示时间戳
    unsigned char marker_bit1: 1;
    unsigned int  pts_29_15 : 15;
    unsigned char marker_bit2 : 1;
    unsigned int  pts_14_0 : 15;
    unsigned char marker_bit3 :1 ;
    unsigned char reserved_2 : 4;
    unsigned char dts_32_30: 3;                  //解码时间戳
    unsigned char marker_bit4 :1;
    unsigned int  dts_29_15 :15;
    unsigned char marker_bit5: 1;
    unsigned int  dts_14_0 :15;
    unsigned char marker_bit6 :1 ;
}TsPtsDts;

//PES包结构体，包括PES包头和ES数据 ,头 19 个字节
typedef struct Tag_TsPes
{
    unsigned int   packet_start_code_prefix : 24;//起始：0x000001
    unsigned char  stream_id : 8;                //基本流的类型和编号
    unsigned int   PES_packet_length : 16;       //包长度,就是帧数据的长度，可能为0,要自己算,做多16位，如果超出则需要自己算
    unsigned char  marker_bit:2;                 //必须是：'10'
    unsigned char  PES_scrambling_control:2;     //pes包有效载荷的加扰方式
    unsigned char  PES_priority:1;               //有效负载的优先级
    unsigned char  data_alignment_indicator:1;   //如果设置为1表明PES包的头后面紧跟着视频或音频syncword开始的代码。
    unsigned char  copyright:1;                  //1:靠版权保护，0：不靠
    unsigned char  original_or_copy:1;           //1;有效负载是原始的，0：有效负载时拷贝的
    unsigned char  PTS_DTS_flags:2;              //'10'：PTS字段存在，‘11’：PTD和DTS都存在，‘00’：都没有，‘01’：禁用。
    unsigned char  ESCR_flag:1;                  //1:escr基准字段 和 escr扩展字段均存在，0：无任何escr字段存在
    unsigned char  ES_rate_flag:1;               //1:es_rate字段存在，0 ：不存在
    unsigned char  DSM_trick_mode_flag:1;        //1;8比特特接方式字段存在，0 ：不存在
    unsigned char  additional_copy_info_flag:1;  //1:additional_copy_info存在，0: 不存在
    unsigned char  PES_CRC_flag:1;               //1:crc字段存在，0：不存在
    unsigned char  PES_extension_flag:1;         //1：扩展字段存在，0：不存在
    unsigned char  PES_header_data_length :8;    //后面数据的长度，
    TsPtsDts       tsptsdts;                     //ptsdts结构体对象，10个字节
    unsigned char  Es[MAX_ONE_FRAME_SIZE];       //一帧 原始数据
    int   Pes_Packet_Length_Beyond;     //如果PES_packet_length的大小不能满足一帧数据的长度则用这个代替
}TsPes;

//ADTS 头中相对有用的信息 采样率、声道数、帧长度
//adts头
typedef struct
{
	unsigned int syncword;  //12 bslbf 同步字The bit string ‘1111 1111 1111’，说明一个ADTS帧的开始
	unsigned int id;        //1 bslbf   MPEG 标示符, 设置为1
	unsigned int layer;     //2 uimsbf Indicates which layer is used. Set to ‘00’
	unsigned int protection_absent;  //1 bslbf  表示是否误码校验
	unsigned int profile;            //2 uimsbf  表示使用哪个级别的AAC，如01 Low Complexity(LC)--- AACLC
	unsigned int sf_index;           //4 uimsbf  表示使用的采样率下标
	unsigned int private_bit;        //1 bslbf 
	unsigned int channel_configuration;  //3 uimsbf  表示声道数
	unsigned int original;               //1 bslbf 
	unsigned int home;                   //1 bslbf 
	/*下面的为改变的参数即每一帧都不同*/
	unsigned int copyright_identification_bit;   //1 bslbf 
	unsigned int copyright_identification_start; //1 bslbf
	unsigned int aac_frame_length;               // 13 bslbf  一个ADTS帧的长度包括ADTS头和raw data block
	unsigned int adts_buffer_fullness;           //11 bslbf     0x7FF 说明是码率可变的码流
	/*no_raw_data_blocks_in_frame 表示ADTS帧中有number_of_raw_data_blocks_in_frame + 1个AAC原始帧.
	所以说number_of_raw_data_blocks_in_frame == 0 
	表示说ADTS帧中有一个AAC数据块并不是说没有。(一个AAC原始帧包含一段时间内1024个采样及相关数据)
    */
	unsigned int no_raw_data_blocks_in_frame;    //2 uimsfb
} ADTS_HEADER;

#define BSWAP16C(x) (((x) << 8 & 0xff00)  | ((x) >> 8 & 0x00ff))
#define BSWAP32C(x) (BSWAP16C(x) << 16 | BSWAP16C((x) >> 16))
#define BSWAP64C(x) (BSWAP32C(x) << 32 | BSWAP32C((x) >> 32))

static unsigned int crc_tab[256] = {
    0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b,
    0x1a864db2, 0x1e475005, 0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
    0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd, 0x4c11db70, 0x48d0c6c7,
    0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
    0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3,
    0x709f7b7a, 0x745e66cd, 0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
    0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5, 0xbe2b5b58, 0xbaea46ef,
    0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
    0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb,
    0xceb42022, 0xca753d95, 0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
    0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d, 0x34867077, 0x30476dc0,
    0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
    0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4,
    0x0808d07d, 0x0cc9cdca, 0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
    0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02, 0x5e9f46bf, 0x5a5e5b08,
    0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
    0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc,
    0xb6238b25, 0xb2e29692, 0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
    0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a, 0xe0b41de7, 0xe4750050,
    0xe9362689, 0xedf73b3e, 0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
    0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34,
    0xdc3abded, 0xd8fba05a, 0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
    0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb, 0x4f040d56, 0x4bc510e1,
    0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
    0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5,
    0x3f9b762c, 0x3b5a6b9b, 0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
    0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623, 0xf12f560e, 0xf5ee4bb9,
    0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
    0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f, 0xc423cd6a, 0xc0e2d0dd,
    0xcda1f604, 0xc960ebb3, 0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
    0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b, 0x9b3660c6, 0x9ff77d71,
    0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
    0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2,
    0x470cdd2b, 0x43cdc09c, 0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
    0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24, 0x119b4be9, 0x155a565e,
    0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
    0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a,
    0x2d15ebe3, 0x29d4f654, 0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
    0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c, 0xe3a1cbc1, 0xe760d676,
    0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
    0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662,
    0x933eb0bb, 0x97ffad0c, 0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
    0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};

#define  MAX_VIDEO_TAG_BUF_SIZE   1024 * 1024
#define  VIDEO_TAG_HEADER_LENGTH  11


//H264一帧数据的结构体
typedef struct Tag_NALU_t
{
    unsigned char forbidden_bit;           //! Should always be FALSE
    unsigned char nal_reference_idc;       //! NALU_PRIORITY_xxxx
    unsigned char nal_unit_type;           //! NALU_TYPE_xxxx  
    unsigned int  startcodeprefix_len;      //! 前缀字节数
    unsigned int  len;                     //! 包含nal 头的nal 长度，从第一个00000001到下一个000000001的长度
    unsigned int  max_size;                //! 做多一个nal 的长度
    unsigned char * buf;                   //! 包含nal 头的nal 数据
    unsigned char Frametype;               //! 帧类型
    unsigned int  lost_packets;            //! 预留
} NALU_t;

//nal类型
enum nal_unit_type_e
{
    NAL_UNKNOWN     = 0,
    NAL_SLICE       = 1,
    NAL_SLICE_DPA   = 2,
    NAL_SLICE_DPB   = 3,
    NAL_SLICE_DPC   = 4,
    NAL_SLICE_IDR   = 5,    /* ref_idc != 0 */
    NAL_SEI         = 6,    /* ref_idc == 0 */
    NAL_SPS         = 7,
    NAL_PPS         = 8
    /* ref_idc == 0 for 6,9,10,11,12 */
};

//帧类型
enum Frametype_e
{
    FRAME_I  = 15,
    FRAME_P  = 16,
    FRAME_B  = 17
};

//读取字节结构体
typedef struct Tag_bs_t
{
    unsigned char *p_start;	               //缓冲区首地址(这个开始是最低地址)
    unsigned char *p;			           //缓冲区当前的读写指针 当前字节的地址，这个会不断的++，每次++，进入一个新的字节
    unsigned char *p_end;		           //缓冲区尾地址		//typedef unsigned char   uint8_t;
    int     i_left;				           // p所指字节当前还有多少 “位” 可读写 count number of available(可用的)位 
}bs_t;	

typedef   HRESULT    (_stdcall *Func) (REFCLSID , REFIID , LPVOID*);  
Func   g_DllGetClassObject=NULL;

unsigned char m_One_Frame_Buf[MAX_ONE_FRAME_SIZE];
TsPes m_video_tspes;
TsPes m_audio_tspes;
unsigned int WritePacketNum = 0;

Ts_Adaptation_field  ts_adaptation_field_Head ; 
Ts_Adaptation_field  ts_adaptation_field_Tail ;

void *g_phTrans = NULL;
int Detach_Head_Aac(ADTS_HEADER * adtsheader,unsigned char *Adts_Headr_Buf)
{
    //unsigned int readsize = 0;
    //readsize = ReadFile(pAudio_Aac_File ,Adts_Headr_Buf,ADTS_HEADER_LENGTH);
    //if (readsize < 0)
    //{
    //    printf("ReadFile : pAudio_Aac_File ERROR\n");
    //    return getchar();
    //}
    //if (readsize == 0)
    //{
    //    return readsize;
    //}
    adtsheader->aac_frame_length = 0;
    if ((Adts_Headr_Buf[0] == 0xFF)&&((Adts_Headr_Buf[1] & 0xF0) == 0xF0))    //syncword 12个1
    {
        adtsheader->syncword = (Adts_Headr_Buf[0] << 4 )  | (Adts_Headr_Buf[1]  >> 4);
        adtsheader->id = ((unsigned int) Adts_Headr_Buf[1] & 0x08) >> 3;
        adtsheader->layer = ((unsigned int) Adts_Headr_Buf[1] & 0x06) >> 1;
        adtsheader->protection_absent = (unsigned int) Adts_Headr_Buf[1] & 0x01;
        adtsheader->profile = ((unsigned int) Adts_Headr_Buf[2] & 0xc0) >> 6;
        adtsheader->sf_index = ((unsigned int) Adts_Headr_Buf[2] & 0x3c) >> 2;
        adtsheader->private_bit = ((unsigned int) Adts_Headr_Buf[2] & 0x02) >> 1;
        adtsheader->channel_configuration = ((((unsigned int) Adts_Headr_Buf[2] & 0x01) << 2) | (((unsigned int) Adts_Headr_Buf[3] & 0xc0) >> 6));
        adtsheader->original = ((unsigned int) Adts_Headr_Buf[3] & 0x20) >> 5;
        adtsheader->home = ((unsigned int) Adts_Headr_Buf[3] & 0x10) >> 4;
        adtsheader->copyright_identification_bit = ((unsigned int) Adts_Headr_Buf[3] & 0x08) >> 3;
        adtsheader->copyright_identification_start = (unsigned int) Adts_Headr_Buf[3] & 0x04 >> 2;		
        adtsheader->aac_frame_length = (((((unsigned int) Adts_Headr_Buf[3]) & 0x03) << 11) | (((unsigned int) Adts_Headr_Buf[4] & 0xFF) << 3)| ((unsigned int) Adts_Headr_Buf[5] & 0xE0) >> 5) ;
        adtsheader->adts_buffer_fullness = (((unsigned int) Adts_Headr_Buf[5] & 0x1f) << 6 | ((unsigned int) Adts_Headr_Buf[6] & 0xfc) >> 2);
        adtsheader->no_raw_data_blocks_in_frame = ((unsigned int) Adts_Headr_Buf[6] & 0x03);
    }
    else 
    {
        TPLOG_ERROR("ADTS_HEADER : BUF ERROR\n");
        //printf("ADTS_HEADER : BUF ERROR\n");
    }
    return adtsheader->aac_frame_length;
}

int Read_One_Aac_Frame(unsigned char * buf)
{
    ADTS_HEADER  adts_header ;
    //unsigned int readsize = 0;

    //读取ADTS头
    if (!Detach_Head_Aac(&adts_header,buf))
    {
        return 0;
    }
    //将data填入bufz中
    //readsize = ReadFile(pAudio_Aac_File ,buf + ADTS_HEADER_LENGTH ,adts_header.aac_frame_length - ADTS_HEADER_LENGTH);
    //if (readsize != adts_header.aac_frame_length - ADTS_HEADER_LENGTH)
    //{
    //    printf("READ ADTS_DATA : BUF LENGTH ERROR\n");
    //    return -1;
    //}
    return adts_header.aac_frame_length;
}

int AAC2PES(TsPes * tsaacpes,unsigned long Adudiopts)
{
    unsigned int aacpes_pos = 0;
    unsigned int OneFrameLen_AAC = 0;

    //读取出一帧数据
    OneFrameLen_AAC = Read_One_Aac_Frame(tsaacpes->Es);
    aacpes_pos += OneFrameLen_AAC ;

    tsaacpes->packet_start_code_prefix = 0x000001;
    tsaacpes->stream_id = TS_AAC_STREAM_ID;                                //E0~EF表示是视频的,C0~DF是音频,H264-- E0
    tsaacpes->PES_packet_length = 0 ; // OneFrameLen_AAC + 8 ;             //一帧数据的长度 不包含 PES包头 ,8自适应段的长度
    tsaacpes->Pes_Packet_Length_Beyond = OneFrameLen_AAC;                  //= OneFrameLen_aac;     //这里读错了一帧  
    if (OneFrameLen_AAC > 0xFFFF)                                          //如果一帧数据的大小超出界限
    {
        tsaacpes->PES_packet_length = 0x00;
        tsaacpes->Pes_Packet_Length_Beyond = OneFrameLen_AAC;  
        aacpes_pos += 16;
    }
    else
    {
        tsaacpes->PES_packet_length = 0x00;
        tsaacpes->Pes_Packet_Length_Beyond = OneFrameLen_AAC;  
        aacpes_pos += 14;
    }
    tsaacpes->marker_bit = 0x02;
    tsaacpes->PES_scrambling_control = 0x00;                               //人选字段 存在，不加扰
    tsaacpes->PES_priority = 0x00;
    tsaacpes->data_alignment_indicator = 0x00;
    tsaacpes->copyright = 0x00;
    tsaacpes->original_or_copy = 0x00;
    tsaacpes->PTS_DTS_flags = 0x02;                                        //10'：PTS字段存在,DTS不存在
    tsaacpes->ESCR_flag = 0x00;
    tsaacpes->ES_rate_flag = 0x00;
    tsaacpes->DSM_trick_mode_flag = 0x00;
    tsaacpes->additional_copy_info_flag = 0x00;
    tsaacpes->PES_CRC_flag = 0x00;
    tsaacpes->PES_extension_flag = 0x00;
    tsaacpes->PES_header_data_length = 0x05;                               //后面的数据 包括了PTS所占的字节数

    //清 0 
    tsaacpes->tsptsdts.pts_32_30  = 0;
    tsaacpes->tsptsdts.pts_29_15 = 0;
    tsaacpes->tsptsdts.pts_14_0 = 0;

    tsaacpes->tsptsdts.reserved_1 = 0x03;                                 //填写 pts信息
    // Adudiopts大于30bit，使用最高三位 
    if(Adudiopts > 0x7FFFFFFF)
    {
        tsaacpes->tsptsdts.pts_32_30 = (Adudiopts >> 30) & 0x07;                 
        tsaacpes->tsptsdts.marker_bit1 = 0x01;
    }
    else 
    {
        tsaacpes->tsptsdts.marker_bit1 = 0;
    }
    // Videopts大于15bit，使用更多的位来存储
    if(Adudiopts > 0x7FFF)
    {
        tsaacpes->tsptsdts.pts_29_15 = (Adudiopts >> 15) & 0x007FFF ;
        tsaacpes->tsptsdts.marker_bit2 = 0x01;
    }
    else
    {
        tsaacpes->tsptsdts.marker_bit2 = 0;
    }
    //使用最后15位
    tsaacpes->tsptsdts.pts_14_0 = Adudiopts & 0x007FFF;
    tsaacpes->tsptsdts.marker_bit3 = 0x01;

    return aacpes_pos;
}

NALU_t *AllocNALU(int buffersize)
{
    NALU_t *n;

    if ((n = (NALU_t*)calloc (1, sizeof(NALU_t))) == NULL)
    {
        TPLOG_ERROR("AllocNALU Error: Allocate Meory To NALU_t Failed ");
        //printf("AllocNALU Error: Allocate Meory To NALU_t Failed ");
        //getchar();
    }

    n->max_size = buffersize;									//Assign buffer size 

    if ((n->buf = (unsigned char*)calloc (buffersize, sizeof (char))) == NULL)
    {
        free (n);
        TPLOG_ERROR("AllocNALU Error: Allocate Meory To NALU_t Buffer Failed ");
        //printf ("AllocNALU Error: Allocate Meory To NALU_t Buffer Failed ");
        //getchar();
    }
    return n;
}

int FindStartCode2 (unsigned char *Buf)
{
    if(Buf[0]!=0 || Buf[1]!=0 || Buf[2] !=1)               //Check whether buf is 0x000001
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}

int FindStartCode3 (unsigned char *Buf)
{
    if(Buf[0]!=0 || Buf[1]!=0 || Buf[2] !=0 || Buf[3] !=1)  //Check whether buf is 0x00000001
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}

int GetAnnexbNALU (NALU_t * nalu, unsigned char * buf, int bufLen)
{
    int pos = 0;                  //一个nal到下一个nal 数据移动的指针
    int StartCodeFound  = 0;      //是否找到下一个nal 的前缀
    int rewind = 0;               //判断 前缀所占字节数 3或 4
    unsigned char * Buf = NULL;
    static int info2 =0 ;
    static int info3 =0 ;
    int iReadLen = 0;

    if ((Buf = (unsigned char*)calloc (nalu->max_size , sizeof(char))) == NULL) 
    {
        TPLOG_ERROR("GetAnnexbNALU Error: Could not allocate Buf memory\n");
        //printf ("GetAnnexbNALU Error: Could not allocate Buf memory\n");
    }

    nalu->startcodeprefix_len = 3;      //初始化前缀位三个字节

    memcpy(Buf, buf, 3);
    //if (3 != fread (Buf, 1, 3, pVideo_H264_File))//从文件读取三个字节到buf
    //{
    //    free(Buf);
    //    return 0;
    //}
    iReadLen = 3;
    info2 = FindStartCode2 (Buf);       //Check whether Buf is 0x000001
    if(info2 != 1) 
    {
        //If Buf is not 0x000001,then read one more byte
        memcpy(Buf+3, buf+3, 1);
        iReadLen += 1;
        //if(1 != fread(Buf + 3, 1, 1, pVideo_H264_File))
        //{
        //    free(Buf);
        //    return 0;
        //}
        info3 = FindStartCode3 (Buf);   //Check whether Buf is 0x00000001
        if (info3 != 1)                 //If not the return -1
        { 
            free(Buf);
            return -1;
        }
        else 
        {
            //If Buf is 0x00000001,set the prefix length to 4 bytes
            pos = 4;
            nalu->startcodeprefix_len = 4;
        }
    } 
    else
    {
        //If Buf is 0x000001,set the prefix length to 3 bytes
        pos = 3;
        nalu->startcodeprefix_len = 3;
    }
    //寻找下一个字符符号位， 即 寻找一个nal 从一个0000001 到下一个00000001
    StartCodeFound = 0;
    info2 = 0;
    info3 = 0;
    while (!StartCodeFound)
    {
        if (iReadLen >= bufLen)                                 //如果到了文件结尾
        {
            nalu->len = (pos-1) - nalu->startcodeprefix_len;  //从0 开始
            memcpy (nalu->buf, &Buf[nalu->startcodeprefix_len], nalu->len);     
            nalu->forbidden_bit = nalu->buf[0] & 0x80;      // 1 bit--10000000
            nalu->nal_reference_idc = nalu->buf[0] & 0x60;  // 2 bit--01100000
            nalu->nal_unit_type = (nalu->buf[0]) & 0x1f;    // 5 bit--00011111
            free(Buf);
            return ((info3 == 1)? 4 : 3);
        }
        //Buf[pos++] = fgetc (pVideo_H264_File);                       //Read one char to the Buffer 一个字节一个字节从文件向后找
        memcpy(((void*)Buf[pos]), buf+iReadLen, 1);
        pos++;
        iReadLen++;
        info3 = FindStartCode3(&Buf[pos-4]);		        //Check whether Buf is 0x00000001 
        if(info3 != 1)
        {
            info2 = FindStartCode2(&Buf[pos-3]);            //Check whether Buf is 0x000001
        }
        StartCodeFound = (info2 == 1 || info3 == 1);        //如果找到下一个前缀
    }

    rewind = (info3 == 1)? -4 : -3;

    //if (0 != fseek (pVideo_H264_File, rewind, SEEK_CUR))			    //将文件内部指针移动到 nal 的末尾
    //{
    //    free(Buf);
    //    printf("GetAnnexbNALU Error: Cannot fseek in the bit stream file");
    //}

    nalu->len = (pos + rewind) -  nalu->startcodeprefix_len;       //设置包含nal 头的数据长度
    memcpy (nalu->buf, &Buf[nalu->startcodeprefix_len], nalu->len);//拷贝一个nal 数据到数组中
    nalu->forbidden_bit = nalu->buf[0] & 0x80;                     //1 bit  设置nal 头
    nalu->nal_reference_idc = nalu->buf[0] & 0x60;                 // 2 bit
    nalu->nal_unit_type = (nalu->buf[0]) & 0x1f;                   // 5 bit
    free(Buf);
    return ((info3 == 1)? 4 : 3);                                               
}

void bs_init( bs_t *s, void *p_data, int i_data )
{
    s->p_start = (unsigned char *)p_data;		//用传入的p_data首地址初始化p_start，只记下有效数据的首地址
    s->p       = (unsigned char *)p_data;		//字节首地址，一开始用p_data初始化，每读完一个整字节，就移动到下一字节首地址
    s->p_end   = s->p + i_data;	                //尾地址，最后一个字节的首地址?
    s->i_left  = 8;				                //还没有开始读写，当前字节剩余未读取的位是8
}

int bs_read( bs_t *s, int i_count )
{
	 static int i_mask[33] ={0x00,
                                  0x01,      0x03,      0x07,      0x0f,
                                  0x1f,      0x3f,      0x7f,      0xff,
                                  0x1ff,     0x3ff,     0x7ff,     0xfff,
                                  0x1fff,    0x3fff,    0x7fff,    0xffff,
                                  0x1ffff,   0x3ffff,   0x7ffff,   0xfffff,
                                  0x1fffff,  0x3fffff,  0x7fffff,  0xffffff,
                                  0x1ffffff, 0x3ffffff, 0x7ffffff, 0xfffffff,
                                  0x1fffffff,0x3fffffff,0x7fffffff,0xffffffff};
	/*
			  数组中的元素用二进制表示如下：

			  假设：初始为0，已写入为+，已读取为-
			  
			  字节:		1		2		3		4
				   00000000 00000000 00000000 00000000		下标

			  0x00:							  00000000		x[0]

			  0x01:							  00000001		x[1]
			  0x03:							  00000011		x[2]
			  0x07:							  00000111		x[3]
			  0x0f:							  00001111		x[4]

			  0x1f:							  00011111		x[5]
			  0x3f:							  00111111		x[6]
			  0x7f:							  01111111		x[7]
			  0xff:							  11111111		x[8]	1字节

			 0x1ff:						 0001 11111111		x[9]
			 0x3ff:						 0011 11111111		x[10]	i_mask[s->i_left]
			 0x7ff:						 0111 11111111		x[11]
			 0xfff:						 1111 11111111		x[12]	1.5字节

			0x1fff:					 00011111 11111111		x[13]
			0x3fff:					 00111111 11111111		x[14]
			0x7fff:					 01111111 11111111		x[15]
			0xffff:					 11111111 11111111		x[16]	2字节

		   0x1ffff:				0001 11111111 11111111		x[17]
		   0x3ffff:				0011 11111111 11111111		x[18]
		   0x7ffff:				0111 11111111 11111111		x[19]
		   0xfffff:				1111 11111111 11111111		x[20]	2.5字节

		  0x1fffff:			00011111 11111111 11111111		x[21]
		  0x3fffff:			00111111 11111111 11111111		x[22]
		  0x7fffff:			01111111 11111111 11111111		x[23]
		  0xffffff:			11111111 11111111 11111111		x[24]	3字节

		 0x1ffffff:	   0001 11111111 11111111 11111111		x[25]
		 0x3ffffff:	   0011 11111111 11111111 11111111		x[26]
		 0x7ffffff:    0111 11111111 11111111 11111111		x[27]
		 0xfffffff:    1111 11111111 11111111 11111111		x[28]	3.5字节

		0x1fffffff:00011111 11111111 11111111 11111111		x[29]
		0x3fffffff:00111111 11111111 11111111 11111111		x[30]
		0x7fffffff:01111111 11111111 11111111 11111111		x[31]
		0xffffffff:11111111 11111111 11111111 11111111		x[32]	4字节

	 */
    int      i_shr;			    //
    int i_result = 0;	        //用来存放读取到的的结果 typedef unsigned   uint32_t;

    while( i_count > 0 )	    //要读取的比特数
    {
        if( s->p >= s->p_end )	//字节流的当前位置>=流结尾，即代表此比特流s已经读完了。
        {						//
            break;
        }

        if( ( i_shr = s->i_left - i_count ) >= 0 )	//当前字节剩余的未读位数，比要读取的位数多，或者相等
        {											//i_left当前字节剩余的未读位数，本次要读i_count比特，i_shr=i_left-i_count的结果如果>=0，说明要读取的都在当前字节内
													//i_shr>=0，说明要读取的比特都处于当前字节内
			//这个阶段，一次性就读完了，然后返回i_result(退出了函数)
            /* more in the buffer than requested */
            i_result |= ( *s->p >> i_shr )&i_mask[i_count];//“|=”:按位或赋值，A |= B 即 A = A|B
									//|=应该在最后执行，把结果放在i_result(按位与优先级高于复合操作符|=)
									//i_mask[i_count]最右侧各位都是1,与括号中的按位与，可以把括号中的结果复制过来
									//!=,左边的i_result在这儿全是0，右侧与它按位或，还是复制结果过来了，好象好几步都多余
			/*读取后，更新结构体里的字段值*/
            s->i_left -= i_count; //即i_left = i_left - i_count，当前字节剩余的未读位数，原来的减去这次读取的
            if( s->i_left == 0 )	//如果当前字节剩余的未读位数正好是0，说明当前字节读完了，就要开始下一个字节
            {
                s->p++;				//移动指针，所以p好象是以字节为步长移动指针的
                s->i_left = 8;		//新开始的这个字节来说，当前字节剩余的未读位数，就是8比特了
            }
            return( i_result );		//可能的返回值之一为：00000000 00000000 00000000 00000001 (4字节长)
        }
        else	/* i_shr < 0 ,跨字节的情况*/
        {
			//这个阶段，是while的一次循环，可能还会进入下一次循环，第一次和最后一次都可能读取的非整字节，比如第一次读了3比特，中间读取了2字节(即2x8比特)，最后一次读取了1比特，然后退出while循环
			//当前字节剩余的未读位数，比要读取的位数少，比如当前字节有3位未读过，而本次要读7位
			//???对当前字节来说，要读的比特，都在最右边，所以不再移位了(移位的目的是把要读的比特放在当前字节最右)
            /* less(较少的) in the buffer than requested */
			i_result |= (*s->p&i_mask[s->i_left]) << -i_shr;	//"-i_shr"相当于取了绝对值
									//|= 和 << 都是位操作符，优先级相同，所以从左往右顺序执行
									//举例:int|char ，其中int是4字节，char是1字节，sizeof(int|char)是4字节
									//i_left最大是8，最小是0，取值范围是[0,8]
			i_count  -= s->i_left;	//待读取的比特数，等于原i_count减去i_left，i_left是当前字节未读过的比特数，而此else阶段，i_left代表的当前字节未读的比特全被读过了，所以减它
			s->p++;	//定位到下一个新的字节
			s->i_left = 8;	//对一个新字节来说，未读过的位数当然是8，即本字节所有位都没读取过
        }
    }

    return( i_result );//可能的返回值之一为：00000000 00000000 00000000 00000001 (4字节长)
}

int bs_read1( bs_t *s )
{

    if( s->p < s->p_end )	
    {
        unsigned int i_result;

        s->i_left--;                           //当前字节未读取的位数少了1位
        i_result = ( *s->p >> s->i_left )&0x01;//把要读的比特移到当前字节最右，然后与0x01:00000001进行逻辑与操作，因为要读的只是一个比特，这个比特不是0就是1，与0000 0001按位与就可以得知此情况
        if( s->i_left == 0 )                   //如果当前字节剩余未读位数是0，即是说当前字节全读过了
        {
            s->p++;			                   //指针s->p 移到下一字节
            s->i_left = 8;                     //新字节中，未读位数当然是8位
        }
        return i_result;                       //unsigned int
    }

    return 0;                                  //返回0应该是没有读到东西
}

int bs_read_ue( bs_t *s )
{
    int i = 0;

    while( bs_read1( s ) == 0 && s->p < s->p_end && i < 32 )	//条件为：读到的当前比特=0，指针未越界，最多只能读32比特
    {
        i++;
    }
    return( ( 1 << i) - 1 + bs_read( s, i ) );	
}

int GetFrameType(NALU_t * nal)
{
    bs_t s;
    int frame_type = 0; 
    unsigned char * OneFrameBuf_H264 = NULL ;
    if ((OneFrameBuf_H264 = (unsigned char *)calloc(nal->len + 4,sizeof(unsigned char))) == NULL)
    {
        TPLOG_ERROR("Error malloc OneFrameBuf_H264\n");
        return -1;
    }
    if (nal->startcodeprefix_len == 3)
    {
        OneFrameBuf_H264[0] = 0x00;
        OneFrameBuf_H264[1] = 0x00;
        OneFrameBuf_H264[2] = 0x01;
        memcpy(OneFrameBuf_H264 + 3,nal->buf,nal->len);
    }
    else if (nal->startcodeprefix_len == 4)
    {
        OneFrameBuf_H264[0] = 0x00;
        OneFrameBuf_H264[1] = 0x00;
        OneFrameBuf_H264[2] = 0x00;
        OneFrameBuf_H264[3] = 0x01;
        memcpy(OneFrameBuf_H264 + 4,nal->buf,nal->len);
    }
    else
    {
        TPLOG_ERROR("H264读取错误");
    }
    bs_init( &s,OneFrameBuf_H264 + nal->startcodeprefix_len + 1  ,nal->len - 1 );


    if (nal->nal_unit_type == NAL_SLICE || nal->nal_unit_type ==  NAL_SLICE_IDR )
    {
        /* i_first_mb */
        bs_read_ue( &s );
        /* picture type */
        frame_type =  bs_read_ue( &s );
        switch(frame_type)
        {
        case 0: case 5: /* P */
            nal->Frametype = FRAME_P;
            break;
        case 1: case 6: /* B */
            nal->Frametype = FRAME_B;
            break;
        case 3: case 8: /* SP */
            nal->Frametype = FRAME_P;
            break;
        case 2: case 7: /* I */
            nal->Frametype = FRAME_I;
            break;
        case 4: case 9: /* SI */
            nal->Frametype = FRAME_I;
            break;
        }
    }
    else if (nal->nal_unit_type == NAL_SEI)
    {
        nal->Frametype = NAL_SEI;
    }
    else if(nal->nal_unit_type == NAL_SPS)
    {
        nal->Frametype = NAL_SPS;
    }
    else if(nal->nal_unit_type == NAL_PPS)
    {
        nal->Frametype = NAL_PPS;
    }
    if (OneFrameBuf_H264)
    {
        free(OneFrameBuf_H264);
        OneFrameBuf_H264 = NULL;
    }
    return 1;
}

void FreeNALU(NALU_t *n)
{
    if (n)
    {
        if (n->buf)
        {
            free(n->buf);
            n->buf=NULL;
        }
        free (n);
    }
}

int Read_One_H264_Frame(unsigned char * buf,unsigned int * videoframetype, int bufLen)
{
    NALU_t * n = NULL;
    unsigned int video_buf_size = 0;

    //分配nal 资源
    n = AllocNALU(MAX_VIDEO_TAG_BUF_SIZE); 

    //读取一帧数据
    int startcodeprefix_size = GetAnnexbNALU(n, buf, bufLen);
    if (startcodeprefix_size == 0)
    {
        return 0;
    }
    //判断帧类型
    GetFrameType(n);
    *videoframetype = n->Frametype;

    if (n->startcodeprefix_len == 3)
    {
        buf[0] = 0x00;
        buf[1] = 0x00;
        buf[2] = 0x01;
        memcpy(buf + 3,n->buf,n->len);
    }
    else if (n->startcodeprefix_len == 4)
    {
        buf[0] = 0x00;
        buf[1] = 0x00;
        buf[2] = 0x00;
        buf[3] = 0x01;
        memcpy(buf + 4,n->buf,n->len);
    }
    else
    {
        TPLOG_ERROR("H264读取错误！\n");
    }

    video_buf_size = n->startcodeprefix_len + n->len;

    FreeNALU(n);                                                   //释放nal 资源 
    return video_buf_size;
}

int H2642PES(TsPes * tsh264pes,unsigned long Videopts,unsigned int * videoframetype)
{
    unsigned int h264pes_pos = 0;
    unsigned int OneFrameLen_H264 = 0;


    //读取出一帧数据
    //OneFrameLen_H264 = Read_One_H264_Frame(tsh264pes->Es,videoframetype,tsh264pes->Pes_Packet_Length_Beyond);
    OneFrameLen_H264 = tsh264pes->Pes_Packet_Length_Beyond;
    *videoframetype = FRAME_I;
    h264pes_pos += OneFrameLen_H264;


    tsh264pes->packet_start_code_prefix = 0x000001;
    tsh264pes->stream_id = TS_H264_STREAM_ID;                               //E0~EF表示是视频的,C0~DF是音频,H264-- E0
    tsh264pes->PES_packet_length = 0 ;                                      //一帧数据的长度 不包含 PES包头 ,这个8 是 自适应的长度,填0 可以自动查找
    tsh264pes->Pes_Packet_Length_Beyond = OneFrameLen_H264;

    if (OneFrameLen_H264 > 0xFFFF)                                          //如果一帧数据的大小超出界限
    {
        tsh264pes->PES_packet_length = 0x00;
        tsh264pes->Pes_Packet_Length_Beyond = OneFrameLen_H264;
        h264pes_pos += 16;
    }
    else
    {
        tsh264pes->PES_packet_length = 0x00;
        tsh264pes->Pes_Packet_Length_Beyond = OneFrameLen_H264;
        h264pes_pos += 14;
    }
    tsh264pes->marker_bit = 0x02;
    tsh264pes->PES_scrambling_control = 0x00;                               //人选字段 存在，不加扰
    tsh264pes->PES_priority = 0x00;
    tsh264pes->data_alignment_indicator = 0x00;
    tsh264pes->copyright = 0x00;
    tsh264pes->original_or_copy = 0x00;
    tsh264pes->PTS_DTS_flags = 0x02;                                         //10'：PTS字段存在,DTS不存在
    tsh264pes->ESCR_flag = 0x00;
    tsh264pes->ES_rate_flag = 0x00;
    tsh264pes->DSM_trick_mode_flag = 0x00;
    tsh264pes->additional_copy_info_flag = 0x00;
    tsh264pes->PES_CRC_flag = 0x00;
    tsh264pes->PES_extension_flag = 0x00;
    tsh264pes->PES_header_data_length = 0x05;                                //后面的数据 包括了PTS所占的字节数

    //清 0 
    tsh264pes->tsptsdts.pts_32_30  = 0;
    tsh264pes->tsptsdts.pts_29_15 = 0;
    tsh264pes->tsptsdts.pts_14_0 = 0;

    tsh264pes->tsptsdts.reserved_1 = 0x0003;                                 //填写 pts信息
    // Videopts大于30bit，使用最高三位 
    if(Videopts > 0x7FFFFFFF)
    {
        tsh264pes->tsptsdts.pts_32_30 = (Videopts >> 30) & 0x07;                 
        tsh264pes->tsptsdts.marker_bit1 = 0x01;
    }
    else 
    {
        tsh264pes->tsptsdts.marker_bit1 = 0;
    }
    // Videopts大于15bit，使用更多的位来存储
    if(Videopts > 0x7FFF)
    {
        tsh264pes->tsptsdts.pts_29_15 = (Videopts >> 15) & 0x007FFF ;
        tsh264pes->tsptsdts.marker_bit2 = 0x01;
    }
    else
    {
        tsh264pes->tsptsdts.marker_bit2 = 0;
    }
    //使用最后15位
    tsh264pes->tsptsdts.pts_14_0 = Videopts & 0x007FFF;
    tsh264pes->tsptsdts.marker_bit3 = 0x01;

    return h264pes_pos;
}

int Take_Out_Pes(TsPes * tspes ,unsigned long time_pts,unsigned int frametype,unsigned int * videoframetype)
{
    unsigned int pes_pos = 0;
    if (frametype == 0x00) //视频
    {
        pes_pos = H2642PES(tspes,time_pts,videoframetype);
    }
    else                   //音频
    {
        pes_pos = AAC2PES(tspes,time_pts);  
    }
    return pes_pos;
}

int WriteAdaptive_flags_Tail(Ts_Adaptation_field  * ts_adaptation_field)
{
    //填写自适应段
    ts_adaptation_field->discontinuty_indicator = 0;
    ts_adaptation_field->random_access_indicator = 0;
    ts_adaptation_field->elementary_stream_priority_indicator = 0;
    ts_adaptation_field->PCR_flag = 0;                                          //只用到这个
    ts_adaptation_field->OPCR_flag = 0;
    ts_adaptation_field->splicing_point_flag = 0;
    ts_adaptation_field->transport_private_data_flag = 0;
    ts_adaptation_field->adaptation_field_extension_flag = 0;

    //需要自己算
    ts_adaptation_field->pcr  = 0;
    ts_adaptation_field->adaptation_field_length = 1;                          //占用1位标志所用的位

    ts_adaptation_field->opcr = 0;
    ts_adaptation_field->splice_countdown = 0;
    ts_adaptation_field->private_data_len = 0;                    
    return 1;
}
Continuity_Counter continuity_counter;     //包类型计数器
int WriteStruct_Packetheader(unsigned char * Buf , unsigned int PID,unsigned char play_init,unsigned char ada_field_C)
{
    TsPacketHeader tspacketheader;

    tspacketheader.sync_byte = TS_SYNC_BYTE;
    tspacketheader.tras_error = 0x00;
    tspacketheader.play_init = play_init;
    tspacketheader.tras_prio = 0x00;
    tspacketheader.PID = PID;
    tspacketheader.tras_scramb = 0x00;
    tspacketheader.ada_field_C = ada_field_C;

    if (PID == TS_PAT_PID)             //这是pat的包
    {
        tspacketheader.conti_cter = (continuity_counter.continuity_counter_pat %16);
        continuity_counter.continuity_counter_pat ++;
    }
    else if (PID == TS_PMT_PID)        //这是pmt的包
    {
        tspacketheader.conti_cter = (continuity_counter.continuity_counter_pmt %16);
        continuity_counter.continuity_counter_pmt ++;
    }
    else if (PID == TS_H264_PID )      //这是H264的包
    {
        tspacketheader.conti_cter = (continuity_counter.continuity_counter_video %16);
        continuity_counter.continuity_counter_video ++;
    }
    else if (PID == TS_AAC_PID)        //这是MP3的包
    {
        tspacketheader.conti_cter = (continuity_counter.continuity_counter_audio %16);
        continuity_counter.continuity_counter_audio ++;
    }
    else                               //其他包出错，或可扩展
    {
        printf("continuity_counter error packet\n");
    }

    Buf[0] = tspacketheader.sync_byte;
    Buf[1] = tspacketheader.tras_error << 7 | tspacketheader.play_init << 6  | tspacketheader.tras_prio << 5 | ((tspacketheader.PID >> 8) & 0x1f);
    Buf[2] = (tspacketheader. PID & 0x00ff);
    Buf[3] = tspacketheader.tras_scramb << 6 | tspacketheader.ada_field_C << 4 | tspacketheader.conti_cter;
    return 4;
}

unsigned int  calc_crc32 (unsigned char *data, unsigned int datalen)
{
    unsigned int i;
    unsigned int crc = 0xffffffff;

    for (i=0; i<datalen; i++) 
    {
        crc = (crc << 8) ^ crc_tab[((crc >> 24) ^ *data++) & 0xff];
    }
    return crc;
}

unsigned int Zwg_ntohl(unsigned int s)
{
    union 
    {
        int  i;
        char buf;
    }a;
    a.i = 0x01;
    if(a.buf)
    {
        // 小端
        s = BSWAP32C(s);
    }
    return s;
}

int WriteStruct_Pat(unsigned char * Buf)
{
    unsigned int pat_pos = 0;  
    TsPat tspat;
    unsigned int PAT_CRC = 0xFFFFFFFF;

    memset(Buf,0xFF,TS_PACKET_SIZE);

    tspat.table_id = 0x00;
    tspat.section_syntax_indicator = 0x01;
    tspat.zero = 0x00;
    tspat.reserved_1 = 0x03;                                               //设置为11；
    tspat.section_length = 0x0d;                                           //pat结构体长度 16个字节减去上面的3个字节
    tspat.transport_stream_id = 0x01;
    tspat.reserved_2 = 0x03;                                               //设置为11；
    tspat.version_number = 0x00;
    tspat.current_next_indicator = 0x01;                                   //当前的pat 有效
    tspat.section_number = 0x00;
    tspat.last_section_number = 0x00;
    tspat.program_number = 0x01;
    tspat.reserved_3 = 0x07;                                               //设置为111；
    tspat.program_map_PID = TS_PMT_PID;                                    //PMT的PID
    tspat.CRC_32 = PAT_CRC;                                                //传输过程中检测的一种算法值 先设定一个填充值

    pat_pos += WriteStruct_Packetheader(Buf,TS_PAT_PID,0x01,0x01);  //PID = 0x00,有效荷载单元起始指示符_play_init = 0x01, ada_field_C,0x01,仅有有效负载 ；
    Buf[4] = 0;                                                     //自适应段的长度为0
    pat_pos ++;

    Buf[pat_pos] = tspat.table_id;
    Buf[pat_pos + 1] = tspat.section_syntax_indicator << 7 | tspat.zero  << 6 | tspat.reserved_1 << 4 | ((tspat.section_length >> 8) & 0x0F);
    Buf[pat_pos + 2] = tspat.section_length & 0x00FF;
    Buf[pat_pos + 3] = tspat.transport_stream_id >> 8;
    Buf[pat_pos + 4] = tspat.transport_stream_id & 0x00FF;
    Buf[pat_pos + 5] = tspat.reserved_2 << 6 | tspat.version_number << 1 | tspat.current_next_indicator;
    Buf[pat_pos + 6] = tspat.section_number;
    Buf[pat_pos + 7] = tspat.last_section_number;
    Buf[pat_pos + 8] = tspat.program_number>>8;
    Buf[pat_pos + 9] = tspat.program_number & 0x00FF;
    Buf[pat_pos + 10]= tspat.reserved_3 << 5 | ((tspat.program_map_PID >> 8) & 0x0F);
    Buf[pat_pos + 11]= tspat.program_map_PID & 0x00FF;
    pat_pos += 12;

    PAT_CRC = Zwg_ntohl(calc_crc32(Buf + 5, pat_pos - 5));
    memcpy(Buf + pat_pos, (unsigned char *)&PAT_CRC, 4);
    pat_pos += 4; 

    return 188;
}

int Write_Pat(unsigned char * buf)
{
    WriteStruct_Pat(buf);
    //test
    if (NULL == g_phTrans)
    {
        TPLOG_ERROR("g_phTrans is NULL");
        return 0;
    }
    int iret = SYSTRANS_InputData(g_phTrans, MULTI_DATA, buf, TS_PACKET_SIZE);
    if (SYSTRANS_OK != iret)
    {
        TPLOG_ERROR("SYSTRANS_InputData failed:%d",iret);
    }

    return TS_PACKET_SIZE;
    //return WriteFile(pVideo_Audio_Ts_File,(char *)buf,TS_PACKET_SIZE);
}

int WriteStruct_Pmt(unsigned char * Buf)
{
    unsigned int pmt_pos = 0;  
    TsPmt tspmt;
    unsigned long long PMT_CRC = 0xFFFFFFFF; 
    int len = 0;

    memset(Buf,0xFF,TS_PACKET_SIZE);

    tspmt.table_id = 0x02;
    tspmt.section_syntax_indicator = 0x01;
    tspmt.zero = 0x00;
    tspmt.reserved_1 = 0x03;
    tspmt.section_length = 0x17;                                           //PMT结构体长度 16 + 5 + 5个字节减去上面的3个字节
    tspmt.program_number = 01;                                             //只有一个节目
    tspmt.reserved_2 = 0x03;
    tspmt.version_number = 0x00;
    tspmt.current_next_indicator = 0x01;                                   //当前的PMT有效
    tspmt.section_number = 0x00;
    tspmt.last_section_number = 0x00;
    tspmt.reserved_3 = 0x07;
    tspmt.PCR_PID = TS_H264_PID ;                                          //视频PID                                   
    tspmt.reserved_4 = 0x0F;
    tspmt.program_info_length = 0x00;                                      //后面无 节目信息描述
    tspmt.stream_type_video = PMT_STREAM_TYPE_VIDEO;                       //视频的类型
    tspmt.reserved_5_video = 0x07;
    tspmt.elementary_PID_video = TS_H264_PID;                              //视频的PID
    tspmt.reserved_6_video= 0x0F;
    tspmt.ES_info_length_video = 0x00;                                     //视频无跟随的相关信息
    tspmt.stream_type_audio = PMT_STREAM_TYPE_AUDIO;                       //音频类型
    tspmt.reserved_5_audio = 0x07;
    tspmt.elementary_PID_audio = TS_AAC_PID;                               //音频PID 
    tspmt.reserved_6_audio = 0x0F;
    tspmt.ES_info_length_audio = 0x00;                                     //音频无跟随的相关信息
    tspmt.CRC_32 = PMT_CRC; 

    pmt_pos += WriteStruct_Packetheader(Buf,TS_PMT_PID,0x01,0x01);           //PID = TS_PMT_PID,有效荷载单元起始指示符_play_init = 0x01, ada_field_C,0x01,仅有有效负载；

    Buf[4] = 0;                                                      //自适应段的长度为0
    pmt_pos ++;

    Buf[pmt_pos + 0] = tspmt.table_id;
    Buf[pmt_pos + 1] = tspmt.section_syntax_indicator << 7 | tspmt.zero  << 6 | tspmt.reserved_1 << 4 | ((tspmt.section_length >> 8) & 0x0F);
    Buf[pmt_pos + 2] = tspmt.section_length & 0x00FF;
    Buf[pmt_pos + 3] = tspmt.program_number >> 8;
    Buf[pmt_pos + 4] = tspmt.program_number & 0x00FF;
    Buf[pmt_pos + 5] = tspmt.reserved_2 << 6 | tspmt.version_number << 1 | tspmt.current_next_indicator;
    Buf[pmt_pos + 6] = tspmt.section_number;
    Buf[pmt_pos + 7] = tspmt.last_section_number;
    Buf[pmt_pos + 8] = tspmt.reserved_3 << 5  | ((tspmt.PCR_PID >> 8) & 0x1F);
    Buf[pmt_pos + 9] = tspmt.PCR_PID & 0x0FF;
    Buf[pmt_pos + 10]= tspmt.reserved_4 << 4 | ((tspmt.program_info_length >> 8) & 0x0F);
    Buf[pmt_pos + 11]= tspmt.program_info_length & 0xFF;
    Buf[pmt_pos + 12]= tspmt.stream_type_video;                               //视频流的stream_type
    Buf[pmt_pos + 13]= tspmt.reserved_5_video << 5 | ((tspmt.elementary_PID_video >> 8 ) & 0x1F);
    Buf[pmt_pos + 14]= tspmt.elementary_PID_video & 0x00FF;
    Buf[pmt_pos + 15]= tspmt.reserved_6_video<< 4 | ((tspmt.ES_info_length_video >> 8) & 0x0F);
    Buf[pmt_pos + 16]= tspmt.ES_info_length_video & 0x0FF;
    Buf[pmt_pos + 17]= tspmt.stream_type_audio;                               //音频流的stream_type
    Buf[pmt_pos + 18]= tspmt.reserved_5_audio<< 5 | ((tspmt.elementary_PID_audio >> 8 ) & 0x1F);
    Buf[pmt_pos + 19]= tspmt.elementary_PID_audio & 0x00FF;
    Buf[pmt_pos + 20]= tspmt.reserved_6_audio << 4 | ((tspmt.ES_info_length_audio >> 8) & 0x0F);
    Buf[pmt_pos + 21]= tspmt.ES_info_length_audio & 0x0FF;
    pmt_pos += 22;

    len  = pmt_pos - 8 + 4;
    len = len > 0xffff ? 0:len;
    Buf[6] = 0xb0 | (len >> 8);
    Buf[7] = len;

    PMT_CRC = Zwg_ntohl(calc_crc32(Buf + 5, pmt_pos - 5));
    memcpy(Buf + pmt_pos , (unsigned char  *)&PMT_CRC, 4);
    pmt_pos += 4;

    return 188;
}

int Write_Pmt(unsigned char * buf)
{
    WriteStruct_Pmt(buf);
    //test
    if (NULL == g_phTrans)
    {
        TPLOG_ERROR("g_phTrans is NULL");
        return 0;
    }
    int iret = SYSTRANS_InputData(g_phTrans, MULTI_DATA, buf, TS_PACKET_SIZE);
    if (SYSTRANS_OK != iret)
    {
        TPLOG_ERROR("SYSTRANS_InputData failed:%d",iret);
    }

    return TS_PACKET_SIZE;
    //return WriteFile(pVideo_Audio_Ts_File,(char *)buf,TS_PACKET_SIZE);
}

int CreateAdaptive_Ts(Ts_Adaptation_field * ts_adaptation_field,unsigned char * buf,unsigned int AdaptiveLength)
{
    unsigned int CurrentAdaptiveLength = 1;                                 //当前已经用的自适应段长度  
    unsigned char Adaptiveflags = 0;                                        //自适应段的标志
    unsigned int adaptive_pos = 0;

    //填写自适应字段
    if (ts_adaptation_field->adaptation_field_length > 0)
    {
        adaptive_pos += 1;                                                  //自适应段的一些标志所占用的1个字节
        CurrentAdaptiveLength += 1;

        if (ts_adaptation_field->discontinuty_indicator)
        {
            Adaptiveflags |= 0x80;
        }
        if (ts_adaptation_field->random_access_indicator)
        {
            Adaptiveflags |= 0x40;
        }
        if (ts_adaptation_field->elementary_stream_priority_indicator)
        {
            Adaptiveflags |= 0x20;
        }
        if (ts_adaptation_field->PCR_flag)
        {
            unsigned long long pcr_base;
            unsigned int pcr_ext;

            pcr_base = (ts_adaptation_field->pcr / 300);
            pcr_ext = (ts_adaptation_field->pcr % 300);

            Adaptiveflags |= 0x10;

            buf[adaptive_pos + 0] = (pcr_base >> 25) & 0xff;
            buf[adaptive_pos + 1] = (pcr_base >> 17) & 0xff;
            buf[adaptive_pos + 2] = (pcr_base >> 9) & 0xff;
            buf[adaptive_pos + 3] = (pcr_base >> 1) & 0xff;
            buf[adaptive_pos + 4] = pcr_base << 7 | pcr_ext >> 8 | 0x7e;
            buf[adaptive_pos + 5] = (pcr_ext) & 0xff;
            adaptive_pos += 6;

            CurrentAdaptiveLength += 6;
        }
        if (ts_adaptation_field->OPCR_flag)
        {
            unsigned long long opcr_base;
            unsigned int opcr_ext;

            opcr_base = (ts_adaptation_field->opcr / 300);
            opcr_ext = (ts_adaptation_field->opcr % 300);

            Adaptiveflags |= 0x08;

            buf[adaptive_pos + 0] = (opcr_base >> 25) & 0xff;
            buf[adaptive_pos + 1] = (opcr_base >> 17) & 0xff;
            buf[adaptive_pos + 2] = (opcr_base >> 9) & 0xff;
            buf[adaptive_pos + 3] = (opcr_base >> 1) & 0xff;
            buf[adaptive_pos + 4] = ((opcr_base << 7) & 0x80) | ((opcr_ext >> 8) & 0x01);
            buf[adaptive_pos + 5] = (opcr_ext) & 0xff;
            adaptive_pos += 6;
            CurrentAdaptiveLength += 6;
        }
        if (ts_adaptation_field->splicing_point_flag)
        {
            buf[adaptive_pos] = ts_adaptation_field->splice_countdown;

            Adaptiveflags |= 0x04;

            adaptive_pos += 1;
            CurrentAdaptiveLength += 1;
        }
        if (ts_adaptation_field->private_data_len > 0)
        {
            Adaptiveflags |= 0x02;
            if (1+ ts_adaptation_field->private_data_len > AdaptiveLength - CurrentAdaptiveLength)
            {
                TPLOG_ERROR("private_data_len error !");
                //printf("private_data_len error !\n");
                return 0;
            }
            else
            {
                buf[adaptive_pos] = ts_adaptation_field->private_data_len;
                adaptive_pos += 1;
                memcpy (buf + adaptive_pos, ts_adaptation_field->private_data, ts_adaptation_field->private_data_len);
                adaptive_pos += ts_adaptation_field->private_data_len;

                CurrentAdaptiveLength += (1 + ts_adaptation_field->private_data_len) ;
            }
        }
        if (ts_adaptation_field->adaptation_field_extension_flag)
        {
            Adaptiveflags |= 0x01;
            buf[adaptive_pos + 1] = 1;
            buf[adaptive_pos + 2] = 0;
            CurrentAdaptiveLength += 2;
        }
        buf[0] = Adaptiveflags;                        //将标志放入内存
    }
    return 1;
}

int PES2TS(TsPes * ts_pes,unsigned int Video_Audio_PID ,Ts_Adaptation_field * ts_adaptation_field_Head ,Ts_Adaptation_field * ts_adaptation_field_Tail,
           unsigned long  Videopts,unsigned long Adudiopts)
{
    TsPacketHeader ts_header;
    unsigned int ts_pos = 0;
    unsigned int FirstPacketLoadLength = 0 ;                                   //分片包的第一个包的负载长度
    unsigned int NeafPacketCount = 0;                                          //分片包的个数
    unsigned int AdaptiveLength = 0;                                           //要填写0XFF的长度
    unsigned char * NeafBuf = NULL;                                            //分片包 总负载的指针
    unsigned char TSbuf[TS_PACKET_SIZE];

    memset(TSbuf,0,TS_PACKET_SIZE); 
    FirstPacketLoadLength = 188 - 4 - 1 - ts_adaptation_field_Head->adaptation_field_length - 14; //计算分片包的第一个包的负载长度
    NeafPacketCount += 1;                                                                   //第一个分片包  

    //一个包的情况
    if (ts_pes->Pes_Packet_Length_Beyond < FirstPacketLoadLength)                           //这里是 sps ，pps ，sei等
    {
        memset(TSbuf,0xFF,TS_PACKET_SIZE);
        WriteStruct_Packetheader(TSbuf,Video_Audio_PID,0x01,0x03);                          //PID = TS_H264_PID,有效荷载单元起始指示符_play_init = 0x01, ada_field_C,0x03,含有调整字段和有效负载 ；
        ts_pos += 4;
        TSbuf[ts_pos + 0] = 184 - ts_pes->Pes_Packet_Length_Beyond - 9 - 5 - 1 ;
        TSbuf[ts_pos + 1] = 0x00;
        ts_pos += 2; 
        memset(TSbuf + ts_pos,0xFF,(184 - ts_pes->Pes_Packet_Length_Beyond - 9 - 5 - 2));
        ts_pos += (184 - ts_pes->Pes_Packet_Length_Beyond - 9 - 5 - 2);

        TSbuf[ts_pos + 0] = (ts_pes->packet_start_code_prefix >> 16) & 0xFF;
        TSbuf[ts_pos + 1] = (ts_pes->packet_start_code_prefix >> 8) & 0xFF; 
        TSbuf[ts_pos + 2] = ts_pes->packet_start_code_prefix & 0xFF;
        TSbuf[ts_pos + 3] = ts_pes->stream_id;
        TSbuf[ts_pos + 4] = ((ts_pes->PES_packet_length) >> 8) & 0xFF;
        TSbuf[ts_pos + 5] = (ts_pes->PES_packet_length) & 0xFF;
        TSbuf[ts_pos + 6] = ts_pes->marker_bit << 6 | ts_pes->PES_scrambling_control << 4 | ts_pes->PES_priority << 3 |
            ts_pes->data_alignment_indicator << 2 | ts_pes->copyright << 1 |ts_pes->original_or_copy;
        TSbuf[ts_pos + 7] = ts_pes->PTS_DTS_flags << 6 |ts_pes->ESCR_flag << 5 | ts_pes->ES_rate_flag << 4 |
            ts_pes->DSM_trick_mode_flag << 3 | ts_pes->additional_copy_info_flag << 2 | ts_pes->PES_CRC_flag << 1 | ts_pes->PES_extension_flag;
        TSbuf[ts_pos + 8] = ts_pes->PES_header_data_length;
        ts_pos += 9;

        if (ts_pes->stream_id == TS_H264_STREAM_ID)
        {
            TSbuf[ts_pos + 0] = (((0x3 << 4) | ((Videopts>> 29) & 0x0E) | 0x01) & 0xff);
            TSbuf[ts_pos + 1]= (((((Videopts >> 14) & 0xfffe) | 0x01) >> 8) & 0xff);
            TSbuf[ts_pos + 2]= ((((Videopts >> 14) & 0xfffe) | 0x01) & 0xff);
            TSbuf[ts_pos + 3]= (((((Videopts << 1) & 0xfffe) | 0x01) >> 8) & 0xff);
            TSbuf[ts_pos + 4]= ((((Videopts << 1) & 0xfffe) | 0x01) & 0xff);
            ts_pos += 5;

        }
        else if (ts_pes->stream_id == TS_AAC_STREAM_ID)
        {
            TSbuf[ts_pos + 0] = (((0x3 << 4) | ((Adudiopts>> 29) & 0x0E) | 0x01) & 0xff);
            TSbuf[ts_pos + 1]= (((((Adudiopts >> 14) & 0xfffe) | 0x01) >> 8) & 0xff);
            TSbuf[ts_pos + 2]= ((((Adudiopts >> 14) & 0xfffe) | 0x01) & 0xff);
            TSbuf[ts_pos + 3]= (((((Adudiopts << 1) & 0xfffe) | 0x01) >> 8) & 0xff);
            TSbuf[ts_pos + 4]= ((((Adudiopts << 1) & 0xfffe) | 0x01) & 0xff);
            ts_pos += 5;
        }
        else
        {
            TPLOG_ERROR("ts_pes->stream_id  error 0x%x \n",ts_pes->stream_id);
            return ts_pes->stream_id;
        }
        memcpy(TSbuf + ts_pos,ts_pes->Es,ts_pes->Pes_Packet_Length_Beyond);  

        //将包写入文件
        //test
        if (NULL == g_phTrans)
        {
            TPLOG_ERROR("g_phTrans is NULL");
            return WritePacketNum;
        }
        int iret = SYSTRANS_InputData(g_phTrans, MULTI_DATA, TSbuf, 188);
        if (SYSTRANS_OK != iret)
        {
            TPLOG_ERROR("SYSTRANS_InputData failed:%d",iret);
        }
        WritePacketNum ++;                                                      //已经写入文件的包个数++
        return WritePacketNum;
    }

    NeafPacketCount += (ts_pes->Pes_Packet_Length_Beyond - FirstPacketLoadLength)/ 184;     
    NeafPacketCount += 1;                                                                   //最后一个分片包
    AdaptiveLength = 188 - 4 - 1 - ((ts_pes->Pes_Packet_Length_Beyond - FirstPacketLoadLength)% 184)  ;  //要填写0XFF的长度
    if ((WritePacketNum % 40) == 0)                                                         //每40个包打一个 pat,一个pmt
    {
        Write_Pat(m_One_Frame_Buf);                                                         //创建PAT
        Write_Pmt(m_One_Frame_Buf);                                                         //创建PMT
    }
    //开始处理第一个包,分片包的个数最少也会是两个 
    WriteStruct_Packetheader(TSbuf,Video_Audio_PID,0x01,0x03);                              //PID = TS_H264_PID,有效荷载单元起始指示符_play_init = 0x01, ada_field_C,0x03,含有调整字段和有效负载 ；
    ts_pos += 4;
    TSbuf[ts_pos] = ts_adaptation_field_Head->adaptation_field_length;                      //自适应字段的长度，自己填写的
    ts_pos += 1;                                                       

    CreateAdaptive_Ts(ts_adaptation_field_Head,TSbuf + ts_pos,(188 - 4 - 1 - 14));          //填写自适应字段
    ts_pos += ts_adaptation_field_Head->adaptation_field_length;                            //填写自适应段所需要的长度

    TSbuf[ts_pos + 0] = (ts_pes->packet_start_code_prefix >> 16) & 0xFF;
    TSbuf[ts_pos + 1] = (ts_pes->packet_start_code_prefix >> 8) & 0xFF; 
    TSbuf[ts_pos + 2] = ts_pes->packet_start_code_prefix & 0xFF;
    TSbuf[ts_pos + 3] = ts_pes->stream_id;
    TSbuf[ts_pos + 4] = ((ts_pes->PES_packet_length) >> 8) & 0xFF;
    TSbuf[ts_pos + 5] = (ts_pes->PES_packet_length) & 0xFF;
    TSbuf[ts_pos + 6] = ts_pes->marker_bit << 6 | ts_pes->PES_scrambling_control << 4 | ts_pes->PES_priority << 3 |
        ts_pes->data_alignment_indicator << 2 | ts_pes->copyright << 1 |ts_pes->original_or_copy;
    TSbuf[ts_pos + 7] = ts_pes->PTS_DTS_flags << 6 |ts_pes->ESCR_flag << 5 | ts_pes->ES_rate_flag << 4 |
        ts_pes->DSM_trick_mode_flag << 3 | ts_pes->additional_copy_info_flag << 2 | ts_pes->PES_CRC_flag << 1 | ts_pes->PES_extension_flag;
    TSbuf[ts_pos + 8] = ts_pes->PES_header_data_length;
    ts_pos += 9;

    if (ts_pes->stream_id == TS_H264_STREAM_ID)
    {
        TSbuf[ts_pos + 0] = (((0x3 << 4) | ((Videopts>> 29) & 0x0E) | 0x01) & 0xff);
        TSbuf[ts_pos + 1]= (((((Videopts >> 14) & 0xfffe) | 0x01) >> 8) & 0xff);
        TSbuf[ts_pos + 2]= ((((Videopts >> 14) & 0xfffe) | 0x01) & 0xff);
        TSbuf[ts_pos + 3]= (((((Videopts << 1) & 0xfffe) | 0x01) >> 8) & 0xff);
        TSbuf[ts_pos + 4]= ((((Videopts << 1) & 0xfffe) | 0x01) & 0xff);
        ts_pos += 5;

    }
    else if (ts_pes->stream_id == TS_AAC_STREAM_ID)
    {
        TSbuf[ts_pos + 0] = (((0x3 << 4) | ((Adudiopts>> 29) & 0x0E) | 0x01) & 0xff);
        TSbuf[ts_pos + 1]= (((((Adudiopts >> 14) & 0xfffe) | 0x01) >> 8) & 0xff);
        TSbuf[ts_pos + 2]= ((((Adudiopts >> 14) & 0xfffe) | 0x01) & 0xff);
        TSbuf[ts_pos + 3]= (((((Adudiopts << 1) & 0xfffe) | 0x01) >> 8) & 0xff);
        TSbuf[ts_pos + 4]= ((((Adudiopts << 1) & 0xfffe) | 0x01) & 0xff);
        ts_pos += 5;
    }
    else
    {
        TPLOG_ERROR("ts_pes->stream_id  error 0x%x \n",ts_pes->stream_id);
        //printf("ts_pes->stream_id  error 0x%x \n",ts_pes->stream_id);
        return 0;
    }

    NeafBuf = ts_pes->Es ;
    memcpy(TSbuf + ts_pos,NeafBuf,FirstPacketLoadLength);  

    NeafBuf += FirstPacketLoadLength;
    ts_pes->Pes_Packet_Length_Beyond -= FirstPacketLoadLength;
    //将包写入文件
    //test
    if (NULL == g_phTrans)
    {
        TPLOG_ERROR("g_phTrans is NULL");
        return 0;
    }
    int iret = SYSTRANS_InputData(g_phTrans, MULTI_DATA, TSbuf, 188);
    if (SYSTRANS_OK != iret)
    {
        TPLOG_ERROR("SYSTRANS_InputData failed:%d",iret);
    }
    WritePacketNum ++;                                                      //已经写入文件的包个数++

    while(ts_pes->Pes_Packet_Length_Beyond)
    {
        ts_pos = 0;
        memset(TSbuf,0,TS_PACKET_SIZE); 

        if ((WritePacketNum % 40) == 0)                                                         //每40个包打一个 pat,一个pmt
        {
            Write_Pat(m_One_Frame_Buf);                                                         //创建PAT
            Write_Pmt(m_One_Frame_Buf);                                                         //创建PMT
        }
        if(ts_pes->Pes_Packet_Length_Beyond >= 184)
        {
            //处理中间包   
            WriteStruct_Packetheader(TSbuf,Video_Audio_PID,0x00,0x01);     //PID = TS_H264_PID,不是有效荷载单元起始指示符_play_init = 0x00, ada_field_C,0x01,仅有有效负载；    
            ts_pos += 4;
            memcpy(TSbuf + ts_pos,NeafBuf,184); 
            NeafBuf += 184;
            ts_pes->Pes_Packet_Length_Beyond -= 184;
            //test
            if (NULL == g_phTrans)
            {
                TPLOG_ERROR("g_phTrans is NULL");
                return 0;
            }
            int iret = SYSTRANS_InputData(g_phTrans, MULTI_DATA, TSbuf, 188);
            if (SYSTRANS_OK != iret)
            {
                TPLOG_ERROR("SYSTRANS_InputData failed:%d",iret);
            }
        }
        else
        {
            if(ts_pes->Pes_Packet_Length_Beyond == 183||ts_pes->Pes_Packet_Length_Beyond == 182)
            {
                if ((WritePacketNum % 40) == 0)                                                         //每40个包打一个 pat,一个pmt
                {
                    Write_Pat(m_One_Frame_Buf);                                                         //创建PAT
                    Write_Pmt(m_One_Frame_Buf);                                                         //创建PMT
                }

                WriteStruct_Packetheader(TSbuf,Video_Audio_PID,0x00,0x03);   //PID = TS_H264_PID,不是有效荷载单元起始指示符_play_init = 0x00, ada_field_C,0x03,含有调整字段和有效负载；
                ts_pos += 4;
                TSbuf[ts_pos + 0] = 0x01;
                TSbuf[ts_pos + 1] = 0x00;
                ts_pos += 2;
                memcpy(TSbuf + ts_pos,NeafBuf,182); 

                NeafBuf += 182;
                ts_pes->Pes_Packet_Length_Beyond -= 182;
                //test
                if (NULL == g_phTrans)
                {
                    TPLOG_ERROR("g_phTrans is NULL");
                    return 0;
                }
                int iret = SYSTRANS_InputData(g_phTrans, MULTI_DATA, TSbuf, 188);
                if (SYSTRANS_OK != iret)
                {
                    TPLOG_ERROR("SYSTRANS_InputData failed:%d",iret);
                }
            }
            else
            {
                if ((WritePacketNum % 40) == 0)                                                         //每40个包打一个 pat,一个pmt
                {
                    Write_Pat(m_One_Frame_Buf);                                                         //创建PAT
                    Write_Pmt(m_One_Frame_Buf);                                                         //创建PMT
                }

                WriteStruct_Packetheader(TSbuf,Video_Audio_PID,0x00,0x03);  //PID = TS_H264_PID,不是有效荷载单元起始指示符_play_init = 0x00, ada_field_C,0x03,含有调整字段和有效负载；
                ts_pos += 4;
                TSbuf[ts_pos + 0] = 184-ts_pes->Pes_Packet_Length_Beyond-1 ;
                TSbuf[ts_pos + 1] = 0x00;
                ts_pos += 2;
                memset(TSbuf + ts_pos,0xFF,(184 - ts_pes->Pes_Packet_Length_Beyond - 2)); 
                ts_pos += (184-ts_pes->Pes_Packet_Length_Beyond-2);
                memcpy(TSbuf + ts_pos,NeafBuf,ts_pes->Pes_Packet_Length_Beyond);
                ts_pes->Pes_Packet_Length_Beyond = 0;
                //test
                if (NULL == g_phTrans)
                {
                    TPLOG_ERROR("g_phTrans is NULL");
                    return 0;
                }
                int iret = SYSTRANS_InputData(g_phTrans, MULTI_DATA, TSbuf, 188);
                if (SYSTRANS_OK != iret)
                {
                    TPLOG_ERROR("SYSTRANS_InputData failed:%d",iret);
                }
                WritePacketNum ++;  
            }
        }
        WritePacketNum ++;  
    }

    return WritePacketNum ;
}

int WriteAdaptive_flags_Head(Ts_Adaptation_field  * ts_adaptation_field,unsigned int Videopts)
{
    //填写自适应段
    ts_adaptation_field->discontinuty_indicator = 0;
    ts_adaptation_field->random_access_indicator = 0;
    ts_adaptation_field->elementary_stream_priority_indicator = 0;
    ts_adaptation_field->PCR_flag = 1;                                          //只用到这个
    ts_adaptation_field->OPCR_flag = 0;
    ts_adaptation_field->splicing_point_flag = 0;
    ts_adaptation_field->transport_private_data_flag = 0;
    ts_adaptation_field->adaptation_field_extension_flag = 0;

    //需要自己算
    ts_adaptation_field->pcr  = Videopts * 300;
    ts_adaptation_field->adaptation_field_length = 7;                          //占用7位

    ts_adaptation_field->opcr = 0;
    ts_adaptation_field->splice_countdown = 0;
    ts_adaptation_field->private_data_len = 0;
    return 1;
}
unsigned long m_Timestamp_audio = 0;
unsigned long m_Timestamp_video = 0;
void __stdcall AudioDataBack(BYTE *pDataInfo, long BufferLen, void *pUser)
{
    //int i = 0;
    //while (m_Timestamp_audio > m_Timestamp_video)
    //{
    //    Sleep(10);
    //    i++;
    //    if (i > 5)
    //    {
    //        return;
    //    }
    //}
    EnterCriticalSection(&(((CCameraDS*)pUser)->m_cs));
    //音频
    memcpy(m_audio_tspes.Es, pDataInfo, BufferLen);
    m_audio_tspes.Pes_Packet_Length_Beyond = BufferLen;
    Take_Out_Pes(&m_audio_tspes, m_Timestamp_audio, 0x01, NULL);
    if (m_audio_tspes.Pes_Packet_Length_Beyond > 0)
    {
        //printf("PES_AUDIO  :  SIZE = %d\n",m_audio_tspes.Pes_Packet_Length_Beyond);
        ////填写自适应段标志
        WriteAdaptive_flags_Tail(&ts_adaptation_field_Head); //填写自适应段标志  ,这里注意 音频类型不要算pcr 所以都用帧尾代替就行
        WriteAdaptive_flags_Tail(&ts_adaptation_field_Tail); //填写自适应段标志帧尾
        PES2TS(&m_audio_tspes,TS_AAC_PID ,&ts_adaptation_field_Head ,&ts_adaptation_field_Tail,m_Timestamp_video,m_Timestamp_audio);
        //计算一帧音频所用时间
        m_Timestamp_audio += 1024*1000* 90/audiosamplerate;
    }
    LeaveCriticalSection(&(((CCameraDS*)pUser)->m_cs));
    Sleep(15);
}
//EnterCriticalSection(m_pcs);
//m_phTrans(pBuffer, BufferLen, m_pUser);

void __stdcall VedioDataBack(BYTE *pDataInfo, long BufferLen, void *pUser)
{
    //int i = 0;
    //while (m_Timestamp_audio > m_Timestamp_video)
    //{
    //    Sleep(10);
    //    i++;
    //    if (i > 5)
    //    {
    //        return;
    //    }
    //}
    EnterCriticalSection(&(((CCameraDS*)pUser)->m_cs));
    memcpy(m_video_tspes.Es, pDataInfo, BufferLen);
    m_video_tspes.Pes_Packet_Length_Beyond = BufferLen;
    Take_Out_Pes(&m_video_tspes ,m_Timestamp_video,0x00,&videoframetype);
    if (m_video_tspes.Pes_Packet_Length_Beyond > 0)
    {
        if (videoframetype == FRAME_I || videoframetype == FRAME_P || videoframetype == FRAME_B)
        {
            //填写自适应段标志
            WriteAdaptive_flags_Head(&ts_adaptation_field_Head,m_Timestamp_video); //填写自适应段标志帧头
            WriteAdaptive_flags_Tail(&ts_adaptation_field_Tail); //填写自适应段标志帧尾
            //计算一帧视频所用时间
            PES2TS(&m_video_tspes,TS_H264_PID ,&ts_adaptation_field_Head ,&ts_adaptation_field_Tail,m_Timestamp_video,m_Timestamp_audio);
            m_Timestamp_video += 1000* 90/framerate;   //90khz
        }
        else
        {
            //填写自适应段标志
            WriteAdaptive_flags_Tail(&ts_adaptation_field_Head); //填写自适应段标志  ,这里注意 其它帧类型不要算pcr 所以都用帧尾代替就行
            WriteAdaptive_flags_Tail(&ts_adaptation_field_Tail); //填写自适应段标志帧尾
            PES2TS(&m_video_tspes,TS_H264_PID ,&ts_adaptation_field_Head ,&ts_adaptation_field_Tail,m_Timestamp_video,m_Timestamp_audio);
        }
    }
    LeaveCriticalSection(&(((CCameraDS*)pUser)->m_cs));
}

CCameraDS::CCameraDS():
m_bConnected(false)
,m_bInit(false)
,m_bFree(true)
,m_nWidth(0)
,m_nHeight(0)
,m_lState(State_Stopped)
,m_pGraph(NULL)
,m_pGraphAudio(NULL)
,m_pVedioDeviceFilter(NULL)
,m_pAudioDeviceFilter(NULL)
,m_pMediaControl(NULL)
,m_pAudioControl(NULL)
,m_pSampleGrabberFilterVedio(NULL)
,m_pSampleGrabberFilterAudio(NULL)
,m_pSampleGrabberVedio(NULL)
,m_pSampleGrabberAudio(NULL)
,m_pCaptureMJPEGOutputPin(NULL)
,m_pCapturePmcOutputPin(NULL)
,m_pGrabberInputVedio(NULL)
,m_pGrabberInputAudio(NULL)
,m_pGrabberOutputVedio(NULL)
,m_pGrabberOutputAudio(NULL)
,m_pCameraOutput(NULL)
//,m_pCameraH264(NULL)
//,m_pMediaEvent(NULL)
,m_pNullFilterVedio(NULL)
,m_pNullInputPinVedio(NULL)
,m_pNullInputPinAudio(NULL)
,m_pMjpegDecInput(NULL)
,m_pMjpegDecOutput(NULL)
,m_p264EncodeInput(NULL)
,m_p264EncodeOutput(NULL)
,m_phTrans(NULL)
{

}

CCameraDS::~CCameraDS()
{
    if (m_bInit && !m_bFree)
    {
        CloseCamera();
        CoUninitialize();
        m_bInit = false;
        m_bFree = true;
    }
}

/**	@fn	    Init
*	@brief	初始化函数
*	@param  
*	@return	
*/
int CCameraDS::Init()
{
    if (m_bInit)
    {
        return 0;
    }

    m_Timestamp_audio = 0;
    m_Timestamp_video = 0;
    int iRet = 0;

    iRet = m_GrabberCBAudio.Init();
    if(iRet < 0)
    {
        return iRet;
    }

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        TPLOG_ERROR("CoInitialize failed %d",hr);
        m_GrabberCBAudio.Fini();
        return hr;
    }


    //海康数据头
    HIK_MEDIAINFO head;
    memset(&head, 0, sizeof(HIK_MEDIAINFO));
    head.media_fourcc = 0x484B4D49;
    head.media_version = 0x0101;
    head.device_id =0x0000;
    head.system_format = 0x0003;  //无封装
    head.video_format = 0x0100; // 视频编码类型 0x0100 标准H.264 0x0003 标准MPEG4; 
    head.audio_format = 0x2001;//aac格式
    head.audio_channels = 2;
    head.audio_samplesrate = 44100;
    //head.audio_bits_per_sample = 16;
    //head.audio_bitrate = 64000;

    //封装ps流设置信息
    SYS_TRANS_PARA stTransPara;
    memset(&stTransPara, 0, sizeof(stTransPara));
    stTransPara.pSrcInfo = (unsigned char *)&head;
    stTransPara.dwSrcInfoLen = 40;
    stTransPara.dwTgtPackSize = 4096;
    stTransPara.enTgtType = TRANS_SYSTEM_MPEG2_PS;

    iRet = SYSTRANS_Create(&m_phTrans, &stTransPara);
    if (iRet)
    {
        TPLOG_ERROR("SYSTRANS_Create failed%d",iRet);
        CoUninitialize();
        m_GrabberCBAudio.Fini();
        return -1;
    }
    g_phTrans = m_phTrans; //ps流封装库句柄
    InitializeCriticalSection(&m_cs);

    m_bInit = true;
    m_bFree = false;
    return 0;
}

/**	@fn	    Fini
*	@brief	反初始化函数
*	@param  
*	@return	
*/
int CCameraDS::Fini()
{
    if (m_bFree)
    {
        return 0;
    }

    m_Timestamp_audio = 0;
    m_Timestamp_video = 0;
    CloseCamera();
    CoUninitialize();
    SYSTRANS_Release(m_phTrans);
    g_phTrans = NULL;
    WritePacketNum = 0;
    m_GrabberCBAudio.Fini();
    DeleteCriticalSection(&m_cs);
    m_bInit = false;
    m_bFree = true;
    return 0;
}

/**	@fn	    CloseCamera
*	@brief	关闭摄像头
*	@param  
*	@return	
*/
void CCameraDS::CloseCamera()
{
    if(m_bConnected)
    {
        if (NULL != m_pMediaControl)
        {
            m_pMediaControl->Stop();
        }

        if (NULL != m_pAudioControl)
        {
            m_pAudioControl->Stop();
        }
    }

    m_pGraph = NULL;
    m_pGraphAudio = NULL;
    m_pVedioDeviceFilter = NULL;
    m_pAudioDeviceFilter = NULL;
    m_pMediaControl = NULL;
    m_pAudioControl = NULL;
    m_pSampleGrabberFilterVedio = NULL;
    m_pSampleGrabberFilterAudio = NULL;
    m_pSampleGrabberVedio = NULL;
    m_pSampleGrabberAudio = NULL;
    m_pCaptureMJPEGOutputPin = NULL;
    m_pCapturePmcOutputPin = NULL;
    m_pGrabberInputVedio = NULL;
    m_pGrabberInputAudio = NULL;
    m_pGrabberOutputVedio = NULL;
    m_pGrabberOutputAudio = NULL;
    m_pCameraOutput = NULL;
    m_pNullFilterVedio = NULL;
    m_pNullInputPinVedio = NULL;
    m_pNullInputPinAudio = NULL;
    m_pMjpegDecInput = NULL;
    m_pMjpegDecOutput = NULL;
    m_p264EncodeInput = NULL;
    m_p264EncodeOutput = NULL;

    m_bConnected = false;
    m_nWidth = 0;
    m_nHeight = 0;
}

/**	@fn	    OpenCamera
*	@brief	打开摄像头
*	@param  [in] nCamID 摄像头编号
*	@param  [in] uiMicIndex 麦克风索引号
*	@param  [in] strMicName 麦克风名字
*	@param  [in] stProperty 分辨率等属性
*	@return	long，大于等于零 表示没有错误，小于零表示出现了错误
*/
long CCameraDS::OpenCamera(unsigned int nCamID,
        unsigned int uiMicIndex,
        const std::string &strMicName,
        const PICTURE_PROPERTY_ST &stProperty)
{
    TPLOG_INFO("OpenCamera nCamID:%d,uiMicIndex:%d,stProperty:%d %d.",nCamID,uiMicIndex,stProperty.uiWidth, stProperty.uiHeight);
    CloseCamera();
    if (nCamID < 0 || uiMicIndex < 0)
    {
        TPLOG_ERROR("设备索引非法 nCamID:%d, uiMicIndex:%d\n", nCamID, uiMicIndex);
        return -1;
    }

    long lRet = 0;
    lRet = InitUsbCamera(nCamID, stProperty);
    if (lRet < 0)
    {
        CloseCamera();
        return lRet;
    }
    lRet = InitMicrophone(uiMicIndex, strMicName);
    if (lRet < 0)
    {
        CloseCamera();
        return lRet;
    }

    //链接pin
    lRet = ConnectPins(stProperty);
    if (lRet < 0)
    {
        CloseCamera();
        return lRet;
    }

    return 0;
}

/**	@fn	    InitUsbCamera
*	@brief	打开摄像头
*	@param  [in] nCamID 摄像头编号
*	@param  [in] stProperty 分辨率等属性
*	@return	long，大于等于零 表示没有错误，小于零表示出现了错误
*/
long CCameraDS::InitUsbCamera(unsigned int nCamID, const PICTURE_PROPERTY_ST &stProperty)
{
    TPLOG_INFO("调用 InitUsbCamera");
    if (nCamID < 0)
    {
        TPLOG_ERROR("nCamID 不合法:%d\n",nCamID);
        return -1;
    }

    HRESULT hr = S_OK;

    // Create the Filter Graph Manager.
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
                            IID_IGraphBuilder, (void **)&m_pGraph);
    if (FAILED(hr))
    {
        TPLOG_ERROR("CoCreateInstance CLSID_FilterGraph failed %d",hr);
        return hr;
    }

    //创建SampleGrabber Filter
    hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, 
                            IID_IBaseFilter, (LPVOID *)&m_pSampleGrabberFilterVedio);
    if (FAILED(hr))
    {
        TPLOG_ERROR("CoCreateInstance CLSID_SampleGrabber failed %d",hr);
        return hr;
    }

    hr = m_pGraph->QueryInterface(IID_IMediaControl, (void **) &m_pMediaControl);
    if (FAILED(hr))
    {
        TPLOG_ERROR("QueryInterface IID_IMediaControl failed %d",hr);
        return hr;
    }

    //创建Renderer Filter
    hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
                            IID_IBaseFilter, (LPVOID*) &m_pNullFilterVedio);
    if (FAILED(hr))
    {
        TPLOG_ERROR("CoCreateInstance CLSID_NullRenderer failed %d",hr);
        return hr;
    }

    //添加设备Renderer Filter到Graph
    hr = m_pGraph->AddFilter(m_pNullFilterVedio, L"NullRenderer");
    if (FAILED(hr))
    {
        TPLOG_ERROR("AddFilter NullRenderer failed %d",hr);
        return hr;
    }

    hr = m_pSampleGrabberFilterVedio->QueryInterface(IID_ISampleGrabber, (void**)&m_pSampleGrabberVedio);
    if (FAILED(hr))
    {
        TPLOG_ERROR("QueryInterface IID_ISampleGrabber failed %d",hr);
        return hr;
    }
    //添加设备SampleGrabber Filter到Graph
    hr = m_pGraph->AddFilter(m_pSampleGrabberFilterVedio, L"Grabber");
    if (FAILED(hr))
    {
        TPLOG_ERROR("AddFilter Grabber failed %d",hr);
        return hr;
    }
 
    // 添加设备filter
    hr = BindVedioFilter(nCamID, &m_pVedioDeviceFilter);
    if (FAILED(hr))
    {
        return hr;
    }

    //添加设备filter到Graph
    hr = m_pGraph->AddFilter(m_pVedioDeviceFilter, L"Audio Capture");
    if (FAILED(hr))
    {
        TPLOG_ERROR("AddFilter m_pVedioDeviceFilter failed %d",hr);
        return hr;
    }

    //调整 osd 时间
    if (!H264_XU_SetOSDRTC(m_pVedioDeviceFilter))
    {
        TPLOG_ERROR("H264_XU_SetOSDRTC failed");
        //return -1;
    }

    // 添加MJPEG解码器滤镜
    hr = CoCreateInstance(CLSID_MjpegDec, NULL, CLSCTX_INPROC_SERVER, 
        IID_IBaseFilter, 
        (void**)&m_pMjpegDecompress);

    if(FAILED(hr))
    {
        return hr;
    }
    hr = m_pGraph->AddFilter(m_pMjpegDecompress, TEXT("Mjpeg Decompress"));
    if(FAILED(hr))
    {
        TPLOG_ERROR("AddFilter m_pMjpegDecompress failed %d",hr);
        return hr;
    }

        //添加h264编码器滤镜
    CLSID CLSID_H264ENCODER = {0x950645d2,0xe74c,0x49d8,{0xb4, 0x97, 0x44, 0xc4, 0xa4, 0xfd, 0x69, 0xa1}};
    hr=CoCreateInstance(CLSID_H264ENCODER,NULL,CLSCTX_INPROC,IID_IBaseFilter,(void**)&m_pH264Encoder);

    if(FAILED(hr))
    {
        TPLOG_ERROR("加载264编码滤镜失败 %d",hr);
        //AfxMessageBox(TEXT("Instance PushSource Desktop Filter failed"));
        return -1;
    }
    hr = m_pGraph->AddFilter(static_cast<IBaseFilter*>(m_pH264Encoder), TEXT("H264 Encoder"));
    if(FAILED(hr))
    {
        TPLOG_ERROR("添加m_pH264Encoder滤镜失败 %d",hr);
        return hr;
    }
    ////添加h264编码器滤镜
    //IClassFactory *pClassFactory = NULL;
    //LPCWSTR H264EncoderName = _T("H264Encoder.ax");

    //CLSID CLSID_H264ENCODER = {0x950645d2,0xe74c,0x49d8,{0xb4, 0x97, 0x44, 0xc4, 0xa4, 0xfd, 0x69, 0xa1}};
    //HMODULE hInstLibrary = LoadLibraryEx(H264EncoderName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    //if(hInstLibrary)
    //{
    //    g_DllGetClassObject = (Func)GetProcAddress(hInstLibrary, "DllGetClassObject");
    //    if(g_DllGetClassObject)
    //    {
    //        g_DllGetClassObject(CLSID_H264ENCODER, IID_IClassFactory, (void**)&pClassFactory);
    //        hr = pClassFactory->CreateInstance(0, IID_IBaseFilter, (void**)&m_pH264Encoder);
    //        if(SUCCEEDED(hr))  // 成功
    //        {
    //            hr = m_pGraph->AddFilter(static_cast<IBaseFilter*>(m_pH264Encoder), TEXT("H264 Encoder"));
    //            if(FAILED(hr))
    //            {
    //                TPLOG_ERROR("添加m_pH264Encoder滤镜失败");
    //                return hr;
    //            }
    //        }
    //    }
    //    else
    //    {
    //        TPLOG_ERROR("提取264编码滤镜的DllGetClassObject失败");
    //        return -1;
    //    }
    //}
    //else
    //{
    //    TPLOG_ERROR("加载264编码滤镜失败");
    //    return -1;
    //}

    return 0;
}

/**	@fn	    InitMicrophone
*	@brief	打开麦克风
*	@param  [in] uiMicIndex 麦克风索引号
*	@param  [in] strMicName 麦克风名字
*	@return	long，大于等于零 表示没有错误，小于零表示出现了错误
*/
long CCameraDS::InitMicrophone(unsigned int uiMicIndex, const std::string &strMicName)
{
    TPLOG_INFO("调用 InitMicrophone");
    if (uiMicIndex < 0)
    {
        TPLOG_ERROR("nCamID 不合法:%d\n",uiMicIndex);
        return -1;
    }

    HRESULT hr = S_OK;

    // Create the Filter Graph Manager.
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
        IID_IGraphBuilder, (void **)&m_pGraphAudio);
    if (FAILED(hr))
    {
        TPLOG_ERROR("CoCreateInstance CLSID_FilterGraph failed %d",hr);
        return hr;
    }

    //创建SampleGrabber Filter
    hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, 
        IID_IBaseFilter, (LPVOID *)&m_pSampleGrabberFilterAudio);
    if (FAILED(hr))
    {
        TPLOG_ERROR("CoCreateInstance CLSID_SampleGrabber failed %d",hr);
        return hr;
    }

    hr = m_pGraphAudio->QueryInterface(IID_IMediaControl, (void **) &m_pAudioControl);
    if (FAILED(hr))
    {
        TPLOG_ERROR("QueryInterface IID_IMediaControl failed %d",hr);
        return hr;
    }

    //创建Renderer Filter
    hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
        IID_IBaseFilter, (LPVOID*) &m_pNullFilterAudio);
    if (FAILED(hr))
    {
        TPLOG_ERROR("CoCreateInstance CLSID_NullRenderer failed %d",hr);
        return hr;
    }

    //添加设备Renderer Filter到Graph
    hr = m_pGraphAudio->AddFilter(m_pNullFilterAudio, L"NullRenderer");
    if (FAILED(hr))
    {
        TPLOG_ERROR("AddFilter NullRenderer failed %d",hr);
        return hr;
    }

    hr = m_pSampleGrabberFilterAudio->QueryInterface(IID_ISampleGrabber, (void**)&m_pSampleGrabberAudio);
    if (FAILED(hr))
    {
        TPLOG_ERROR("QueryInterface m_pSampleGrabberFilterAudio failed %d",hr);
        return hr;
    }
    //添加设备SampleGrabber Filter到Graph
    hr = m_pGraphAudio->AddFilter(m_pSampleGrabberFilterAudio, L"Grabber");
    if (FAILED(hr))
    {
        TPLOG_ERROR("AddFilter Grabber failed %d",hr);
        return hr;
    }

    // 添加设备filter
    hr = BindAudioFilter(uiMicIndex, &m_pAudioDeviceFilter);
    if (FAILED(hr))
    {
        return hr;
    }

    //添加设备filter到Graph
    hr = m_pGraphAudio->AddFilter(m_pAudioDeviceFilter, L"Audio Capture");
    if (FAILED(hr))
    {
        TPLOG_ERROR("AddFilter m_pAudioDeviceFilter failed %d",hr);
        return hr;
    }

    //创建Renderer Filter
    hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
        IID_IBaseFilter, (LPVOID*) &m_pNullFilterAudio);
    if (FAILED(hr))
    {
        TPLOG_ERROR("CoCreateInstance CLSID_NullRenderer failed %d",hr);
        return hr;
    }

    //添加设备Renderer Filter到Graph
    hr = m_pGraphAudio->AddFilter(m_pNullFilterAudio, L"NullRenderer");
    if (FAILED(hr))
    {
        TPLOG_ERROR("AddFilter m_pNullFilterAudio failed %d",hr);
        return hr;
    }

    return 0;
}

/**	@fn	    ConnectPins
*	@brief	链接PIN
*	@return	long，大于等于零 表示没有错误，小于零表示出现了错误
*/
long CCameraDS::ConnectPins(const PICTURE_PROPERTY_ST &stProperty)
{
    TPLOG_INFO("调用 ConnectPins");
    HRESULT hr = S_OK;

    //获取SampleGrabber PIN 视频
    CComPtr<IEnumPins> pEnum;
    pEnum = NULL; 
    m_pSampleGrabberFilterVedio->EnumPins(&pEnum);
    pEnum->Reset();
    hr = pEnum->Next(1, &m_pGrabberInputVedio, NULL);
    if (FAILED(hr))
    {
        TPLOG_ERROR("get pin m_pGrabberInputVedio failed %d",hr);
        return hr;
    }
    pEnum = NULL;
    m_pSampleGrabberFilterVedio->EnumPins(&pEnum);
    pEnum->Reset();
    pEnum->Skip(1);
    hr = pEnum->Next(1, &m_pGrabberOutputVedio, NULL);
    if (FAILED(hr))
    {
        TPLOG_ERROR("get pin m_pGrabberInputVedio failed %d",hr);
        return hr;
    }

    //获取SampleGrabber PIN 音频
    pEnum = NULL; 
    m_pSampleGrabberFilterAudio->EnumPins(&pEnum);
    pEnum->Reset();
    hr = pEnum->Next(1, &m_pGrabberInputAudio, NULL);
    if (FAILED(hr))
    {
        TPLOG_ERROR("get pin m_pGrabberInputAudio failed %d",hr);
        return hr;
    }
    pEnum = NULL;
    m_pSampleGrabberFilterAudio->EnumPins(&pEnum);
    pEnum->Reset();
    pEnum->Skip(1);
    hr = pEnum->Next(1, &m_pGrabberOutputAudio, NULL);
    if (FAILED(hr))
    {
        TPLOG_ERROR("get pin m_pGrabberOutputAudio failed %d",hr);
        return hr;
    }
    

    //获取vedio Render PIN
    pEnum = NULL;
    m_pNullFilterVedio->EnumPins(&pEnum);
    pEnum->Reset();
    hr = pEnum->Next(1, &m_pNullInputPinVedio, NULL);
    if (FAILED(hr))
    {
        TPLOG_ERROR("get pin m_pNullInputPinVedio failed %d",hr);
        return hr;
    }


    //获取Audio Render PIN
    pEnum = NULL;
    m_pNullFilterAudio->EnumPins(&pEnum);
    pEnum->Reset();
    hr = pEnum->Next(1, &m_pNullInputPinAudio, NULL);
    if (FAILED(hr))
    {
        TPLOG_ERROR("get pin m_pNullInputPinAudio failed %d",hr);
        return hr;
    }


    //获取mjpeg解码器 PIN
    pEnum = NULL; 
    m_pMjpegDecompress->EnumPins(&pEnum);
    pEnum->Reset();
    hr = pEnum->Next(1, &m_pMjpegDecInput, NULL);
    if (FAILED(hr))
    {
        TPLOG_ERROR("get pin m_pMjpegDecInput failed %d",hr);
        return hr;
    }
    pEnum = NULL;
    m_pMjpegDecompress->EnumPins(&pEnum);
    pEnum->Reset();
    pEnum->Skip(1);
    hr = pEnum->Next(1, &m_pMjpegDecOutput, NULL);
    if (FAILED(hr))
    {
        TPLOG_ERROR("get pin m_pMjpegDecOutput failed %d",hr);
        return hr;
    }

    //获取264编码器 PIN
    pEnum = NULL; 
    m_pH264Encoder->EnumPins(&pEnum);
    pEnum->Reset();
    hr = pEnum->Next(1, &m_p264EncodeInput, NULL);
    if (FAILED(hr))
    {
        TPLOG_ERROR("get pin m_p264EncodeInput failed %d",hr);
        return hr;
    }
    pEnum = NULL;
    m_pH264Encoder->EnumPins(&pEnum);
    pEnum->Reset();
    pEnum->Skip(1);
    hr = pEnum->Next(1, &m_p264EncodeOutput, NULL);
    if (FAILED(hr))
    {
        TPLOG_ERROR("get pin m_p264EncodeOutput failed %d",hr);
        return hr;
    }

    //获取视频设备数据输出pin
    hr = m_pVedioDeviceFilter->FindPin(L"0", &m_pCaptureMJPEGOutputPin);
    if (FAILED(hr))
    {
        TPLOG_ERROR("Find pin m_pCaptureMJPEGOutputPin failed %d",hr);
        return hr;
    }

    //设置分辨率
    IAMStreamConfig *pVSC;      // for video cap
    hr = m_pCaptureMJPEGOutputPin->QueryInterface(IID_IAMStreamConfig, (void **)&pVSC);
    if (FAILED(hr))
    {
        TPLOG_ERROR("QueryInterface IID_IAMStreamConfig failed %d",hr);
        return hr;
    }
    AM_MEDIA_TYPE *pmt;
    if (S_OK == pVSC->GetFormat(&pmt))
    {
        HEADER(pmt->pbFormat)->biWidth = stProperty.uiWidth;
        HEADER(pmt->pbFormat)->biHeight = stProperty.uiHeight;
        hr = pVSC->SetFormat(pmt);
        if (FAILED(hr))
        {
            TPLOG_ERROR("SetFormat failed.Width:%d,Height:%d %d",stProperty.uiWidth,stProperty.uiHeight,hr);
            return hr;
        }
    }
    pVSC->Release();


    //获取音频设备数据输出pin
    pEnum = NULL;
    m_pAudioDeviceFilter->EnumPins(&pEnum);
    pEnum->Reset();
    hr = pEnum->Next(1, &m_pCapturePmcOutputPin, NULL);
    if (FAILED(hr))
    {
        TPLOG_ERROR("get pin m_pCapturePmcOutputPin failed %d",hr);
        return hr;
    }

    //IAMStreamConfig *pVSC1;      // for video cap
    //hr = m_pCapturePmcOutputPin->QueryInterface(IID_IAMStreamConfig, (void **)&pVSC1);
    //if (FAILED(hr))
    //{
    //    TPLOG_ERROR("QueryInterface IID_IAMStreamConfig failed");
    //    return hr;
    //}
    //AM_MEDIA_TYPE *pmt1;
    //if (S_OK == pVSC1->GetFormat(&pmt1))
    //{

    //}
    //pVSC1->Release();

    //链接PINs
    hr = m_pGraph->Connect(m_pCaptureMJPEGOutputPin, m_pMjpegDecInput);
    if (FAILED(hr))
    {
        TPLOG_ERROR("Connect pin m_pCaptureMJPEGOutputPin m_pMjpegDecInput failed %d",hr);
        return hr;
    }

    hr = m_pGraph->Connect(m_pMjpegDecOutput, m_p264EncodeInput);
    if (FAILED(hr))
    {
        TPLOG_ERROR("Connect pin m_pMjpegDecOutput m_p264EncodeInput failed %d",hr);
        return hr;
    }

    hr = m_pGraph->Connect(m_p264EncodeOutput, m_pGrabberInputVedio);
    if (FAILED(hr))
    {
        TPLOG_ERROR("Connect pin m_p264EncodeOutput m_pGrabberInputVedio failed %d",hr);
        return hr;
    }

    hr = m_pGraph->Connect(m_pGrabberOutputVedio, m_pNullInputPinVedio);
    if (FAILED(hr))
    {
        TPLOG_ERROR("Connect pin m_pGrabberOutputVedio m_pNullInputPinVedio failed %d",hr);
        return hr;
    }

    hr = m_pGraphAudio->Connect(m_pCapturePmcOutputPin, m_pGrabberInputAudio);
    if (FAILED(hr))
    {
        TPLOG_ERROR("Connect pin m_pCaptureMJPEGOutputPin m_pMjpegDecInput failed %d",hr);
        return hr;
    }

    hr = m_pGraphAudio->Connect(m_pGrabberOutputAudio, m_pNullInputPinAudio);
    if (FAILED(hr))
    {
        TPLOG_ERROR("Connect pin m_pGrabberOutputAudio m_pNullInputPinVedio failed %d",hr);
        return hr;
    }

    hr = m_pSampleGrabberVedio->SetBufferSamples(FALSE);
    if (FAILED(hr))
    {
        TPLOG_ERROR("SetBufferSamples failed %d",hr);
        return hr;
    }
    hr = m_pSampleGrabberVedio->SetOneShot(FALSE);
    if (FAILED(hr))
    {
        TPLOG_ERROR("SetOneShot failed %d",hr);
        return hr;
    }
    hr = m_pSampleGrabberVedio->SetCallback(&m_GrabberCBVedio,1);
    if (FAILED(hr))
    {
        TPLOG_ERROR("SetCallback failed %d",hr);
        return hr;
    }

    AM_MEDIA_TYPE   mt;
    ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
    MYFREEMEDIATYPE(mt);

    hr = m_pSampleGrabberVedio->GetConnectedMediaType(&mt);
    if (FAILED(hr))
    {
        TPLOG_ERROR("GetConnectedMediaType failed %d",hr);
        return hr;
    }

    VIDEOINFOHEADER *videoHeader;
    videoHeader = reinterpret_cast<VIDEOINFOHEADER*>(mt.pbFormat);
    if (NULL == videoHeader)
    {
        TPLOG_ERROR("videoHeader is NULL!");
        return -1;
    }
    m_nWidth = videoHeader->bmiHeader.biWidth;
    m_nHeight = videoHeader->bmiHeader.biHeight;

    hr = m_pSampleGrabberAudio->SetBufferSamples(FALSE);
    if (FAILED(hr))
    {
        TPLOG_ERROR("SetBufferSamples failed %d",hr);
        return hr;
    }
    hr = m_pSampleGrabberAudio->SetOneShot(FALSE);
    if (FAILED(hr))
    {
        TPLOG_ERROR("SetOneShot failed %d",hr);
        return hr;
    }
    hr = m_pSampleGrabberAudio->SetCallback(&m_GrabberCBAudio,1);
    if (FAILED(hr))
    {
        TPLOG_ERROR("SetCallback failed %d",hr);
        return hr;
    }

    m_bConnected = true;

    pEnum = NULL;
    TPLOG_INFO("open camera success!");

    return 0;
}

/**	@fn	    BindVedioFilter
*	@brief	绑定各个滤镜
*	@param  [in] nCamIDX 摄像头编号
*	@param  [in] pFilter 滤镜
*	@return	
*/
long CCameraDS::BindVedioFilter(int nCamID, IBaseFilter **pFilter)
{
    TPLOG_INFO("调用 BindVedioFilter");
    if (nCamID < 0)
    {
        TPLOG_ERROR("nCamID 不合法:%d\n",nCamID);
        return -1;
    }

    // enumerate all video capture devices
    CComPtr<ICreateDevEnum> pCreateDevEnum;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
                                IID_ICreateDevEnum, (void**)&pCreateDevEnum);
    if (hr != NOERROR)
    {
        return hr;
    }

    CComPtr<IEnumMoniker> pEm;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
        &pEm, 0);
    if (hr != NOERROR) 
    {
        return hr;
    }

    if (NULL == pEm)
    {
        TPLOG_ERROR("pEm is NULL!");
        return -1;
    }

    pEm->Reset();
    ULONG cFetched;
    IMoniker *pM;
    int index = 0;
    while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK, index <= nCamID)
    {
        IPropertyBag *pBag;
        hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
        if(SUCCEEDED(hr)) 
        {
            VARIANT var;
            var.vt = VT_BSTR;
            hr = pBag->Read(L"FriendlyName", &var, NULL);
            if (hr == NOERROR) 
            {
                if (index == nCamID)
                {
                    pM->BindToObject(0, 0, IID_IBaseFilter, (void**)pFilter);
                }
                SysFreeString(var.bstrVal);
            }
            else
            {
                TPLOG_ERROR("Read FriendlyName failed! %d", hr);
            }
            pBag->Release();
        }
        pM->Release();
        index++;
    }

    pCreateDevEnum = NULL;
    return 0;
}

/**	@fn	    BindAudioFilter
*	@brief	添加声音设备滤镜
*	@param  [in] nCamIDX 麦克风编号
*	@param  [in] pFilter 滤镜
*	@return	
*/
long CCameraDS::BindAudioFilter(int nCamID, IBaseFilter **pFilter)
{
    TPLOG_INFO("调用 BindAudioFilter");
    if (nCamID < 0)
    {
        TPLOG_ERROR("nCamID 不合法:%d\n",nCamID);
        return -1;
    }

    // enumerate all video capture devices
    CComPtr<ICreateDevEnum> pCreateDevEnum;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
        IID_ICreateDevEnum, (void**)&pCreateDevEnum);
    if (hr != NOERROR)
    {
        return hr;
    }

    CComPtr<IEnumMoniker> pEm;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory,
        &pEm, 0);
    if (hr != NOERROR) 
    {
        return hr;
    }

    if (NULL == pEm)
    {
        TPLOG_ERROR("pEm is NULL!");
        return -1;
    }

    pEm->Reset();
    ULONG cFetched;
    IMoniker *pM;
    int index = 0;
    while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK, index <= nCamID)
    {
        IPropertyBag *pBag;
        hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
        if(SUCCEEDED(hr)) 
        {
            VARIANT var;
            var.vt = VT_BSTR;
            hr = pBag->Read(L"FriendlyName", &var, NULL);
            if (hr == NOERROR) 
            {
                if (index == nCamID)
                {
                    pM->BindToObject(0, 0, IID_IBaseFilter, (void**)pFilter);
                }
                SysFreeString(var.bstrVal);
            }
            else
            {
                TPLOG_ERROR("Read FriendlyName failed! %d", hr);
            }
            pBag->Release();
        }
        pM->Release();
        index++;
    }

    pCreateDevEnum = NULL;
    return 0;
}

/**	@fn	    GetState
*	@brief	获取取数据流当前状态
*	@param  [out] lState 数据流当前状态
*	@return	大于等于零 表示没有错误，小于零表示出现了错误
*/
int CCameraDS::GetState(long &lState)
{
    HRESULT hr = S_OK;
    hr = m_pMediaControl->GetState(100, &lState);
    if (FAILED(hr))
    {
        TPLOG_ERROR("GetState failed:%d",hr);
        return hr;
    }
    m_lState = lState;
    return 0;
}

/**	@fn	    GetPictureInfo
*	@brief	获取当前获取的数据图像属性信息
*	@param  [out] stProperty 图像属性信息
*	@return	大于等于零 表示没有错误，小于零表示出现了错误
*/
int CCameraDS::GetPictureInfo(PICTURE_PROPERTY_ST &stProperty)
{
    stProperty.uiHeight = (unsigned int)m_nHeight;
    stProperty.uiWidth = (unsigned int)m_nWidth;
    return 0;
}

/**	@fn	    Run
*	@brief	开始取数据流
*	@param
*	@return	大于等于零 表示没有错误，小于零表示出现了错误
*/
HRESULT CCameraDS::Run()
{
    TPLOG_INFO("调用 Run");
    HRESULT hr = S_OK;

    if( !m_pMediaControl )
    {
        return E_UNEXPECTED;
    }

    hr = m_pMediaControl->Run();
    if (FAILED(hr))
    {
        TPLOG_ERROR("Run failed:%d",hr);
        return hr;
    }

    if( !m_pAudioControl )
    {
        TPLOG_ERROR("m_pAudioControl is NULL");
        return E_UNEXPECTED;
    }
    hr = m_pAudioControl->Run();
    if (FAILED(hr))
    {
        TPLOG_ERROR("Run failed:%d",hr);
        return hr;
    }

    //获取当前系统时间
    time_t rawtime; 
    struct tm * timeinfo; 
    time ( &rawtime ); 
    timeinfo = localtime ( &rawtime ); 	

    HK_SYSTEM_TIME stHkTime;
    // localtime: current_year = tm_year + 1900;
    stHkTime.dwYear        = timeinfo->tm_year + 1900;
    // localtime: tm_mon (0 ~ 11; January = 0 )
    stHkTime.dwMonth       = timeinfo->tm_mon + 1;
    stHkTime.dwDay         = timeinfo->tm_mday;
    stHkTime.dwHour        = timeinfo->tm_hour;
    stHkTime.dwMinute      = timeinfo->tm_min;
    stHkTime.dwSecond      = timeinfo->tm_sec;
    stHkTime.dwMilliSecond = 0;

    SYSTRANS_SetGlobalTime(m_phTrans, &stHkTime);


    int iRet = 0;
    iRet = SYSTRANS_Start(m_phTrans, NULL, NULL);
    if(iRet)
    {
        TPLOG_ERROR("SYSTRANS_Start failed%d",iRet);
        return -1;
    }

    //hr = m_GrabberCBAudio.Run();
    //if (hr)
    //{
    //    TPLOG_ERROR("m_GrabberCBAudio Run failed:%d",hr);
    //    return hr;
    //}

    m_lState = State_Running;
    return hr;
}

/**	@fn	    Pause
*	@brief	暂停取数据流
*	@param
*	@return	大于等于零 表示没有错误，小于零表示出现了错误
*/
HRESULT CCameraDS::Pause()
{
TPLOG_INFO("调用 Pause");
    HRESULT hr = S_OK;
    hr = m_pMediaControl->Pause();
    if (FAILED(hr))
    {
        TPLOG_ERROR("Pause failed:%d",hr);
        return hr;
    }

    hr = m_pAudioControl->Pause();
    if (FAILED(hr))
    {
        TPLOG_ERROR("Pause failed:%d",hr);
        return hr;
    }
    
    m_lState = State_Paused;
    return 0;
}

/**	@fn	    Stop
*	@brief	停止取数据流
*	@param
*	@return	大于等于零 表示没有错误，小于零表示出现了错误
*/
HRESULT CCameraDS::Stop()
{
TPLOG_INFO("调用 Stop");
    HRESULT hr = S_OK;
    SYSTRANS_Stop(m_phTrans);
	if (m_pMediaControl == NULL)
	{
		return 0;
	}
    hr = m_pMediaControl->Stop();
    if (FAILED(hr))
    {
        TPLOG_ERROR("Stop failed:%d",hr);
        return hr;
    }

    hr = m_pAudioControl->Stop();
    if (FAILED(hr))
    {
        TPLOG_ERROR("Stop failed:%d",hr);
        return hr;
    }

    m_Timestamp_audio = 0;
    m_Timestamp_video = 0;
    m_lState = State_Stopped;
    return 0;
}

/**	@fn	    SetDataCallback
*	@brief	设置数据流回调函数
*	@param  [in] datafun 回调函数
*	@param  [in] pUser 用户信息
*	@return	大于等于零 表示没有错误，小于零表示出现了错误
*/
long CCameraDS::SetDataCallback(const pCameraDSDataBack datafun, void *pUser)
{
    TPLOG_INFO("调用 SetDataCallback");
    if (NULL == m_phTrans)
    {
        TPLOG_ERROR("m_phTrans is NULL");
        return -1;
    }
    int iRet = SYSTRANS_RegisterOutputDataCallBack(m_phTrans, datafun, pUser);
    if (iRet)
    {
        TPLOG_ERROR("SYSTRANS_RegisterOutputDataCallBack failed:%d", iRet);
        return -1;
    }
    m_GrabberCBVedio.SetDataCB(VedioDataBack, &m_cs, (void*)this);
    m_GrabberCBAudio.SetDataCB(AudioDataBack, &m_cs, (void*)this);
    return 0;
}

/**	@fn	    GetAllCameraInfo
*	@brief	获取所有摄像头信息
*	@param  [out] lstCameraInfo 摄像头信息列表
*	@return	int，大于等于零 表示没有错误，小于零表示出现了错误
*/
int CCameraDS::GetAllCameraInfo(std::list<CAMERA_INFO_ST> &lstCameraInfo, std::list<MICOR_INFO_ST> &lstMicInfo)
{
    int nCount = 0;
    char sName[1024] = {0};
    int nBufferSize = sizeof(sName);
   // enumerate all video capture devices
    CComPtr<ICreateDevEnum> pCreateDevEnum;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
                                    IID_ICreateDevEnum, (void**)&pCreateDevEnum);
    if (FAILED(hr))
    {
        TPLOG_ERROR("CoCreateInstance CLSID_SystemDeviceEnum failed:%d", hr);
        return hr;
    }

    CComPtr<IEnumMoniker> pEm;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
        &pEm, 0);
    if (FAILED(hr))
    {
        TPLOG_ERROR("CreateClassEnumerator CLSID_VideoInputDeviceCategory failed:%d", hr);
        return hr;
    }

    if (NULL == pEm)
    {
        TPLOG_ERROR("pEm is NULL!");
        return -1;
    }
    pEm->Reset();
    ULONG cFetched;
    IMoniker *pM;
    while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)
    {
        IPropertyBag *pBag=0;
        CAMERA_INFO_ST stCamera;
        hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
        if(SUCCEEDED(hr))
        {
            VARIANT var;
            var.vt = VT_BSTR;
            hr = pBag->Read(L"FriendlyName", &var, NULL); //还有其他属性,像描述信息等等
            if(hr == NOERROR)
            {
                //获取设备名称      
                WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, sName, nBufferSize , "", NULL);
                SysFreeString(var.bstrVal);
                stCamera.uiIndex = nCount;
                stCamera.strName = (std::string)sName;
                lstCameraInfo.push_back(stCamera);
            }
            else
            {
                TPLOG_ERROR("Read FriendlyName failed! %d",hr);
            }
            pBag->Release();
            memset(sName, 0, sizeof(sName));
        }
        nCount++;
        pM->Release();
    }

    //get All Audio Renderer Device Friendly Name
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory,&pEm, 0);
    if (hr == S_FALSE)
    {
        TPLOG_ERROR("CreateClassEnumerator CLSID_AudioInputDeviceCategory failed! %d",hr);
        hr = VFW_E_NOT_FOUND;
        return hr;
    }
    pEm->Reset();
    nCount = 0;
    while(hr=pEm->Next(1,&pM,&cFetched),hr == S_OK)
    {
        IPropertyBag *pBag;
        MICOR_INFO_ST stMicro;
        hr = pM->BindToStorage(0,0,IID_IPropertyBag,(LPVOID*)&pBag);
        if (SUCCEEDED(hr))
        {
            VARIANT varTemp;
            varTemp.vt = VT_BSTR;
            hr = pBag->Read(L"FriendlyName", &varTemp, NULL); //还有其他属性,像描述信息等等
            if(hr == NOERROR)
            {
                //获取设备名称      
                WideCharToMultiByte(CP_ACP, 0, varTemp.bstrVal, -1, sName, nBufferSize , "", NULL);
                SysFreeString(varTemp.bstrVal);
                stMicro.uiIndex = nCount;
                stMicro.strName = (std::string)sName;
                lstMicInfo.push_back(stMicro);
            }
            else
            {
                TPLOG_ERROR("Read FriendlyName failed! %d",hr);
            }
            pBag->Release();
            memset(sName, 0, sizeof(sName));
        }
        nCount++;
        pM->Release();
    }

    pCreateDevEnum = NULL;
    pEm = NULL;

    return 0;
}

