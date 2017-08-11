/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年10月12日 下午3:08:24
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.csclient;

import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.hikvision.finance.core.util.CollectionUtils;
import com.hikvision.finance.fms.cms.hpp.csclient.ParamDictionaryProto;
import com.hikvision.finance.fms.cms.hpp.csclient.ParamDictionaryProto.ParamDictionaryList;
import com.hikvision.finance.fms.cms.hpp.csclient.ParamDictionaryProto.ParamDictionaryService;
import com.hikvision.finance.fms.cms.hpp.csclient.ParamDictionaryProto.ReqParamDictionary;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.cms.hppservice.verify.HClientVerifyServiceImpl;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.model.Paramdictionary;
import com.hikvision.finance.fms.modules.card.dao.ICardDao;
import com.hikvision.finance.fms.modules.card.qo.ParamdictionaryQo;
import com.ivms6.core.hpp.codec.HppConstants;


/**
 * <p>CS客户端获取证件类型</p>
 * @author jinchenwu 2016年10月12日 下午3:08:24
 * @version V1.0 
 */
public class HParamDictionaryServiceImpl extends ParamDictionaryService {
	
	private static Logger logger = LoggerFactory.getLogger(HClientVerifyServiceImpl.class);
	
	private ICardDao cardDao;
	
	/*
	 * CS客户端获取证件类型
	 * @see com.hikvision.finance.fms.cms.hpp.csclient.ParamDictionaryProto.ParamDictionaryService#getParamDictionaryList(com.google.protobuf.RpcController,
	 * com.hikvision.finance.fms.cms.hpp.csclient.ParamDictionaryProto.ReqParamDictionary, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void getParamDictionaryList(RpcController controller, ReqParamDictionary request,
	        RpcCallback<ParamDictionaryList> done) {
		logger.info("开始获取证件类型List，Cmd：【" + CMD.CMD_DT_PARAMDICTIONARY_LIST_VALUE + "】 request：【" + request.toString()
		        + "】 ");
		ParamDictionaryProto.ParamDictionaryList.Builder paramDictionaryList = ParamDictionaryProto.ParamDictionaryList
		        .newBuilder();
		paramDictionaryList.setCmd(CMD.CMD_DT_PARAMDICTIONARY_LIST_VALUE);
		try {
			ParamdictionaryQo paramdictionaryQo = new ParamdictionaryQo();
			paramdictionaryQo.setEnable(Constants.CONS_1);
			List<Paramdictionary> cardList = cardDao.queryList(paramdictionaryQo);
			if (CollectionUtils.isNotEmpty(cardList)) {
				for (Paramdictionary card : cardList) {
					ParamDictionaryProto.ParamDictionary.Builder paramDictionary = ParamDictionaryProto.ParamDictionary
					        .newBuilder();
					if (null != card.getId()) {
						paramDictionary.setId(card.getId());
					}
					if (null != card.getStrName()) {
						paramDictionary.setStrName(card.getStrName());
					}
					if (null != card.getStrCode()) {
						paramDictionary.setStrCode(card.getStrCode());
					}
					if (null != card.getNsortId()) {
						paramDictionary.setNsortId(card.getNsortId());
					}
					if (null != card.getEnable()) {
						paramDictionary.setEnable(card.getEnable());
					}
					paramDictionaryList.addParamDictionary(paramDictionary);
				}
			}
			done.run(paramDictionaryList.build());
		} catch (Exception e) {
			logger.error("获取证件类型异常：" + e);
			paramDictionaryList.setResult(HppConstants.RESULT_ERROR);
			paramDictionaryList.setResultInfo("获取证件类型异常");
			done.run(paramDictionaryList.build());
		}
	}
	
	public ICardDao getCardDao() {
		return cardDao;
	}
	
	public void setCardDao(ICardDao cardDao) {
		this.cardDao = cardDao;
	}
	
}
