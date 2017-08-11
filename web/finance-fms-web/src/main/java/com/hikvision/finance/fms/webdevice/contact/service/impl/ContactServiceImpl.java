/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-17 下午4:46:16
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.contact.service.impl;

import java.io.Serializable;
import java.net.URLDecoder;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.hikvision.finance.core.util.BeanUtils;
import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.enums.ProSaveRecordEnum;
import com.hikvision.finance.fms.common.init.FmsPropertyPlaceholder;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.LockClient;
import com.hikvision.finance.fms.common.util.Constants.OperLogType;
import com.hikvision.finance.fms.common.util.Constants.ProductKeepType;
import com.hikvision.finance.fms.common.util.Constants.RecordStatus;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.model.Businessorder;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.model.Cvrassociatemapping;
import com.hikvision.finance.fms.model.Paramdictionary;
import com.hikvision.finance.fms.model.Procomplaint;
import com.hikvision.finance.fms.model.Product;
import com.hikvision.finance.fms.model.Producttype;
import com.hikvision.finance.fms.model.Prosaverecord;
import com.hikvision.finance.fms.model.Records;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.model.Sysconfig;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.card.dao.ICardDao;
import com.hikvision.finance.fms.modules.device.dao.ICvrassociatemappingDao;
import com.hikvision.finance.fms.modules.product.dao.IProductDao;
import com.hikvision.finance.fms.modules.product.dao.IProductTypeDao;
import com.hikvision.finance.fms.modules.product.service.IProductService;
import com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.sale.dao.IClientInputDao;
import com.hikvision.finance.fms.modules.sale.dao.IProcomplaintDao;
import com.hikvision.finance.fms.modules.sysconfig.dao.ISysconfigDao;
import com.hikvision.finance.fms.modules.user.dao.IUsersDao;
import com.hikvision.finance.fms.webdevice.contact.dao.IBusinessOrderDao;
import com.hikvision.finance.fms.webdevice.contact.dto.BasicProductInfo;
import com.hikvision.finance.fms.webdevice.contact.dto.ClientinputInfo;
import com.hikvision.finance.fms.webdevice.contact.dto.OrderEnableInfo;
import com.hikvision.finance.fms.webdevice.contact.dto.PrintOrderInfo;
import com.hikvision.finance.fms.webdevice.contact.dto.ProductCodeInfo;
import com.hikvision.finance.fms.webdevice.contact.dto.RealOrdersInfo;
import com.hikvision.finance.fms.webdevice.contact.qo.BusinessOrderQo;
import com.hikvision.finance.fms.webdevice.contact.service.IContactService;
import com.hikvision.finance.fms.webdevice.datacollect.dao.IRecordsDao;
import com.hikvision.finance.fms.webdevice.log.dao.ICollLogDao;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.AjaxData;

/**
 * <p>业务单数据service</p>
 * @author xujiangfei 2016-4-17 下午4:46:16
 * @version V1.0 
 */
public class ContactServiceImpl extends BaseServiceImpl< Businessorder,  BusinessOrderQo, IBusinessOrderDao> implements IContactService{
	
	private IProductService productService;
	private IBusinessOrderDao businessOrderDao;
	private IProductDao productDao;
	private ICardDao cardDao;
	private IProSaveRecordDao proSaveRecordDao;
	private IRecordsDao recordsDao;
	private ICollLogDao collLogDao;//记录日志
	private ICvrassociatemappingDao cvrassociatemappingDao;
	private IUsersDao usersDao;
	private IRegionDao regionDao;
	private IClientInputDao clientInputDao; 
	private IProductTypeDao productTypeDao;
	private ISysconfigDao sysconfigDao;
	private IProcomplaintDao procomplaintDao;
	
	/*
	 * (non-Javadoc)获取业务单，包括签约单和投诉单，关联时使用，获取OSD使用
	 * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#getOrder()
	 */
	public Businessorder getOrder(){
		Businessorder businessorder = businessOrderDao.findUniqueBy("nuserId", SessionUtil.getUserSession().getUserId());
		if (businessorder == null || (new Integer(Constants.BusiComplainStatus.NOT_COMPLAIN)).equals(businessorder.getStatus())) {// 没有业务单或者是签约单
			if (businessorder != null) {
				businessorder.setComplainTime(null);
				businessorder.setCompReason(null);
			}
			return businessorder;
		}
		Clientinput clientinput = null;
		if (businessorder.getClientId() != null) {
			clientinput = clientInputDao.get(businessorder.getClientId());
		}
		if(clientinput!=null){
			businessorder.setInputRegionId(clientinput.getInputRegionId());// 交易区域Id
			businessorder.setProductName(clientinput.getProductName());// 产品名称
			businessorder.setProductCode(clientinput.getProductCode());// 产品编码
			businessorder.setStrEndDate(clientinput.getStrEndDate());// 产品到期日
			businessorder.setGuaranteeYears(clientinput.getGuaranteeYears());// 保障期限
			businessorder.setProductSaveMonth(clientinput.getProductSaveMonth());// 视频保存周期
			businessorder.setStrGrade(clientinput.getStrGrade());// 产品发行机构
			businessorder.setStrRisklevel(clientinput.getStrRisklevel());// 产品风险等级
			businessorder.setCustomerName(clientinput.getName());// 客户姓名
			businessorder.setCreditCode(clientinput.getCreditCode());// 客户证件号
			businessorder.setCreditId(clientinput.getCreditId());// 客户证件类型Id
			businessorder.setStrCreditPhone(clientinput.getStrCreditPhone());// 客户电话
			businessorder.setStrBankNo(clientinput.getStrBankNo());// 客户银行卡号
			businessorder.setStrBuyMoney(clientinput.getStrBuyMoney());// 购买金额
			businessorder.setBusinessTime(clientinput.getBusinessTime());// 业务时间
			businessorder.setStrRemarks(clientinput.getStrRemarks());// 备注
		}
		return businessorder;
	}
	
	/* (non-Javadoc)获取话术模板
     * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#getWordTemplate(java.lang.Integer)
     */
    @Override
    public String getWordTemplate(int id) {
    	Product product = productDao.get(id);
    	String wordTemplate = product.getWordTemplate();
	    return wordTemplate;
    }
    
	//*************************签约单相关操作*********************************//
	/* (non-Javadoc)获取录入的签约单，打开签约单录单页面时使用
     * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#getSignOrder()
     */
    @Override
	public Businessorder getSignOrder() {
		List<Businessorder> businessorderList = businessOrderDao.findBy(new String[] {"nuserId", "status"}, new Object[] {
		        SessionUtil.getUserSession().getUserId(), Constants.BusiComplainStatus.NOT_COMPLAIN});
		if (businessorderList.size() > 0) {// 已经录过单，底层实现决定了businessorderList不可能是null，不用判断空指针
			return businessorderList.get(0);
		} else {// 没有录过单，默认提供网点区域
			Integer userId = SessionUtil.getUserSession().getUserId();
			Businessorder businessorder = new Businessorder();
			businessorder.setNuserId(userId);// 未录单情况下，action也可以查询用户信息
			Users user = usersDao.get(userId);
			if(user==null){
				throw new ExpectedException("","用户不存在，请联系管理员");
			}
			businessorder.setInputRegionId(user.getNetId());// 未录单情况下，action也可以查询区域信息
			return businessorder;
		}
	}
    
    /* 
     * 获取业务单使能状态
     * (non-Javadoc)
     * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#getOrderEnable()
     */
    @Override
    public OrderEnableInfo getOrderEnable() {
    	List<Sysconfig> sysconfigList = sysconfigDao.findBy("ntype", Constants.SysConfigType.SysBigType.SYS_B_BUSINESS);
		Map<Integer, Integer> sysconfigMap = new HashMap<Integer, Integer>();
		for (Sysconfig singleSystemConfig : sysconfigList) {
			sysconfigMap.put(singleSystemConfig.getNkey(), singleSystemConfig.getNenabled());
		}
		OrderEnableInfo orderEnableInfo = new OrderEnableInfo();
		orderEnableInfo.setInputRegionCodeEnable(sysconfigMap.get(Constants.SysConfigType.TRANSACTION_ZONE_NUMBER));// 交易区域号
		orderEnableInfo.setStrCreditPhoneEnable(sysconfigMap.get(Constants.SysConfigType.CUSTOMER_TEL));// 客户电话
		orderEnableInfo.setStrBankNoEnable(sysconfigMap.get(Constants.SysConfigType.CUSTOMER_BANKNO));// 客户银行卡号
		orderEnableInfo.setStrGradeEnable(sysconfigMap.get(Constants.SysConfigType.PRODUCT_ORG));// 产品发行机构
		orderEnableInfo.setGuaranteeYearsEnable(sysconfigMap.get(Constants.SysConfigType.PRODUCT_SAVEYEAR));// 产品保障期限
		orderEnableInfo.setStrVideoEndDateEnable(sysconfigMap.get(Constants.SysConfigType.PRODUCT_ENDDATE));// 产品到期日
		orderEnableInfo.setProductSaveMonthEnable(sysconfigMap.get(Constants.SysConfigType.VIDEO_SAVEYEAR));// 视频保存时间
		orderEnableInfo.setStrRisklevelEnable(sysconfigMap.get(Constants.SysConfigType.PRODUCT_RISKLEVEL));// 产品风险等级
		orderEnableInfo.setStrBuyMoneyEnable(sysconfigMap.get(Constants.SysConfigType.PURCHASE_AMOUNT));// 购买金额
		orderEnableInfo.setProUserNoEnable(sysconfigMap.get(Constants.SysConfigType.CLERK_NUMBER));// 业务员编号
		orderEnableInfo.setStrRemarksEnable(sysconfigMap.get(Constants.SysConfigType.TRANSACTION_REMARK));// 备注
	    return orderEnableInfo;
    }
	
    /*
     * 理财经理获取有权限的产品编码
     * (non-Javadoc)
     * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#getPermitProductCodeList()
     */
    public List<ProductCodeInfo> getPermitProductCodeList(){
    	List<Product> permitProductList = productService.getPermitProductList();
    	List<ProductCodeInfo> permitProductCodeList = new ArrayList<ProductCodeInfo>();
		for (Product product : permitProductList) {
			ProductCodeInfo productCodeInfo = new ProductCodeInfo();
			productCodeInfo.setValue(product.getId());
			productCodeInfo.setLabel(product.getStrCode());
			productCodeInfo.setProduct(product);
			permitProductCodeList.add(productCodeInfo);
		}
    	return permitProductCodeList;
    }
	
    /*
     * 理财经理选择产品编码后获取基本产品信息
     * (non-Javadoc)
     * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#getBasicProductbyId(int)
     */
	public BasicProductInfo getBasicProductbyId(int id){
		Product product = productDao.get(id);
		BasicProductInfo basicProductInfo = new BasicProductInfo();
		try {
	        BeanUtils.copyNotNullProperties(basicProductInfo, product);
        } catch (Exception e) {
	        throw new ExpectedException("", "拷贝bean出错");
        }
		return basicProductInfo;
	}
    
	/* 
	 * 理财经理获取证件类型
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#getCardList()
	 */
	public List<Paramdictionary> getCardList(){
		List<Paramdictionary> cardList = cardDao.findBy("enable", 1);
		return cardList;
	}
	
	/* 添加签约单
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#addSignOrder(com.hikvision.finance.fms.model.Businessorder)
	 */
	@Override
	public AjaxData addSignOrder(Businessorder businessorder) {
		AjaxData ajaxData = new AjaxData(false);
		businessOrderDao.deleteBy(new String[] {"nuserId"}, new Object[] {SessionUtil.getUserSession().getUserId()});// 删除原业务单
		businessorder.setNuserId(Integer.valueOf(SessionUtil.getUserSession().getUserId()));// 设置用户id
		businessorder.setStatus(Constants.BusiComplainStatus.NOT_COMPLAIN);// 设置单子的业务类型 0：签约单 1：投诉单
		//校验提交签约单的时候 产品是否过期  禁用等
		Integer productId = businessorder.getProductId();
		Product product = productDao.get(productId);
		if (productId != null) {
			List<Product> products = productService.getPermitProductList();
			if (products == null || product == null) {
				throw new ExpectedException("", "获取产品或产品权限出错");
			} else {// 否则判断是否包含这个产品，未包含则抛出异常
				boolean flag = false;
				for (Product singleProduct : products) {
					if (singleProduct.getId().equals(product.getId())) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					throw new ExpectedException("", "该产品无法被使用");
				}
			}
		} else {
			throw new ExpectedException("", "产品id为空,未获取到产品信息");
		}
		Producttype producttype = productTypeDao.get(businessorder.getProductTypeId());
		businessorder.setProductTypeName(producttype.getStrName());// 产品类型名称
		businessorder.setGuaranteeType(product.getGuaranteeType());
		businessorder.setGuaranteeYears(product.getGuaranteeYears());
		businessorder.setProductSaveType(product.getSaveType());
		businessorder.setProductSaveMonth(product.getSaveYears());
		businessOrderDao.save(businessorder);// 保存新业务单
		// 记录操作日志
		String note = "录入签约单，业务单流水号：" + businessorder.getNproId() + "，客户姓名：" + businessorder.getCustomerName();
		collLogDao.writeOperLog(OperLogType.OperOrder, note);
		return ajaxData;
	}
	
	//*************************投诉单相关操作*********************************//
	/* 
	 * 录入投诉单和选择历史业务单时获取历史业务单
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#getDataPage(int, int, java.lang.String)
	 */
	@Override
	public Page getDataPage(int limit, int start, Clientinput clientinput, Integer type) {
		Page page = businessOrderDao.getDataPage(limit, start, clientinput, type);
		for (ClientinputInfo clientinputInfo : (List<ClientinputInfo>)page.getResult()) {
			clientinputInfo.setComplainTime(DateUtils.getStringDateTime(new Date()));
		}
		return page;
	}
	
	/* 
	 * 添加投诉单
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#addcomplainOrder(com.hikvision.finance.fms.model.Businessorder)
	 */
	@Override
	public AjaxData addcomplainOrder(Businessorder businessorder) {
		AjaxData ajaxData = new AjaxData(false);
		// 删除原有业务单
		businessOrderDao.deleteBy("nuserId", SessionUtil.getUserSession().getUserId());
		// 添加投诉单
		businessorder.setNuserId(SessionUtil.getUserSession().getUserId());
		businessorder.setStatus(Constants.BusiComplainStatus.COMPLAINED);
		businessOrderDao.save(businessorder);
		// 记录操作日志
		String note = "录入投诉单，业务单流水号：" + businessorder.getNproId() + "，客户姓名：" + businessorder.getCustomerName();
		collLogDao.writeOperLog(OperLogType.OperOrder, note);
		return ajaxData;
	}
	
	//*************************关联操作*********************************//
	/*
	 * 关联业务单并存储（新签约单or投诉单）
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#saveRelateRecords(java.lang.Integer, java.lang.String)
	 */
	@Override
	public AjaxData saveRelateRecords(Integer recordid) {
		AjaxData ajaxData = new AjaxData(false);
		Clientinput clientInput = null;
		Businessorder businessorder = businessOrderDao.findUniqueBy("nuserId", SessionUtil.getUserSession().getUserId());
		if(businessorder!=null){
			if((new Integer(Constants.BusiComplainStatus.NOT_COMPLAIN)).equals(businessorder.getStatus())){//签约单逻辑
				//把business中的数据放到clientinput中
				clientInput = saveClientinput(businessorder);
				//把records中的数据放到ProSaveRecord中
				Records record = recordsDao.get(recordid);
				if(record!=null&&businessorder!=null){
					Prosaverecord proSaveRecord = new Prosaverecord();
					proSaveRecord.setStateType(Constants.RecordType.RECORD_SIGN);//1-签约录像
					String returnStr = saveProsaveRecord(record, businessorder, clientInput, proSaveRecord);// 依据record和businessorder生成prosaverecord
					if(returnStr!=null){
						ajaxData.setMsg(returnStr);
					}
					// 记录操作日志
					String note = "关联新签约单，录像名：" + record.getStrRecordName() + "，业务单流水号：" + businessorder.getNproId() + "，客户姓名：" + businessorder.getCustomerName();
					collLogDao.writeOperLog(OperLogType.OperRel, note);
				}
			}else{//投诉单逻辑
				//1.投诉单的迁移
				//向产品投诉表中（ProComplaint）写数据，写投诉的原因，以及关联的签约单
				
				Procomplaint procomplaint = new Procomplaint();
				clientInput = clientInputDao.get(businessorder.getClientId());//businessorder
				
				procomplaint.setClientId(clientInput.getId());//客户端录入ID
				procomplaint.setCompReason(businessorder.getCompReason());//投诉原因
				procomplaint.setCompTime(businessorder.getComplainTime());//投诉日期
				procomplaint.setCompUser(businessorder.getCustomerName());//投诉录入人
				
				procomplaint.setRegionId(clientInput.getInputRegionId());//区域
				procomplaint.setDealTime(new Date());//投诉处理日期
				
				//保存clientinput
				procomplaintDao.save(procomplaint);
				
				//2.录像的迁移
				Records record = recordsDao.get(recordid);
				if (record != null && businessorder != null) {
					Prosaverecord proSaveRecord = new Prosaverecord();
					proSaveRecord.setClientId(clientInput.getId());// 业务单ID
					proSaveRecord.setStateType(Constants.RecordType.RECORD_COMPLAINNT);// 投诉录像
					String returnStr = saveProsaveRecord(record, businessorder, clientInput, proSaveRecord);// 依据record，businessorder和clientinput生成prosaverecord
					if(returnStr!=null){
						ajaxData.setMsg(returnStr);
					}
					procomplaint.setnRecordId(proSaveRecord.getId());
					// 记录操作日志
					String note = "关联投诉单，录像名：" + record.getStrRecordName() + "，业务单流水号：" + businessorder.getNproId() + "，客户姓名：" + businessorder.getCustomerName();
					collLogDao.writeOperLog(OperLogType.OperRel, note);
				}
				clientInput.setStatus(Constants.BusiComplainStatus.COMPLAINED);// 业务单转为投诉单
			}
		} else {
			ajaxData.setFailureMsg("没有可以关联的签约单,请先录制签约单!");
		}
		return ajaxData;
	}
	
	/*
	 * 关联历史业务单并存储
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#saveHistoryOrder(java.lang.Integer, java.lang.Integer)
	 */
	@Override
	public AjaxData saveHistoryOrder(Integer recordsid, Integer clientinputid) {
		AjaxData ajaxData = new AjaxData(false);
		Clientinput clientInput = clientInputDao.get(clientinputid);
		clientInput.setLockClient(LockClient.NO_AUDIT);
		clientInputDao.save(clientInput);
		Records records = recordsDao.get(recordsid);
		Prosaverecord proSaveRecord = new Prosaverecord();
		proSaveRecord.setClientId(clientInput.getId());// 业务单ID
		proSaveRecord.setStateType(Constants.RecordType.RECORD_SIGN);// 关联历史录像默认签约录像
		String returnStr = saveProsaveRecord(records, null, clientInput, proSaveRecord);// 依据record和businessorder生成prosaverecord
		if(returnStr!=null){
			ajaxData.setMsg(returnStr);
		}
		// 记录操作日志
		String note = "关联历史业务单，录像名：" + records.getStrRecordName() + "，业务单流水号：" + clientInput.getNproId() + "，客户姓名：" + clientInput.getName();
		collLogDao.writeOperLog(OperLogType.OperRel, note);
		return ajaxData;
	}
	
	/* 
	 * 数据采集中的打印
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#getPrintOrderInfo()
	 */
	@Override
	public PrintOrderInfo getPrintInfo(Integer recordid) {
		Records records = recordsDao.get(recordid);
		Prosaverecord prosaveRecord = proSaveRecordDao.get(records.getnHistoryRecordId());
		return getPrint(prosaveRecord);
	}
	
	/*
	 * 业务关联中的打印
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#getPermPrintOrderInfo(int)
	 */
    @Override
    public PrintOrderInfo getPermPrintInfo(int id) {
		Prosaverecord prosaveRecord = proSaveRecordDao.get(id);
		return getPrint(prosaveRecord);
    }
    
	//*************************业务关联模块*********************************//
	/* 
	 * 获取录像信息和业务信息分页数据
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#getRecordsDataPage(int, int, java.lang.String)
	 */
	@Override
	public Page getRecordsDataPage(int limit, int start, RealOrdersInfo realOrdersInfo) {
		return businessOrderDao.getRecordsDataPage(limit, start, realOrdersInfo);
	}
	
	/*
	 * 获取编辑业务单信息
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#getOrderEditInfo(int)
	 */
	public Businessorder getOrderEditInfo(int id, Integer recordId){
		Clientinput clientinput = clientInputDao.get(id);
		Businessorder businessorder = new Businessorder();
		//关联投诉了的签约单的状态也是投诉
		if (Constants.ClientStatus.CLIENT_COMPLAINNT == clientinput.getStatus()) {
			Procomplaint procomplaint = procomplaintDao.findUniqueBy("nRecordId", recordId);
			if (procomplaint != null) {
				businessorder.setComplainTime(procomplaint.getCompTime());
				businessorder.setCompReason(procomplaint.getCompReason());
			}
		}
		try {
	        BeanUtils.copyNotNullProperties(businessorder, clientinput);
        } catch (Exception e) {
	        throw new ExpectedException("", "拷贝bean出错");
        }
		businessorder.setStrEndDate(clientinput.getStrEndDate());// 只有这样拷贝前端日期格式才正确
		// 拷贝名称不一致的字段
		businessorder.setCustomerName(clientinput.getName());// 客户姓名
		businessorder.setNuserId(clientinput.getInputUser());
		return businessorder;
	}
	
	/*
	 * 编辑业务单
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#editOrderInfo(int, com.hikvision.finance.fms.model.Businessorder)
	 */
	public AjaxData editOrderInfo(int clientId, Businessorder businessorder, Integer recordId) {
		AjaxData ajaxData = new AjaxData(false);
		//校验提交签约单的时候 产品是否过期  禁用等
		Integer productId = businessorder.getProductId();
		if (productId != null) {
			List<Product> products = productService.getPermitProductList();
			Product product = productDao.get(productId);
			if (products == null || product == null) {
				throw new ExpectedException("", "获取产品或产品权限出错");
			} else {// 否则判断是否包含这个产品，未包含则抛出异常
				boolean flag = false;
				for (Product singleProduct : products) {
					if (singleProduct.getId().equals(product.getId())) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					throw new ExpectedException("", "该产品无法被使用");
				}
			}
		} else {
			throw new ExpectedException("", "产品id为空,未获取到产品信息");
		}
		Clientinput clientinput = clientInputDao.get(clientId);
		if (checkModifyClientInput(clientinput, businessorder)){
			clientinput.setLockClient(Constants.LockClient.NO_AUDIT);
		}
		if (!clientinput.getProductId().equals(businessorder.getProductId())) {// 产品不同
			clientinput.setDelaydays(0);// 延迟天数恢复成0
		}
		try {
	        BeanUtils.copyNotNullProperties(clientinput, businessorder);
        } catch (Exception e) {
        	throw new ExpectedException("", "拷贝bean出错");
        }
		clientinput.setName(businessorder.getCustomerName());// 客户姓名
		clientinput.setStrEndDate(businessorder.getStrEndDate());// 可能为空，需要重新设置产品到期时间
		clientinput.setGuaranteeYears(businessorder.getGuaranteeYears());//可能为空，需要重新设置保障周期
		clientinput.setGuaranteeType(businessorder.getGuaranteeType());
		clientinput.setProductSaveMonth(businessorder.getProductSaveMonth());//到期后保存时长
		clientinput.setGuaranteeType(businessorder.getGuaranteeType());
		Date videoEndDate = getVideoEndDate(businessorder);
		clientinput.setStrVideoEndDate(videoEndDate);// 视频到期日
		Region region = regionDao.get(businessorder.getInputRegionId());//录单中交易区域ID
		clientinput.setInputRegionName(region.getStrName());// 区域名称
		clientinput.setInputRegionCode(region.getStrCode());// 区域代码
		// 记录操作日志.
		
		Procomplaint procomplaint = procomplaintDao.findUniqueBy("nRecordId", recordId);
		if (procomplaint != null) {
			procomplaint.setCompReason(businessorder.getCompReason());
			procomplaint.setCompTime(businessorder.getComplainTime());
			procomplaintDao.update(procomplaint);
		}
		String note = "业务单流水号：" + businessorder.getNproId();
		collLogDao.writeOperLog(OperLogType.OperEditOrder, note);
		return ajaxData;
	}
	
	/**
	 * 检查是否修改了业务单信息
	 * @author zhoujiajun 2016年11月17日 下午6:54:01
	 * @param clientinput
	 * @param businessorder
	 * @return
	 */
	private boolean checkModifyClientInput(Clientinput clientinput, Businessorder businessorder) {
		boolean isModify = false;
		if (clientinput.getInputRegionId() != businessorder.getInputRegionId()) {
			isModify = true;
		}
		if (clientinput.getProductId() != businessorder.getProductId()) {
			isModify = true;
		}
		if (clientinput.getName() != null && !clientinput.getName().equals(businessorder.getCustomerName())) {
			isModify = true;
		}
		if (clientinput.getCreditId() != businessorder.getCreditId()) {
			isModify = true;
		}
		if (clientinput.getCreditCode() != null && !clientinput.getCreditCode().equals(businessorder.getCreditCode())) {
			isModify = true;
		}
		if (clientinput.getStrCreditPhone() != null && !clientinput.getStrCreditPhone().equals(businessorder.getStrCreditPhone())) {
			isModify = true;
		}
		if (clientinput.getStrBankNo() != null && !clientinput.getStrBankNo().equals(businessorder.getStrBankNo())) {
			isModify = true;
		}
		if (clientinput.getStrBuyMoney() != null && !clientinput.getStrBuyMoney().equals(businessorder.getStrBuyMoney())) {
			isModify = true;
		}
		if (clientinput.getBusinessTime() != null && businessorder.getBusinessTime() != null 
				&& clientinput.getBusinessTime().getTime() != businessorder.getBusinessTime().getTime()) {
			isModify = true;
		}
		if (clientinput.getStrRemarks() != null && !clientinput.getStrRemarks().equals(businessorder.getStrRemarks())) {
			isModify = true;
		}
		return isModify;
	}

	/**
	 * 把business中的数据放到clientinput中
	 * @author jinchenwu 2016年5月10日 上午10:14:19
	 * @param businessorder
	 * @return
	 */
	private Clientinput saveClientinput(Businessorder businessorder){
		Clientinput clientInput = clientInputDao.findUniqueBy("nproId", businessorder.getNproId());
		if(clientInput==null){// 业务单不存在，则新建
			clientInput = new Clientinput();
			clientInput.setNproId(businessorder.getNproId());//业务单号--业务流水号
			clientInput.setInputTime(new Date());//关联业务单时间
			clientInput.setBusinessTime(businessorder.getBusinessTime());//业务时间
			clientInput.setInputRegionId(businessorder.getInputRegionId());//理财系统建单区域号
			Region region = regionDao.get(businessorder.getInputRegionId());//录单中交易区域ID
			clientInput.setInputRegionCode(region.getStrCode());// 交易区域编码
			clientInput.setInputRegionName(region.getStrName());// 交易区域名称
			clientInput.setStatus(businessorder.getStatus());// 业务类型
			clientInput.setNisRead(Constants.BusiReadStatus.NOT_READ);
			clientInput.setNisPay(Constants.VideoDueStatus.NOT_DUE);
			clientInput.setName(businessorder.getCustomerName());//客户姓名
			clientInput.setCreditCode(businessorder.getCreditCode());//证件号
			clientInput.setCreditId(businessorder.getCreditId());//证件类型Id
			clientInput.setInputUser(businessorder.getNuserId());//理财系统建单用户Id				
			Users users = usersDao.get(businessorder.getNuserId());
			clientInput.setProName(users.getStrCode());// 业务员姓名
			clientInput.setProUserNo(users.getUserNo());// 业务员工号
			clientInput.setProductTypeId(businessorder.getProductTypeId());// 产品类型Id
			clientInput.setProductTypeName(businessorder.getProductTypeName());// 产品类型名称
			clientInput.setProductId(businessorder.getProductId());//产品Id
			clientInput.setProductCode(businessorder.getProductCode());//产品名称
			clientInput.setProductName(businessorder.getProductName());//产品代码
			clientInput.setStrEndDate(businessorder.getStrEndDate());//产品到期日
			clientInput.setGuaranteeType(businessorder.getGuaranteeType());//保障期限
			clientInput.setGuaranteeYears(businessorder.getGuaranteeYears());//保障期限
			clientInput.setProductSaveType(businessorder.getProductSaveType());//视频保存周期
			clientInput.setProductSaveMonth(businessorder.getProductSaveMonth());//视频保存周期
			clientInput.setStrGrade(businessorder.getStrGrade());//产品发行机构
			clientInput.setStrRisklevel(businessorder.getStrRisklevel());//产品风险等级
			clientInput.setStrBankNo(businessorder.getStrBankNo());//客户银行卡号
			clientInput.setStrCreditPhone(businessorder.getStrCreditPhone());//客户电话				
			clientInput.setStrBuyMoney(businessorder.getStrBuyMoney());//购买金额
			clientInput.setStrRemarks(businessorder.getStrRemarks());//业务单备注
			clientInput.setDelaydays(0);// 设置初始延期天数为0
			Date videoEndDate = getVideoEndDate(businessorder);
			clientInput.setStrVideoEndDate(videoEndDate);//视频到期日
			clientInput.setLockClient(Constants.LockClient.NO_AUDIT);//待审批
			//保存签约单
			clientInputDao.save(clientInput);
		}
		return clientInput;
	}
	
	/**
	 * 将录像数据从records迁移到prosaverecord
	 * @author jinchenwu 2016年5月9日 下午6:04:21
	 */
	private String saveProsaveRecord(Records records, Businessorder businessorder, Clientinput clientinput, Prosaverecord proSaveRecord){
		String returnStr = null;
		proSaveRecord.setClientId(clientinput.getId());// 业务单ID
		if (businessorder != null && (new Integer(Constants.BusiComplainStatus.NOT_COMPLAIN)).equals(businessorder.getStatus())) {// 签约单逻辑
			proSaveRecord.setRegionId(businessorder.getInputRegionId());// 录单中交易区域ID
		} else {// 投诉单，历史业务单
			proSaveRecord.setRegionId(clientinput.getInputRegionId());// 原业务单区域ID
		}
		proSaveRecord.setUserId(records.getNuserId());// 员工的id
		proSaveRecord.setFileName(records.getStrRecordName());// 文件名
		proSaveRecord.setStartTime(records.getStartTime());// 开始时间
		proSaveRecord.setEndTime(records.getEndTime());// 结束时间
		proSaveRecord.setUpload(ProSaveRecordEnum.VideoUploadStatus.NOT_UPLOAD.getNType());// 录像上传状态
		proSaveRecord.setRecordCheck(ProSaveRecordEnum.RecordCheckStatus.NOT_CHECK.getNType());// 合规检查
		proSaveRecord.setScreenshotSrc(records.getScreenshotSrc());// 截图路径
		if (Constants.Records.recordFileType.DVR_RECORD==records.getNrecordFileType()) {// IPC-CVR模式
			String isNonCenterStorage = FmsPropertyPlaceholder.getProperty("nonCenterStorage");
			if (StringUtils.isEmpty(isNonCenterStorage)) {
				throw new ExpectedException("", "未获取到中心存储的状态");
			}
			if (!"true".equals(isNonCenterStorage)) {//有中心存储
				List<Cvrassociatemapping> cvrassociatemapping = cvrassociatemappingDao.findBy("ncollId", records.getNrepairDevId());
				if (cvrassociatemapping.size() == 0) {
					returnStr = "DVR/NVR没有关联CVR存储池，会导致录像无法上传，请联系管理员!";
				}
			}
			proSaveRecord.setStreamNo(records.getStreamNo());// 通道在CVR上的唯一标识
			proSaveRecord.setDevId(records.getNrepairDevId());// DVR的ID
			proSaveRecord.setDevChannel(records.getNcollectChan());// DVR的通道号
		} else if (new Integer(Constants.Records.recordFileType.LOCAL_RECORD).equals(records.getNrecordFileType())) {// USB-KMS模式
			proSaveRecord.setStrUrl(records.getStrUrl());// USB本地文件路径
			proSaveRecord.setNfileSize(records.getNfileSize());// 录像大小，只有USB-KMS模式有
			proSaveRecord.setDevIp(records.getStrIp());
		} else {
			throw new ExpectedException("", "录像文件类型错误");
		}
		Integer nHistoryRecordId = (Integer)proSaveRecordDao.save(proSaveRecord);
		records.setnHistoryRecordId(nHistoryRecordId);
		records.setNrecordStatus(RecordStatus.CONTRACTED);// 录像状态修改为已关联
		return returnStr;
	}
	
	/**
	 * 获取打印信息的私有方法
	 * @author jinchenwu 2016年5月12日 下午1:57:19
	 * @param prosaveRecord
	 * @return
	 */
	private PrintOrderInfo getPrint(Prosaverecord prosaveRecord) {
		Clientinput clientinput = clientInputDao.get(prosaveRecord.getClientId());
		PrintOrderInfo printOrderInfo = new PrintOrderInfo();
		try {
	        BeanUtils.copyNotNullProperties(printOrderInfo, clientinput);
        } catch (Exception e) {
	        throw new ExpectedException("", "拷贝bean出错");
        }
		Product product = productDao.get(clientinput.getProductId());
		Producttype producttype = productTypeDao.get(product.getPid());
		Paramdictionary paramdictionary = cardDao.get(clientinput.getCreditId());
		printOrderInfo.setCreditname(paramdictionary.getStrName());// 证件类型名称
		printOrderInfo.setProductType(producttype.getStrName());// 产品类型名称
		printOrderInfo.setProsaverecordId(prosaveRecord.getId());//录像Id
		if (prosaveRecord.getScreenshotSrc() != null) {
			String[] screenshotSrc = prosaveRecord.getScreenshotSrc().split(",");
			for (int i = 0; i < screenshotSrc.length; i++) {
				if(i==0){
					printOrderInfo.setPictureStrart(screenshotSrc[i]);
				}
				if(i==1){
					printOrderInfo.setPictureEnd(screenshotSrc[i]);
				}
			}
		}
		return printOrderInfo;
	}
	
	/**
	 * 计算视频到期日
	 * @author jinchenwu 2016年5月17日 上午9:55:12
	 * @return
	 */
	private Date getVideoEndDate(Businessorder businessorder){
		Date videoEndDate = new Date();
		if (businessorder.getStrEndDate() != null) {// 产品到期日不为空，则以产品到期日为起点
			videoEndDate = businessorder.getStrEndDate();
		} else if (businessorder.getGuaranteeYears() != null && businessorder.getGuaranteeYears() > 0) {// 保证年期不为空，则以签约时间为起点
			switch (businessorder.getGuaranteeType()) {
				case ProductKeepType.YEAR:
					videoEndDate = DateUtils.addYears(businessorder.getBusinessTime(), businessorder.getGuaranteeYears());// 加上保障年期
					break;
				case ProductKeepType.MONTH:
					videoEndDate = DateUtils.addMonths(businessorder.getBusinessTime(), businessorder.getGuaranteeYears());// 加上保障年期
					break;
				case ProductKeepType.DAY:
					videoEndDate = DateUtils.addDays(businessorder.getBusinessTime(), businessorder.getGuaranteeYears());// 加上保障年期
					break;	
			}
		}
		if (businessorder.getProductSaveMonth() != null && businessorder.getProductSaveMonth() > 0) {// 视频保存周期不为空，则加上视频保存周期
			switch (businessorder.getProductSaveType()) {
				case ProductKeepType.YEAR:
					videoEndDate = DateUtils.addYears(videoEndDate, businessorder.getProductSaveMonth());
					break;
				case ProductKeepType.MONTH:
					videoEndDate = DateUtils.addMonths(videoEndDate, businessorder.getProductSaveMonth());
					break;
				case ProductKeepType.DAY:
					videoEndDate = DateUtils.addDays(videoEndDate, businessorder.getProductSaveMonth());
					break;
			}
		}
		return videoEndDate;
	}
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#getOrderForTip()
     */
    @Override
    public Businessorder getOrderForTip() {
		Businessorder businessorder = businessOrderDao.findUniqueBy("nuserId", SessionUtil.getUserSession().getUserId());
		return businessorder;
    }
	/* (non-Javadoc)
	 * @see com.hikvision.finance.extend.service.IBaseService#deleteByIds(java.io.Serializable[])
	 */
	@Override
	public void deleteByIds(Serializable[] paramArrayOfSerializable) {
		
	}
	
	@Override
	public AjaxData updateProsaverecord(PrintOrderInfo printOrderInfo){
		AjaxData ajaxData = new AjaxData(false);
		Prosaverecord dbProsaverecord = proSaveRecordDao.get(printOrderInfo.getProsaverecordId());
		if (StringUtils.isEmpty(dbProsaverecord.getScreenshotSrc())) {//补录，没有图片
			if (StringUtils.isNotEmpty(printOrderInfo.getPictureStrart())) {
				dbProsaverecord.setScreenshotSrc(printOrderInfo.getPictureStrart().replaceAll("\\\\","\\\\\\\\") + "," + " ");//加上了" "，否则加完第一张图片后，加第二张时arr[1]获取不到，故加" "
			} else if (StringUtils.isNotEmpty(printOrderInfo.getPictureEnd())) {
				dbProsaverecord.setScreenshotSrc(printOrderInfo.getPictureEnd().replaceAll("\\\\","\\\\\\\\"));
			}
		} else {
			try {
				String[] arr = dbProsaverecord.getScreenshotSrc().split(",");
				if (null == arr[0] && null != arr[1]) {
					dbProsaverecord.setScreenshotSrc(URLDecoder.decode(printOrderInfo.getPictureStrart(), "utf-8").replaceAll("\\\\","\\\\\\\\") + "," + arr[1]);
				} else if (null != arr[0] && null == arr[1]) {
					dbProsaverecord.setScreenshotSrc(arr[0] + "," + URLDecoder.decode(printOrderInfo.getPictureEnd(), "utf-8").replaceAll("\\\\","\\\\\\\\"));
				} else if (null != arr[0] && null != arr[1]) {
					if (StringUtils.isNotEmpty(printOrderInfo.getPictureStrart())) {
						dbProsaverecord.setScreenshotSrc(URLDecoder.decode(printOrderInfo.getPictureStrart(), "utf-8").replaceAll("\\\\","\\\\\\\\") + "," + arr[1]);
					} else if (StringUtils.isNotEmpty(printOrderInfo.getPictureEnd())) {
						dbProsaverecord.setScreenshotSrc(arr[0] + URLDecoder.decode(printOrderInfo.getPictureEnd(), "utf-8").replaceAll("\\\\","\\\\\\\\"));
					}		
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return ajaxData;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.BaseServiceImpl#getDao()
     */
    @Override
    protected IBusinessOrderDao getDao() {
	    return businessOrderDao;
    }
	
	// **********************setter--getter*****************************//

	public IProductDao getProductDao() {
		return productDao;
	}
	
	public void setProductDao(IProductDao productDao) {
		this.productDao = productDao;
	}
	
	public IProductService getProductService() {
		return productService;
	}
	
	public void setProductService(IProductService productService) {
		this.productService = productService;
	}
	
	public ICardDao getCardDao() {
		return cardDao;
	}
	
	public void setCardDao(ICardDao cardDao) {
		this.cardDao = cardDao;
	}
	
	public IProSaveRecordDao getProSaveRecordDao() {
		return proSaveRecordDao;
	}
	
	public void setProSaveRecordDao(IProSaveRecordDao proSaveRecordDao) {
		this.proSaveRecordDao = proSaveRecordDao;
	}
	
	public ICvrassociatemappingDao getCvrassociatemappingDao() {
		return cvrassociatemappingDao;
	}
	
	public void setCvrassociatemappingDao(ICvrassociatemappingDao cvrassociatemappingDao) {
		this.cvrassociatemappingDao = cvrassociatemappingDao;
	}
	
	public IUsersDao getUsersDao() {
		return usersDao;
	}
	
	public void setUsersDao(IUsersDao usersDao) {
		this.usersDao = usersDao;
	}
	
	public IRegionDao getRegionDao() {
		return regionDao;
	}
	
	public void setRegionDao(IRegionDao regionDao) {
		this.regionDao = regionDao;
	}
	
	public IProductTypeDao getProductTypeDao() {
		return productTypeDao;
	}
	
	public void setProductTypeDao(IProductTypeDao productTypeDao) {
		this.productTypeDao = productTypeDao;
	}
	
	public ICollLogDao getCollLogDao() {
		return collLogDao;
	}
	
	public void setCollLogDao(ICollLogDao collLogDao) {
		this.collLogDao = collLogDao;
	}
	
	public ISysconfigDao getSysconfigDao() {
		return sysconfigDao;
	}
	
	public void setSysconfigDao(ISysconfigDao sysconfigDao) {
		this.sysconfigDao = sysconfigDao;
	}
	
	public IProcomplaintDao getProcomplaintDao() {
		return procomplaintDao;
	}
	
	public void setProcomplaintDao(IProcomplaintDao procomplaintDao) {
		this.procomplaintDao = procomplaintDao;
	}
	
	public IRecordsDao getRecordsDao() {
		return recordsDao;
	}
	
	public void setRecordsDao(IRecordsDao recordsDao) {
		this.recordsDao = recordsDao;
	}
	
	public IBusinessOrderDao getBusinessOrderDao() {
		return businessOrderDao;
	}
	
	public void setBusinessOrderDao(IBusinessOrderDao businessOrderDao) {
		this.businessOrderDao = businessOrderDao;
	}
	
	public IClientInputDao getClientInputDao() {
		return clientInputDao;
	}
	
	public void setClientInputDao(IClientInputDao clientInputDao) {
		this.clientInputDao = clientInputDao;
	}
	
}
