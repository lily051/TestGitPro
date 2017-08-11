/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-15 上午10:33:36
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.contact.action;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.Random;

import com.google.common.collect.Maps;
import com.hikvision.finance.fms.model.Businessorder;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.model.Paramdictionary;
import com.hikvision.finance.fms.model.Product;
import com.hikvision.finance.fms.model.Records;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.product.dao.IProductDao;
import com.hikvision.finance.fms.modules.product.service.IProductService;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.sale.dao.IClientInputDao;
import com.hikvision.finance.fms.modules.sysconfig.dao.ISysconfigDao;
import com.hikvision.finance.fms.modules.user.dao.IUsersDao;
import com.hikvision.finance.fms.webdevice.contact.dao.IBusinessOrderDao;
import com.hikvision.finance.fms.webdevice.contact.dto.BasicProductInfo;
import com.hikvision.finance.fms.webdevice.contact.dto.OrderEnableInfo;
import com.hikvision.finance.fms.webdevice.contact.dto.PrintOrderInfo;
import com.hikvision.finance.fms.webdevice.contact.dto.ProductCodeInfo;
import com.hikvision.finance.fms.webdevice.contact.dto.RealOrdersInfo;
import com.hikvision.finance.fms.webdevice.contact.service.IContactService;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.www.action.BaseAction;

/**
 * <p>业务单数据action</p>
 * @author xujiangfei 2016-4-15 上午10:33:36
 * @version V1.0 
 */
public class ContactAction extends BaseAction<Businessorder>{

	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = 5583378064317831632L;
	private IContactService contactService;
	private IRegionDao regionDao;
	private IUsersDao usersDao;
	private ISysconfigDao sysconfigDao;
	private Users user;
	private Businessorder businessorder;
	private Product product;
	private Clientinput clientinput;
	private PrintOrderInfo printOrderInfo;
	private RealOrdersInfo realOrdersInfo;
	private Records records;
	private Region region;
	private String username;
	private OrderEnableInfo orderEnableInfo;
	private IProductDao productDao;
	private IClientInputDao clientInputDao; 
	private IBusinessOrderDao businessOrderDao;
	private IProductService productService;
	private String tip;//话术模板
	private Integer flag;//显示话术模板
	private Integer type;//区分历史业务单(签约单获取到的业务单type=1, 投诉单获取到的业务单type=2)
	private Integer recordId;//录像id
	
	/**
	 * 跳转到话术模版页面 
	 * @author jinxindong 2016年5月25日 上午10:43:06
	 * @return
	 */
	public String toOpenPageProductTree(){
        //查询临时业务单表
		businessorder = contactService.getOrderForTip();
		if(businessorder!=null){//根据产品id查询话术模板信息
			Integer productId = null;
			//判断是投诉单还是签约单,投诉单去历史业务单表取
			if(businessorder.getStatus()==1){
				if(businessorder.getClientId()==null){
					throw new ExpectedException("", "未获取到历史业务单id"); 
				}	
				productId = clientInputDao.get(businessorder.getClientId()).getProductId();
			}else{
				if(businessorder.getProductId()==null){
					throw new ExpectedException("", "未获取到产品id"); 
				}	
				productId = businessorder.getProductId();
			}
			tip = productDao.get(productId).getWordTemplate();
			flag=1;//不去取树的
		}else{
			flag=0;//取树的
		}
		operPage = "/web/webDevice/contact/dialogs/productTree.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 获取osd信息
	 * @author jinchenwu 2016年5月10日 下午1:42:34
	 * @return
	 */
	public String getosdInfo() {
		businessorder = contactService.getOrder();
		if (businessorder != null) {
			user = usersDao.get(businessorder.getNuserId());
		}
		ajaxData.put("businessorder", businessorder);
		ajaxData.put("user", user);
		return AJAX;
	}
	
	/**
	 * 获取话术模板信息
	 * @author jinchenwu 2016年4月23日 上午10:29:37
	 * @return
	 */
	public String getWordTemplate(){
		String wordTemplate = contactService.getWordTemplate(id.intValue());
		ajaxData.put("wordTemplate", wordTemplate);
		return AJAX;
	}
	
	//*************************录单操作*********************************//
	/**
	 * 打开签约单录单页面
	 * @author xujiangfei 2016-4-17 下午6:46:21
	 * @return
	 */
	public String toOpenPageSignOrder() {
		// 查出该理财经理 未关联 的签约单，如果有返回businessorder，没有返回null
		businessorder = contactService.getSignOrder();
		if (businessorder.getInputRegionId() != null) {
			region = regionDao.get(businessorder.getInputRegionId());
		}
		if (businessorder.getNuserId() != null) {
			user = usersDao.get(businessorder.getNuserId());
		}
		
		String proId = "";
		int rannum = 0;
		SimpleDateFormat simpleDateFormat = null;
		simpleDateFormat = new SimpleDateFormat("yyyyMMddHHmmss");
		Date date = new Date();
		String str = simpleDateFormat.format(date);
		Random random = new Random();
		for (int i = 0; i <= 300; i++) {// 循环三百次取到
			rannum = (int)(random.nextDouble() * (99999 - 10000 + 1)) + 10000;// 获取5位随机数
			proId =  str + rannum;
			// 去数据库查询 业务单是否重复
			List<Clientinput> clList = clientInputDao.findBy("nproId", proId);
			List<Businessorder> boList = businessOrderDao.findBy("nproId", proId);
			if ((clList == null || clList.size() < 1) && (boList == null || boList.size() < 1)) {
				break;
			}
			if (i == 300) {
				throw new ExpectedException("", "生成业务单号失败");
			}
		}
		if(businessorder.getProductId()!=null){
			//产品信息取实时的，不取业务单的 2016-06-28 by jinxd
			getNewProInfo(businessorder);	
		}
		businessorder.setNproId(proId);
		businessorder.setBusinessTime(new Date());
		//因为signOrder.jsp在编辑业务单复用, 在填写签约单的时候不能出现在投诉信息, 所以置空
		businessorder.setComplainTime(null);
		businessorder.setCompReason(null);
		orderEnableInfo = contactService.getOrderEnable();
		operPage = "/web/webDevice/business/dialogs/signOrder.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 产品信息取实时的，不取数据库
	 * @author jinxindong 2016年6月28日 下午8:01:55
	 * @param businessorder
	 */
	private void getNewProInfo(Businessorder businessorder){
		businessorder.setProductCode(null);
		businessorder.setProductName(null);
		businessorder.setProductSaveMonth(null);
		businessorder.setStrGrade(null);
		businessorder.setStrRisklevel(null);
		businessorder.setStrEndDate(null);
		businessorder.setGuaranteeYears(null);
		
		Map<Integer,Product>  proMap = Maps.newHashMap(); 
		List<ProductCodeInfo> permitProductCodeList = contactService.getPermitProductCodeList();
		for(ProductCodeInfo info :permitProductCodeList){
			proMap.put(info.getValue(), info.getProduct());
		}
		Integer productId = businessorder.getProductId();
		Product product = productDao.get(productId);
		Product pro = proMap.get(productId);
		if (product != null && pro!=null) {			
			businessorder.setProductCode(product.getStrCode());
			businessorder.setProductName(product.getStrName());
			businessorder.setProductSaveMonth(product.getSaveYears());
			businessorder.setStrGrade(product.getStrGrade());
			businessorder.setStrRisklevel(product.getStrRisklevel());
			businessorder.setStrEndDate(product.getStrEndDate());
			businessorder.setGuaranteeYears(product.getGuaranteeYears());			
		}
	}
	
	/**
	 * 理财经理获取有权限的产品编码
	 * @author jinchenwu 2016年4月23日 上午10:30:45
	 * @return
	 */
	public String getPermitProductCodeList() {
		List<ProductCodeInfo> permitProductCodeList = contactService.getPermitProductCodeList();
		ajaxData.put("permitProductCodeList", permitProductCodeList);
		return AJAX;
	}
	
	/**
	 * 理财经理选择产品编码后获取基本产品信息
	 * @author jinchenwu 2016年4月23日 上午11:38:15
	 * @return
	 */
	public String getBasicProductbyId(){
		BasicProductInfo basicProductInfo = contactService.getBasicProductbyId(id.intValue());
		ajaxData.put("basicProductInfo", basicProductInfo);
		return AJAX;
	}
	
	/**
	 * 理财经理获取证件类型
	 * @author jinchenwu 2016年4月23日 下午12:50:58
	 * @return
	 */
	public String getCardList(){
		List<Paramdictionary> cardList = contactService.getCardList();
		ajaxData.put("cardList", cardList);
		return AJAX;
	} 
	
	/**
	 * 添加/编辑签约单
	 * @author xujiangfei 2016-4-18 下午4:08:40
	 * @return
	 */
	public String addSignOrder(){
		ajaxData = contactService.addSignOrder(businessorder);
		return AJAX;
	}
	
	/**
	 * 录入投诉单和选择历史业务单时获取历史业务单
	 * @author xujiangfei 2016-4-19 下午4:42:17
	 * @return
	 */
	public String getDataPage() {
		page = contactService.getDataPage(limit,start,clientinput, type);
		return PAGE;
	}
	
	/**
	 * 添加投诉单
	 * @author xujiangfei 2016-4-21 下午4:42:17
	 * @return
	 */
	public String addComplainOrder() {
		ajaxData = contactService.addcomplainOrder(businessorder);
		return AJAX;
	}
	
	//*************************关联操作*********************************//
	/**
	 * 打开关联页面（新签约单or投诉单）
	 * @author xujiangfei 2016-4-22 上午10:00:17
	 * @return
	 */
	public String toOpenPageViewSignOrder() {
		// 查出该理财经理 未关联 的签约单，如果有返回businessorder，没有返回null
		businessorder = contactService.getOrder();
		if(businessorder!=null){// 没有单子的时候是null
			if (businessorder.getInputRegionId() != null) {
				region = regionDao.get(businessorder.getInputRegionId());
			}
			if (businessorder.getNuserId() != null) {
				user = usersDao.get(businessorder.getNuserId());
			}
		}
		
		//产品信息取实时的，不取业务单的 2016-06-28 by jinxd
		//getNewProInfo(businessorder);
		
		orderEnableInfo = contactService.getOrderEnable();
		operPage = "/web/webDevice/business/dialogs/viewSignOrder.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 关联业务单并存储（新签约单or投诉单）
	 * @author xujiangfei 2016-4-22 下午6:39:41
	 * @return
	 */
	public String signRelaRecords(){
		ajaxData = contactService.saveRelateRecords(records.getId());
		return AJAX;
	}
	
	/**
	 * 关联历史业务单并存储
	 * @author xujiangfei 2016-4-24 下午6:23:25
	 * @return
	 */
	public String oldSignRelaRecords(){
		ajaxData = contactService.saveHistoryOrder(records.getId(),clientinput.getId());
		return AJAX;
	}
	
	/**
	 * 数据采集中的打印
	 * @author jinchenwu 2016年5月12日 上午11:45:14
	 * @return
	 */
	public String getPrintInfo(){
		printOrderInfo = contactService.getPrintInfo(id.intValue());
		operPage = "/web/webDevice/business/dialogs/printBusiness.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 业务关联中的打印
	 * @author jinchenwu 2016年5月12日 上午11:45:14
	 * @return
	 */
	public String getPermPrintInfo(){
		printOrderInfo = contactService.getPermPrintInfo(id.intValue());
		operPage = "/web/webDevice/business/dialogs/printBusiness.jsp";
		return DISPATCHER;
	}
	
	//*************************业务关联模块*********************************//
	/**
	 * 获取录像信息和业务信息分页数据
	 * @author jinchenwu 2016年5月10日 下午10:27:51
	 * @return
	 */
	public String getRecordsDataPage(){
		page = contactService.getRecordsDataPage(limit,start,realOrdersInfo);//获得录像分页数据
		return PAGE;
	}
	
	/**
	 * 跳转到编辑业务单页面
	 * @author xujiangfei 2016-4-27 下午7:30:54
	 * @return
	 */
	public String toOpenPageEditOrder(){
		businessorder = contactService.getOrderEditInfo(id.intValue(), recordId);
		if (businessorder.getInputRegionId() != null) {
			region = regionDao.get(businessorder.getInputRegionId());
		}
		if (businessorder.getNuserId() != null) {
			user = usersDao.get(businessorder.getNuserId());
		}
		orderEnableInfo = contactService.getOrderEnable();
		operPage = "/web/webDevice/business/dialogs/signOrder.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 编辑业务单
	 * @author xujiangfei 2016-4-27 下午7:26:55
	 * @return
	 */
	public String editBusinessOrder(){
		ajaxData = contactService.editOrderInfo(id.intValue(), businessorder, recordId);
		return AJAX;
	}
	
	/**
	 * 编辑历史录像记录
	 * @author zhengquanwu 2016年10月20日 下午8:33:28
	 */
	public String updateProsaverecord() {
		ajaxData = contactService.updateProsaverecord(printOrderInfo);
		return AJAX;
	}
	
	/**
	 * 获取业务单详情页面
	 * @author qindeng 2016年9月13日 下午3:21:29
	 * @return
	 */
	public String toOpenPageOrderDetial(){
		businessorder = contactService.getOrderEditInfo(id.intValue(), recordId);
		if (businessorder.getInputRegionId() != null) {
			region = regionDao.get(businessorder.getInputRegionId());
		}
		if (businessorder.getNuserId() != null) {
			user = usersDao.get(businessorder.getNuserId());
		}
		orderEnableInfo = contactService.getOrderEnable();
		operPage = "/web/webDevice/business/dialogs/signOrderDetail.jsp";
		return DISPATCHER;
	}
	
	//*************************setter--getter*********************************//
	public Businessorder getBusinessorder() {
		return businessorder;
	}

	public void setBusinessorder(Businessorder businessorder) {
		this.businessorder = businessorder;
	}
	
	public IContactService getContactService() {
		return contactService;
	}
	
	public void setContactService(IContactService contactService) {
		this.contactService = contactService;
	}
	
	public Clientinput getClientinput() {
		return clientinput;
	}
	
	public void setClientinput(Clientinput clientinput) {
		this.clientinput = clientinput;
	}

	public Records getRecords() {
		return records;
	}

	public void setRecords(Records records) {
		this.records = records;
	}

	public String getUsername() {
		return username;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public PrintOrderInfo getPrintOrderInfo() {
		return printOrderInfo;
	}
	
	public void setPrintOrderInfo(PrintOrderInfo printOrderInfo) {
		this.printOrderInfo = printOrderInfo;
	}
	
	public RealOrdersInfo getRealOrdersInfo() {
		return realOrdersInfo;
	}
	
	public void setRealOrdersInfo(RealOrdersInfo realOrdersInfo) {
		this.realOrdersInfo = realOrdersInfo;
	}
	
	public IRegionDao getRegionDao() {
		return regionDao;
	}
	
	public void setRegionDao(IRegionDao regionDao) {
		this.regionDao = regionDao;
	}
	
	public Region getRegion() {
		return region;
	}
	
	public void setRegion(Region region) {
		this.region = region;
	}
	
	public Users getUser() {
		return user;
	}
	
	public void setUser(Users user) {
		this.user = user;
	}
	
	public IUsersDao getUsersDao() {
		return usersDao;
	}
	
	public void setUsersDao(IUsersDao usersDao) {
		this.usersDao = usersDao;
	}
	
	public ISysconfigDao getSysconfigDao() {
		return sysconfigDao;
	}
	
	public void setSysconfigDao(ISysconfigDao sysconfigDao) {
		this.sysconfigDao = sysconfigDao;
	}
	
	public OrderEnableInfo getOrderEnableInfo() {
		return orderEnableInfo;
	}
	
	public void setOrderEnableInfo(OrderEnableInfo orderEnableInfo) {
		this.orderEnableInfo = orderEnableInfo;
	}
	
	public IProductDao getProductDao() {
		return productDao;
	}
	
	public void setProductDao(IProductDao productDao) {
		this.productDao = productDao;
	}
	
	public String getTip() {
		return tip;
	}
	
	public void setTip(String tip) {
		this.tip = tip;
	}
	
	public Integer getFlag() {
		return flag;
	}
	
	public void setFlag(Integer flag) {
		this.flag = flag;
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
	
	public IProductService getProductService() {
		return productService;
	}
	
	public void setProductService(IProductService productService) {
		this.productService = productService;
	}
	
	public Product getProduct() {
		return product;
	}
	
	public void setProduct(Product product) {
		this.product = product;
	}

	public Integer getType() {
		return type;
	}

	public void setType(Integer type) {
		this.type = type;
	}

	public Integer getRecordId() {
		return recordId;
	}

	public void setRecordId(Integer recordId) {
		this.recordId = recordId;
	}
	
}
