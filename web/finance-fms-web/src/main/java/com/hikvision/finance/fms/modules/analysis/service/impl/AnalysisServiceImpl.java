/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月13日 下午7:08:28
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.analysis.service.impl;

import java.io.Serializable;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import com.hikvision.finance.core.util.ListUtil;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.modules.analysis.dao.IAnalysisDao;
import com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo;
import com.hikvision.finance.fms.modules.analysis.dto.PieChat;
import com.hikvision.finance.fms.modules.analysis.qo.AnalysisQo;
import com.hikvision.finance.fms.modules.analysis.service.IAnalysisService;
import com.hikvision.finance.fms.modules.product.dao.IProductDao;
import com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.sale.dao.IClientInputDao;


/**
 * <p></p>
 * @author jinxindong 2016年4月13日 下午7:08:28
 * @version V1.0 
 */
public class AnalysisServiceImpl extends BaseServiceImpl<AnalysisInfo,AnalysisQo,IAnalysisDao> implements IAnalysisService{

	private IRegionDao regionDao;
	private IClientInputDao clientInputDao;
	private IProductDao productDao;
	private IProSaveRecordDao proSaveRecordDao;
	
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
    protected IAnalysisDao getDao() {
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.analysis.service.IAnalysisService#getSalesNum(com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo)
     */
/*    @Override
	public AnalysisInfo getOrdersNum(AnalysisInfo analysisInfo) {
		AnalysisInfo info = new AnalysisInfo();
		// 1、已选组织个数 已检查组织个数 合规率
		Integer nregionId = analysisInfo.getNregionId();// 所选组织
		List<Region> regionList = regionDao.findBy("nparentId", nregionId, "nsortId", true);
		Integer nregionNum = regionList.size()+1;
		info.setNregionNum(nregionNum);// 子组织个数
		regionList.add(regionDao.get(nregionId));
		
		// 获取业务单信息
		List<Clientinput> orders = saleDao.getSaleByAnalysisInfo(analysisInfo);
		Map<String, Double> columnMapOrder = new HashMap<String, Double>();// 订单柱状<区域名-数据><x-y>
		Map<String, Double> columnMapComplaint = new HashMap<String, Double>();// 投诉柱状<区域名-数据><x-y>
		double orderMaxAll = 0;// 总签约单数量
		double rateNumAll = 0;// 总投诉单数量
		for (Region region : regionList) {
			double orderMax = 0;// 签约单数量
			double rateNum = 0;// 投诉单数量
			//如果是本级节点
			if(nregionId.equals(region.getId())){
				for (Clientinput order : orders) {
					if (region.getId().equals(order.getInputRegionId())) {
						if (BusiComplainStatus.NOT_COMPLAIN == order.getStatus()) {// 签约
							orderMax++;
						}
						if (BusiComplainStatus.COMPLAINED == order.getStatus()) {// 投诉
							rateNum++;
						}
						
					}
				}
			}else{
				List<Region> strpathList = regionDao.getLikeStrPath(region);
				for (Region regionChild : strpathList) {
					for (Clientinput order : orders) {
						if (regionChild.getId().equals(order.getInputRegionId())) {
							if (BusiComplainStatus.NOT_COMPLAIN == order.getStatus()) {// 签约
								orderMax++;
							}
							if (BusiComplainStatus.COMPLAINED == order.getStatus()) {// 投诉
								rateNum++;
							}
							
						}
					}
				}
			}
			orderMaxAll = orderMaxAll + orderMax;
			rateNumAll = rateNumAll + rateNum;
			if(nregionId.equals(region.getId())){
				columnMapOrder.put(region.getStrName()+"直属", orderMax);
				columnMapComplaint.put(region.getStrName()+"直属", rateNum);
			}else{
				columnMapOrder.put(region.getStrName(), orderMax);
				columnMapComplaint.put(region.getStrName(), rateNum);
			}
		}
		info.setColumnMapOrder(columnMapOrder);// 签约单柱状图
		info.setColumnMapComplaint(columnMapComplaint);// 投诉单柱状图
		info.setOrderMax(String.valueOf((int)(orderMaxAll + rateNumAll)));// 签约总数
		info.setComplaintMax(String.valueOf((int)rateNumAll));// 投诉总数
		if ((orderMaxAll + rateNumAll) == 0) {
			info.setComplaintOrder(String.valueOf(0));
		} else {
			info.setComplaintOrder(String.valueOf(rateNumAll / (orderMaxAll + rateNumAll)*100));// 投诉占比
		}
		return info;
	}*/


	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.analysis.service.IAnalysisService#getRateNum(com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo)
     */
 /*   @Override
	public AnalysisInfo getRateNum(AnalysisInfo analysisInfo) {
		AnalysisInfo info = new AnalysisInfo();
		// 1、已选组织个数 已检查组织个数 合规率
		Integer nregionId = analysisInfo.getNregionId();// 所选组织
		List<Region> regionList = regionDao.findBy("nparentId", nregionId, "nsortId", true);
		Integer nregionNum = regionList.size()+1;
		info.setNregionNum(nregionNum);// 子组织个数
		regionList.add(regionDao.get(nregionId));
		
		// 获取已检查录像
		List<Prosaverecord> prosaverecords = proSaveRecordDao.getRecordByCheck(analysisInfo);
		List<PieChat> pieChats = new ArrayList<PieChat>();
		List<PieChat> pieCheckChats = new ArrayList<PieChat>();
		
		List<PieChat> chats = new ArrayList<PieChat>();
		
		Map<String, Double> columnMap = new LinkedHashMap<String, Double>();
		double conformedNumAll = 0;// 总的组织合规数量
		double notconformedConNumAll = 0;// 总的组织不合规数量
		double uncheckNumAll = 0;// 总的组织未检查数量
		double checkNumAll = 0;// 总的组织检查数量
		for (Region region : regionList) {
			double conformedNum = 0;// 单个组织合规数量
			double notconformedConNum = 0;// 单个组织不合规数量
			double uncheckNum = 0;// 单个组织未检查数量
			//如果是本级节点
			if(nregionId.equals(region.getId())){
				for (Prosaverecord prosaverecord : prosaverecords) {
					if (region.getId().equals(prosaverecord.getRegionId())) {
						if (RecordCheckStatus.CONFORMED == prosaverecord.getRecordCheck()) {// 合规
							conformedNum++;
						}
						if (RecordCheckStatus.NOT_CONFORMED == prosaverecord.getRecordCheck()) {// 不合规
							notconformedConNum++;
						}
						if (RecordCheckStatus.NOT_CHECK == prosaverecord.getRecordCheck()) {// 未检查
							uncheckNum++;
						}						
					}
				}
			}else{
				List<Region> strpathList = regionDao.getLikeStrPath(region);
				for (Region regionChild : strpathList) {
					for (Prosaverecord prosaverecord : prosaverecords) {
						if (regionChild.getId().equals(prosaverecord.getRegionId())) {
							if (RecordCheckStatus.CONFORMED == prosaverecord.getRecordCheck()) {// 合规
								conformedNum++;
							}
							if (RecordCheckStatus.NOT_CONFORMED == prosaverecord.getRecordCheck()) {// 不合规
								notconformedConNum++;
							}
							if (RecordCheckStatus.NOT_CHECK == prosaverecord.getRecordCheck()) {// 未检查
								uncheckNum++;
							}
							
						}
					}
				}	
			}
			conformedNumAll = conformedNumAll + conformedNum;// 合规数量
			notconformedConNumAll = notconformedConNumAll + notconformedConNum;// 不合规
			uncheckNumAll = uncheckNumAll + uncheckNum; // 未检查
			checkNumAll = conformedNumAll + notconformedConNumAll; // 检查
			double compliance = 0;
			if ((notconformedConNum + conformedNum) == 0) {
				compliance = 0;// 合规+不合规=0的情况算 合规率为0
			} else {
				compliance = conformedNum / (notconformedConNum + conformedNum);
			}
			if(nregionId.equals(region.getId())){
				PieChat pieChat = new PieChat();
				columnMap.put(region.getStrName()+"直属", compliance);// 单个合规率
				pieChat.setName(region.getStrName()+"直属");
				pieChat.setValue(String.valueOf(compliance));
				chats.add(pieChat);
			}else{
				PieChat pieChat = new PieChat();
				columnMap.put(region.getStrName(), compliance);// 单个合规率
				pieChat.setName(region.getStrName());
				pieChat.setValue(String.valueOf(compliance));
				chats.add(pieChat);
			}
		
		}
		info.setColumnMap(columnMap);// 柱状图
		info.setColumnList(chats);
		PieChat pieChat1 = new PieChat();
		PieChat pieChat2 = new PieChat();
		PieChat pieCheckChat1 = new PieChat();
		PieChat pieCheckChat2 = new PieChat();
		pieChat1.setName("合规");
		pieChat1.setValue(String.valueOf((int)conformedNumAll));
		pieChat2.setName("不合规");
		pieChat2.setValue(String.valueOf((int)notconformedConNumAll));
		pieCheckChat1.setName("检查");
		pieCheckChat1.setValue(String.valueOf((int)checkNumAll));
		pieCheckChat2.setName("未检查");
		pieCheckChat2.setValue(String.valueOf((int)uncheckNumAll));
		
		pieChats.add(pieChat1);
		pieChats.add(pieChat2);
		pieCheckChats.add(pieCheckChat1);
		pieCheckChats.add(pieCheckChat2);
		
		info.setPeList(pieChats);// 饼图 合规不合规
		info.setPeCheckList(pieCheckChats);// 饼图 检查未检查
		
		if (checkNumAll == 0) {
			info.setRate(String.valueOf(0));// 合规率
		} else {
			double conformed = (conformedNumAll / checkNumAll) * 100;
			DecimalFormat df = new DecimalFormat("0.0");
			String result = df.format(conformed);
			info.setRate(result);// 合规率
		}
		
		//columnMap = MapUtils.sortByValue(columnMap);
		List<String> strings = new ArrayList<String>();
		for (String key : columnMap.keySet()) {
			strings.add(key);
		}
		info.setChecdRecord((int)(uncheckNumAll + checkNumAll));// 检查录像
		info.setRateMinRgeionName(strings.get(0));
		info.setRateMaxRgeionName(strings.get(strings.size() - 1));
		return info;
	}
*/
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.analysis.service.IAnalysisService#getIndexRateNum(com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo)
     */
/*    @Override
    public AnalysisInfo getIndexRateNum(AnalysisInfo analysisInfo) {
		AnalysisInfo info = new AnalysisInfo();
		// 1、已选组织个数 已检查组织个数 合规率
		Integer nregionId = analysisInfo.getNregionId();// 所选组织
		List<Region> regionList = regionDao.findBy("nparentId", nregionId, "nsortId", true);
		Integer nregionNum = regionList.size()+1;
		info.setNregionNum(nregionNum);// 子组织个数
		regionList.add(regionDao.get(nregionId));
		
		// 获取已检查录像 签约单录像
		List<Prosaverecord> prosaverecords = proSaveRecordDao.getIndexRecordByCheck(analysisInfo);
		List<PieChat> pieChats = new ArrayList<PieChat>();
		List<PieChat> pieCheckChats = new ArrayList<PieChat>();
		
		List<PieChat> chats = new ArrayList<PieChat>();
		
		Map<String, Double> columnMap = new LinkedHashMap<String, Double>();
		double conformedNumAll = 0;// 总的组织合规数量
		double notconformedConNumAll = 0;// 总的组织不合规数量
		double uncheckNumAll = 0;// 总的组织未检查数量
		double checkNumAll = 0;// 总的组织检查数量
		for (Region region : regionList) {
			double conformedNum = 0;// 单个组织合规数量
			double notconformedConNum = 0;// 单个组织不合规数量
			double uncheckNum = 0;// 单个组织未检查数量
			//如果是本级节点
			if(nregionId.equals(region.getId())){
				for (Prosaverecord prosaverecord : prosaverecords) {
					if (region.getId().equals(prosaverecord.getRegionId())) {
						if (RecordCheckStatus.CONFORMED == prosaverecord.getRecordCheck()) {// 合规
							conformedNum++;
						}
						if (RecordCheckStatus.NOT_CONFORMED == prosaverecord.getRecordCheck()) {// 不合规
							notconformedConNum++;
						}
						if (RecordCheckStatus.NOT_CHECK == prosaverecord.getRecordCheck()) {// 未检查
							uncheckNum++;
						}						
					}
				}
			}else{
				List<Region> strpathList = regionDao.getLikeStrPath(region);
				for (Region regionChild : strpathList) {
					for (Prosaverecord prosaverecord : prosaverecords) {
						if (regionChild.getId().equals(prosaverecord.getRegionId())) {
							if (RecordCheckStatus.CONFORMED == prosaverecord.getRecordCheck()) {// 合规
								conformedNum++;
							}
							if (RecordCheckStatus.NOT_CONFORMED == prosaverecord.getRecordCheck()) {// 不合规
								notconformedConNum++;
							}
							if (RecordCheckStatus.NOT_CHECK == prosaverecord.getRecordCheck()) {// 未检查
								uncheckNum++;
							}
							
						}
					}
				}	
			}
			conformedNumAll = conformedNumAll + conformedNum;// 合规数量
			notconformedConNumAll = notconformedConNumAll + notconformedConNum;// 不合规
			uncheckNumAll = uncheckNumAll + uncheckNum; // 未检查
			checkNumAll = conformedNumAll + notconformedConNumAll; // 检查
			double compliance = 0;
			if ((notconformedConNum + conformedNum) == 0) {
				compliance = 0;// 合规+不合规=0的情况算 合规率为0
			} else {
				compliance = conformedNum / (notconformedConNum + conformedNum);
			}
			if(nregionId.equals(region.getId())){
				PieChat pieChat = new PieChat();
				pieChat.setName(region.getStrName()+"直属");
				pieChat.setValue(String.valueOf(compliance));
				chats.add(pieChat);
				columnMap.put(region.getStrName()+"直属", compliance);// 单个合规率
			}else{
				PieChat pieChat = new PieChat();
				pieChat.setName(region.getStrName());
				pieChat.setValue(String.valueOf(compliance));
				chats.add(pieChat);
				columnMap.put(region.getStrName(), compliance);// 单个合规率
			}
		}
		info.setColumnMap(columnMap);// 柱状图
		info.setColumnList(chats);
		PieChat pieChat1 = new PieChat();
		PieChat pieChat2 = new PieChat();
		PieChat pieCheckChat1 = new PieChat();
		PieChat pieCheckChat2 = new PieChat();
		pieChat1.setName("合规");
		pieChat1.setValue(String.valueOf((int)conformedNumAll));
		pieChat2.setName("不合规");
		pieChat2.setValue(String.valueOf((int)notconformedConNumAll));
		pieCheckChat1.setName("检查");
		pieCheckChat1.setValue(String.valueOf((int)checkNumAll));
		pieCheckChat2.setName("未检查");
		pieCheckChat2.setValue(String.valueOf((int)uncheckNumAll));
		
		pieChats.add(pieChat1);
		pieChats.add(pieChat2);
		pieCheckChats.add(pieCheckChat1);
		pieCheckChats.add(pieCheckChat2);
		
		info.setPeList(pieChats);// 饼图 合规不合规
		info.setPeCheckList(pieCheckChats);// 饼图 检查未检查
		
		if (checkNumAll == 0) {
			info.setRate(String.valueOf(0));// 合规率
		} else {
			double conformed = (conformedNumAll / checkNumAll) * 100;
			DecimalFormat df = new DecimalFormat("0.0");
			String result = df.format(conformed);
			info.setRate(result);// 合规率
		}
		
		//columnMap = MapUtils.sortByValue(columnMap);
		List<String> strings = new ArrayList<String>();
		for (String key : columnMap.keySet()) {
			strings.add(key);
		}

		info.setRateMinRgeionName(strings.get(0));
		info.setRateMaxRgeionName(strings.get(strings.size() - 1));
		info.setChecdRecord((int)(uncheckNumAll + checkNumAll));// 检查录像
		return info;
    }*/
/*******************************************重构代码*********************************************************************/
    /**
     * 业务统计方法
     */
	public AnalysisInfo getOrdersNum(AnalysisInfo analysisInfo) {
		AnalysisInfo info = new AnalysisInfo();
		// 1、已选组织个数 已检查组织个数 合规率
		Integer nregionId = analysisInfo.getNregionId();// 所选组织
		List<Region> regionList = regionDao.findBy("nparentId", nregionId, "nsortId", true);
		Integer nregionNum = regionList.size() + 1;
		/** 已选组织个数 **/
		info.setNregionNum(nregionNum);
		regionList.add(regionDao.get(nregionId));
		
		Map<String, Double> columnMapOrder = new HashMap<String, Double>();// 订单柱状<区域名-数据><x-y>
		Map<String, Double> columnMapComplaint = new HashMap<String, Double>();// 投诉柱状<区域名-数据><x-y>
		
		double orderMaxAll = 0;// 总签约单数量
		double rateNumAll = 0;// 总投诉单数量
		for (Region region : regionList) {
			double orderMax = 0;// 签约单数量
			double rateNum = 0;// 投诉单数量
			if (nregionId.equals(region.getId())) {
				
				orderMax = clientInputDao.getSaleByAnalysis(analysisInfo, nregionId.toString(), 0);
				rateNum = clientInputDao.getSaleByAnalysis(analysisInfo, nregionId.toString(), 1);
				/** 获取直属节点签约单和投诉单 **/
				columnMapOrder.put(region.getStrName() + "直属", orderMax);
				columnMapComplaint.put(region.getStrName() + "直属", rateNum);
				orderMaxAll = orderMaxAll+orderMax;
				rateNumAll = rateNumAll + rateNum;
			} else {
				List<Integer> strpathListIds = regionDao.getIdsLikeStrPath(region);
				String regionListIds = listToString(strpathListIds);;
				
				orderMax = clientInputDao.getSaleByAnalysis(analysisInfo, regionListIds, 0);
				rateNum = clientInputDao.getSaleByAnalysis(analysisInfo, regionListIds, 1);
				/** 获取每级签约单和投诉单 **/
				columnMapOrder.put(region.getStrName(), orderMax);
				columnMapComplaint.put(region.getStrName(), rateNum);
				
				orderMaxAll = orderMaxAll+orderMax;
				rateNumAll = rateNumAll + rateNum;
			}
		}
		
		info.setColumnMapOrder(columnMapOrder);// 签约单柱状图
		info.setColumnMapComplaint(columnMapComplaint);// 投诉单柱状图
		info.setOrderMax(String.valueOf((int)(orderMaxAll + rateNumAll)));// 签约总数
		info.setComplaintMax(String.valueOf((int)rateNumAll));// 投诉总数
		if ((orderMaxAll + rateNumAll) == 0) {
			info.setComplaintOrder(String.valueOf(0));
		} else {
			info.setComplaintOrder(String.valueOf(rateNumAll / (orderMaxAll + rateNumAll) * 100));// 投诉占比
		}
		return info;
	}
    
    
	/**
	 * 合规统计方法
	 */
	public AnalysisInfo getRateNum(AnalysisInfo analysisInfo) {
		AnalysisInfo info = new AnalysisInfo();
		// 1、已选组织个数 已检查组织个数 合规率
		Integer nregionId = analysisInfo.getNregionId();// 所选组织
		List<Region> regionList = regionDao.findBy("nparentId", nregionId, "nsortId", true);
		Integer nregionNum = regionList.size() + 1;
		/** 已选组织个数 **/
		info.setNregionNum(nregionNum);
		regionList.add(regionDao.get(nregionId));
		
		List<PieChat> pieChats = new ArrayList<PieChat>();
		List<PieChat> pieCheckChats = new ArrayList<PieChat>();
		List<PieChat> chats = new ArrayList<PieChat>();
		Map<String, Double> columnMap = new LinkedHashMap<String, Double>();
		
		double conformedNumAll = 0;// 总的组织合规数量
		double notconformedConNumAll = 0;// 总的组织不合规数量
		double uncheckNumAll = 0;// 总的组织未检查数量
		double checkNumAll = 0;// 总的组织检查数量	合规总数+不合规总数		
		for (Region region : regionList) {
			double conformedNum = 0;// 单个组织合规数量
			double notconformedConNum = 0;// 单个组织不合规数量
			double uncheckNum = 0;// 单个组织未检查数量
			
			if (nregionId.equals(region.getId())) {
				/**直属组织合规率**/
				conformedNum = proSaveRecordDao.getRecordByCheck(analysisInfo, nregionId.toString(), 2);//合规
				notconformedConNum = proSaveRecordDao.getRecordByCheck(analysisInfo, nregionId.toString(), 1);//不合规
				uncheckNum = proSaveRecordDao.getRecordByCheck(analysisInfo, nregionId.toString(), 0);//未检查
				
				//累计合规 不合规 未检查
				conformedNumAll = conformedNumAll + conformedNum;
				notconformedConNumAll = notconformedConNumAll + notconformedConNum;
				uncheckNumAll = uncheckNumAll + uncheckNum;
				
				double compliance = 0;
				if ((notconformedConNum + conformedNum) == 0) {
					compliance = 0;// 合规+不合规=0的情况算 合规率为0
				} else {
					compliance = conformedNum / (notconformedConNum + conformedNum);
				}
				
				PieChat pieChat = new PieChat();
				columnMap.put(region.getStrName() + "直属", compliance);// 单个合规率
				pieChat.setName(region.getStrName() + "直属");
				pieChat.setValue(String.valueOf(compliance));
				chats.add(pieChat);
			} else {
				List<Integer> strpathListIds = regionDao.getIdsLikeStrPath(region);
				String regionListIds =  listToString(strpathListIds);;
				
				/**单个组织合规率**/
				conformedNum = proSaveRecordDao.getRecordByCheck(analysisInfo, regionListIds, 2);//合规
				notconformedConNum = proSaveRecordDao.getRecordByCheck(analysisInfo, regionListIds, 1);//不合规
				uncheckNum = proSaveRecordDao.getRecordByCheck(analysisInfo, regionListIds, 0);//未检查
				
				//累计合规 不合规 未检查
				conformedNumAll = conformedNumAll + conformedNum;
				notconformedConNumAll = notconformedConNumAll + notconformedConNum;
				uncheckNumAll = uncheckNumAll + uncheckNum;
				
				double compliance = 0;
				if ((notconformedConNum + conformedNum) == 0) {
					compliance = 0;// 合规+不合规=0的情况算 合规率为0
				} else {
					compliance = conformedNum / (notconformedConNum + conformedNum);
				}
				
				PieChat pieChat = new PieChat();
				columnMap.put(region.getStrName(), compliance);// 单个合规率
				pieChat.setName(region.getStrName());
				pieChat.setValue(String.valueOf(compliance));
				chats.add(pieChat);
			}
		}		
		checkNumAll = conformedNumAll + notconformedConNumAll;// 总的组织不合规数量+总的不合规数量
		info.setColumnMap(columnMap);// 柱状图
		info.setColumnList(chats);
		PieChat pieChat1 = new PieChat();
		PieChat pieChat2 = new PieChat();
		PieChat pieCheckChat1 = new PieChat();
		PieChat pieCheckChat2 = new PieChat();
		pieChat1.setName("合规");
		pieChat1.setValue(String.valueOf((int)conformedNumAll));
		pieChat2.setName("不合规");
		pieChat2.setValue(String.valueOf((int)notconformedConNumAll));
		pieCheckChat1.setName("检查");
		pieCheckChat1.setValue(String.valueOf((int)checkNumAll));
		pieCheckChat2.setName("未检查");
		pieCheckChat2.setValue(String.valueOf((int)uncheckNumAll));
		
		pieChats.add(pieChat1);
		pieChats.add(pieChat2);
		pieCheckChats.add(pieCheckChat1);
		pieCheckChats.add(pieCheckChat2);
		
		info.setPeList(pieChats);// 饼图 合规不合规
		info.setPeCheckList(pieCheckChats);// 饼图 检查未检查
		
		if (checkNumAll == 0) {
			info.setRate(String.valueOf(0));// 合规率
		} else {
			double conformed = (conformedNumAll / checkNumAll) * 100;
			DecimalFormat df = new DecimalFormat("0.0");
			String result = df.format(conformed);
			info.setRate(result);// 合规率
		}
		
		// columnMap = MapUtils.sortByValue(columnMap);
		List<String> strings = new ArrayList<String>();
		for (String key : columnMap.keySet()) {
			strings.add(key);
		}
		info.setChecdRecord((int)(uncheckNumAll + checkNumAll));// 检查录像
		info.setRateMinRgeionName(strings.get(0));
		info.setRateMaxRgeionName(strings.get(strings.size() - 1));
		return info;
	}

	
	
	
    /**
     * 首页合规统计
     */
    public AnalysisInfo getIndexRateNum(AnalysisInfo analysisInfo) {
    	AnalysisInfo info = new AnalysisInfo();
		// 1、已选组织个数 已检查组织个数 合规率
		Integer nregionId = analysisInfo.getNregionId();// 所选组织
		List<Region> regionList = regionDao.findBy("nparentId", nregionId, "nsortId", true);
		Integer nregionNum = regionList.size() + 1;
		/** 已选组织个数 **/
		info.setNregionNum(nregionNum);
		regionList.add(regionDao.get(nregionId));
		
		List<PieChat> pieChats = new ArrayList<PieChat>();
		List<PieChat> pieCheckChats = new ArrayList<PieChat>();
		List<PieChat> chats = new ArrayList<PieChat>();
		Map<String, Double> columnMap = new LinkedHashMap<String, Double>();
		
		double conformedNumAll = 0;// 总的组织合规数量
		double notconformedConNumAll = 0;// 总的组织不合规数量
		double uncheckNumAll = 0;// 总的组织未检查数量
		double checkNumAll = 0;// 总的组织检查数量	合规总数+不合规总数		
		for (Region region : regionList) {
			double conformedNum = 0;// 单个组织合规数量
			double notconformedConNum = 0;// 单个组织不合规数量
			double uncheckNum = 0;// 单个组织未检查数量
			
			if (nregionId.equals(region.getId())) {
				/**直属组织合规率**/
				conformedNum = proSaveRecordDao.getIndexRecordByCheck(analysisInfo, nregionId.toString(), 2);//合规
				notconformedConNum = proSaveRecordDao.getIndexRecordByCheck(analysisInfo, nregionId.toString(), 1);//不合规
				uncheckNum = proSaveRecordDao.getIndexRecordByCheck(analysisInfo, nregionId.toString(), 0);//未检查
				
				//累计合规 不合规 未检查
				conformedNumAll = conformedNumAll + conformedNum;
				notconformedConNumAll = notconformedConNumAll + notconformedConNum;
				uncheckNumAll = uncheckNumAll + uncheckNum;
				
				double compliance = 0;
				if ((notconformedConNum + conformedNum) == 0) {
					compliance = 0;// 合规+不合规=0的情况算 合规率为0
				} else {
					compliance = conformedNum / (notconformedConNum + conformedNum);
				}
				
				PieChat pieChat = new PieChat();
				columnMap.put(region.getStrName() + "直属", compliance);// 单个合规率
				pieChat.setName(region.getStrName() + "直属");
				pieChat.setValue(String.valueOf(compliance));
				chats.add(pieChat);
			} else {
				List<Integer> strpathListIds = regionDao.getIdsLikeStrPath(region);
				String regionListIds = listToString(strpathListIds);
				
				/**单个组织合规率**/
				conformedNum = proSaveRecordDao.getIndexRecordByCheck(analysisInfo, regionListIds, 2);//合规
				notconformedConNum = proSaveRecordDao.getIndexRecordByCheck(analysisInfo, regionListIds, 1);//不合规
				uncheckNum = proSaveRecordDao.getIndexRecordByCheck(analysisInfo, regionListIds, 0);//未检查
				
				//累计合规 不合规 未检查
				conformedNumAll = conformedNumAll + conformedNum;
				notconformedConNumAll = notconformedConNumAll + notconformedConNum;
				uncheckNumAll = uncheckNumAll + uncheckNum;
				
				double compliance = 0;
				if ((notconformedConNum + conformedNum) == 0) {
					compliance = 0;// 合规+不合规=0的情况算 合规率为0
				} else {
					compliance = conformedNum / (notconformedConNum + conformedNum);
				}
				
				PieChat pieChat = new PieChat();
				columnMap.put(region.getStrName(), compliance);// 单个合规率
				pieChat.setName(region.getStrName());
				pieChat.setValue(String.valueOf(compliance));
				chats.add(pieChat);
			}
		}		
		checkNumAll = conformedNumAll + notconformedConNumAll;// 总的组织不合规数量+总的不合规数量
		info.setColumnMap(columnMap);// 柱状图
		info.setColumnList(chats);
		PieChat pieChat1 = new PieChat();
		PieChat pieChat2 = new PieChat();
		PieChat pieCheckChat1 = new PieChat();
		PieChat pieCheckChat2 = new PieChat();
		pieChat1.setName("合规");
		pieChat1.setValue(String.valueOf((int)conformedNumAll));
		pieChat2.setName("不合规");
		pieChat2.setValue(String.valueOf((int)notconformedConNumAll));
		pieCheckChat1.setName("检查");
		pieCheckChat1.setValue(String.valueOf((int)checkNumAll));
		pieCheckChat2.setName("未检查");
		pieCheckChat2.setValue(String.valueOf((int)uncheckNumAll));
		
		pieChats.add(pieChat1);
		pieChats.add(pieChat2);
		pieCheckChats.add(pieCheckChat1);
		pieCheckChats.add(pieCheckChat2);
		
		info.setPeList(pieChats);// 饼图 合规不合规
		info.setPeCheckList(pieCheckChats);// 饼图 检查未检查
		
		if (checkNumAll == 0) {
			info.setRate(String.valueOf(0));// 合规率
		} else {
			double conformed = (conformedNumAll / checkNumAll) * 100;
			DecimalFormat df = new DecimalFormat("0.0");
			String result = df.format(conformed);
			info.setRate(result);// 合规率
		}
		
		// columnMap = MapUtils.sortByValue(columnMap);
		List<String> strings = new ArrayList<String>();
		for (String key : columnMap.keySet()) {
			strings.add(key);
		}
		info.setChecdRecord((int)(uncheckNumAll + checkNumAll));// 检查录像
		info.setRateMinRgeionName(strings.get(0));
		info.setRateMaxRgeionName(strings.get(strings.size() - 1));
		return info;
    }
	
	
	
	private String listToString(List<Integer> strpathListIds) {
		String str = ListUtil.listToStr(strpathListIds);
		String str1 = str.replaceAll("\\[", "");
		String str2 = str1.replaceAll("]", "");
		return str2;
	}
	
	
    public IRegionDao getRegionDao() {
		return regionDao;
	}
	
	public void setRegionDao(IRegionDao regionDao) {
		this.regionDao = regionDao;
	}
	
	public IProductDao getProductDao() {
		return productDao;
	}
	
	public void setProductDao(IProductDao productDao) {
		this.productDao = productDao;
	}
	
	public IProSaveRecordDao getProSaveRecordDao() {
		return proSaveRecordDao;
	}
	
	public void setProSaveRecordDao(IProSaveRecordDao proSaveRecordDao) {
		this.proSaveRecordDao = proSaveRecordDao;
	}
	
	public IClientInputDao getClientInputDao() {
		return clientInputDao;
	}
	
	public void setClientInputDao(IClientInputDao clientInputDao) {
		this.clientInputDao = clientInputDao;
	}
	
}
