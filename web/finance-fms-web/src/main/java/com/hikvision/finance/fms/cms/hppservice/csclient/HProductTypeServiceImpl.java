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
import com.hikvision.finance.fms.cms.hpp.csclient.ProductTypeProto;
import com.hikvision.finance.fms.cms.hpp.csclient.ProductTypeProto.ProductTypeList;
import com.hikvision.finance.fms.cms.hpp.csclient.ProductTypeProto.ProductTypeService;
import com.hikvision.finance.fms.cms.hpp.csclient.ProductTypeProto.ReqProductType;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.model.Producttype;
import com.hikvision.finance.fms.modules.product.dao.IProductTypeDao;
import com.ivms6.core.hpp.codec.HppConstants;


/**
 * <p>CS客户端获取产品类型</p>
 * @author jinchenwu 2016年10月13日 下午1:55:21
 * @version V1.0 
 */
public class HProductTypeServiceImpl extends ProductTypeService {
	
	private static Logger logger = LoggerFactory.getLogger(HProductTypeServiceImpl.class);
	
	private IProductTypeDao productTypeDao;
	
	/*
	 * CS客户端获取产品类型
	 * @see com.hikvision.finance.fms.cms.hpp.csclient.ProductTypeProto.ProductTypeService#getProductTypeList(com.google.protobuf.RpcController,
	 * com.hikvision.finance.fms.cms.hpp.csclient.ProductTypeProto.ReqProductType, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void getProductTypeList(RpcController controller, ReqProductType request, RpcCallback<ProductTypeList> done) {
		logger.info("开始获取产品类型List，Cmd：【" + CMD.CMD_DT_PRODUCTTYPE_LIST_VALUE + "】 request：【" + request.toString() + "】 ");
		ProductTypeProto.ProductTypeList.Builder productTypeList = ProductTypeProto.ProductTypeList.newBuilder();
		productTypeList.setCmd(CMD.CMD_DT_PRODUCTTYPE_LIST_VALUE);
		try {
			List<Producttype> dbProductTypeList = productTypeDao.getAll();
			if (CollectionUtils.isNotEmpty(dbProductTypeList)) {
				for (Producttype dbProductType : dbProductTypeList) {
					ProductTypeProto.ProductType.Builder productType = ProductTypeProto.ProductType.newBuilder();
					if (null != dbProductType.getId()) {
						productType.setId(dbProductType.getId());
					}
					if (null != dbProductType.getStrName()) {
						productType.setStrName(dbProductType.getStrName());
					}
					if (null != dbProductType.getPid()) {
						productType.setPid(dbProductType.getPid());
					}
					productTypeList.addProductType(productType);
				}
			}
			done.run(productTypeList.build());
		} catch (Exception e) {
			logger.error("获取产品类型列表异常：" + e);
			productTypeList.setResult(HppConstants.RESULT_ERROR);
			productTypeList.setResultInfo("获取产品类型列表异常");
			done.run(productTypeList.build());
		}
	}
	
	public IProductTypeDao getProductTypeDao() {
		return productTypeDao;
	}
	
	public void setProductTypeDao(IProductTypeDao productTypeDao) {
		this.productTypeDao = productTypeDao;
	}
	
}
