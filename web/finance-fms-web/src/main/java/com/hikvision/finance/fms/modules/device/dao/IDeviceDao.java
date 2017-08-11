/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 上午10:25:05
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.dao;

import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Device;
import com.hikvision.finance.fms.modules.device.dto.DeviceInfo;
import com.hikvision.finance.fms.modules.device.qo.DeviceQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月9日 上午10:25:05
 * @version V1.0 
 */
public interface IDeviceDao extends IBaseDao<Device,DeviceQo>{
	/**
	 * 通过区域id回去设备列表
	 * @author jinxindong 2016年3月9日 上午10:54:16
	 * @return
	 */
	public List<Device> getDeviceByRegionId(Integer nregionId);
	
	/**
	 * 获取存储设备分页信息
	 * @author jinxindong 2016年4月5日 下午3:54:57
	 * @param limit
	 * @param start
	 * @param deviceInfo
	 * @return
	 */
	public Page getStoDataPage(Integer limit,Integer start,DeviceInfo deviceInfo);
	
	/**
	 * 获取采集设备分页信息
	 * @author jinxindong 2016年4月6日 上午9:51:06
	 * @param limit
	 * @param start
	 * @param deviceInfo
	 * @return
	 */
	public Page getCollDataPage(Integer limit,Integer start,DeviceInfo deviceInfo);
	
	/**
	 * 获取全部存储设备信息
	 * @author jinxindong 2016年4月6日 下午4:31:14
	 * @return
	 */
	public List<Device> getAllStoDevice();
	
	/**
	 * 获取所有的DVR设备信息--DC中心端接口
	 * @author xujiangfei 2016-3-22 下午1:57:22
	 * @return
	 */
	public List<Device> getAllDvrServiceInfo();


	/**获取存储设备信息--DC中心接口
	 * @author xujiangfei 2016-3-23 下午1:53:16
	 * @param nType
	 * @param strName
	 * @return
	 */
	public List<Device> getDCServiceInfobyCvrid(int cvrid);
	
	/**
	 * 导出存储设备表格
	 * @author jinxindong 2016年4月12日 下午6:26:07
	 * @param deviceInfo
	 * @return
	 */
	public List<DeviceInfo> exportStoDevExcel(DeviceInfo deviceInfo);
	
	/**
	 * 导出采集设备
	 * @author jinxindong 2016年4月12日 下午6:51:05
	 * @param deviceInfo
	 * @return
	 */
	public List<DeviceInfo> exportCollDevExcel(DeviceInfo deviceInfo);

	/**
	 * 通过设备名称获取设备信息
	 * @author jinchenwu 2016年4月15日 下午8:15:11
	 * @param strName
	 * @return
	 */
	public Device getDeviceByName(String strName);

	/**根据设备类型获取设备信息
	 * @author xujiangfei 2016-5-5 下午4:09:54
	 * @return
	 */
	public List<Device> getDeviceinfobyntype();

	/**DC系统巡检服务--更新设备在线状态
	 * @author xujiangfei 2016-5-5 下午7:03:32
	 * @param strIp
	 * @param port
	 * @param online
	 */
	public void updateDeviceOnline(String strIp, int port, int online);

	/**--DC系统巡检服务--更新设备可用空间
	 * @author xujiangfei 2016-5-5 下午8:23:20
	 * @param strIp
	 * @param port
	 * @param uiTotalSpace
	 * @param uiFreeSpace
	 */
	public void updateDeviceSpace(String strIp, int port, int uiTotalSpace,
			int uiFreeSpace);

	/**--DC上传服务--获取存储设备参数
	 * @author xujiangfei 2016-5-6 下午7:43:33
	 * @param ip
	 * @param port
	 * @return
	 */
	public com.hikvision.finance.fms.cms.dto.DeviceInfo getStorageInfo(String ip, int port);
	
	/**
	 * 更新设备NTP信息字段
	 * @author fuqunqing 2016年9月22日 下午6:49:32
	 * @param ntp NTP状态值
	 * @param id 设备id
	 */
	public void updateDeviceNtp(Integer ntp, Integer id);
	
	/**
	 * 获取usb模式下的所有设备信息
	 * @author zhengquanwu 2016年10月12日 下午7:45:38
	 * @return 设备列表
	 */
	public List<Device> getUsbDeviceByNtype();

	
}
