/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 上午10:22:54
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.service;

import java.io.File;
import java.util.List;
import java.util.Map;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.common.excel.model.ExcelHead;
import com.hikvision.finance.fms.model.Device;
import com.hikvision.finance.fms.modules.device.dto.CloudPoolInfo;
import com.hikvision.finance.fms.modules.device.dto.DeviceChannelInfo;
import com.hikvision.finance.fms.modules.device.dto.DeviceInfo;
import com.hikvision.finance.fms.modules.device.qo.DeviceQo;
import com.hikvision.finance.fms.modules.user.model.RecordPojo;
import com.hikvision.finance.fwork.util.AjaxData;


/**
 * <p>资源管理service接口</p>
 * @author jinxindong 2016年3月9日 上午10:22:54
 * @version V1.0 
 */
public interface IDeviceService extends IBaseService<Device,DeviceQo>{

	/**
	 * 进入存储设备数据获取页面
	 * @author jinxindong 2016年4月5日 下午3:27:14
	 * @param limit
	 * @param start
	 * @param deviceInfo
	 * @return
	 */
	public Page getStoDataPage(Integer limit,Integer start,DeviceInfo deviceInfo);
	
	/**
	 * 添加存储设备
	 * @author jinxindong 2016年4月5日 下午3:27:29
	 * @param deviceInfo
	 * @param jsonStr
	 * @return
	 */
	public AjaxData addSto(DeviceInfo deviceInfo,String jsonStr);
	
	/**
	 * 根据存储设备id获取设备信息
	 * @author jinxindong 2016年4月5日 下午3:27:48
	 * @return
	 */
	public DeviceInfo getStoDevById(Integer deviceId);
	
	/**
	 * 修改存储设备
	 * @author jinxindong 2016年4月5日 下午3:28:13
	 * @param deviceInfo
	 * @param jsonStr
	 * @return
	 */
	public AjaxData updateSto(DeviceInfo deviceInfo,String jsonStr);
	
	/**
	 * 删除存储设备IDS
	 * @author jinxindong 2016年4月5日 下午3:28:31
	 * @param ids
	 * @return
	 */
	public AjaxData deleteStos(List<Integer> ids);
	
	/**
	 * 导出存储设备
	 * @author jinxindong 2016年4月12日 下午6:23:41
	 * @param deviceInfo
	 * @param titleName
	 * @param attrName
	 * @param path
	 * @return
	 */
	public boolean exportStoDevExcel(DeviceInfo deviceInfo, String[] titleName, String[] attrName, String path);
	
	/**
	 * 导出采集设备
	 * @author jinxindong 2016年4月12日 下午6:49:56
	 * @param deviceInfo
	 * @param titleName
	 * @param attrName
	 * @param path
	 * @return
	 */
	public boolean exportCollDevExcel(DeviceInfo deviceInfo, String[] titleName, String[] attrName, String path);
	
	/**
	 * 进入采集设备数据获取页面
	 * @author jinxindong 2016年4月6日 上午9:46:58
	 * @param limit
	 * @param start
	 * @param deviceInfo
	 * @return
	 */
	public Page getCollDataPage(Integer limit,Integer start,DeviceInfo deviceInfo);
	
	/**
	 * 添加采集设备
	 * @author jinxindong 2016年4月6日 上午9:47:05
	 * @param deviceInfo
	 * @param jsonStr
	 * @return
	 */
	public AjaxData addColl(DeviceInfo deviceInfo,String jsonStr);
	
	/**
	 * 根据采集设备id获取设备信息
	 * @author jinxindong 2016年4月6日 上午9:47:20
	 * @param deviceId
	 * @return
	 */
	public DeviceInfo getCollDevById(Integer deviceId);
	
    /**
     * 更新采集设备
     * @author jinxindong 2016年4月6日 上午9:48:02
     * @param deviceInfo
     * @param jsonStr
     * @return
     */
	public AjaxData updateColl(DeviceInfo deviceInfo,String jsonStr);
	
	/**
	 * 删除采集设备
	 * @author jinxindong 2016年4月6日 上午9:48:16
	 * @param ids
	 * @return
	 */
	public AjaxData deleteColls(List<Integer> ids);
	
	/**
	 * 根据采集设备id获取设备
	 * @author jinxindong 2016年4月6日 上午9:48:36
	 * @param ids
	 * @return
	 */
	public List<DeviceChannelInfo> getDevChannelByIds(List<Integer> ids);
	
	/**
	 * 通过存储设备id获取采集设备分券信息和已选分券
	 * @author jinxindong 2016年4月6日 下午6:41:04
	 * @param deviceInfo
	 * @return
	 */
	public List<CloudPoolInfo> getPoolsByDevId(DeviceInfo deviceInfo);
	
	
	/**
	 * 下载中心设备模板
	 * @author jinxindong 2016年4月25日 下午4:11:35
	 * @param path
	 * @return
	 */
	public boolean downLoadStoTemplate(String path);
	
	/**
	 * 下载采集设备模板
	 * @author jinxindong 2016年4月25日 下午4:28:02
	 * @param path
	 * @return
	 */
	public boolean downLoadCollTemplate(String path);
	
	/**
	 * 校验导入存储表格的字段
	 * @author jinxindong 2016年6月2日 下午2:09:01
	 * @param devFile
	 * @param devFileFileName
	 * @param excelHead
	 * @return
	 */
	public AjaxData addAndValidateStoExcel(File devFile, String devFileFileName, ExcelHead excelHead);
	
	/**
	 * 校验导入采集表格的字段
	 * @author jinxindong 2016年6月2日 下午2:09:33
	 * @param devFile
	 * @param devFileFileName
	 * @param excelHead
	 * @return
	 */
	public AjaxData addAndValidateCollExcel(File devFile, String devFileFileName, ExcelHead excelHead);
	
	/**
	 * 导入采集设备表格
	 * @author jinxindong 2016年4月25日 下午5:41:49
	 * @param devFile
	 * @param devFileFileName
	 * @param excelHead
	 * @return
	 */
	public Map<String,Object> addCollDevFromExcel(File devFile, String devFileFileName, ExcelHead excelHead);
	
	/**
	 * 导入时存储设备信息（所有设备导入都使用该方法）
	 * @author jinxindong 2016年4月25日 下午6:00:10
	 * @param deviceInfos
	 * @return
	 */
	public AjaxData addAllDevs(List<DeviceInfo> deviceInfos);

	/**
	 * 获取通道分页信息
     * @author fuqunqing 2016年9月23日 上午9:59:27
     * @param limit
     * @param start
     * @param deviceChannelInfo
     * @return
     */
    public Page getChannelsDataPage(int limit, int start, DeviceChannelInfo deviceChannelInfo);

	/**
	 * 更新通道基本信息
     * @author fuqunqing 2016年9月23日 上午10:13:56
     * @param deviceChannelInfo
     * @return
     */
    public AjaxData updateChannel(DeviceChannelInfo deviceChannelInfo);

	/**
	 * 删除制定通道
     * @author fuqunqing 2016年9月23日 上午10:16:50
     * @param ids
     * @return
     */
    public AjaxData deleteChannel(List<Integer> ids);
    
    /**
     * 
     * @author fuqunqing 2016年9月23日 上午10:58:23
     * @param id
     * @return
     */
    public DeviceChannelInfo getChannelInfoById(Integer id);
    
    /**
     * 获取设备基本信息
     * @author fuqunqing 2016年9月23日 下午5:58:10
     * @param id
     * @return
     */
    public DeviceInfo getDeviceInfoById(Integer id);
    
    /**
     * 更新设备NTP校时状态
     * @author fuqunqing 2016年9月23日 下午6:03:18
     * @param id
     */
    public void updateDeviceNtp(Integer id);
    
    /**
     * 预览通道信息
     * @author zhengquanwu 2016年11月5日 上午11:40:33
     * @param channelId 通道id
     * @return
     */
    public RecordPojo getDevChannelInfo(Integer channelId);
    
}
