/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年10月21日 上午10:43:14
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.csclient;

import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.hikvision.finance.core.util.CollectionUtils;
import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.fms.cms.hpp.csclient.HistoryClientInputProto;
import com.hikvision.finance.fms.cms.hpp.csclient.HistoryClientInputProto.HisClientInputService;
import com.hikvision.finance.fms.cms.hpp.csclient.HistoryClientInputProto.ReqHisClientInput;
import com.hikvision.finance.fms.cms.hpp.csclient.HistoryClientInputProto.RspCHisClientInput;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.model.Prosaverecord;
import com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao;
import com.hikvision.finance.fms.modules.sale.dao.IClientInputDao;
import com.hikvision.finance.fms.modules.sale.dto.SaleInfo;
import com.ivms6.core.hpp.codec.HppConstants;

/**
 * <p></p>
 * @author zhoujiajun 2016年10月21日 上午10:43:14
 * @version V1.0 
 */
public class HHisClientInputServiceImpl extends HisClientInputService {

	private static Logger logger = LoggerFactory.getLogger(HProductServiceImpl.class);
	
	private IClientInputDao clientInputDao;
	private IProSaveRecordDao proSaveRecordDao;
	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.cms.hpp.csclient.HistoryClientInputProto.HisClientInputService#queryHisClientInput(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.csclient.HistoryClientInputProto.ReqHisClientInput, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void queryHisClientInput(RpcController controller, ReqHisClientInput request,
			RpcCallback<RspCHisClientInput> done) {
		logger.info("开始获取历史业务单信息list，Cmd：【" + CMD.CMD_DT_HIS_CLIENTINPUT_VALUE + "】 request：【" + request.toString() + "】 ");
		HistoryClientInputProto.RspCHisClientInput.Builder hisClientInputList = HistoryClientInputProto.RspCHisClientInput.newBuilder();
		hisClientInputList.setCmd(CMD.CMD_DT_HIS_CLIENTINPUT_VALUE);
		try {
			
			List<SaleInfo> dbSaleInfoList = getHisClientInputList(request, hisClientInputList);
			if (HppConstants.RESULT_ERROR == hisClientInputList.getResult()) {
				done.run(hisClientInputList.build());
				return;
			}
			if (CollectionUtils.isNotEmpty(dbSaleInfoList)) {
				for (SaleInfo dbSaleInfo : dbSaleInfoList) {
					//获取审批退回的业务单
					if (Constants.LockClient.SEND_BACK != dbSaleInfo.getLockClient()){
						continue;
					}
					HistoryClientInputProto.HisClientInput.Builder hisClientInput = HistoryClientInputProto.HisClientInput.newBuilder();
					if (dbSaleInfo.getNproId() != null) {
						hisClientInput.setNProId(dbSaleInfo.getNproId());
					}
					if (dbSaleInfo.getInputRegionId() != null) {
						hisClientInput.setInputRegionId(dbSaleInfo.getInputRegionId());
					}
					if (dbSaleInfo.getInputRegionCode() != null) {
						hisClientInput.setInputRegionCode(dbSaleInfo.getInputRegionCode());
					}
					if (dbSaleInfo.getInputRegionName() != null) {
						hisClientInput.setInputRegionName(dbSaleInfo.getInputRegionName());
					}
					if (dbSaleInfo.getName() != null) {
						hisClientInput.setName(dbSaleInfo.getName());
					}
					if (dbSaleInfo.getStrCreditPhone() != null) {
						hisClientInput.setStrCreditPhone(dbSaleInfo.getStrCreditPhone());
					}
					if (dbSaleInfo.getCreditCode() != null) {
						hisClientInput.setCreditCode(dbSaleInfo.getCreditCode());
					}
					if (dbSaleInfo.getCreditId() != null) {
						hisClientInput.setCreditId(dbSaleInfo.getCreditId());
					}
					if (dbSaleInfo.getProductTypeId() != null) {
						hisClientInput.setProductTypeId(dbSaleInfo.getProductTypeId());
					}
					if (dbSaleInfo.getProductTypeName() != null) {
						hisClientInput.setProductTypeName(dbSaleInfo.getProductTypeName());
					}
					if (dbSaleInfo.getProductId() != null) {
						hisClientInput.setProductId(dbSaleInfo.getProductId());
					}
					if (dbSaleInfo.getProductCode() != null) {
						hisClientInput.setProductCode(dbSaleInfo.getProductCode());
					}
					if (dbSaleInfo.getProductName() != null) {
						hisClientInput.setProductName(dbSaleInfo.getProductName());
					}
					if (dbSaleInfo.getStrGrade() != null) {
						hisClientInput.setStrGrade(dbSaleInfo.getStrGrade());
					}
					if (dbSaleInfo.getStrBuyMoney() != null) {
						hisClientInput.setStrBuyMoney(dbSaleInfo.getStrBuyMoney());
					}
					if (dbSaleInfo.getInputUser() != null) {
						hisClientInput.setInputUser(dbSaleInfo.getInputUser());
					}
					if (dbSaleInfo.getBusinessTime() != null) {
						hisClientInput.setBusinessTime(DateUtils.getStringDateTime(dbSaleInfo.getBusinessTime()));
					}
					if (dbSaleInfo.getId() != null) {
						hisClientInput.setClientId(dbSaleInfo.getId());
					}
					if (dbSaleInfo.getScreenshotSrc() != null) {
						String[] screenshotSrc = dbSaleInfo.getScreenshotSrc().split(",");
						for (int i = 0; i < screenshotSrc.length; i++) {
							if(i==0){
								hisClientInput.setPictureStrart(screenshotSrc[i]);
							}
							if(i==1){
								hisClientInput.setPictureEnd(screenshotSrc[i]);
							}
						}
					}else {
						//跟csclient打印页面约定好没有图片就插入nopic
						hisClientInput.setPictureStrart("nopic"); 
						hisClientInput.setPictureEnd("nopic");
					}
					hisClientInputList.addHisClientInput(hisClientInput);
				}
			}
			hisClientInputList.setResult(HppConstants.RESULT_OK);
			hisClientInputList.setResultInfo("获取历史业务单信息成功");
			done.run(hisClientInputList.build());
		} catch (Exception e) {
			logger.error("获取历史业务单信息异常：" + e);
			hisClientInputList.setResult(HppConstants.RESULT_ERROR);
			hisClientInputList.setResultInfo("获取历史业务单信息异常");
			done.run(hisClientInputList.build());
		}
	}
	
	
	private List<SaleInfo> getHisClientInputList(ReqHisClientInput request, HistoryClientInputProto.RspCHisClientInput.Builder hisClientInputList) {
		
		List<SaleInfo> saleInfoList = null;
		Integer inputUser = request.getInputUser();
		if (inputUser == null || inputUser < 0) {
			hisClientInputList.setResult(HppConstants.RESULT_ERROR);
			hisClientInputList.setResultInfo("业务员Id不存在");
			return saleInfoList;
		}
		SaleInfo saleInfo = new SaleInfo();
		saleInfo.setBusinessTimeStart(request.getStartDate());
		saleInfo.setBusinessTimeEnd(request.getEndDate());
		saleInfo.setName(request.getCustName());
		saleInfo.setNproId(request.getNproId());
		saleInfo.setInputUser(inputUser);
		saleInfoList = clientInputDao.getHisClientInputList(saleInfo);
		for (SaleInfo dbSaleInfo : saleInfoList) {
			//录像关联表中的, 获取的图片数据都是相同的, 所以取第一个(CS客户端)
			List<Prosaverecord> prosaverecords = proSaveRecordDao.findBy("clientId", dbSaleInfo.getId()); 
			if (CollectionUtils.isNotEmpty(prosaverecords)) {
				dbSaleInfo.setScreenshotSrc(prosaverecords.get(0).getScreenshotSrc());
			}
		}
		return saleInfoList;
	}
	
	public IClientInputDao getClientInputDao() {
		return clientInputDao;
	}
	
	public void setClientInputDao(IClientInputDao clientInputDao) {
		this.clientInputDao = clientInputDao;
	}
	
	public IProSaveRecordDao getProSaveRecordDao() {
		return proSaveRecordDao;
	}

	public void setProSaveRecordDao(IProSaveRecordDao proSaveRecordDao) {
		this.proSaveRecordDao = proSaveRecordDao;
	}
	
}
