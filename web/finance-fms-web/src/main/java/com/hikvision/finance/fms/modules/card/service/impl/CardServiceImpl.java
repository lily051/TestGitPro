/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月28日 下午1:55:25
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.card.service.impl;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import com.hikvision.finance.core.util.BeanUtils;
import com.hikvision.finance.core.util.ListUtil;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.enums.LogModuledEnum;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.OperStatus;
import com.hikvision.finance.fms.common.util.Constants.SysDictionary;
import com.hikvision.finance.fms.model.Paramdictionary;
import com.hikvision.finance.fms.modules.card.dao.ICardDao;
import com.hikvision.finance.fms.modules.card.dto.ParamdictionaryInfo;
import com.hikvision.finance.fms.modules.card.qo.ParamdictionaryQo;
import com.hikvision.finance.fms.modules.card.service.ICardService;
import com.hikvision.finance.fms.modules.log.dao.IConfigLogDao;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.AjaxData;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;



/**
 * <p></p>
 * @author jinxindong 2016年3月28日 下午1:55:25
 * @version V1.0 
 */
public class CardServiceImpl extends BaseServiceImpl< Paramdictionary,ParamdictionaryQo, ICardDao> implements ICardService{

	private ICardDao cardDao;
	private IConfigLogDao configLogDao;
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
    protected ICardDao getDao() {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.card.service.ICardService#getDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.card.dto.ParamdictionaryInfo)
     */
    @Override
	public Page getDataPage(Integer limit, Integer start, ParamdictionaryInfo paramdictionaryInfo) {
		Page page = cardDao.getDataPage(limit, start, paramdictionaryInfo);
		return page;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.card.service.ICardService#addCard(com.hikvision.finance.fms.modules.card.dto.ParamdictionaryInfo)
     */
    @Override
    public AjaxData addCard(ParamdictionaryInfo paramdictionaryInfo) {
    	AjaxData ajaxData = new AjaxData(false);
    	//校验证书名字和编码是否存在相同
		if(cardDao.findUniqueBy("strName", paramdictionaryInfo.getStrName()) != null){
			return ajaxData.setFailureMsg("证件名已存在");
		}
		if(cardDao.findUniqueBy("strCode", paramdictionaryInfo.getStrCode()) != null){
			return ajaxData.setFailureMsg("证件编号已存在");
		}
    	Paramdictionary paramdictionary = new Paramdictionary();
    	paramdictionary.setStrName(paramdictionaryInfo.getStrName());
    	paramdictionary.setStrCode(paramdictionaryInfo.getStrCode());
    	paramdictionary.setEnable(Constants.CONS_0);
    	cardDao.save(paramdictionary);
    	configLogDao.setLogContent(OperStatus.ADD, LogModuledEnum.CONFIG.getCode(),GlobalMessageUtil.getMessage("log.card.add",new String[] {paramdictionaryInfo.getStrName()}));
	    return ajaxData;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.card.service.ICardService#getCardById(java.lang.Integer)
     */
    @Override
    public ParamdictionaryInfo getCardById(Integer cardId) {
    	ParamdictionaryInfo paramdictionaryInfo = new ParamdictionaryInfo();
    	Paramdictionary paramdictionary = cardDao.findUniqueBy("id", cardId);
		try {
			BeanUtils.copyNotNullProperties(paramdictionaryInfo, paramdictionary);
		} catch (Exception e) {
			throw new ExpectedException("", "拷贝bean失败[getCardById]");
		}
	    return paramdictionaryInfo;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.card.service.ICardService#updateCard(com.hikvision.finance.fms.modules.card.dto.ParamdictionaryInfo)
     */
    @Override
    public AjaxData updateCard(ParamdictionaryInfo paramdictionaryInfo) {
    	AjaxData ajaxData = new AjaxData(false);
    	Paramdictionary paramdictionary = cardDao.get(paramdictionaryInfo.getId());
    	int value = paramdictionaryInfo.getId();
		if(cardDao.findUniqueBy("strName",  paramdictionaryInfo.getStrName(), value) != null){
			return ajaxData.setFailureMsg("证件名已存在");
		}
		if(cardDao.findUniqueBy("strCode",  paramdictionaryInfo.getStrCode(), value) != null){
			return ajaxData.setFailureMsg("证件编号已存在");
		}
    	paramdictionary.setStrName(paramdictionaryInfo.getStrName());
    	paramdictionary.setStrCode(paramdictionaryInfo.getStrCode());
    	cardDao.update(paramdictionary);
    	configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.CONFIG.getCode(),GlobalMessageUtil.getMessage("log.card.update",new String[] {paramdictionaryInfo.getStrName()}));
	    return ajaxData;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.card.service.ICardService#deleteCards(java.util.List)
     */
    @Override
    public AjaxData deleteCards(List<Integer> ids) {
    	AjaxData ajaxData = new AjaxData(false);
    	List<String> names = new ArrayList<String>();
    	for(Integer id : ids){
    		Paramdictionary paramdictionary = cardDao.get(id);
    		cardDao.deleteById(id);
    		names.add(paramdictionary.getStrName()+ " ");
    	}
    	configLogDao.setLogContent(OperStatus.DELETE, LogModuledEnum.CONFIG.getCode(),GlobalMessageUtil.getMessage("log.card.delete",new String[] {ListUtil.listToStr(names)}));
	    return ajaxData;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.card.service.ICardService#enableCards(java.util.List)
     */
    @Override
	public AjaxData enableCards(List<Integer> ids) {
		AjaxData ajaxData = new AjaxData(false);
		List<String> names = new ArrayList<String>();
		for (Integer id : ids) {
			Paramdictionary paramdictionary = cardDao.get(id);
			paramdictionary.setEnable(SysDictionary.enable);
			cardDao.update(paramdictionary);
			names.add(paramdictionary.getStrName());
		}
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.CONFIG.getCode(),GlobalMessageUtil.getMessage("log.card.enable",new String[] {ListUtil.listToStr(names)}));
		return ajaxData;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.card.service.ICardService#disableCards(java.util.List)
     */
    @Override
	public AjaxData disableCards(List<Integer> ids) {
		AjaxData ajaxData = new AjaxData(false);
		List<String> names = new ArrayList<String>();
		for (Integer id : ids) {
			Paramdictionary paramdictionary = cardDao.get(id);
			paramdictionary.setEnable(SysDictionary.disable);
			cardDao.update(paramdictionary);
			names.add(paramdictionary.getStrName());
		}
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.CONFIG.getCode(),GlobalMessageUtil.getMessage("log.card.disable",new String[] {ListUtil.listToStr(names)}));
		return ajaxData;
	}
	
    public ICardDao getCardDao() {
    	return cardDao;
    }

	
    public void setCardDao(ICardDao cardDao) {
    	this.cardDao = cardDao;
    }


	
    public IConfigLogDao getConfigLogDao() {
    	return configLogDao;
    }

	
    public void setConfigLogDao(IConfigLogDao configLogDao) {
    	this.configLogDao = configLogDao;
    }
	
    
    
}
