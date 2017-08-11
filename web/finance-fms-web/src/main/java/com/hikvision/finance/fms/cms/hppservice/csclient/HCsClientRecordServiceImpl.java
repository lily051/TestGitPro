/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年10月19日 下午4:07:36
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.csclient;

import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.hikvision.finance.core.util.BeanUtils;
import com.hikvision.finance.core.util.CollectionUtils;
import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto;
import com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.ModifyCsClientRecordService;
import com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.ReqCsClientRecord;
import com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.ReqErrorRecord;
import com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.ReqPrintRecord;
import com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.ReqRecordsInfo;
import com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.RspCsClientRecord;
import com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.RspCsClientRecordsInfo;
import com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.RspErrRecordsInfo;
import com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.RspPrintRecord;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.cms.hppservice.model.RecordsInfoVO;
import com.hikvision.finance.fms.cms.hppservice.model.VideoDetailInfoVO;
import com.hikvision.finance.fms.common.enums.ProSaveRecordEnum;
import com.hikvision.finance.fms.common.exception.CMSException;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.LockClient;
import com.hikvision.finance.fms.common.util.Constants.OperLogType;
import com.hikvision.finance.fms.common.util.Constants.ProductKeepType;
import com.hikvision.finance.fms.common.util.Constants.RecordStatus;
import com.hikvision.finance.fms.common.util.Constants.VideoUploadStatus;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.model.Paramdictionary;
import com.hikvision.finance.fms.model.Product;
import com.hikvision.finance.fms.model.Producttype;
import com.hikvision.finance.fms.model.Prosaverecord;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.card.dao.ICardDao;
import com.hikvision.finance.fms.modules.product.dao.IProductDao;
import com.hikvision.finance.fms.modules.product.dao.IProductTypeDao;
import com.hikvision.finance.fms.modules.product.qo.ProductQo;
import com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao;
import com.hikvision.finance.fms.modules.record.dto.RecordInfo;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.sale.dao.IClientInputDao;
import com.hikvision.finance.fms.modules.user.dao.IUsersDao;
import com.hikvision.finance.fms.webdevice.contact.dto.PrintOrderInfo;
import com.hikvision.finance.fms.webdevice.datacollect.dao.IRecordsDao;
import com.hikvision.finance.fms.webdevice.log.dao.ICollLogDao;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.ivms6.core.hpp.HppChannel;
import com.ivms6.core.hpp.codec.HppConstants;
import com.ivms6.core.hpp.service.RpcControllerImpl;
import com.ivms6.core.hpp.util.ContextUtil;

/**
 * <p></p>
 * @author zhoujiajun 2016年10月19日 下午4:07:36
 * @version V1.0 
 */
public class HCsClientRecordServiceImpl extends ModifyCsClientRecordService {

	private static Logger logger = LoggerFactory.getLogger(HCsClientRecordServiceImpl.class);
	
	private ICollLogDao collLogDao;//记录日志
	private IRecordsDao recordsDao;
	private IProductTypeDao productTypeDao;
	private IProductDao productDao;
	private IClientInputDao clientInputDao; 
	private IRegionDao regionDao;
	private IUsersDao usersDao;
	private IProSaveRecordDao proSaveRecordDao;
	private ICardDao cardDao;
	
	/**
	 * 保存业务信息
	 */
	@Override
	public void saveCsClientRecord(RpcController controller, ReqCsClientRecord request,
			RpcCallback<RspCsClientRecord> done)  {
		logger.info("开始保存业务信息，Cmd：【" + CMD.CMD_WDT_CLIENT_RECORD_VALUE + "】 request：【" + request.toString() + "】 ");
		CsClientRecordProto.RspCsClientRecord.Builder rsqCsClientRecord = CsClientRecordProto.RspCsClientRecord.newBuilder();
		rsqCsClientRecord.setCmd(CMD.CMD_WDT_CLIENT_RECORD_VALUE);
		try {
			RpcControllerImpl rpcControllerImpl = (RpcControllerImpl)controller;
			long sessionId = rpcControllerImpl.getSessionId();
			HppChannel hppChannel = ContextUtil.getHppChannel(HppConstants.TYPE_SERVER + sessionId);
	    	if(null == hppChannel){// Coverity提示需要加上非空判断
	    		rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
	    		rsqCsClientRecord.setResultInfo("系统错误");
				done.run(rsqCsClientRecord.build());
				logger.error("结束 保存业务信息  请求处理异常: hppChannel为空");
				return;
	    	}
			String clientIP = ((InetSocketAddress)hppChannel.getSession().getRemoteAddress()).getAddress().getHostAddress();
			
			com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.CSClientInput csClientInput = request.getClientInput();
			// 保存业务单信息到clientInput表中
			com.hikvision.finance.fms.model.Clientinput dbClientInput = saveClientInput(csClientInput, rsqCsClientRecord);
			if (HppConstants.RESULT_ERROR == rsqCsClientRecord.getResult()) {
				done.run(rsqCsClientRecord.build());
				return;
			}
			clientInputDao.save(dbClientInput);// 保存新业务单
			// 记录操作日志
			String note = "录入签约单，业务单流水号：" + csClientInput.getNproId() + "，客户姓名：" + csClientInput.getName();
			collLogDao.writeOperLog(OperLogType.OperRel, note, csClientInput.getInputUser());
			
			List<com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.CSRecords> recordsList = request.getRecordsList();
			if (CollectionUtils.isEmpty(recordsList)) {
				rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
				rsqCsClientRecord.setResultInfo("必须存在录像数据");
				done.run(rsqCsClientRecord.build());
				return;
			}
			// 保存录像信息到records表中
			List<com.hikvision.finance.fms.model.Records> dbRecordsList = saveRecords(recordsList, clientIP, rsqCsClientRecord);
			if (HppConstants.RESULT_ERROR == rsqCsClientRecord.getResult()) {
				throw new CMSException(CMSException.INSERT_REQ_REQUIRED, rsqCsClientRecord.getResultInfo());
			}
			for (com.hikvision.finance.fms.model.Records records: dbRecordsList) {
				note = "录像名：" + records.getStrRecordName();
				recordsDao.save(records);
				// 记录操作日志
				collLogDao.writeOperLog(OperLogType.OperAddLocalVideo, note, records.getNuserId());//
				// 保存录像和业务单关联信息到prosaverecord表中  依据records和clientInput生成prosaverecord
				com.hikvision.finance.fms.model.Prosaverecord proSaveRecord = getProsaverecord(records, dbClientInput, rsqCsClientRecord);
				proSaveRecordDao.save(proSaveRecord);
				records.setnHistoryRecordId(proSaveRecord.getId());
				recordsDao.update(records);
				note = "关联新签约单，录像名：" + records.getStrRecordName() + "，业务单流水号：" + dbClientInput.getNproId() + "，客户姓名：" + dbClientInput.getName();
				collLogDao.writeOperLog(OperLogType.OperRel, note, records.getNuserId());
			}
			rsqCsClientRecord.setResult(HppConstants.RESULT_OK);
			rsqCsClientRecord.setResultInfo("保存业务信息成功");
			done.run(rsqCsClientRecord.build());
		} catch (Exception e) {
			logger.error("保存业务信息异常：" + e);
			rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			if (StringUtils.isBlank(rsqCsClientRecord.getResultInfo())) {
				rsqCsClientRecord.setResultInfo("保存业务信息异常");
			}
			done.run(rsqCsClientRecord.build());
			throw new CMSException(CMSException.INS_CLIENTRECORDS_ERR, e);
		}
	}
	
	//获取业务单关联信息
	private Prosaverecord getProsaverecord(com.hikvision.finance.fms.model.Records records, com.hikvision.finance.fms.model.Clientinput dbClientInput, CsClientRecordProto.RspCsClientRecord.Builder rsqCsClientRecord) {
		
		Prosaverecord proSaveRecord = new Prosaverecord();
		proSaveRecord.setStateType(Constants.RecordType.RECORD_SIGN);//1-签约录像
		proSaveRecord.setClientId(dbClientInput.getId());// 业务单ID
		proSaveRecord.setRegionId(dbClientInput.getInputRegionId());// 录单中交易区域ID
		proSaveRecord.setUserId(Integer.valueOf(dbClientInput.getInputUser()));// 员工的id
		proSaveRecord.setFileName(records.getStrRecordName());// 文件名
		proSaveRecord.setStartTime(records.getStartTime());// 开始时间
		proSaveRecord.setEndTime(records.getEndTime());// 结束时间
		proSaveRecord.setUpload(ProSaveRecordEnum.VideoUploadStatus.NOT_UPLOAD.getNType());// 录像上传状态
		proSaveRecord.setRecordCheck(ProSaveRecordEnum.RecordCheckStatus.NOT_CHECK.getNType());// 合规检查
		if (StringUtils.isNotBlank(records.getScreenshotSrc())) {
			proSaveRecord.setScreenshotSrc(records.getScreenshotSrc().replaceAll("\\\\","\\\\\\\\"));// 截图路径
		}
		proSaveRecord.setStrUrl(records.getStrUrl());// USB本地文件路径
		proSaveRecord.setNfileSize(records.getNfileSize());// 录像大小，只有USB-KMS模式有
		proSaveRecord.setDevIp(records.getStrIp()); //录该条录像的设备所在电脑的ip(用于usb模式录像数据的过滤)
		return proSaveRecord;
	}

	/**
	 * 更新业务单信息
	 */
	@Override
	public void updateCsClientRecord(RpcController controller, ReqCsClientRecord request,
			RpcCallback<RspCsClientRecord> done) {
		
		logger.info("开始修改业务信息，Cmd：【" + CMD.CMD_MDT_CLIENT_RECORD_VALUE + "】 request：【" + request.toString() + "】 ");
		CsClientRecordProto.RspCsClientRecord.Builder rsqCsClientRecord = CsClientRecordProto.RspCsClientRecord.newBuilder();
		rsqCsClientRecord.setCmd(CMD.CMD_MDT_CLIENT_RECORD_VALUE);
		try {
			RpcControllerImpl rpcControllerImpl = (RpcControllerImpl)controller;
			long sessionId = rpcControllerImpl.getSessionId();
			HppChannel hppChannel = ContextUtil.getHppChannel(HppConstants.TYPE_SERVER + sessionId);
			if(null == hppChannel){// Coverity提示需要加上非空判断
				rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
				rsqCsClientRecord.setResultInfo("系统错误");
				done.run(rsqCsClientRecord.build());
				logger.error("结束 更新业务单信息  请求处理异常: hppChannel为空");
				return;
	    	}
			String clientIP = ((InetSocketAddress)hppChannel.getSession().getRemoteAddress()).getAddress().getHostAddress();
			//获取业务单信息
			com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.CSClientInput csClientInput = request.getClientInput();
			//检查业务单审批状态
			checkLockClient(csClientInput.getId(), rsqCsClientRecord);
			if (HppConstants.RESULT_ERROR == rsqCsClientRecord.getResult()) {
				done.run(rsqCsClientRecord.build());
				return;
			}
			//更新业务单信息
			com.hikvision.finance.fms.model.Clientinput dbClientInput = updateClientInput(csClientInput, rsqCsClientRecord);
			if (null == dbClientInput) {
				throw new CMSException(CMSException.INSERT_REQ_REQUIRED, rsqCsClientRecord.getResultInfo());
			}
			clientInputDao.update(dbClientInput);
			//记录操作日志
			String note = "更新签约单，业务单流水号：" + dbClientInput.getNproId() + "，客户姓名：" + dbClientInput.getName();
			collLogDao.writeOperLog(OperLogType.OperEditOrder, note, csClientInput.getInputUser());
			//获取录像信息
			List<com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.CSRecords> csRecordsList = request.getRecordsList();
			//保存录像信息
			List<com.hikvision.finance.fms.model.Records> dbRecordsList = saveRecords(csRecordsList, clientIP, rsqCsClientRecord);
			if (HppConstants.RESULT_ERROR == rsqCsClientRecord.getResult()) {
				throw new CMSException(CMSException.INSERT_REQ_REQUIRED, rsqCsClientRecord.getResultInfo());
			}
			for (com.hikvision.finance.fms.model.Records records: dbRecordsList) {
				note = "录像名：" + records.getStrRecordName();
				recordsDao.save(records);
				// 记录操作日志
				collLogDao.writeOperLog(OperLogType.OperAddLocalVideo, note, records.getNuserId());//
				// 保存录像和业务单关联信息到prosaverecord表中  依据records和clientInput生成prosaverecord
				com.hikvision.finance.fms.model.Prosaverecord proSaveRecord = getProsaverecord(records, dbClientInput, rsqCsClientRecord);
				proSaveRecordDao.save(proSaveRecord);
				records.setnHistoryRecordId(proSaveRecord.getId());
				recordsDao.update(records);
				note = "关联修改的签约单，录像名：" + records.getStrRecordName() + "，业务单流水号：" + dbClientInput.getNproId() + "，客户姓名：" + dbClientInput.getName();
				collLogDao.writeOperLog(OperLogType.OperRel, note, records.getNuserId());
			}
			rsqCsClientRecord.setResult(HppConstants.RESULT_OK);
			rsqCsClientRecord.setResultInfo("修改业务信息成功");
			done.run(rsqCsClientRecord.build());
		} catch (Exception e) {
			logger.error("修改业务信息异常：" + e);
			rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			if (StringUtils.isBlank(rsqCsClientRecord.getResultInfo())) {
				rsqCsClientRecord.setResultInfo("保存业务信息异常");
			}
			done.run(rsqCsClientRecord.build());
			throw new CMSException(CMSException.UPD_CLIENTRECORDS_ERR, e);
		}
	}
	
	/**
	 * 查询业务单信息
	 */
	@Override
	public void queryCsClientRecords(RpcController controller, ReqRecordsInfo request,
			RpcCallback<RspCsClientRecordsInfo> done) {
		
		logger.info("获取业务信息，Cmd：【" + CMD.CMD_DT_CLIENT_RECORD_VALUE + "】 request：【" + request.toString() + "】 ");
		CsClientRecordProto.RspCsClientRecordsInfo.Builder rsqCsClientRecordsInfo = CsClientRecordProto.RspCsClientRecordsInfo.newBuilder();
		rsqCsClientRecordsInfo.setCmd(CMD.CMD_DT_CLIENT_RECORD_VALUE);
		try {
			List<RecordsInfoVO> recordInfoList = getRecordsInfo(request, rsqCsClientRecordsInfo);
			if (HppConstants.RESULT_ERROR == rsqCsClientRecordsInfo.getResult()) {
				done.run(rsqCsClientRecordsInfo.build());
				return;
			}
			for (RecordsInfoVO recordsInfoVO: recordInfoList) {
				CsClientRecordProto.RecordsInfo.Builder recordsInfo = CsClientRecordProto.RecordsInfo.newBuilder();
				if (recordsInfoVO.getId() != null) {
					recordsInfo.setId(recordsInfoVO.getId());
				}
				if (recordsInfoVO.getnProId() != null) {
					recordsInfo.setNProId(recordsInfoVO.getnProId());
				}
				if (recordsInfoVO.getInputRegionId() != null) {
					recordsInfo.setInputRegionId(recordsInfoVO.getInputRegionId());
				}
				if (recordsInfoVO.getInputRegionCode() != null) {
					recordsInfo.setInputRegionCode(recordsInfoVO.getInputRegionCode());
				}
				if (StringUtils.isNotBlank(recordsInfoVO.getInputRegionName())) {
					recordsInfo.setInputRegionName(recordsInfoVO.getInputRegionName());
				}
				if (StringUtils.isNotBlank(recordsInfoVO.getCustName())) {
					recordsInfo.setCustName(recordsInfoVO.getCustName());
				}
				if (StringUtils.isNotBlank(recordsInfoVO.getStrCreditPhone())) {
					recordsInfo.setStrCreditPhone(recordsInfoVO.getStrCreditPhone());
				}
				if (recordsInfoVO.getCreditId() != null) {
					recordsInfo.setCreditId(recordsInfoVO.getCreditId());
				}
				if (StringUtils.isNotBlank(recordsInfoVO.getCreditName())) {
					recordsInfo.setCreditName(recordsInfoVO.getCreditName());
				}
				if (StringUtils.isNotBlank(recordsInfoVO.getCreditCode())) {
					recordsInfo.setCreditCode(recordsInfoVO.getCreditCode());
				}
				if (recordsInfoVO.getProductTypeId() != null) {
					recordsInfo.setProductTypeId(recordsInfoVO.getProductTypeId());
				}
				if (StringUtils.isNotBlank(recordsInfoVO.getProductTypeName())) {
					recordsInfo.setProductTypeName(recordsInfoVO.getProductTypeName());
				}
				if (recordsInfoVO.getProductId() != null) {
					recordsInfo.setProductId(recordsInfoVO.getProductId());
				}
				if (StringUtils.isNotBlank(recordsInfoVO.getProductCode())) {
					recordsInfo.setProductCode(recordsInfoVO.getProductCode());
				}
				if (StringUtils.isNotBlank(recordsInfoVO.getProductName())) {
					recordsInfo.setProductName(recordsInfoVO.getProductName());
				}
				if (StringUtils.isNotBlank(recordsInfoVO.getStrBuyMoney())) {
					recordsInfo.setStrBuyMoney(recordsInfoVO.getStrBuyMoney());
				}
				if (StringUtils.isNotBlank(recordsInfoVO.getStrGrade())) {
					recordsInfo.setStrGrade(recordsInfoVO.getStrGrade());
				}
				if (recordsInfoVO.getVideoNum() != null) {
					recordsInfo.setVideoNum(recordsInfoVO.getVideoNum());
				}
				if (StringUtils.isNotBlank(recordsInfoVO.getBusinessTime())) {
					recordsInfo.setBusinessTime(recordsInfoVO.getBusinessTime());
				}
				if (recordsInfoVO.getLockClient() != null) {
					recordsInfo.setLockClient(recordsInfoVO.getLockClient());
				}
				if (StringUtils.isNotBlank(recordsInfoVO.getStrlockClient())) {
					recordsInfo.setStrlockClient(recordsInfoVO.getStrlockClient());
				}
				if (recordsInfoVO.getRecordStatus() != null) {
					recordsInfo.setRecordStatus(recordsInfoVO.getRecordStatus());
				}				
				if (StringUtils.isNotBlank(recordsInfoVO.getStrRecordStatus())) {
					recordsInfo.setStrRecordStatus(recordsInfoVO.getStrRecordStatus());
				}
				if (StringUtils.isNotBlank(recordsInfoVO.getPictureStrart())) {
					recordsInfo.setPictureStrart(recordsInfoVO.getPictureStrart());
				}
				if (StringUtils.isNotBlank(recordsInfoVO.getPictureEnd())) {
					recordsInfo.setPictureEnd(recordsInfoVO.getPictureEnd());
				}
				if (CollectionUtils.isNotEmpty(recordsInfoVO.getRecordsDetailInfos())){
					for (VideoDetailInfoVO videoDetailInfoVO : recordsInfoVO.getRecordsDetailInfos()){
						CsClientRecordProto.RecordsDetailInfo.Builder recordsDetailInfo = CsClientRecordProto.RecordsDetailInfo.newBuilder();
						if (videoDetailInfoVO.getVideoId() != null) {
							recordsDetailInfo.setVideoId(videoDetailInfoVO.getVideoId());
						}
						if (StringUtils.isNotBlank(videoDetailInfoVO.getFileName())) {
							recordsDetailInfo.setFileName(videoDetailInfoVO.getFileName());
						}
						if (videoDetailInfoVO.getRecordStatus() != null) {
							recordsDetailInfo.setRecordStatus(videoDetailInfoVO.getRecordStatus());
						}
						if (StringUtils.isNotBlank(videoDetailInfoVO.getStrRecordStatus())) {
							recordsDetailInfo.setStrRecordStatus(videoDetailInfoVO.getStrRecordStatus());
						}
						if (StringUtils.isNotBlank(videoDetailInfoVO.getErrorMsg())) {
							recordsDetailInfo.setErrorMsg(videoDetailInfoVO.getErrorMsg());
						}
						if (StringUtils.isNotBlank(videoDetailInfoVO.getStrUrl())) {
							recordsDetailInfo.setStrUrl(videoDetailInfoVO.getStrUrl());
						}
						recordsInfo.addRecordsDetailInfo(recordsDetailInfo);
					}
				}
				rsqCsClientRecordsInfo.addRecordsInfo(recordsInfo);
			}
			rsqCsClientRecordsInfo.setResult(HppConstants.RESULT_OK);
			rsqCsClientRecordsInfo.setResultInfo("获取业务信息成功");
			done.run(rsqCsClientRecordsInfo.build());
		} catch (Exception e) {
			logger.error("获取业务信息异常：" + e);
			rsqCsClientRecordsInfo.setResult(HppConstants.RESULT_ERROR);
			rsqCsClientRecordsInfo.setResultInfo("获取业务信息异常");
			done.run(rsqCsClientRecordsInfo.build());
		}
	}
	
	/**
	 * 删除异常录像信息
	 */
	@Override
	public void deleteErrorRecords(RpcController controller, ReqErrorRecord request,
			RpcCallback<RspCsClientRecord> done) {
		logger.info("开始删除异常录像信息，Cmd：【" + CMD.CMD_DDT_ERROR_RECORD_VALUE + "】 request：【" + request.toString() + "】 ");
		CsClientRecordProto.RspCsClientRecord.Builder rspCsClientRecord = CsClientRecordProto.RspCsClientRecord.newBuilder();
		rspCsClientRecord.setCmd(CMD.CMD_DDT_ERROR_RECORD_VALUE);
		try {
			
			int inputUser = request.getInputUser();
			com.hikvision.finance.fms.model.Users user = usersDao.get(inputUser);
			if (user == null) {
				rspCsClientRecord.setResult(HppConstants.RESULT_ERROR);
				rspCsClientRecord.setResultInfo("业务员不存在");
				done.run(rspCsClientRecord.build());
				return;
			}
			//异常录像信息ids
			List<Integer> Ids = request.getIdList(); 
			List<Prosaverecord> dbProsaverecords = proSaveRecordDao.findAllEntitysBy("id", Ids);
			if (CollectionUtils.isNotEmpty(dbProsaverecords)) {
				for (Prosaverecord dbProsaverecord : dbProsaverecords) {
					com.hikvision.finance.fms.model.Records dbRecords = recordsDao.findUniqueBy("nHistoryRecordId", dbProsaverecord.getId());
					if (dbRecords == null || dbRecords.getNrecordStatus() != Constants.RecordStatus.UPLOAD_FAIL) {
						rspCsClientRecord.setResult(HppConstants.RESULT_ERROR);
						rspCsClientRecord.setResultInfo("非上传失败的录像不能进行删除");
						done.run(rspCsClientRecord.build());
						return; 
					}
					recordsDao.deleteById(dbRecords.getId());
					proSaveRecordDao.deleteById(dbProsaverecord.getId());
					String note = "删除异常录像数据，业务关联id：" + dbProsaverecord.getId();
					collLogDao.writeOperLog(OperLogType.OperDelVideo, note, inputUser);
					rspCsClientRecord.setResult(HppConstants.RESULT_OK);
					rspCsClientRecord.setResultInfo("删除业务信息成功");
					done.run(rspCsClientRecord.build());
				}
			}else {
				rspCsClientRecord.setResult(HppConstants.RESULT_ERROR);
				rspCsClientRecord.setResultInfo("录像信息不存在");
				done.run(rspCsClientRecord.build());
			}
		} catch (Exception e) {
			logger.error("删除业务信息异常：" + e);
			rspCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rspCsClientRecord.setResultInfo("删除业务信息异常");
			done.run(rspCsClientRecord.build());
			throw new CMSException(CMSException.DEL_CLIENTRECORDS_ERR, e);
		}
		
	}
	
	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.ModifyCsClientRecordService#queryErrorRecords(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.ReqRecordsInfo, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void queryErrorRecords(RpcController controller, ReqRecordsInfo request,
			RpcCallback<RspErrRecordsInfo> done) {
		logger.info("开始查询异常录像信息，Cmd：【" + CMD.CMD_DT_ERROR_RECORD_VALUE + "】 request：【" + request.toString() + "】 ");
		CsClientRecordProto.RspErrRecordsInfo.Builder rspErrRecordsInfo = CsClientRecordProto.RspErrRecordsInfo.newBuilder();
		rspErrRecordsInfo.setCmd(CMD.CMD_DT_ERROR_RECORD_VALUE);
		try {
			Integer inputUser = request.getInputUser();
			if (inputUser == null || inputUser < 0) {
				rspErrRecordsInfo.setResult(HppConstants.RESULT_ERROR);
				rspErrRecordsInfo.setResultInfo("业务员不存在");
				done.run(rspErrRecordsInfo.build());
				return;
			}
			RecordInfo recordInfo = new RecordInfo();
			recordInfo.setInputUser(inputUser);
			if(StringUtils.isNotBlank(request.getStartDate())){
				recordInfo.setStartTime(DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", request.getStartDate()));// 业务时间 开始时间	
			}
			if(StringUtils.isNotBlank(request.getEndDate())){
				recordInfo.setEndTime(DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", request.getEndDate()));// 业务时间 开始时间	
			}
			recordInfo.setName(request.getCustName());
			recordInfo.setProductName(request.getProductName());
			//根据业务单来查询出 异常录像信息(等待上传, 上传失败, 录像不合规) 
			//获取没有上传的, 录像不合规的. 
			List<RecordInfo> recordInfoList = proSaveRecordDao.getDataList(recordInfo);
			if (CollectionUtils.isNotEmpty(recordInfoList)) {
				for (RecordInfo dbRecordInfo : recordInfoList) {
					CsClientRecordProto.ErrRecordsInfo.Builder errRecordsInfo = CsClientRecordProto.ErrRecordsInfo.newBuilder();
					VideoDetailInfoVO videoDetailInfoVO = null;
					if (dbRecordInfo.getUpload() == VideoUploadStatus.UPLOADED) { // 已上传成功的录像
						//录像不合规的
						if (dbRecordInfo.getRecordCheck() == Constants.RecordCheckStatus.NOT_CONFORMED) {
							videoDetailInfoVO = getVideoDetailInfo(dbRecordInfo);
							errRecordsInfo.setRecordCheck(Constants.RecordCheckStatus.NOT_CONFORMED);
							errRecordsInfo.setStrRecordCheck("不合规");
						} else { // 如果不是不合规的录像就过滤掉. 
							continue;
						}
					} else { // 没有上传成功的录像
						videoDetailInfoVO = getErrRecords(dbRecordInfo);
						if (videoDetailInfoVO == null) { // 如果不是已关联, 上传失败的录像就过滤掉. 
							continue;
						}
						errRecordsInfo.setRecordCheck(dbRecordInfo.getRecordCheck());
						if (StringUtils.isNotBlank(dbRecordInfo.getStrRecordCheck())) {
							errRecordsInfo.setStrRecordCheck(dbRecordInfo.getStrRecordCheck());
						}
					}
					if (videoDetailInfoVO.getVideoId() != null) { 
						errRecordsInfo.setId(videoDetailInfoVO.getVideoId());
					}
					if (StringUtils.isNotBlank(videoDetailInfoVO.getFileName())) {
						errRecordsInfo.setFileName(videoDetailInfoVO.getFileName());
					}
					if (dbRecordInfo.getNproId() != null) {
						errRecordsInfo.setNProId(dbRecordInfo.getNproId());
					}
					if (StringUtils.isNotBlank(dbRecordInfo.getName())) {
						errRecordsInfo.setCustName(dbRecordInfo.getName());
					}
					if (dbRecordInfo.getBusinessTime() != null ) {
						errRecordsInfo.setBusinessTime(DateUtils.getStringDateTime("yyyy-MM-dd HH:mm:ss", dbRecordInfo.getBusinessTime()));
					}
					if (videoDetailInfoVO.getRecordStatus() != null) {
						errRecordsInfo.setRecordStatus(videoDetailInfoVO.getRecordStatus());
					}
					if (StringUtils.isNotBlank(videoDetailInfoVO.getStrRecordStatus())) {
						errRecordsInfo.setStrRecordStatus(videoDetailInfoVO.getStrRecordStatus());
					}
					if (dbRecordInfo.getProductTypeId() != null) {
						errRecordsInfo.setProductTypeId(dbRecordInfo.getProductTypeId());
					}
					if (StringUtils.isNotBlank(dbRecordInfo.getProductTypeName())) {
						errRecordsInfo.setProductTypeName(dbRecordInfo.getProductTypeName());
					}
					if (dbRecordInfo.getProductId() != null) {
						errRecordsInfo.setProductId(dbRecordInfo.getProductId());
					}
					if (StringUtils.isNotBlank(dbRecordInfo.getProductCode())) {
						errRecordsInfo.setProductCode(dbRecordInfo.getProductCode());
					}
					if (StringUtils.isNotBlank(dbRecordInfo.getProductName())) {
						errRecordsInfo.setProductName(dbRecordInfo.getProductName());
					}
					if (StringUtils.isNotBlank(dbRecordInfo.getStrBuyMoney())) {
						errRecordsInfo.setStrBuyMoney(dbRecordInfo.getStrBuyMoney());
					}
					if (StringUtils.isNotBlank(dbRecordInfo.getStrUrl())) {
						errRecordsInfo.setStrUrl(dbRecordInfo.getStrUrl());
					}
					rspErrRecordsInfo.addErrRecordsInfo(errRecordsInfo);
					videoDetailInfoVO = null;
				}
			}
			rspErrRecordsInfo.setResult(HppConstants.RESULT_OK);
			rspErrRecordsInfo.setResultInfo("查询异常录像信息成功");
			done.run(rspErrRecordsInfo.build());
		} catch (Exception e) {
			logger.error("查询异常录像信息异常：" + e);
			rspErrRecordsInfo.setResult(HppConstants.RESULT_ERROR);
			rspErrRecordsInfo.setResultInfo("查询异常录像信息异常");
			done.run(rspErrRecordsInfo.build());
		}
	}
	
	
	private VideoDetailInfoVO getErrRecords(RecordInfo dbRecordInfo) {
		
		VideoDetailInfoVO videoDetailInfoVO = null; 
		com.hikvision.finance.fms.model.Records dbRecords = recordsDao.findUniqueBy("nHistoryRecordId", dbRecordInfo.getVideoId());//获取具体的录像信息
		if (dbRecords != null) { //不存在管理的录像
			switch (dbRecords.getNrecordStatus()) {
				case RecordStatus.CONTRACTED: //2-已关联（未上传）
					videoDetailInfoVO = new VideoDetailInfoVO();
					videoDetailInfoVO.setFileName(dbRecordInfo.getFileName());
					videoDetailInfoVO.setStrUrl(dbRecordInfo.getStrUrl());
					videoDetailInfoVO.setVideoId(dbRecordInfo.getVideoId());
					videoDetailInfoVO.setRecordStatus(RecordStatus.CONTRACTED);
					videoDetailInfoVO.setStrRecordStatus("未上传");
					dbRecordInfo.setStrRecordCheck("未检查");
					break;
				case RecordStatus.UPLOAD_FAIL: //9-上传失败
					videoDetailInfoVO = new VideoDetailInfoVO();
					videoDetailInfoVO.setFileName(dbRecordInfo.getFileName());
					videoDetailInfoVO.setStrUrl(dbRecordInfo.getStrUrl());
					videoDetailInfoVO.setVideoId(dbRecordInfo.getVideoId());
					videoDetailInfoVO.setRecordStatus(RecordStatus.UPLOAD_FAIL);
					videoDetailInfoVO.setStrRecordStatus("上传失败");
					dbRecordInfo.setStrRecordCheck("未检查");
					break;
				default:
					break;
				}
		}
		return videoDetailInfoVO;
	}
	
	/**
	 * 重新上传异常录像
	 */
	@Override
	public void uploadErrorRecords(RpcController controller, ReqErrorRecord request,
			RpcCallback<RspCsClientRecord> done) {
		logger.info("开始重新上传异常录像，Cmd：【" + CMD.CMD_MDT_ERROR_RECORD_VALUE + "】 request：【" + request.toString() + "】 ");
		CsClientRecordProto.RspCsClientRecord.Builder rspCsClientRecord = CsClientRecordProto.RspCsClientRecord.newBuilder();
		rspCsClientRecord.setCmd(CMD.CMD_MDT_ERROR_RECORD_VALUE);
		try {
			
			int inputUser = request.getInputUser();
			com.hikvision.finance.fms.model.Users user = usersDao.get(inputUser);
			if (user == null) {
				rspCsClientRecord.setResult(HppConstants.RESULT_ERROR);
				rspCsClientRecord.setResultInfo("业务员不存在");
				done.run(rspCsClientRecord.build());
				return;
			}
			//异常录像信息ids
			List<Integer> Ids = request.getIdList(); 
			List<Prosaverecord> dbProsaverecords = proSaveRecordDao.findAllEntitysBy("id", Ids);
			if (CollectionUtils.isNotEmpty(dbProsaverecords)) {
				for (Prosaverecord dbProsaverecord : dbProsaverecords) {
					com.hikvision.finance.fms.model.Records dbRecords = recordsDao.findUniqueBy("nHistoryRecordId", dbProsaverecord.getId());
					if (dbRecords == null || dbRecords.getNrecordStatus() != Constants.RecordStatus.UPLOAD_FAIL) {
						rspCsClientRecord.setResult(HppConstants.RESULT_ERROR);
						rspCsClientRecord.setResultInfo("非上传失败的录像不能进行上传");
						done.run(rspCsClientRecord.build());
						return; 
					}
					dbRecords.setNrecordStatus(Constants.RecordStatus.CONTRACTED);
					recordsDao.update(dbRecords);
					String note = "重新上传异常录像，业务关联id：" + dbProsaverecord.getId();
					collLogDao.writeOperLog(OperLogType.OperEditVideo, note, inputUser);
					rspCsClientRecord.setResult(HppConstants.RESULT_OK);
					rspCsClientRecord.setResultInfo("重新上传异常录像成功");
					done.run(rspCsClientRecord.build());
				}
			}else {
				rspCsClientRecord.setResult(HppConstants.RESULT_ERROR);
				rspCsClientRecord.setResultInfo("录像信息不存在");
				done.run(rspCsClientRecord.build());
			}
		} catch (Exception e) {
			logger.error("重新上传异常录像异常：" + e);
			rspCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rspCsClientRecord.setResultInfo("重新上传异常录像异常");
			done.run(rspCsClientRecord.build());
			throw new CMSException(CMSException.DEL_CLIENTRECORDS_ERR, e);
		}
		
	}
	
	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.ModifyCsClientRecordService#printCsClientRecords(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.ReqPrintRecord, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void printCsClientRecords(RpcController controller, ReqPrintRecord request,
			RpcCallback<RspPrintRecord> done) {
		logger.info("开始获取打印业务信息，Cmd：【" + CMD.CMD_DT_PRINT_RECORD_VALUE + "】 request：【" + request.toString() + "】 ");
		CsClientRecordProto.RspPrintRecord.Builder rspPrintRecord = CsClientRecordProto.RspPrintRecord.newBuilder();
		rspPrintRecord.setCmd(CMD.CMD_DT_PRINT_RECORD_VALUE);
		try {
			// 业务单id
			int clientId = request.getId();
			PrintOrderInfo printOrderInfo = getPrint(clientId);
			rspPrintRecord.setId(clientId);
			if (StringUtils.isNotBlank(printOrderInfo.getName())) {
				rspPrintRecord.setName(printOrderInfo.getName());
			}
			if (StringUtils.isNotBlank(printOrderInfo.getCreditname())) {
				rspPrintRecord.setCreditName(printOrderInfo.getCreditname());
			}
			if (StringUtils.isNotBlank(printOrderInfo.getCreditCode())) {
				rspPrintRecord.setCreditCode(printOrderInfo.getCreditCode());
			}
			if (StringUtils.isNotBlank(printOrderInfo.getNproId())) {
				rspPrintRecord.setNproId(printOrderInfo.getNproId());
			}
			if (StringUtils.isNotBlank(printOrderInfo.getStrGrade())) {
				rspPrintRecord.setStrGrade(printOrderInfo.getStrGrade());
			}
			if (StringUtils.isNotBlank(printOrderInfo.getProductType())) {
				rspPrintRecord.setProductType(printOrderInfo.getProductType());
			}
			if (StringUtils.isNotBlank(printOrderInfo.getProductName())) {
				rspPrintRecord.setProductName(printOrderInfo.getProductName());
			}
			if (StringUtils.isNotBlank(printOrderInfo.getProName())) {
				rspPrintRecord.setProName(printOrderInfo.getProName());
			}
			if (StringUtils.isNotBlank(printOrderInfo.getProUserNo())) {
				rspPrintRecord.setProUserNo(printOrderInfo.getProUserNo());
			}
			if (StringUtils.isNotBlank(printOrderInfo.getPictureStrart())) {
				rspPrintRecord.setPictureStrart(printOrderInfo.getPictureStrart());
			}
			if (StringUtils.isNotBlank(printOrderInfo.getPictureEnd())) {
				rspPrintRecord.setPictureEnd(printOrderInfo.getPictureEnd());
			}
			if (printOrderInfo.getBusinessTime() != null) {
				rspPrintRecord.setBusinessTime(DateUtils.getStringDateTime(printOrderInfo.getBusinessTime()));
			}
			rspPrintRecord.setResult(HppConstants.RESULT_OK);
			rspPrintRecord.setResultInfo("获取打印业务信息成功");
			done.run(rspPrintRecord.build());
		} catch (Exception e) {
			logger.error("获取打印业务信息异常：" + e);
			rspPrintRecord.setResult(HppConstants.RESULT_ERROR);
			rspPrintRecord.setResultInfo("获取打印业务信息异常");
			done.run(rspPrintRecord.build());
		}
	}
	
	/**
	 * 获取打印信息的私有方法
	 * @author zhoujiajun 2016年11月1日 下午2:21:21
	 * @param clientId
	 * @return
	 */
	private PrintOrderInfo getPrint(int clientId) {
		Clientinput clientinput = clientInputDao.get(clientId);
		PrintOrderInfo printOrderInfo = new PrintOrderInfo();
		try {
	        BeanUtils.copyNotNullProperties(printOrderInfo, clientinput);
        } catch (Exception e) {
	        throw new ExpectedException("", "拷贝bean出错");
        }
		Product product = productDao.get(clientinput.getProductId());
		if (product != null) {
			Producttype producttype = productTypeDao.get(product.getPid());
			printOrderInfo.setProductType(producttype.getStrName());// 产品类型名称
		}
		Paramdictionary paramdictionary = cardDao.get(clientinput.getCreditId());
		if (paramdictionary != null) {
			printOrderInfo.setCreditname(paramdictionary.getStrName());// 证件类型名称
		}
		List<Prosaverecord> prosaveRecordList = proSaveRecordDao.findBy("clientId", clientId);
		if (CollectionUtils.isNotEmpty(prosaveRecordList)) {
			Prosaverecord prosaverecord = prosaveRecordList.get(0);
			if (prosaverecord.getScreenshotSrc() != null) {
				String[] screenshotSrc = prosaverecord.getScreenshotSrc().split(",");
				for (int i = 0; i < screenshotSrc.length; i++) {
					if(i==0){
						printOrderInfo.setPictureStrart(screenshotSrc[i]);
					}
					if(i==1){
						printOrderInfo.setPictureEnd(screenshotSrc[i]);
					}
				}
			}else {
				//跟csclient打印页面约定好没有图片就插入nopic
				printOrderInfo.setPictureStrart("nopic"); 
				printOrderInfo.setPictureEnd("nopic");
			}
		}
		return printOrderInfo;
	}
	
	/**
	 * 检查业务单审批状态
	 * @author zhoujiajun 2016年10月25日 下午3:11:46
	 * @param clientId
	 * @param rspCsClientRecord
	 */
	private com.hikvision.finance.fms.model.Clientinput checkLockClient(Integer clientId, CsClientRecordProto.RspCsClientRecord.Builder rspCsClientRecord) {
		//业务单id
		com.hikvision.finance.fms.model.Clientinput dbClientInput = clientInputDao.get(clientId);
		if (dbClientInput == null) {
			rspCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rspCsClientRecord.setResultInfo("该业务单不存在");
			return null;
		}
		Integer lockClient = dbClientInput.getLockClient();
		if (lockClient != null && lockClient != LockClient.SEND_BACK) {
			rspCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rspCsClientRecord.setResultInfo("不是退回的业务单不能进行修改");
			return null;
		}
		return dbClientInput;
	}
	
	/**
	 * 更新业务单信息
	 * @author zhoujiajun 2016年10月25日 上午10:58:38
	 * @param request
	 * @param rsqCsClientRecord
	 * @return
	 */
	private com.hikvision.finance.fms.model.Clientinput updateClientInput(com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.CSClientInput csClientInput, CsClientRecordProto.RspCsClientRecord.Builder rsqCsClientRecord) {
		com.hikvision.finance.fms.model.Clientinput dbClientInput = clientInputDao.get(csClientInput.getId());
		// 更新业务单信息 
		if (dbClientInput == null) {
			rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rsqCsClientRecord.setResultInfo("业务单信息不存在");
			return null;
		}
		Users users = usersDao.get(csClientInput.getInputUser());
		if (users == null) {
			rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rsqCsClientRecord.setResultInfo("业务员不存在");
			return dbClientInput;
		}
		dbClientInput.setInputUser(csClientInput.getInputUser());//理财系统建单用户Id
		dbClientInput.setProName(users.getStrCode());// 业务员姓名
		dbClientInput.setProUserNo(users.getUserNo());// 业务员工号
		com.hikvision.finance.fms.model.Region dbRegion = regionDao.get(csClientInput.getInputRegionId());
		if (dbRegion == null) {
			rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rsqCsClientRecord.setResultInfo("区域信息不存在");
			return null;
		}
		dbClientInput.setInputRegionCode(dbRegion.getStrCode());
		dbClientInput.setInputRegionName(dbRegion.getStrName());
		dbClientInput.setInputRegionId(csClientInput.getInputRegionId());
		if (StringUtils.isNotBlank(csClientInput.getName())) {
			dbClientInput.setName(csClientInput.getName());
		}
		com.hikvision.finance.fms.model.Paramdictionary paramdictionary = cardDao.get(csClientInput.getCreditId());
		if (paramdictionary == null) {
			rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rsqCsClientRecord.setResultInfo("证件类型不存在");
			return null;
		}
		dbClientInput.setCreditId(csClientInput.getCreditId());
		if (StringUtils.isNotBlank(csClientInput.getCreditCode())) {
			dbClientInput.setCreditCode(csClientInput.getCreditCode());
		}
		ProductQo productQo = new ProductQo();
		productQo.setId(csClientInput.getProductId());
		productQo.setNenabled(Constants.ProductEnable.STARTUSING);
		com.hikvision.finance.fms.model.Product dbProduct = productDao.queryUnique(productQo);
		if (dbProduct == null){
			rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rsqCsClientRecord.setResultInfo("该产品不存在");
			return null;
		}
		dbClientInput.setProductId(csClientInput.getProductId());
		dbClientInput.setProductName(dbProduct.getStrName());
		dbClientInput.setProductCode(dbProduct.getStrCode());
		dbClientInput.setStrGrade(dbProduct.getStrGrade());
		com.hikvision.finance.fms.model.Producttype dbProducttype = productTypeDao.get(dbProduct.getPid());
		if (dbProducttype == null) {
			rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rsqCsClientRecord.setResultInfo("该产品类型不存在");
			return null;
		}
		dbClientInput.setProductTypeId(dbProduct.getPid());
		dbClientInput.setProductTypeName(dbProducttype.getStrName());
		dbClientInput.setLockClient(Constants.LockClient.NO_AUDIT);
		if (StringUtils.isNotBlank(csClientInput.getStrBankNo())) {
			dbClientInput.setStrBankNo(csClientInput.getStrBankNo());
		}
		if (StringUtils.isNotBlank(csClientInput.getStrBuyMoney())) {
			dbClientInput.setStrBuyMoney(csClientInput.getStrBuyMoney());
		}
		if (StringUtils.isNotBlank(csClientInput.getStrCreditPhone())) {
			dbClientInput.setStrCreditPhone(csClientInput.getStrCreditPhone());
		}
		return dbClientInput;
	}
	
	/**
	 * 获取业务单对应的录像上传状态
	 * @author zhoujiajun 2016年10月25日 下午7:21:36
	 * @param dbRecordInfo
	 * @return
	 */
	private VideoDetailInfoVO getVideoDetailInfo(RecordInfo dbRecordInfo) {
		
		String strRecordStatus = null;
		String errorMsg = null;
		VideoDetailInfoVO videoDetailInfoVO = new VideoDetailInfoVO();
		videoDetailInfoVO.setFileName(dbRecordInfo.getFileName());
		videoDetailInfoVO.setStrUrl(dbRecordInfo.getStrUrl());
		videoDetailInfoVO.setVideoId(dbRecordInfo.getVideoId());
		Integer upload =  dbRecordInfo.getUpload(); // 录像上传状态
		if (upload != null && VideoUploadStatus.NOT_UPLOAD == upload) { //上传不成功就要去录像表中查看具体的信息
			com.hikvision.finance.fms.model.Records dbRecords = recordsDao.findUniqueBy("nHistoryRecordId", dbRecordInfo.getVideoId());//获取具体的录像信息
			if (dbRecords != null) { //不存在管理的录像
				switch (dbRecords.getNrecordStatus()) {
					case RecordStatus.CONTRACTED: //2-已关联（未上传）
						videoDetailInfoVO.setRecordStatus(RecordStatus.CONTRACTED);
						strRecordStatus = "未上传";
						break;
					case RecordStatus.UPLOADING: //3-上传中
						videoDetailInfoVO.setRecordStatus(RecordStatus.UPLOADING);
						strRecordStatus = "上传中";
						break;
					case RecordStatus.UPLOAD_FAIL: //9-上传失败
						videoDetailInfoVO.setRecordStatus(RecordStatus.UPLOAD_FAIL);
						strRecordStatus = "上传失败";
						errorMsg = dbRecords.getErrorMsg();
						break;
					default:
						break;
					}
				videoDetailInfoVO.setErrorMsg(errorMsg);
				videoDetailInfoVO.setStrRecordStatus(strRecordStatus);
			}
		} else if (upload != null && VideoUploadStatus.UPLOADED == upload) {
			videoDetailInfoVO.setRecordStatus(RecordStatus.UPLOADED);
			strRecordStatus = "上传成功";
			videoDetailInfoVO.setStrRecordStatus(strRecordStatus);
		}
		return videoDetailInfoVO;
	}
	
	/**
	 * 根据审判id来返回对应的名称
	 * @author zhoujiajun 2016年10月25日 下午7:14:28
	 * @param lockClientId
	 * @return
	 */
	private String getStrLockClientById(Integer lockClientId) {
		
		String strLockClient = null;
		switch (lockClientId) {
		case LockClient.NO_AUDIT :
			strLockClient = "待审批";
			break;
		case LockClient.PASS :
			strLockClient = "审批通过";
			break;
		case LockClient.SEND_BACK :
			strLockClient = "审批退回";
			break;
		default:
			break;
		}
		return strLockClient;
	}
	
	private List<com.hikvision.finance.fms.model.Records> saveRecords(List<com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.CSRecords> recordsList, String ip, 
			CsClientRecordProto.RspCsClientRecord.Builder rsqCsClientRecord) {
		
		List<com.hikvision.finance.fms.model.Records> dbRecordsList = new ArrayList<com.hikvision.finance.fms.model.Records>();
		for (com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.CSRecords csRecords : recordsList) {
			com.hikvision.finance.fms.model.Records dbRecords = new com.hikvision.finance.fms.model.Records();
			Users users = usersDao.get(csRecords.getInputUser());
			if (users == null) {
				rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
				rsqCsClientRecord.setResultInfo("业务员不存在");
				return dbRecordsList;
			}
			dbRecords.setNuserId(csRecords.getInputUser());// 用户Id
			// 对没有结束的录像设置为异常结束录像
			recordsDao.saveErrorRecordStatus(users.getId());
			dbRecords.setStrRecordName(csRecords.getStrRecordName());
			dbRecords.setNrecordType(Constants.Records.recordType.NORMAL_RECORD);// 录像类型
			dbRecords.setNrecordFileType(csRecords.getNrecordFileType());
			dbRecords.setNrecordStatus(Constants.RecordStatus.CONTRACTED);// 关联 ???
			Date startTime = DateUtils.getDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, csRecords.getStartTime());
			Date endTime = DateUtils.getDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, csRecords.getEndTime());
			if (endTime.getTime() < startTime.getTime()) { //本地录像就会出现相同的时间
				rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
				rsqCsClientRecord.setResultInfo("控件录像结束时间不正确");
				return dbRecordsList;
			}
			dbRecords.setStartTime(startTime);// 控件记录的录像开始时间
			dbRecords.setEndTime(endTime);// 录像结束时间
			dbRecords.setStrUrl(csRecords.getStrUrl());// USB模式结束时会修改url
			dbRecords.setNupLoadPercent(csRecords.getNupLoadPercent());
			dbRecords.setNfileSize(csRecords.getNfileSize());
			dbRecords.setScreenshotSrc(csRecords.getScreenshotSrc());
			dbRecords.setNcollectRegionId(csRecords.getInputRegionId());// 用户的区域ID
			dbRecords.setStrIp(ip);//录该条录像的设备所在电脑的ip(用于usb模式录像数据的过滤)
			dbRecords.setNrecordStatus(RecordStatus.CONTRACTED);
			dbRecordsList.add(dbRecords);
		}
		return dbRecordsList;
	}
	
	/**
	 * 获取业务单详细信息(针对一个业务记录获取多个录像记录)
	 * @author zhoujiajun 2016年10月25日 下午8:10:28
	 * @param request
	 * @param rsqCsClientRecordsInfo
	 * @return
	 */
	private List<RecordsInfoVO> getRecordsInfo(ReqRecordsInfo request,
			CsClientRecordProto.RspCsClientRecordsInfo.Builder rsqCsClientRecordsInfo) {
		Integer inputUser = request.getInputUser();
		if (inputUser == null || inputUser < 0) {
			rsqCsClientRecordsInfo.setResult(HppConstants.RESULT_ERROR);
			rsqCsClientRecordsInfo.setResultInfo("业务员Id不存在");
			return null;
		}
		RecordInfo recordInfo = new RecordInfo();
		recordInfo.setInputUser(inputUser);
		recordInfo.setProductName(request.getProductName());
		recordInfo.setName(request.getCustName());
		if(StringUtils.isNotBlank(request.getStartDate())){
			recordInfo.setStartTime(DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", request.getStartDate()));// 业务时间 开始时间	
		}
		if(StringUtils.isNotBlank(request.getEndDate())){
			recordInfo.setEndTime(DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", request.getEndDate()));// 业务时间 开始时间	
		}
		List<RecordsInfoVO> recordInfos = new ArrayList<RecordsInfoVO>();
		//获取业务单信息(多个clientId)
		List<RecordInfo> recordInfoList = proSaveRecordDao.getClientRecordsList(recordInfo);
		//获取视频的数量(1个clientId)
		List<RecordInfo> recordCountList = proSaveRecordDao.getVideoCount(recordInfo);
		if (CollectionUtils.isEmpty(recordInfoList) || CollectionUtils.isEmpty(recordCountList)) {
			return recordInfos;
		}
		boolean isFlag = true; 	
		for (RecordInfo recordCount: recordCountList){
			//视频的具体信息
			List<VideoDetailInfoVO> videoDetailInfoVOList = new ArrayList<VideoDetailInfoVO>();
			RecordsInfoVO rspRecordInfo = null;
			//出现异常录像的标记
			Integer recordStatus = 0;
			//出现正常录像的标记
			Integer recordStatusUploaded = 0;
			for (RecordInfo dbRecordInfo: recordInfoList){
				VideoDetailInfoVO videoDetailInfoVO = null;
				if (recordCount.getBusiId() == dbRecordInfo.getBusiId() && isFlag){
					rspRecordInfo = new RecordsInfoVO();
					rspRecordInfo.setId(dbRecordInfo.getBusiId());
					rspRecordInfo.setnProId(dbRecordInfo.getNproId());
					rspRecordInfo.setInputRegionId(dbRecordInfo.getInputRegionId());
					rspRecordInfo.setInputRegionCode(dbRecordInfo.getInputRegionCode());
					rspRecordInfo.setInputRegionName(dbRecordInfo.getInputRegionName());
					rspRecordInfo.setCustName(dbRecordInfo.getName());
					rspRecordInfo.setStrCreditPhone(dbRecordInfo.getStrCreditPhone());
					rspRecordInfo.setCreditId(dbRecordInfo.getCreditId());
					Paramdictionary paramdictionary = cardDao.get(dbRecordInfo.getCreditId());
					if (paramdictionary != null) {
						rspRecordInfo.setCreditName(paramdictionary.getStrName());// 证件类型名称
					}
					rspRecordInfo.setCreditCode(dbRecordInfo.getCreditCode());
					rspRecordInfo.setProductTypeId(dbRecordInfo.getProductTypeId());
					rspRecordInfo.setProductTypeName(dbRecordInfo.getProductTypeName());
					rspRecordInfo.setProductId(dbRecordInfo.getProductId());
					rspRecordInfo.setProductCode(dbRecordInfo.getProductCode());
					rspRecordInfo.setProductName(dbRecordInfo.getProductName());
					rspRecordInfo.setStrBuyMoney(dbRecordInfo.getStrBuyMoney());
					rspRecordInfo.setStrGrade(dbRecordInfo.getStrGrade());
					if (dbRecordInfo.getScreenshotSrc() != null) {
						String[] screenshotSrc = dbRecordInfo.getScreenshotSrc().split(",");
						for (int i = 0; i < screenshotSrc.length; i++) {
							if(i==0){
								rspRecordInfo.setPictureStrart(screenshotSrc[i]);
							}
							if(i==1){
								rspRecordInfo.setPictureEnd(screenshotSrc[i]);
							}
						}
					}else {
						//跟csclient打印页面约定好没有图片就插入nopic
						rspRecordInfo.setPictureStrart("nopic"); 
						rspRecordInfo.setPictureEnd("nopic");
					}
					if (dbRecordInfo.getBusinessTime() != null) {
						String businessTime = DateUtils.getStringDateTime(dbRecordInfo.getBusinessTime());
						rspRecordInfo.setBusinessTime(businessTime);
					}
					String strLockClient = getStrLockClientById(dbRecordInfo.getLockClient());
					if (StringUtils.isBlank(strLockClient)) {
						rsqCsClientRecordsInfo.setResult(HppConstants.RESULT_ERROR);
						rsqCsClientRecordsInfo.setResultInfo("审批状态不存在");
						return null;
					}
					rspRecordInfo.setLockClient(dbRecordInfo.getLockClient());
					rspRecordInfo.setStrlockClient(strLockClient);
					if (recordCount.getVideoCount() != null) {
						rspRecordInfo.setVideoNum(recordCount.getVideoCount().intValue());
					}
					//获取业务单对应的录像上传状态
					//`nRecordStatus` int(11)'记录状态，2-已关联（未上传），3-上传中，4-已上传，9-上传失败',
					videoDetailInfoVO = getVideoDetailInfo(dbRecordInfo);
					isFlag = false;
				} else if (recordCount.getBusiId() == dbRecordInfo.getBusiId() && !isFlag) {
					videoDetailInfoVO = getVideoDetailInfo(dbRecordInfo);
				}
				if (videoDetailInfoVO != null) {
					if (videoDetailInfoVO.getRecordStatus() == null) {
						recordStatus = -1;
					} else if (videoDetailInfoVO.getRecordStatus() == RecordStatus.UPLOADED) {
						recordStatusUploaded = RecordStatus.UPLOADED; 
					} else if (recordStatus < videoDetailInfoVO.getRecordStatus()) {
						recordStatus = videoDetailInfoVO.getRecordStatus();
					}
					videoDetailInfoVOList.add(videoDetailInfoVO);
				}
			}
			if (rspRecordInfo != null) {
				
				String strRecordStatus = null;
				if (recordStatus == RecordStatus.CONTRACTED) {//2-已关联（未上传）
					rspRecordInfo.setRecordStatus(RecordStatus.CONTRACTED);
					strRecordStatus = "未上传";
				} else if (recordStatus == RecordStatus.UPLOADING){ //3-上传中
					rspRecordInfo.setRecordStatus(RecordStatus.UPLOADING);
					strRecordStatus = "上传中";
				} else if (recordStatus == RecordStatus.UPLOAD_FAIL){ //9-上传失败
					rspRecordInfo.setRecordStatus(RecordStatus.UPLOAD_FAIL);
					strRecordStatus = "上传失败";
				} else if (recordStatus == 0 && recordStatusUploaded == RecordStatus.UPLOADED) {
					strRecordStatus = "上传成功";
					rspRecordInfo.setRecordStatus(RecordStatus.UPLOADED);
				} 
				rspRecordInfo.setStrRecordStatus(strRecordStatus);
				rspRecordInfo.setRecordsDetailInfos(videoDetailInfoVOList);
				recordInfos.add(rspRecordInfo);
			}
			isFlag = true;
		}
		return recordInfos;
	}
	
	private com.hikvision.finance.fms.model.Clientinput saveClientInput(com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.CSClientInput csClientInput, CsClientRecordProto.RspCsClientRecord.Builder rsqCsClientRecord) {
		com.hikvision.finance.fms.model.Clientinput dbClientInput = new com.hikvision.finance.fms.model.Clientinput();
		//理财系统建单区域号
		if (csClientInput.getInputRegionId() < 0) {
			rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rsqCsClientRecord.setResultInfo("理财系统建单区域号不存在");
			return dbClientInput;
		}
		Region region = regionDao.get(csClientInput.getInputRegionId());//录单中交易区域ID
		if (region == null) {
			rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rsqCsClientRecord.setResultInfo("理财系统建单区域不存在");
			return dbClientInput;
		}
		dbClientInput.setInputRegionId(csClientInput.getInputRegionId());
		dbClientInput.setInputRegionCode(region.getStrCode());// 交易区域编码
		dbClientInput.setInputRegionName(region.getStrName());// 交易区域名称
		Users users = usersDao.get(csClientInput.getInputUser());
		if (users == null) {
			rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rsqCsClientRecord.setResultInfo("业务员不存在");
			return dbClientInput;
		}
		dbClientInput.setInputUser(csClientInput.getInputUser());//理财系统建单用户Id
		dbClientInput.setProName(users.getStrCode());// 业务员姓名
		dbClientInput.setProUserNo(users.getUserNo());// 业务员工号
		dbClientInput.setStatus(Constants.BusiComplainStatus.NOT_COMPLAIN);// 设置单子的业务类型 0：签约单 1：投诉单
		List<Clientinput> clientinputs = clientInputDao.findBy("nproId", csClientInput.getNproId());
		if (CollectionUtils.isNotEmpty(clientinputs)) {
			rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rsqCsClientRecord.setResultInfo("业务单号重复, 不能重复提交");
			return dbClientInput;
		}
		dbClientInput.setNproId(csClientInput.getNproId()); //业务单号--业务流水号
		dbClientInput.setNisRead(Constants.BusiReadStatus.NOT_READ);
		dbClientInput.setNisPay(Constants.VideoDueStatus.NOT_DUE);
		//业务时间
		dbClientInput.setBusinessTime(DateUtils.getDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, csClientInput.getBusinessTime()));
		//关联业务单时间
		dbClientInput.setInputTime(DateUtils.getDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, csClientInput.getInputTime()));
		dbClientInput.setName(csClientInput.getName());// 客户姓名
		dbClientInput.setCreditCode(csClientInput.getCreditCode()); //证件号
		dbClientInput.setCreditId(csClientInput.getCreditId());//证件类型Id	
		
		ProductQo productQo = new ProductQo();
		productQo.setId(csClientInput.getProductId());
		productQo.setNenabled(Constants.ProductEnable.STARTUSING);
		Product product = productDao.queryUnique(productQo);
		if (product == null) {
			rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rsqCsClientRecord.setResultInfo("获取产品或产品权限出错");
			return dbClientInput;
		}
		dbClientInput.setProductId(csClientInput.getProductId());
		Producttype producttype = productTypeDao.get(product.getPid());
		if (producttype == null) {
			rsqCsClientRecord.setResult(HppConstants.RESULT_ERROR);
			rsqCsClientRecord.setResultInfo("获取产品类型出错");
			return dbClientInput;
		}
		dbClientInput.setProductTypeId(product.getPid()); // 产品类型Id
		dbClientInput.setProductTypeName(producttype.getStrName());// 产品类型名称
		dbClientInput.setProductName(product.getStrName()); //产品代码
		dbClientInput.setProductCode(product.getStrCode()); //产品名称
		dbClientInput.setGuaranteeType(product.getGuaranteeType()); //保障期限
		dbClientInput.setGuaranteeYears(product.getGuaranteeYears());
		dbClientInput.setStrEndDate(product.getStrEndDate()); //产品的到期日
		dbClientInput.setProductSaveType(product.getSaveType()); //视频保存周期
		dbClientInput.setProductSaveMonth(product.getSaveYears());//视频保存周期
		dbClientInput.setStrGrade(product.getStrGrade()); //产品发行机构
		dbClientInput.setStrBankNo(csClientInput.getStrBankNo());//客户银行卡号
		dbClientInput.setStrCreditPhone(csClientInput.getStrCreditPhone());//客户电话				
		dbClientInput.setStrBuyMoney(csClientInput.getStrBuyMoney());//购买金额
		dbClientInput.setDelaydays(0);// 设置初始延期天数为0
		dbClientInput.setLockClient(LockClient.NO_AUDIT);
		Date videoEndDate = getVideoEndDate(product, csClientInput);
		dbClientInput.setStrVideoEndDate(videoEndDate);//视频到期日
		return dbClientInput;
	}
	
	private Date getVideoEndDate(Product product, com.hikvision.finance.fms.cms.hpp.csclient.CsClientRecordProto.CSClientInput csClientInput){
		Date videoEndDate = new Date();
		if (product.getStrEndDate() != null) {// 产品到期日不为空，则以产品到期日为起点
			videoEndDate = product.getStrEndDate();
		} else if (product.getGuaranteeYears() != null && product.getGuaranteeYears() > 0) {// 保证年期不为空，则以签约时间为起点
			Date businessTime = DateUtils.getDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, csClientInput.getBusinessTime());
			switch (product.getGuaranteeType()) {
				case ProductKeepType.YEAR:
					videoEndDate = DateUtils.addYears(businessTime, product.getGuaranteeYears());// 加上保障年期
					break;
				case ProductKeepType.MONTH:
					videoEndDate = DateUtils.addMonths(businessTime, product.getGuaranteeYears());// 加上保障年期
					break;
				case ProductKeepType.DAY:
					videoEndDate = DateUtils.addDays(businessTime, product.getGuaranteeYears());// 加上保障年期
					break;	
			}
		}
		if (product.getSaveYears() != null && product.getSaveYears() > 0) {// 视频保存周期不为空，则加上视频保存周期
			switch (product.getSaveType()) {
				case ProductKeepType.YEAR:
					videoEndDate = DateUtils.addYears(videoEndDate, product.getSaveYears());
					break;
				case ProductKeepType.MONTH:
					videoEndDate = DateUtils.addMonths(videoEndDate, product.getSaveYears());
					break;
				case ProductKeepType.DAY:
					videoEndDate = DateUtils.addDays(videoEndDate, product.getSaveYears());
					break;
			}
		}
		return videoEndDate;
	}

	public ICollLogDao getCollLogDao() {
		return collLogDao;
	}

	public void setCollLogDao(ICollLogDao collLogDao) {
		this.collLogDao = collLogDao;
	}

	public IProductTypeDao getProductTypeDao() {
		return productTypeDao;
	}

	public void setProductTypeDao(IProductTypeDao productTypeDao) {
		this.productTypeDao = productTypeDao;
	}

	public IProductDao getProductDao() {
		return productDao;
	}

	public void setProductDao(IProductDao productDao) {
		this.productDao = productDao;
	}

	public IRegionDao getRegionDao() {
		return regionDao;
	}

	public void setRegionDao(IRegionDao regionDao) {
		this.regionDao = regionDao;
	}

	public IUsersDao getUsersDao() {
		return usersDao;
	}

	public void setUsersDao(IUsersDao usersDao) {
		this.usersDao = usersDao;
	}
	
	public ICardDao getCardDao() {
		return cardDao;
	}
	
	public void setCardDao(ICardDao cardDao) {
		this.cardDao = cardDao;
	}
	
	public IRecordsDao getRecordsDao() {
		return recordsDao;
	}
	
	public void setRecordsDao(IRecordsDao recordsDao) {
		this.recordsDao = recordsDao;
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
