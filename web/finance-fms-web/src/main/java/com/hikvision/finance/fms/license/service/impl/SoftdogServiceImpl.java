/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月19日 上午11:00:06
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.license.service.impl;

import java.io.Serializable;
import java.util.Date;

import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.exception.CMSException;
import com.hikvision.finance.fms.license.dao.ISoftdogDao;
import com.hikvision.finance.fms.license.qo.SoftdogQo;
import com.hikvision.finance.fms.license.service.ISoftdogService;
import com.hikvision.finance.fms.license.util.LicenseConstant;
import com.hikvision.finance.fms.license.util.LicenseConstant.DOGTYPE;
import com.hikvision.finance.fms.model.Softdongle;
import com.hikvision.finance.fms.modules.sysconfig.dao.ISysconfigDao;


/**
 * <p></p>
 * @author jinxindong 2016年4月19日 上午11:00:06
 * @version V1.0 
 */
public class SoftdogServiceImpl extends BaseServiceImpl<Softdongle, SoftdogQo, ISoftdogDao> implements ISoftdogService{
	
	private ISoftdogDao softdogDao;
	private ISysconfigDao sysconfigDao;

    /* 
     * 检查授权
     * @see com.hikvision.finance.fms.license.service.ISoftdogService#checkDBAuthorization()
     */
    @Override
	public Boolean checkDBAuthorization() {
		
		SoftdogQo softdogQo = new SoftdogQo();
		softdogQo.setnType(DOGTYPE.DOG_VERSION);
		Softdongle softdog = softdogDao.queryUnique(softdogQo);
		
		// 1. 检查授权是否存在
		if (softdog == null) {
			return false;// 无授权信息，授权失效！
		}
		
		// 2. 检查是否失效
		Date nowDate = softdogDao.getSysDate();
		long diffTime = (nowDate.getTime() - softdog.getDtUpdateTime().getTime()) / 1000;
		if (diffTime > LicenseConstant.SOFT_TIME || diffTime < 0) {
			return false;// 授权信息未更新，授权失效！
		}
		return true;
	}

	/* 
	 * 写入或者更新授权信息
     * @see com.hikvision.finance.fms.license.service.ISoftdogService#saveOrUpdateSoftDog(com.hikvision.finance.fms.model.Softdongle)
     */
    @Override
    public void saveOrUpdateSoftDog(Softdongle softdog) {
    	// 1. 检查参数
		if (softdog == null || softdog.getNtype() == null || softdog.getNvalue() == null) {
			throw new CMSException(CMSException.INT_REQ_REQUIRED, "添加/更新 授权狗信息 属性不足！");
		}
    			
    	Softdongle softdogDb = softdogDao.findUniqueBy("ntype", softdog.getNtype());
    	
		// 2. 更新/写入授权
		if (softdogDb != null) {
			// 2.1  更新
			softdogDb.setNvalue(softdog.getNvalue());
			softdogDb.setStrDescribe(softdog.getStrDescribe());
			softdogDb.setDtUpdateTime(getSysDate());
			
			softdogDao.update(softdogDb);
		} else {
			// 2.2 写入
			softdogDb = new Softdongle();
			softdogDb.setNtype(softdog.getNtype());
			softdogDb.setNvalue(softdog.getNvalue());
			softdogDb.setStrDescribe(softdog.getStrDescribe());
			softdogDb.setDtUpdateTime(getSysDate());
			
			softdogDao.save(softdogDb);
		}
	    
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.license.service.ISoftdogService#getSysDate()
     */
    @Override
    public Date getSysDate() {
    	return softdogDao.getSysDate();
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.license.service.ISoftdogService#updateSysConfig()
     */
    @Override
	public void updateSysConfig(int key, String value) {
		sysconfigDao.updateBy("nkey", key, "strValue", value);
	}

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
	protected ISoftdogDao getDao() {
		return this.softdogDao;
	}
	
	public ISoftdogDao getSoftdogDao() {
		return softdogDao;
	}
	
	public void setSoftdogDao(ISoftdogDao softdogDao) {
		this.softdogDao = softdogDao;
	}
	
	public ISysconfigDao getSysconfigDao() {
		return sysconfigDao;
	}
	
	public void setSysconfigDao(ISysconfigDao sysconfigDao) {
		this.sysconfigDao = sysconfigDao;
	}
	
}
