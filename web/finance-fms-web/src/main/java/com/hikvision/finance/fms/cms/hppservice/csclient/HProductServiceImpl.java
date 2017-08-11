/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年10月12日 下午3:08:24
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.csclient;

import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.hikvision.finance.core.util.CollectionUtils;
import com.hikvision.finance.core.util.DateFormatUtils;
import com.hikvision.finance.core.util.ListUtil;
import com.hikvision.finance.fms.cms.hpp.csclient.ProductProto;
import com.hikvision.finance.fms.cms.hpp.csclient.ProductProto.ProductInfo;
import com.hikvision.finance.fms.cms.hpp.csclient.ProductProto.ProductList;
import com.hikvision.finance.fms.cms.hpp.csclient.ProductProto.ProductService;
import com.hikvision.finance.fms.cms.hpp.csclient.ProductProto.ReqProduct;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.model.Product;
import com.hikvision.finance.fms.model.Producttype;
import com.hikvision.finance.fms.model.Usercrtificatemapping;
import com.hikvision.finance.fms.modules.product.dao.IProductDao;
import com.hikvision.finance.fms.modules.product.dao.IProductTypeDao;
import com.hikvision.finance.fms.modules.product.qo.ProductQo;
import com.hikvision.finance.fms.modules.user.dao.IUsercrtificatemappingDao;
import com.ivms6.core.hpp.codec.HppConstants;


/**
 * <p>CS客户端获取产品</p>
 * @author jinchenwu 2016年10月17日 下午1:49:39
 * @version V1.0
 */
public class HProductServiceImpl extends ProductService {
	
	private static Logger logger = LoggerFactory.getLogger(HProductServiceImpl.class);
	
	private IProductDao productDao;
	private IUsercrtificatemappingDao usercrtificatemappingDao;
	private IProductTypeDao productTypeDao;
	/*
	 * CS客户端获取产品List
	 * @see com.hikvision.finance.fms.cms.hpp.csclient.ProductProto.ProductService#getProductList(com.google.protobuf.RpcController,
	 * com.hikvision.finance.fms.cms.hpp.csclient.ProductProto.ReqProduct, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void getProductList(RpcController controller, ReqProduct request, RpcCallback<ProductList> done) {
		logger.info("开始获取产品List，Cmd：【" + CMD.CMD_DT_PRODUCT_LIST_VALUE + "】 request：【" + request.toString() + "】 ");
		ProductProto.ProductList.Builder productList = ProductProto.ProductList.newBuilder();
		productList.setCmd(CMD.CMD_DT_PRODUCT_LIST_VALUE);
		try {
			List<Product> dbProductList = getPermitProductList(request.getPId(), request.getInputUser(), null, null);
			if (CollectionUtils.isNotEmpty(dbProductList)) {
				for (Product dbProduct : dbProductList) {
					ProductProto.Product.Builder product = ProductProto.Product.newBuilder();
					if (null != dbProduct.getId()) {
						product.setId(dbProduct.getId());
					}
					if (null != dbProduct.getPid()) {
						product.setPId(dbProduct.getPid());
					}
					if (null != dbProduct.getStrName()) {
						product.setStrName(dbProduct.getStrName());
					}
					if (null != dbProduct.getStrCode()) {
						product.setStrCode(dbProduct.getStrCode());
					}
					if (null != dbProduct.getStrGrade()) {
						product.setStrGrade(dbProduct.getStrGrade());
					}
					productList.addProduct(product);
				}
			}
			done.run(productList.build());
		} catch (Exception e) {
			logger.error("获取产品列表异常：" + e);
			productList.setResult(HppConstants.RESULT_ERROR);
			productList.setResultInfo("获取产品列表异常");
			done.run(productList.build());
		}
	}
	
	/**
	 * 获取产品List
	 * @author jinchenwu 2016年10月17日 上午11:29:47
	 * @param productTypeId
	 * @return
	 */
	private List<Product> getPermitProductList(Integer productTypeId, Integer inputUser, String strCode, Integer productId) {
		List<Usercrtificatemapping> usercrtificatemappingList = usercrtificatemappingDao.findBy("userId", inputUser);
		String crtificateIds = new String();
		if (usercrtificatemappingList != null && usercrtificatemappingList.size() > 0) {// 用户有关联证书，则获取证书
			List<Integer> crtificateIdList = new ArrayList<Integer>();
			for (Usercrtificatemapping usercrtificatemapping : usercrtificatemappingList) {
				crtificateIdList.add(usercrtificatemapping.getNcrtificateId());
			}
			crtificateIds = ListUtil.listToStr(crtificateIdList);// 证书id
		}
		List<Product> permitProductList = productDao.getPermitProductList(crtificateIds, productTypeId, strCode, productId);// 要显示的产品List
		return permitProductList;
	}
	
	/*
	 * CS客户端获取产品详情
	 * @see com.hikvision.finance.fms.cms.hpp.csclient.ProductProto.ProductService#getProductInfo(com.google.protobuf.RpcController,
	 * com.hikvision.finance.fms.cms.hpp.csclient.ProductProto.ReqProduct, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void getProductInfo(RpcController controller, ReqProduct request, RpcCallback<ProductInfo> done) {
		logger.info("开始获取产品信息，Cmd：【" + CMD.CMD_DT_PRODUCT_INFO_VALUE + "】" + "request：【" + request.toString() + "】 ");
		ProductProto.ProductInfo.Builder productInfo = ProductProto.ProductInfo.newBuilder();
		productInfo.setCmd(CMD.CMD_DT_PRODUCT_INFO_VALUE);
		try {
			List<Product> dbProductList = getPermitProductList(null, request.getInputUser(), request.getStrCode(), request.getId());
			if (CollectionUtils.isNotEmpty(dbProductList)) {
				for (Product dbProduct : dbProductList) {
					ProductProto.Product.Builder product = ProductProto.Product.newBuilder();
					if (dbProduct.getId() != null) {
						product.setId(dbProduct.getId());
					}
					if (dbProduct.getStrName() != null) {
						product.setStrName(dbProduct.getStrName());
					}
					if (dbProduct.getStrCode() != null) {
						product.setStrCode(dbProduct.getStrCode());
					}
					if (dbProduct.getStrEndDate() != null) {
						String formatDate = DateFormatUtils.defaultFormat(dbProduct.getStrEndDate());
						product.setStrEndDate(formatDate);
					}
					if (dbProduct.getGuaranteeType() != null) {
						product.setGuaranteeType(dbProduct.getGuaranteeType());
					}
					if (dbProduct.getSaveType() != null) {
						product.setSaveType(dbProduct.getSaveType());
					}
					if (dbProduct.getSaveYears() != null) {
						product.setSaveYears(dbProduct.getSaveYears());
					}
					if (dbProduct.getStrGrade() != null) {
						product.setStrGrade(dbProduct.getStrGrade());
					}
					if (dbProduct.getStrRisklevel() != null) {
						product.setStrRisklevel(dbProduct.getStrRisklevel());
					}
					if (dbProduct.getPid() != null) {
						product.setPId(dbProduct.getPid());
						Producttype producttype = productTypeDao.get(dbProduct.getPid());
						product.setProductType(producttype != null ? producttype.getStrName(): "");
					}
					productInfo.setProduct(product);
					productInfo.setResult(HppConstants.RESULT_OK);
					productInfo.setResultInfo("获取产品列表成功");
				}
			}else {
				productInfo.setResult(HppConstants.RESULT_ERROR);
				productInfo.setResultInfo("获取产品列表异常");
			}
			done.run(productInfo.build());
		} catch (Exception e) {
			logger.error("获取产品信息异常：" + e);
			productInfo.setResult(HppConstants.RESULT_ERROR);
			productInfo.setResultInfo("获取产品信息异常");
			done.run(productInfo.build());
		}
	}
	
	/* CS客户端获取产品话术
	 * @see com.hikvision.finance.fms.cms.hpp.csclient.ProductProto.ProductService#getProductWordTemplate(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.csclient.ProductProto.ReqProduct, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void getProductWordTemplate(RpcController controller, ReqProduct request, RpcCallback<ProductInfo> done) {
		logger.info("开始获取产品话术，Cmd：【" + CMD.CMD_DT_PRODUCT_WORD_TEMPLATE_VALUE + "】request：【" + request.toString() + "】 ");
		ProductProto.ProductInfo.Builder productInfo = ProductProto.ProductInfo.newBuilder();
		productInfo.setCmd(CMD.CMD_DT_PRODUCT_WORD_TEMPLATE_VALUE);
		try {
			ProductQo productQo = new ProductQo();
			productQo.setId(request.getId());
			Product dbProduct = productDao.queryUnique(productQo);
			if (dbProduct != null) {
				ProductProto.Product.Builder product = ProductProto.Product.newBuilder();
				if (dbProduct.getId() != null) {
					product.setId(dbProduct.getId());
				}
				if (dbProduct.getStrName() != null) {
					product.setStrName(dbProduct.getStrName());
				}
				if (dbProduct.getStrCode() != null) {
					product.setStrCode(dbProduct.getStrCode());
				}
				if (dbProduct.getWordTemplate() != null) {
					product.setWordTemplate(dbProduct.getWordTemplate());
				}
				productInfo.setProduct(product);
			}
			done.run(productInfo.build());
		} catch (Exception e) {
			logger.error("获取产品话术异常：" + e);
			productInfo.setResult(HppConstants.RESULT_ERROR);
			productInfo.setResultInfo("获取产品话术异常");
			done.run(productInfo.build());
		}
	}
	
	public IProductDao getProductDao() {
		return productDao;
	}
	
	public void setProductDao(IProductDao productDao) {
		this.productDao = productDao;
	}
	
	public IUsercrtificatemappingDao getUsercrtificatemappingDao() {
		return usercrtificatemappingDao;
	}
	
	public void setUsercrtificatemappingDao(IUsercrtificatemappingDao usercrtificatemappingDao) {
		this.usercrtificatemappingDao = usercrtificatemappingDao;
	}

	public IProductTypeDao getProductTypeDao() {
		return productTypeDao;
	}

	public void setProductTypeDao(IProductTypeDao productTypeDao) {
		this.productTypeDao = productTypeDao;
	}

}
