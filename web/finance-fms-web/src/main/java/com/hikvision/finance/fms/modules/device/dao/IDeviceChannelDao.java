/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 上午10:36:39
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.dao;

import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Devicechannel;
import com.hikvision.finance.fms.modules.device.dto.DeviceChannelInfo;
import com.hikvision.finance.fms.modules.device.qo.DeviceChannelQo;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年3月9日 上午10:36:39
 * @version V1.0
 */
public interface IDeviceChannelDao extends IBaseDao<Devicechannel, DeviceChannelQo> {
	
	/**
	 * 根据用户ID获取通道信息
	 * @author jinchenwu 2016年4月19日 下午3:31:11
	 * @param userId
	 * @return
	 */
	public List<Devicechannel> getChannels(Integer userId);
	
	/**
	 * 通道分页信息
	 * @author fuqunqing 2016年9月23日 上午10:02:57
	 * @param limit
	 * @param start
	 * @param deviceChannelInfo
	 * @return
	 */
	public Page getChannelsDataPage(int limit, int start, DeviceChannelInfo deviceChannelInfo);
	
}
