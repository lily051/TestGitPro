/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-27 下午8:05:30
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.log.service.impl;

import java.io.Serializable;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.model.Operlog;
import com.hikvision.finance.fms.webdevice.log.dao.ICollLogDao;
import com.hikvision.finance.fms.webdevice.log.dto.OperlogInfo;
import com.hikvision.finance.fms.webdevice.log.qo.OperlogsQo;
import com.hikvision.finance.fms.webdevice.log.service.ICollLogService;

/**
 * <p></p>
 * @author xujiangfei 2016-4-27 下午8:05:30
 * @version V1.0 
 */
public class CollLogServiceImpl extends BaseServiceImpl<Operlog, OperlogsQo, ICollLogDao> implements ICollLogService {
	
	private ICollLogDao collLogDao;
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.log.service.ICollLogService#getDataPage(int, int, com.hikvision.finance.fms.model.Operlog)
	 */
	@Override
	public Page getDataPage(int limit, int start, OperlogInfo operlogInfo) {
		return collLogDao.getDataPage(limit, start, operlogInfo);
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.extend.service.IBaseService#deleteByIds(java.io.Serializable[])
	 */
	@Override
	public void deleteByIds(Serializable[] paramArrayOfSerializable) {
		
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.extend.service.BaseServiceImpl#getDao()
	 */
	@Override
	protected ICollLogDao getDao() {
		return this.getCollLogDao();
	}
	
	public ICollLogDao getCollLogDao() {
		return collLogDao;
	}
	
	public void setCollLogDao(ICollLogDao collLogDao) {
		this.collLogDao = collLogDao;
	}
	
}
