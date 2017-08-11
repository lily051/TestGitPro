/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 下午4:41:54
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysconfig.service.impl;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import com.google.common.collect.Lists;
import com.hikvision.cms.cache.manager.DataLoadException;
import com.hikvision.cms.cache.manager.DataManager;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.enums.LogModuledEnum;
import com.hikvision.finance.fms.common.init.FmsPropertyPlaceholder;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.CacheKeyPrefix;
import com.hikvision.finance.fms.common.util.Constants.OperStatus;
import com.hikvision.finance.fms.common.util.Constants.SysConfigType;
import com.hikvision.finance.fms.common.util.Constants.SysConfigType.SysBigType;
import com.hikvision.finance.fms.common.util.Constants.SysDictionary;
import com.hikvision.finance.fms.model.Sysconfig;
import com.hikvision.finance.fms.modules.device.dao.IDeviceDao;
import com.hikvision.finance.fms.modules.log.dao.IConfigLogDao;
import com.hikvision.finance.fms.modules.sysconfig.dao.ISysconfigDao;
import com.hikvision.finance.fms.modules.sysconfig.dto.LogParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.ModeParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.NtpParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.PasswordParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.PlatformParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.RecordParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.SysconfigDto;
import com.hikvision.finance.fms.modules.sysconfig.qo.SysconfigQo;
import com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.AjaxData;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;


/**
 * @author jinxindong 2016年3月9日 下午4:41:54
 * @version V1.0 
 */
public class SysconfigServiceImpl extends BaseServiceImpl<Sysconfig, SysconfigQo, ISysconfigDao> implements ISysconfigService{
	private ISysconfigDao sysconfigDao;
	private DataManager dataManager;
	private IConfigLogDao configLogDao;
	private IDeviceDao deviceDao;
	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.IBaseService#deleteByIds(java.io.Serializable[])
     */
    @Override
    public void deleteByIds(Serializable[] paramArrayOfSerializable) {

    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.BaseServiceImpl#getDao()
     */
    @Override
    protected ISysconfigDao getDao() {
	    return sysconfigDao;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#getSysconfig()
     */
    @Override
    public SysconfigDto getSysconfig() {
    	SysconfigDto sysconfigDto = new SysconfigDto();
    	List<Integer> nkeys = Lists.newArrayList(
        		SysConfigType.DEFAULT_PASSWORD,
        		SysConfigType.PWD_FRESH_DAYS,
        		SysConfigType.PASSWORD_SECRUITY_LEVEL,
        		SysConfigType.OVERLAY_OSD,
        		SysConfigType.CHOICE_IPC_USB, 
        		SysConfigType.FAIL_COUNT, 
        		SysConfigType.DISTANCE_MINUTES
        		);
    	SysconfigQo sysconfigQo = new SysconfigQo();
    	sysconfigQo.setNkeys(nkeys);
    	List<Sysconfig> configs = sysconfigDao.queryList(sysconfigQo);
    	for(Sysconfig config:configs) {
			switch(config.getNkey()) {
				case SysConfigType.DEFAULT_PASSWORD:
					sysconfigDto.setPasswordDefault(config.getStrValue());
					break;
				case SysConfigType.PASSWORD_SECRUITY_LEVEL:
					sysconfigDto.setPasswordSecruityLevel(config.getStrValue());
					break;
				case SysConfigType.PWD_FRESH_DAYS:
					sysconfigDto.setPasswordFresh(config.getStrValue());
					break;
				case SysConfigType.OVERLAY_OSD:
					sysconfigDto.setOsdEnabled(config.getStrValue());
					break;
				case SysConfigType.CHOICE_IPC_USB:
					sysconfigDto.setUsbmodelinfo(config.getStrValue());
					break;
				case SysConfigType.FAIL_COUNT:
					sysconfigDto.setFailCount(config.getStrValue());
					break;
				case SysConfigType.DISTANCE_MINUTES:
					sysconfigDto.setDistanceMinutes(config.getStrValue());
					break;
			}
    	}
    	return sysconfigDto;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#getSysconfigByKey(java.util.List)
     */
    @Override
    public List<Sysconfig> getSysconfigByKey(List<Integer> keys) {
    	 return sysconfigDao.getSysconfigByKey(keys);
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#updateRecordParam(com.hikvision.finance.fms.modules.sysconfig.dto.RecordParamInfo)
     */
    @Override
	public AjaxData updateRecordParam(RecordParamInfo recordParamInfo) {
		AjaxData ajaxData = new AjaxData(false);
		List<Sysconfig> sysconfigs = sysconfigDao.findBy("ntype", SysBigType.SYS_B_VIDEO);
		for (Sysconfig config : sysconfigs) {
			switch (config.getNkey()) {
				case SysConfigType.MAXIMUM_VIDEO_LENGTH: // 最大录像时长
					config.setStrValue(recordParamInfo.getMaximumVideoLength());
					config.setStrUpdatetime(new Date());
					break;
/*				case SysConfigType.PLAYBACK_VIDEO_NUM: // 同时回放录像数
					config.setStrValue(recordParamInfo.getPlayBackVideoNum());
					config.setStrUpdatetime(new Date());
					break;*/
				case SysConfigType.VIDEO_PLAYBACK_BANDWIDTH: // 录像回放带宽
					config.setStrValue(recordParamInfo.getVideoPlayBackBandWidth());
					config.setStrUpdatetime(new Date());
					break;
/*				case SysConfigType.VIDEO_UPLOAD_BANDWIDTH: // 上传录像带宽
					config.setStrValue(recordParamInfo.getVideoUploadBandWidth());
					config.setStrUpdatetime(new Date());
					break;*/
				case SysConfigType.UPLOAD_TIME_START: // 守卫服务工作时间段开始时间
					config.setStrValue(recordParamInfo.getUploadTimeStart());
					config.setStrUpdatetime(new Date());
					break;
				case SysConfigType.UPLOAD_TIME_END: // 守卫服务工作时间段结束时间
					config.setStrValue(recordParamInfo.getUploadTimeEnd());
					config.setStrUpdatetime(new Date());
					break;
				case SysConfigType.VIDEO_DELETE_DAYS: // 录像删除提示天数
					config.setStrValue(recordParamInfo.getVideoDeleteDays());
					config.setStrUpdatetime(new Date());
					break;
			}
			sysconfigDao.update(config);
		}
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.CONFIG.getCode(),GlobalMessageUtil.getMessage("log.video.app"));
		return ajaxData;
		
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#getRecordParam()
     */
    @Override
	public RecordParamInfo getRecordParam() {
		RecordParamInfo recordParamInfo = new RecordParamInfo();
		List<Sysconfig> sysconfigs = sysconfigDao.findBy("ntype", SysBigType.SYS_B_VIDEO);
		try{
		for (Sysconfig config : sysconfigs) {
			switch (config.getNkey()) {
				case SysConfigType.MAXIMUM_VIDEO_LENGTH: // 最大录像时长
					recordParamInfo.setMaximumVideoLength(config.getStrValue());
					dataManager.set(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.MAXIMUM_VIDEO_LENGTH,
							recordParamInfo.getMaximumVideoLength());
					break;
/*				case SysConfigType.PLAYBACK_VIDEO_NUM: // 同时回放录像数
					recordParamInfo.setPlayBackVideoNum(config.getStrValue());
					break;*/
				case SysConfigType.VIDEO_PLAYBACK_BANDWIDTH: // 录像回放带宽
					recordParamInfo.setVideoPlayBackBandWidth(config.getStrValue());
					dataManager.set(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.VIDEO_PLAYBACK_BANDWIDTH,
							recordParamInfo.getVideoPlayBackBandWidth());
					break;
/*				case SysConfigType.VIDEO_UPLOAD_BANDWIDTH: // 上传录像带宽
					recordParamInfo.setVideoUploadBandWidth(config.getStrValue());
					break;*/
				case SysConfigType.UPLOAD_TIME_START: // 守卫服务工作时间段开始时间
					recordParamInfo.setUploadTimeStart(config.getStrValue());
					break;
				case SysConfigType.UPLOAD_TIME_END: // 守卫服务工作时间段结束时间
					recordParamInfo.setUploadTimeEnd(config.getStrValue());
					break;
				case SysConfigType.VIDEO_DELETE_DAYS: // 录像删除提示天数
					recordParamInfo.setVideoDeleteDays(config.getStrValue());
					break;
			}
		}
		} catch (DataLoadException e) {
			throw new ExpectedException("应用录像参数配置出错");
		}
		return recordParamInfo;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#updateModeParam(com.hikvision.finance.fms.modules.sysconfig.dto.ModeParamInfo)
     */
    @Override
	public AjaxData updateModeParam(ModeParamInfo modeParamInfo) {
		AjaxData ajaxData = new AjaxData(false);
		try {
			List<Sysconfig> sysconfigs = sysconfigDao.findBy("ntype", SysBigType.SYS_B_MODE);
			for (Sysconfig config : sysconfigs) {
				switch (config.getNkey()) {
/*					case SysConfigType.VERIFICATION_CODE: // 启用禁用验证码
						config.setStrValue(modeParamInfo.getVerificTionCode());
						config.setStrUpdatetime(new Date());
						dataManager.set(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.VERIFICATION_CODE,
						        modeParamInfo.getVerificTionCode());
						break;
					case SysConfigType.UPLOAD_VIDEO_AUDIT: // 启用禁用录像审核后上传
						config.setStrValue(modeParamInfo.getUploadVideoAudit());
						config.setStrUpdatetime(new Date());
						break;*/
					case SysConfigType.OVERLAY_OSD: // 启用禁用OSD叠加
						config.setStrValue(modeParamInfo.getOverlayOsd());
						config.setStrUpdatetime(new Date());
						dataManager.set(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.OVERLAY_OSD,
						        modeParamInfo.getOverlayOsd());
						break;
					case SysConfigType.PRODUCT_REVIEW: // 启用禁用产品复核
						config.setStrValue(modeParamInfo.getProductReview());
						config.setStrUpdatetime(new Date());
						dataManager.set(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.PRODUCT_REVIEW,
						        modeParamInfo.getProductReview());
						break;
/*					case SysConfigType.CARD_READER_VIDEO: // 启用禁用身份证读卡器刷卡开启录像
						config.setStrValue(modeParamInfo.getCardReaderVideo());
						config.setStrUpdatetime(new Date());
						break;*/
					case SysConfigType.CHOICE_IPC_USB: // 选择usb还是ipc
						if(modeParamInfo.getChoiceUsbIpc()!=null){
							config.setStrValue(modeParamInfo.getChoiceUsbIpc());
							config.setStrUpdatetime(new Date());
							dataManager.set(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.CHOICE_IPC_USB,
							        modeParamInfo.getChoiceUsbIpc());
						}
						break;
				}
				sysconfigDao.update(config);
			}
		} catch (DataLoadException e) {
			throw new ExpectedException("应用模式参数配置出错");
		}
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.CONFIG.getCode(),GlobalMessageUtil.getMessage("log.model.app"));
		return ajaxData;
		
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#getModeParam()
     */
    @Override
	public ModeParamInfo getModeParam() {
		ModeParamInfo modeParamInfo = new ModeParamInfo();
		List<Sysconfig> sysconfigs = sysconfigDao.findBy("ntype", SysBigType.SYS_B_MODE);
		for (Sysconfig config : sysconfigs) {
			switch (config.getNkey()) {
				case SysConfigType.OVERLAY_OSD: // 启用禁用OSD叠加
					modeParamInfo.setOverlayOsd(config.getStrValue());
					break;
				case SysConfigType.PRODUCT_REVIEW: // 启用禁用产品复核
					modeParamInfo.setProductReview(config.getStrValue());
					break;
			}
		}
		
		
		String strChoiceUsbIpc = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
		if (StringUtils.isEmpty(strChoiceUsbIpc)) {
			throw new ExpectedException("", "未获取到通道方式");
		}
		if ("ipc".equals(strChoiceUsbIpc)) {//ipc模式
			modeParamInfo.setChoiceUsbIpc("ipc");
			
			String isNonCenterStorage = FmsPropertyPlaceholder.getProperty("nonCenterStorage");
			if (StringUtils.isEmpty(isNonCenterStorage)) {
				throw new ExpectedException("", "未获取到中心存储的状态");
			}
			if ("true".equals(isNonCenterStorage)) {
				modeParamInfo.setNonCenterStorage("true");
			} else {
				modeParamInfo.setNonCenterStorage("false");
			}
			
			String isChannelIpBind = FmsPropertyPlaceholder.getProperty("channelIpBind");
			if (StringUtils.isEmpty(isChannelIpBind)) {
				throw new ExpectedException("", "未获取到通道ip绑定的状态");
			}
			if ("true".equals(isChannelIpBind)) {
				modeParamInfo.setNipLimit("true");
			} else {
				modeParamInfo.setNipLimit("false");
			}
			
		} else {//usb模式
			modeParamInfo.setChoiceUsbIpc("usb");
			
			String isLogReport = FmsPropertyPlaceholder.getProperty("logReport");
			if (StringUtils.isEmpty(isLogReport)) {
				throw new ExpectedException("", "未获取到客户端日志上报的状态");
			}
			if ("true".equals(isLogReport)) {
				modeParamInfo.setLogReport("true");
			} else {
				modeParamInfo.setLogReport("false");
			}
			
			
			String strLogReportMethod = FmsPropertyPlaceholder.getProperty("logReportMethod");
			if (StringUtils.isEmpty(strLogReportMethod)) {
				throw new ExpectedException("", "未获取到客户端日志上报的方式");
			}
			if ("ftp".equals(strLogReportMethod)) {
				modeParamInfo.setLogReportMethod("ftp");
			} else {
				modeParamInfo.setLogReportMethod("http");
			}
		}
		
		return modeParamInfo;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#updatePasswordParam(com.hikvision.finance.fms.modules.sysconfig.dto.PasswordParamInfo)
     */
    @Override
	public AjaxData updatePasswordParam(PasswordParamInfo passwordParamInfo) {
		AjaxData ajaxData = new AjaxData(false);
		List<Sysconfig> sysconfigs = sysconfigDao.findBy("ntype", SysBigType.SYS_B_PWD);
		for (Sysconfig config : sysconfigs) {
			switch (config.getNkey()) {
				case SysConfigType.PWD_FRESH_DAYS: // 密码保鲜天数
					config.setStrValue(passwordParamInfo.getPwdFreshDays());
					config.setStrUpdatetime(new Date());
					break;
				case SysConfigType.DEFAULT_PASSWORD: // 平台初始密码
					config.setStrValue(passwordParamInfo.getDefaultPassword());
					config.setStrUpdatetime(new Date());
					break;
				case SysConfigType.PASSWORD_SECRUITY_LEVEL: // 密码安全等级
					config.setStrValue(passwordParamInfo.getPasswordSecruityLevel());
					config.setStrUpdatetime(new Date());
					break;
				case SysConfigType.LOGIN_TIMES: // 登录次数
					config.setStrValue(passwordParamInfo.getLoginTimes());
					config.setStrUpdatetime(new Date());
					break;
				case SysConfigType.LOCK_TIME: // 账户锁定次数
					config.setStrValue(passwordParamInfo.getLockTime());
					config.setStrUpdatetime(new Date());
					break;
			}
			sysconfigDao.update(config);
		}
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.CONFIG.getCode(),GlobalMessageUtil.getMessage("log.safe.app"));
		return ajaxData;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#getPasswordParam()
     */
    @Override
	public PasswordParamInfo getPasswordParam() {
		PasswordParamInfo passwordParamInfo = new PasswordParamInfo();
		List<Sysconfig> sysconfigs = sysconfigDao.findBy("ntype", SysBigType.SYS_B_PWD);
		for (Sysconfig config : sysconfigs) {
			switch (config.getNkey()) {
				case SysConfigType.PWD_FRESH_DAYS: // 密码保鲜天数
					passwordParamInfo.setPwdFreshDays(config.getStrValue());
					break;
				case SysConfigType.DEFAULT_PASSWORD: // 平台初始密码
					passwordParamInfo.setDefaultPassword(config.getStrValue());
					break;
				case SysConfigType.PASSWORD_SECRUITY_LEVEL: // 密码安全等级
					passwordParamInfo.setPasswordSecruityLevel(config.getStrValue());
					break;
				case SysConfigType.LOGIN_TIMES: // 登录次数
					passwordParamInfo.setLoginTimes(config.getStrValue());
					break;
				case SysConfigType.LOCK_TIME: // 登录锁定时间
					passwordParamInfo.setLockTime(config.getStrValue());
					break;
			}
		}
		return passwordParamInfo;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#updateLogParam(com.hikvision.finance.fms.modules.sysconfig.dto.LogParamInfo)
     */
    @Override
	public AjaxData updateLogParam(LogParamInfo logParamInfo) {
		AjaxData ajaxData = new AjaxData(false);
		List<Sysconfig> sysconfigs = sysconfigDao.findBy("ntype", SysBigType.SYS_B_LOG);
		for (Sysconfig config : sysconfigs) {
			switch (config.getNkey()) {
				case SysConfigType.CONFIG_LOG: // 配置日志
					config.setStrValue(logParamInfo.getConfigLog());
					config.setStrUpdatetime(new Date());
					break;
				case SysConfigType.SYSTEM_LOG: // 系统日志
					config.setStrValue(logParamInfo.getSystemLog());
					config.setStrUpdatetime(new Date());
					break;
				case SysConfigType.DELETE_LOG: // 删除日志
					config.setStrValue(logParamInfo.getNdeleteLog());
					config.setStrUpdatetime(new Date());
					break;
				case SysConfigType.REVIEW_LOG: // 回放日志
					config.setStrValue(logParamInfo.getReviewLog());
					config.setStrUpdatetime(new Date());
					break;
				case SysConfigType.ALARM_LOG: // 报警日志
					config.setStrValue(logParamInfo.getAlarmLog());
					config.setStrUpdatetime(new Date());
					break;
				case SysConfigType.OPERA_LOG: // 操作日志
					config.setStrValue(logParamInfo.getOperaLog());
					config.setStrUpdatetime(new Date());
					break;
			}
			sysconfigDao.update(config);
		}
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.CONFIG.getCode(),GlobalMessageUtil.getMessage("log.logger.app"));
		return ajaxData;
		
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#getLogParam()
     */
    @Override
	public LogParamInfo getLogParam() {
		LogParamInfo logParamInfo = new LogParamInfo();
		List<Sysconfig> sysconfigs = sysconfigDao.findBy("ntype", SysBigType.SYS_B_LOG);
		for (Sysconfig config : sysconfigs) {
			switch (config.getNkey()) {
				case SysConfigType.CONFIG_LOG: // 配置日志
					logParamInfo.setConfigLog(config.getStrValue());
					break;
				case SysConfigType.SYSTEM_LOG: // 系统日志
					logParamInfo.setSystemLog(config.getStrValue());
					break;
				case SysConfigType.DELETE_LOG: // 删除日志
					logParamInfo.setNdeleteLog(config.getStrValue());
					break;
				case SysConfigType.REVIEW_LOG: // 回放日志
					logParamInfo.setReviewLog(config.getStrValue());
					break;
				case SysConfigType.ALARM_LOG: // 报警日志
					logParamInfo.setAlarmLog(config.getStrValue());
					break;
				case SysConfigType.OPERA_LOG: // 操作日志
					logParamInfo.setOperaLog(config.getStrValue());
					break;
			}
		}
		return logParamInfo;
	}
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#updatePlatformParam(com.hikvision.finance.fms.modules.sysconfig.dto.PlatformParamInfo)
     */
    @Override
	public AjaxData updatePlatformParam(PlatformParamInfo platformParamInfo) {
		AjaxData ajaxData = new AjaxData(false);
		try {
			List<Sysconfig> sysconfigs = sysconfigDao.findBy("ntype", SysBigType.SYS_B_LOGO);
			for (Sysconfig config : sysconfigs) {
				switch (config.getNkey()) {
					case SysConfigType.PLATFORM_DISCRIBE: // 平台描述文字
						config.setStrValue(platformParamInfo.getPlatformDiscribe());
						config.setStrUpdatetime(new Date());
						dataManager.set(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.PLATFORM_DISCRIBE,
						        platformParamInfo.getPlatformDiscribe());
						break;
					case SysConfigType.PLATFORM_COPYRIGHT: // 版权描述文字
						config.setStrValue(platformParamInfo.getPlatformCopyright());
						config.setStrUpdatetime(new Date());
						dataManager.set(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.PLATFORM_COPYRIGHT,
						        platformParamInfo.getPlatformCopyright());
						break;
					case SysConfigType.IMG_LOGO: // 登录页LOGO图标
						if (platformParamInfo.getImgLogo() != null) {
							config.setStrValue(platformParamInfo.getImgLogo());
							config.setStrUpdatetime(new Date());
							dataManager.set(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.IMG_LOGO,
							        platformParamInfo.getImgLogo());
						}
						break;
					case SysConfigType.IMG_BAR: // 导航栏logo
						if (platformParamInfo.getImgBar() != null) {
							config.setStrValue(platformParamInfo.getImgBar());
							config.setStrUpdatetime(new Date());
							dataManager.set(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.IMG_BAR,
							        platformParamInfo.getImgBar());
						}
						break;
					case SysConfigType.IMG_LOGIN: // 登录页图片
						if (platformParamInfo.getImgLogin() != null) {
							config.setStrValue(platformParamInfo.getImgLogin());
							config.setStrUpdatetime(new Date());
							dataManager.set(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.IMG_LOGIN,
							        platformParamInfo.getImgLogin());
						}
						break;
				}
				sysconfigDao.update(config);
			}
			
		} catch (DataLoadException e) {
			throw new ExpectedException("应用平台信息出错");
		}
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.CONFIG.getCode(),GlobalMessageUtil.getMessage("log.plat.app"));
		return ajaxData;
		
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#resetPlatformParam(com.hikvision.finance.fms.modules.sysconfig.dto.PlatformParamInfo)
     */
    @Override
	public AjaxData resetPlatformParam(PlatformParamInfo platformParamInfo) {
		AjaxData ajaxData = new AjaxData(false);
		try {
			List<Sysconfig> sysconfigs = sysconfigDao.findBy("ntype", SysBigType.SYS_B_LOGO);
			for (Sysconfig config : sysconfigs) {
				switch (config.getNkey()) {
					case SysConfigType.PLATFORM_DISCRIBE: // 平台描述文字
						config.setStrValue(platformParamInfo.getPlatformDiscribe());
						config.setStrUpdatetime(new Date());
						dataManager.set(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.PLATFORM_DISCRIBE,
						        platformParamInfo.getPlatformDiscribe());
						break;
					case SysConfigType.PLATFORM_COPYRIGHT: // 版权描述文字
						config.setStrValue(platformParamInfo.getPlatformCopyright());
						config.setStrUpdatetime(new Date());
						dataManager.set(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.PLATFORM_COPYRIGHT,
						        platformParamInfo.getPlatformCopyright());
						break;
					case SysConfigType.IMG_LOGO: // 登录页LOGO图标
						config.setStrValue(platformParamInfo.getImgLogo());
						config.setStrUpdatetime(new Date());
						dataManager.set(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.IMG_LOGO,
						        platformParamInfo.getImgLogo());
						break;
					case SysConfigType.IMG_BAR: // 导航栏logo
						config.setStrValue(platformParamInfo.getImgBar());
						config.setStrUpdatetime(new Date());
						dataManager.set(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.IMG_BAR,
						        platformParamInfo.getImgBar());
						break;
					case SysConfigType.IMG_LOGIN: // 登录页图片
						config.setStrValue(platformParamInfo.getImgLogin());
						config.setStrUpdatetime(new Date());
						dataManager.set(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.IMG_LOGIN,
						        platformParamInfo.getImgLogin());
						break;
				}
				sysconfigDao.update(config);
			}
			
		} catch (DataLoadException e) {
			throw new ExpectedException("恢复默认平台信息出错");
		}
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.CONFIG.getCode(),GlobalMessageUtil.getMessage("log.plat.default"));
		return ajaxData;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#getPlatformParam()
     */
    @Override
	public PlatformParamInfo getPlatformParam() {
		PlatformParamInfo platformParamInfo = new PlatformParamInfo();
		List<Sysconfig> sysconfigs = sysconfigDao.findBy("ntype", SysBigType.SYS_B_LOGO);
		for (Sysconfig config : sysconfigs) {
			switch (config.getNkey()) {
				case SysConfigType.PLATFORM_DISCRIBE: // 平台描述文字
					platformParamInfo.setPlatformDiscribe(config.getStrValue());
					break;
				case SysConfigType.PLATFORM_COPYRIGHT: // 版权描述文字
					platformParamInfo.setPlatformCopyright(config.getStrValue());
					break;
				case SysConfigType.IMG_LOGO: // 平台LOGO图标
					platformParamInfo.setImgLogo(config.getStrValue());
					break;
				case SysConfigType.IMG_BAR: // 登录页logo
					platformParamInfo.setImgBar(config.getStrValue());
					break;
				case SysConfigType.IMG_LOGIN: // 登录页图片
					platformParamInfo.setImgLogin(config.getStrValue());
					break;
			}
		}
		return platformParamInfo;
	}

    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#getBussinessParamIn()
     */
    @Override
    public List<Sysconfig> getBussinessParamIn() {
    	//获取大类型为65的  可用的数据
    	List<Sysconfig> sysconfigs = sysconfigDao.findBy(new String[]{"ntype","nenabled"}, new Object[]{SysBigType.SYS_B_BUSINESS,SysDictionary.enable});
    	List<Sysconfig> list = new ArrayList<Sysconfig>();
    	for(Sysconfig sysconfig: sysconfigs){
    		if(sysconfig.getStrValue()==null){
    			list.add(sysconfig);
    		}
    	}
	    return list;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#getBussinessParamOut()
     */
    @Override
    public List<Sysconfig> getBussinessParamOut() {
    	//获取大类型为65的  禁用的数据
    	List<Sysconfig> sysconfigs = sysconfigDao.findBy(new String[]{"ntype","nenabled"}, new Object[]{SysBigType.SYS_B_BUSINESS,SysDictionary.disable});
    	List<Sysconfig> list = new ArrayList<Sysconfig>();
    	for(Sysconfig sysconfig: sysconfigs){
    		if(sysconfig.getStrValue()==null){
    			list.add(sysconfig);
    		}
    	}
	    return list;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#enableBussinessParam(java.util.List)
     */
    @Override
    public AjaxData enableBussinessParam(List<Integer> keys) {
    	AjaxData ajaxData = new AjaxData(false);
	   for(Integer key:keys){
		   Sysconfig sysconfig = sysconfigDao.findUniqueBy("nkey", key);
		   sysconfig.setNenabled(SysDictionary.enable);
		   sysconfigDao.update(sysconfig);
	   }
	   configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.CONFIG.getCode(),GlobalMessageUtil.getMessage("log.biz.app"));
	    return ajaxData;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#disableBussinessParam(java.util.List)
     */
    @Override
    public AjaxData disableBussinessParam(List<Integer> keys) {
    	AjaxData ajaxData = new AjaxData(false);
 	   for(Integer key:keys){
		   Sysconfig sysconfig = sysconfigDao.findUniqueBy("nkey", key);
		   sysconfig.setNenabled(SysDictionary.disable);
		   sysconfigDao.update(sysconfig);
	   }
 	  configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.CONFIG.getCode(),GlobalMessageUtil.getMessage("log.biz.app"));
	    return ajaxData;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#editBussinessName(java.lang.Integer)
     */
    @Override
	public AjaxData editBussinessName(Sysconfig sysconfig) {
		AjaxData ajaxData = new AjaxData(false);
		Sysconfig sysconfigNew = sysconfigDao.findUniqueBy("nkey", sysconfig.getNkey());
		sysconfigNew.setStrDescribe(sysconfig.getStrDescribe());
		sysconfigDao.update(sysconfigNew);
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.CONFIG.getCode(),GlobalMessageUtil.getMessage("log.biz.app"));
		return ajaxData;
	}

    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#getNtpParam()
     */
    @Override
    public NtpParamInfo getNtpParam() {
    	NtpParamInfo ntpParamInfo = new NtpParamInfo();
    	List<Sysconfig> sysconfigs = sysconfigDao.findBy("ntype", SysBigType.SYS_B_NTP);
    	for (Sysconfig config : sysconfigs) {
    		switch (config.getNkey()) {
    			case SysConfigType.NTP_ADDRESS:
    				ntpParamInfo.setNenable(Constants.SysDictionary.disable);
    				if(StringUtils.isNotBlank(config.getStrValue())){
    					ntpParamInfo.setNenable(Constants.SysDictionary.enable);
    				}
    				ntpParamInfo.setNtpAddress(config.getStrValue());
    				break;
    			case SysConfigType.NTP_PORT:
    				ntpParamInfo.setNtpPortNo(config.getStrValue());
        			break;
    			case SysConfigType.NTP_INTERVAL:
    				ntpParamInfo.setNtpInterval(config.getStrValue());
        			break;
    		}
    	}
	    return ntpParamInfo;
    }
    

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService#updateNtpParam(com.hikvision.finance.fms.modules.sysconfig.dto.NtpParamInfo)
     */
    @Override
    public AjaxData updateNtpParam(NtpParamInfo ntpParamInfo) {
    	AjaxData ajaxData = new AjaxData(false);
    	Date date = new Date();
    	Sysconfig address = sysconfigDao.findUniqueBy("nkey", SysConfigType.NTP_ADDRESS);
    	Sysconfig port = sysconfigDao.findUniqueBy("nkey", SysConfigType.NTP_PORT);
    	Sysconfig interval = sysconfigDao.findUniqueBy("nkey", SysConfigType.NTP_INTERVAL);
    	
    	//NTP信息变更，重置设备NTP状态
    	if(StringUtils.isNotBlank(address.getStrValue()) && (
    			address.getStrValue() != ntpParamInfo.getNtpAddress() || 
    			port.getStrValue() != ntpParamInfo.getNtpPortNo() || 
    			interval.getStrValue() != ntpParamInfo.getNtpInterval())){
    			deviceDao.updateDeviceNtp(Constants.SysDictionary.disable, null);
    	}
    	address.setStrValue(ntpParamInfo.getNtpAddress());
    	address.setStrUpdatetime(date);
    	port.setStrValue(ntpParamInfo.getNtpPortNo());
    	port.setStrUpdatetime(date);
    	interval.setStrValue(ntpParamInfo.getNtpInterval());
    	interval.setStrUpdatetime(date);
    	
    	sysconfigDao.update(address);
    	sysconfigDao.update(port);
    	sysconfigDao.update(interval);
    	
    	configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.CONFIG.getCode(),GlobalMessageUtil.getMessage("log.ntp.update"));
	    return ajaxData;
    }
    
    //=======================================getter/setter=================================

	
    public ISysconfigDao getSysconfigDao() {
    	return sysconfigDao;
    }

	
    public void setSysconfigDao(ISysconfigDao sysconfigDao) {
    	this.sysconfigDao = sysconfigDao;
    }

	
    public DataManager getDataManager() {
    	return dataManager;
    }

	
    public void setDataManager(DataManager dataManager) {
    	this.dataManager = dataManager;
    }

	
    public IConfigLogDao getConfigLogDao() {
    	return configLogDao;
    }

	
    public void setConfigLogDao(IConfigLogDao configLogDao) {
    	this.configLogDao = configLogDao;
    }

	public IDeviceDao getDeviceDao() {
	    return deviceDao;
    }

	public void setDeviceDao(IDeviceDao deviceDao) {
	    this.deviceDao = deviceDao;
    }

}
