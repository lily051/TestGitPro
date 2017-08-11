/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-3-11 下午3:07:33
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.util;

import java.util.Collection;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import com.google.common.collect.HashBasedTable;
import com.google.common.collect.Table;
import com.ivms6.core.hpp.HppChannel;
import com.ivms6.core.hpp.util.ContextUtil;

/**
 * <p></p>
 * @author xujiangfei 2016-3-11 下午3:07:33
 * @version V1.0 
 */
public class CsClientContextUtil {

private static Table<Integer,String,String> csClientTable=HashBasedTable.create();
	
	/**
	 * 获得指定用户在线sessionId集合
	 * @author fuqunqing 2015年10月23日 下午3:38:00
	 * @param userId
	 * @return
	 */
	private static Collection<String> getcsClientSession(Integer userId){
		if(userId!=null){
			Map<String,String> map = csClientTable.row(userId);
			Collection<String> collection = map.keySet();
			return collection;
		}
		return null;
	}
	
	/**
	 * 添加客户端在线信息
	 * @author fuqunqing 2015年10月23日 下午3:42:48
	 * @param userId
	 * @param sessionId
	 * @param clientIp
	 */
	public static void addCsClientSession(Integer userId, String sessionId, String clientIp) {
		csClientTable.put(userId, sessionId, clientIp);
	}
	
	/**
	 * 删除客户端在线信息
	 * @author fuqunqing 2015年10月23日 下午3:43:33
	 * @param userId
	 * @param sessionId
	 * @return
	 */
	public static void delClientSession(String sessionId) {
		Map<Integer, String> map = csClientTable.column(sessionId);
		
		Collection<Integer> collection = map.keySet();
		
		if (collection != null && collection.size() > 0){
			for(Integer userId: collection){
				csClientTable.remove(userId, sessionId);
			}
		}
	}
	
	/**
	 * 获取用户在线信息
	 * @author fuqunqing 2015年10月23日 下午3:44:03
	 * @param userIds
	 * @return
	 */
	public static List<HppChannel> getHppChannel(Collection<Integer> userIds){
		List<HppChannel> channels = new LinkedList<HppChannel>();
		for(Integer id: userIds){
			Collection<String> sessionIds = getcsClientSession(id);
			if(sessionIds!=null&&sessionIds.size()>0){
				for(String sessionId: sessionIds){
					HppChannel hppChannel = ContextUtil.getHppChannel(sessionId);
					if(hppChannel!=null){
						channels.add(hppChannel);
					}
				}
			}
		}
		return channels;
	}

}
