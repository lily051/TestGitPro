/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月19日 下午7:15:34
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.cache;

import java.util.Calendar;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * <p>心跳缓存</p>
 * @author jinchenwu 2016年4月19日 下午7:15:34
 * @version V1.0 
 */
public class HeartbeatCache {
	
	private static Logger logger = LoggerFactory.getLogger(HeartbeatCache.class);
	
	// 心跳过期阈值（毫秒）,前端发送间隔是10秒
	private static final int DUE_THRESHOLD_MILLISECOND = 15000;
	// 心跳缓存
	private static final ConcurrentHashMap<Integer, Calendar> heartbeatCache = new ConcurrentHashMap<Integer, Calendar>();
	
	private static final ConcurrentHashMap<Integer, Calendar> channelCache = new ConcurrentHashMap<Integer, Calendar>();
	
	/**
	 * 登录时判断用户是否在线                                  
	 * @author jinchenwu 2016年4月20日 上午10:50:34
	 * @param userId
	 * @return
	 */
	public static boolean isOnline(Integer userId) {
		Calendar lastTime = heartbeatCache.get(userId);// 上次心跳时间
		long timeNow = Calendar.getInstance().getTimeInMillis();
		// 没有心跳信息，或心跳超时，说明用户不在线
		if (isTimeOut(timeNow, lastTime)) {
			heartbeatCache.put(userId, Calendar.getInstance());// 设置心跳信息
			return false;
		}else{
			return true;
		}
	}
	
	/**
	 * 前端发心跳，设置心跳缓存，设置通道缓存
	 * @author jinchenwu 2016年4月19日 下午7:29:21
	 * @param userId
	 */
	public static void setHeartBeatTime(Integer userId, Integer channelId) {
		heartbeatCache.put(userId, Calendar.getInstance());
		if (channelId != null) {
			channelCache.put(channelId, Calendar.getInstance());
		}
	}
	
	/**
	 * 用户注销时移除心跳
	 * @author jinchenwu 2016年5月13日 下午2:28:57
	 */
	public static void removeHeartBeat(int userId, Integer channelId){
		heartbeatCache.remove(userId);
		if(channelId != null){
			channelCache.remove(channelId);
		}
	}
	
	/**
	 * 选择通道时判断通道是否正在使用
	 * @author jinchenwu 2016年5月12日 下午9:25:02
	 * @param channelId
	 * @return
	 */
	public static boolean isChannelUsed(Integer channelId) {
		Calendar lastTime = channelCache.get(channelId);// 上次心跳时间
		long timeNow = Calendar.getInstance().getTimeInMillis();
		// 没有心跳信息，或心跳超时，说明通道不在使用
		if (isTimeOut(timeNow, lastTime)) {
			channelCache.put(channelId, Calendar.getInstance());// 设置心跳信息
			return false;
		}else{
			return true;
		}
	}
	
	/**
	 * 定时移除过期缓存信息
	 * @author jinchenwu 2016年5月13日 下午7:15:17
	 */
	public static void removeHeartBeat() {
		long timeNow = Calendar.getInstance().getTimeInMillis();
		logger.info("删除前用户缓存数：" + heartbeatCache.size() + "，删除前通道缓存数：" + channelCache.size());
		for (Map.Entry<Integer, Calendar> entry : heartbeatCache.entrySet()) {
			Integer id = entry.getKey();
			Calendar lastTime = entry.getValue();
			if (isTimeOut(timeNow, lastTime)) {
				heartbeatCache.remove(id);// 移除用户缓存信息
			}
		}
		timeNow = Calendar.getInstance().getTimeInMillis();
		for (Map.Entry<Integer, Calendar> entry : channelCache.entrySet()) {
			Integer id = entry.getKey();
			Calendar lastTime = entry.getValue();
			if (isTimeOut(timeNow, lastTime)) {
				channelCache.remove(id);// 移除通道缓存信息
			}
		}
		logger.info("删除后用户缓存数：" + heartbeatCache.size() + "，删除后通道缓存数：" + channelCache.size());
	}
	
	/**
	 * 判断超时的逻辑
	 * @author jinchenwu 2016年6月20日 下午2:07:38
	 * @param timeNow
	 * @param lastTime
	 * @return
	 */
	private static boolean isTimeOut(long timeNow, Calendar lastTime) {
		if (lastTime == null || timeNow - lastTime.getTimeInMillis() > DUE_THRESHOLD_MILLISECOND || timeNow < lastTime.getTimeInMillis()) {
			return true;
		} else {
			return false;
		}
	}
	
	/**
	 *  判断缓存中是否存在当前用户(CS心跳使用, 判断是修改密码[会移除用户缓存]还是无法连接DC)
	 * @author zhoujiajun 2016年12月1日 上午10:50:35
	 * @param userId
	 * @return
	 */
	public static boolean isExistUser(Integer userId){
		return heartbeatCache.containsKey(userId);
	}
	
}
