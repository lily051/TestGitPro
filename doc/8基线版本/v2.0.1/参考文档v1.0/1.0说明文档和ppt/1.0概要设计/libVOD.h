int libVOD_Init(void);
void libVOD_Fini(void);

int libVOD_Login(const char *host, int port);
int loginID = libVOD_Login("127.0.0.1", 8129);
int libVOD_Logout(int loginID);

typedef struct _vod_play_info_t {
    char url[256];  // local://F:/dddd
}vod_play_info_t;
typedef void (CALLBACK *CBF_Data)(int playID, int dataType, const void* pData, int dataSize, void *userData);

int libVOD_Play(int loginID, cosnt vod_play_t *playInfo, CBF_Data dataCallback, void *userData);
int libVOD_Stop(int playID);
