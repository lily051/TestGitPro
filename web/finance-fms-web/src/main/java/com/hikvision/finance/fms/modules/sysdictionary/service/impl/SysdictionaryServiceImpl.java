/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 下午4:30:38
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysdictionary.service.impl;

import java.io.Serializable;
import java.util.List;

import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.model.Sysdictionary;
import com.hikvision.finance.fms.modules.sysdictionary.dao.ISysdictionaryDao;
import com.hikvision.finance.fms.modules.sysdictionary.qo.SysdictionaryQo;
import com.hikvision.finance.fms.modules.sysdictionary.service.ISysdictionaryService;


/**
 * <p></p>
 * @author jinxindong 2016年3月9日 下午4:30:38
 * @version V1.0 
 */
public class SysdictionaryServiceImpl extends BaseServiceImpl<Sysdictionary, SysdictionaryQo, ISysdictionaryDao> implements ISysdictionaryService {
 
	private ISysdictionaryDao sysdictionaryDao;
	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.IBaseService#deleteByIds(java.io.Serializable[])
     */
    @Override
    public void deleteByIds(Serializable[] paramArrayOfSerializable) {
	    // TODO Auto-generated method stub
	    
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.BaseServiceImpl#getDao()
     */
    @Override
    protected ISysdictionaryDao getDao() {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysdictionary.service.ISysdictionaryService#getDicByNtype(java.lang.Integer)
     */
    @Override
    public List<Sysdictionary> getDicByNtype(Integer ntype) {
    	SysdictionaryQo sysdictionaryQo = new SysdictionaryQo();
    	sysdictionaryQo.setNtype(ntype);
    	sysdictionaryQo.setNenabled(Constants.SysDictionary.enable);
    	List<Sysdictionary> sysdictionaries = sysdictionaryDao.queryList(sysdictionaryQo);
	    return sysdictionaries;
    }

	
    public ISysdictionaryDao getSysdictionaryDao() {
    	return sysdictionaryDao;
    }

	
    public void setSysdictionaryDao(ISysdictionaryDao sysdictionaryDao) {
    	this.sysdictionaryDao = sysdictionaryDao;
    }
	

	
}
