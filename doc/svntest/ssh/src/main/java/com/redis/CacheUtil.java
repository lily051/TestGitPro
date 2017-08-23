package com.redis;

public class CacheUtil {
    public final static String NAMESPACE_ROLE = "role$";
    public final static String NAMESPACE_MONITORBOARD = "mb$";
    public final static String NAMESPACE_VIOLATION = "violation$";
    public final static String NAMESPACE_LOGIN = "login$";

    /** 图片在memcached中保存一天，之后过期 */
    public final static int EXPIRE_TIME_PIC = 24 * 60 * 60;
    /** GPS在memcached中保存7天，之后过期 */
    public final static int EXPIRE_TIME_GPS = 7 * 24 * 60 * 60;
    /** 错误登录限制在memcached钟保存一个小时，之后过期 */
    public final static int EXPIRE_TIME_INVALID_LOGIN = 60 * 60;
}
