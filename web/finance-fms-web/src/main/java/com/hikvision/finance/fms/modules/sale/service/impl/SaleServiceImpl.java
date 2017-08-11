/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月7日 下午6:52:43
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sale.service.impl;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import com.hikvision.finance.core.util.BeanUtils;
import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.enums.LogModuledEnum;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.OperStatus;
import com.hikvision.finance.fms.common.util.ExcelUtil;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.model.Paramdictionary;
import com.hikvision.finance.fms.model.Procomplaint;
import com.hikvision.finance.fms.modules.card.dao.ICardDao;
import com.hikvision.finance.fms.modules.log.dao.IConfigLogDao;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.sale.dao.IClientInputDao;
import com.hikvision.finance.fms.modules.sale.dao.IProcomplaintDao;
import com.hikvision.finance.fms.modules.sale.dto.SaleInfo;
import com.hikvision.finance.fms.modules.sale.qo.SaleQo;
import com.hikvision.finance.fms.modules.sale.service.ISaleService;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;


/**
 * <p></p>
 * @author jinxindong 2016年4月7日 下午6:52:43
 * @version V1.0 
 */
public class SaleServiceImpl extends BaseServiceImpl<Clientinput,SaleQo,IClientInputDao> implements ISaleService{

	private IClientInputDao clientInputDao; 
	private ICardDao cardDao;
	private IRegionDao regionDao; 
	private IConfigLogDao configLogDao;
	private IProcomplaintDao procomplaintDao;
	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.IBaseService#deleteByIds(java.io.Serializable[])
     */
    @Override
    public void deleteByIds(Serializable[] paramArrayOfSerializable) {
	    
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sale.service.ISaleService#getSaleDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.sale.dto.SaleInfo)
     */
    @Override
    public Page getSaleDataPage(Integer limit, Integer start, SaleInfo saleInfo) {
    	return this.clientInputDao.getSaleDataPage(limit, start, saleInfo);
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sale.service.ISaleService#getBizOrderInfo(java.lang.Integer)
     */
    @Override
    public SaleInfo getBizOrderInfo(SaleInfo condition) {
	   Clientinput clientinput = clientInputDao.get(condition.getId());
	   SaleInfo saleInfo = new SaleInfo();
	    try {
	        BeanUtils.copyNotNullProperties(saleInfo, clientinput);
	        Paramdictionary card = cardDao.get(clientinput.getCreditId());
	        saleInfo.setCreditCodeName(card.getStrName());
	        saleInfo.setInputRegionCode(regionDao.get(saleInfo.getInputRegionId()).getStrCode());
	        //显示投诉信息
	        if (clientinput.getStatus() == Constants.ClientStatus.CLIENT_COMPLAINNT) {
	        	// 针对于理财录像的投诉签约tab, 分别取出对应的投诉信息
	        	if (Constants.StrRecordType.STR_RECORD_COMPLAINNT.equals(condition.getVideoType())) {
		        	Procomplaint procomplaint = procomplaintDao.findUniqueBy("nRecordId", condition.getVideoId());
		        	saleInfo.setCompTime(procomplaint.getCompTime());
		        	saleInfo.setCompReason(procomplaint.getCompReason());
		        }else { // 针对于理财录像的签约录像tab, 销售查询, 到期管理, 取出最后一条投诉信息
		        	List<Procomplaint> procomplaints = procomplaintDao.findBy("clientId", clientinput.getId());
		        	//投诉状态, 一定有投诉信息 //取出最后一条信息
		        	Procomplaint procomplaint = procomplaints.get(procomplaints.size() - 1);
		        	saleInfo.setCompTime(procomplaint.getCompTime());
		        	saleInfo.setCompReason(procomplaint.getCompReason());
		        } 
	        }
        } catch (Exception e) {
	        throw new ExpectedException("", "拷贝bean出错[getBizOrderInfo]");
        }
	    return saleInfo;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sale.service.ISaleService#exportSaleExcel(com.hikvision.finance.fms.modules.sale.dto.SaleInfo, java.lang.String[], java.lang.String[], java.lang.String)
     */
    @Override
    public boolean exportSaleExcel(SaleInfo saleInfo, String[] titleName, String[] attrName, String path) {
		// 1，依次准备标题、标题映射属性、Sheet以及对应内容
		List<String[]> titleNames = new ArrayList<String[]>();
		List<String[]> attrNames = new ArrayList<String[]>();
		LinkedHashMap<String, List<?>> map = new LinkedHashMap<String, List<?>>();
		titleNames.add(titleName);
		attrNames.add(attrName);
		List<SaleInfo> saleInfos1 = clientInputDao.getSaleDataList(saleInfo);
		List<SaleInfo> saleInfos = new ArrayList<SaleInfo>();
		//if(saleInfos1!=null && saleInfos1.size()>0){
			for(SaleInfo info: saleInfos1){
				String date = DateUtils.getStringDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, info.getBusinessTime());
				info.setBusinessTimeString(date);
				Map<String, String> levelMap = new HashMap<String, String>(){
					//{put("0", "低");    put("1", "中"); put("2", "高");}
					{
						put("1", Constants.RiskLevel.RISKLEVEL_ONE); 
						put("2", Constants.RiskLevel.RISKLEVEL_TWO); 
						put("3", Constants.RiskLevel.RISKLEVEL_THREE); 
						put("4", Constants.RiskLevel.RISKLEVEL_FOUR); 
						put("5", Constants.RiskLevel.RISKLEVEL_FIVE); 
						put("6", Constants.RiskLevel.RISKLEVEL_SIX); 
						put("0", Constants.RiskLevel.RISKLEVEL_NONE);
					}
				};
				info.setStrRisklevel(levelMap.get(info.getStrRisklevel()));
				
				Map<Integer, String> saveType = new HashMap<Integer, String>(){
					{ put(0, "年");    put(1, "月"); put(2, "日"); }
				};
				if(info.getGuaranteeYears()!=null){
					info.setGuarantee(info.getGuaranteeYears()+""+saveType.get(info.getGuaranteeType()));
				}
				if(info.getProductSaveMonth()!=null){
					info.setProductSave(info.getProductSaveMonth()+""+saveType.get(info.getProductSaveType()));
				}
				if (info.getLockClient() == 1) {
					info.setStrLockClient("审批通过");
				} else if (info.getLockClient() == 2) {
					info.setStrLockClient("审批退回");
				} else {
					info.setStrLockClient("待审批");
				}
				saleInfos.add(info);
			}	
		//}
		map.put("Sheet1", saleInfos);
		// 2，创建Excel的数据对象并导出表格
		ExcelUtil.ExcelExportData setInfo = new ExcelUtil.ExcelExportData();
		setInfo.setColumnNames(titleNames);
		setInfo.setFieldNames(attrNames);
		setInfo.setDataMap(map);
		boolean exportResult = false;
		try {
			exportResult = ExcelUtil.export2File(setInfo, path);
			configLogDao.setLogContent(OperStatus.EXPORT, LogModuledEnum.SALE.getCode(),GlobalMessageUtil.getMessage("log.sale.export"));
		} catch (Exception e) {
			e.printStackTrace();
		}
		return exportResult;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.BaseServiceImpl#getDao()
     */
    @Override
    protected IClientInputDao getDao() {
	    return clientInputDao;
    }
    
	//==================getter/setter========================================================
	
	public ICardDao getCardDao() {
		return cardDao;
	}
	
	public void setCardDao(ICardDao cardDao) {
		this.cardDao = cardDao;
	}
	
	public IRegionDao getRegionDao() {
		return regionDao;
	}
	
	public void setRegionDao(IRegionDao regionDao) {
		this.regionDao = regionDao;
	}
	
	public IConfigLogDao getConfigLogDao() {
		return configLogDao;
	}
	
	public void setConfigLogDao(IConfigLogDao configLogDao) {
		this.configLogDao = configLogDao;
	}
	
	public IClientInputDao getClientInputDao() {
		return clientInputDao;
	}
	
	public void setClientInputDao(IClientInputDao clientInputDao) {
		this.clientInputDao = clientInputDao;
	}

	public IProcomplaintDao getProcomplaintDao() {
		return procomplaintDao;
	}

	public void setProcomplaintDao(IProcomplaintDao procomplaintDao) {
		this.procomplaintDao = procomplaintDao;
	}
}
