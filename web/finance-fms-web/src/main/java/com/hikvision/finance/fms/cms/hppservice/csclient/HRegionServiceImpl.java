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
import com.hikvision.finance.fms.cms.hpp.csclient.RegionProto;
import com.hikvision.finance.fms.cms.hpp.csclient.RegionProto.RegionList;
import com.hikvision.finance.fms.cms.hpp.csclient.RegionProto.RegionService;
import com.hikvision.finance.fms.cms.hpp.csclient.RegionProto.ReqRegion;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.ivms6.core.hpp.codec.HppConstants;


/**
 * <p>CS客户端获取区域信息</p>
 * @author jinchenwu 2016年10月12日 下午3:08:24
 * @version V1.0 
 */
public class HRegionServiceImpl extends RegionService {
	
	private static Logger logger = LoggerFactory.getLogger(HRegionServiceImpl.class);
	
	private IRegionDao regionDao;
	
	/*
	 * CS客户端获取区域信息
	 * @see com.hikvision.finance.fms.cms.hpp.csclient.RegionProto.RegionService#getRegionList(com.google.protobuf.RpcController,
	 * com.hikvision.finance.fms.cms.hpp.csclient.RegionProto.ReqRegion, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void getRegionList(RpcController controller, ReqRegion request, RpcCallback<RegionList> done) {
		logger.info("开始获取组织List，Cmd：【" + CMD.CMD_DT_REGION_LIST_VALUE + "】" + "request：【" + request.toString() + "】 ");
		RegionProto.RegionList.Builder regionList = RegionProto.RegionList.newBuilder();
		regionList.setCmd(CMD.CMD_DT_REGION_LIST_VALUE);
		try {
			List<Region> dbRegionList = regionDao.getAll();
			if (CollectionUtils.isNotEmpty(dbRegionList)) {
				for (Region dbRegion : dbRegionList) {
					RegionProto.Region.Builder region = RegionProto.Region.newBuilder();
					if (null != dbRegion.getId()) {
						region.setId(dbRegion.getId());
					}
					if (null != dbRegion.getNparentId()) {
						region.setNParentId(dbRegion.getNparentId());
					}
					if (null != dbRegion.getStrName()) {
						region.setStrName(dbRegion.getStrName());
					}
					regionList.addRegion(region);
				}
			}
			done.run(regionList.build());
		} catch (Exception e) {
			logger.error("获取区域列表异常：" + e);
			regionList.setResult(HppConstants.RESULT_ERROR);
			regionList.setResultInfo("获取区域列表异常");
			done.run(regionList.build());
		}
	}
	
	public IRegionDao getRegionDao() {
		return regionDao;
	}
	
	public void setRegionDao(IRegionDao regionDao) {
		this.regionDao = regionDao;
	}
	
}
