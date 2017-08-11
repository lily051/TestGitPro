/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 下午4:41:18
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysconfig.service;

import java.util.List;

import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.model.Sysconfig;
import com.hikvision.finance.fms.modules.sysconfig.dto.LogParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.ModeParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.NtpParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.PasswordParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.PlatformParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.RecordParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.SysconfigDto;
import com.hikvision.finance.fms.modules.sysconfig.qo.SysconfigQo;
import com.hikvision.finance.fwork.util.AjaxData;

/**
 * @author jinxindong 2016年3月9日 下午4:41:18
 * @version V1.0
 */
public interface ISysconfigService extends IBaseService<Sysconfig, SysconfigQo> {
	
	
	/**
	 * 获取系统配置信息
	 * @author jinxindong 2016年3月9日 下午4:48:50
	 * @return
	 */
	public SysconfigDto getSysconfig();
	
	/**
	 * 获取指定key的系统配置信息列表
	 * @author jinxindong 2016年3月31日 下午3:47:00
	 * @param keys
	 * @return
	 */
	public List<Sysconfig> getSysconfigByKey(List<Integer> keys);
	
	/**
	 * 提交录像参数配置
	 * @author jinxindong 2016年3月31日 上午9:50:37
	 * @param recordParamInfo
	 * @return
	 */
	public AjaxData updateRecordParam(RecordParamInfo recordParamInfo);
	
	/**
	 * 获取录像参数配置
	 * @author jinxindong 2016年3月31日 上午9:54:27
	 * @return
	 */
	public RecordParamInfo getRecordParam();
	
	/**
	 * 提交模式参数配置
	 * @author jinxindong 2016年3月31日 上午10:00:11
	 * @param modeParamInfo
	 * @return
	 */
	public AjaxData updateModeParam(ModeParamInfo modeParamInfo);
	
	/**
	 * 获取模式参数配置
	 * @author jinxindong 2016年3月31日 上午10:01:35
	 * @return
	 */
	public ModeParamInfo getModeParam();
	
	/**
	 * 提交安全策略参数配置
	 * @author jinxindong 2016年3月31日 上午10:04:05
	 * @param passwordParamInfo
	 * @return
	 */
	public AjaxData updatePasswordParam(PasswordParamInfo passwordParamInfo);
	
	/**
	 * 获取安全策略参数配置
	 * @author jinxindong 2016年3月31日 上午10:04:24
	 * @return
	 */
	public PasswordParamInfo getPasswordParam();
	
	/**
	 * 提交日志参数配置
	 * @author jinxindong 2016年3月31日 上午10:05:05
	 * @param logParamInfo
	 * @return
	 */
	public AjaxData updateLogParam(LogParamInfo logParamInfo);
	
	/**
	 * 获取日志参数配置
	 * @author jinxindong 2016年3月31日 上午10:05:22
	 * @return
	 */
	public LogParamInfo getLogParam();
	
	/**
	 * 提交平台信息参数配置
	 * @author jinxindong 2016年3月31日 下午2:46:21
	 * @param platformParamInfo
	 * @return
	 */
	public AjaxData updatePlatformParam(PlatformParamInfo platformParamInfo);
	
	/**
	 * 重置平台信息参数配置
	 * @author jinxindong 2016年3月31日 下午2:48:08
	 * @param platformParamInfo
	 * @return
	 */
	public AjaxData resetPlatformParam(PlatformParamInfo platformParamInfo);
	
	/**
	 * 获取平台信息参数配置
	 * @author jinxindong 2016年3月31日 下午2:46:27
	 * @return
	 */
	public PlatformParamInfo getPlatformParam();
	
	/**
	 * 获取可用的业务参数
	 * @author jinxindong 2016年3月31日 下午4:42:02
	 * @return
	 */
	public List<Sysconfig> getBussinessParamIn();
	
	/**
	 * 获取禁用的业务参数
	 * @author jinxindong 2016年3月31日 下午4:42:16
	 * @return
	 */
	public List<Sysconfig> getBussinessParamOut();
	
	/**
	 * 启用业务参数
	 * @author jinxindong 2016年3月31日 下午4:43:53
	 * @param keys
	 * @return
	 */
	public AjaxData enableBussinessParam(List<Integer> keys);
	
	/**
	 * 禁用业务参数
	 * @author jinxindong 2016年3月31日 下午4:44:06
	 * @param keys
	 * @return
	 */
	public AjaxData disableBussinessParam(List<Integer> keys);
	
	/**
	 * 根据key值，修改参数名称
	 * @author jinxindong 2016年3月31日 下午6:16:52
	 * @param key
	 * @return
	 */
	public AjaxData editBussinessName(Sysconfig sysconfig);

	/**
	 * 获取NTP校时服务配置信息
	 * @author fuqunqing 2016年9月21日 下午5:56:47
	 * @return
	 */
	public NtpParamInfo getNtpParam();
	
	/**
	 * 更新NTP配置信息
	 * @author fuqunqing 2016年9月21日 下午6:19:33
	 * @param logParamInfo
	 * @return
	 */
	public AjaxData updateNtpParam(NtpParamInfo ntpParamInfo);

}
