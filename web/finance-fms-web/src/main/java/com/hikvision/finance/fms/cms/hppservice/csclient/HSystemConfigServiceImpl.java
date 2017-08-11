/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年10月24日 下午6:58:01
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.csclient;

import java.util.List;

import org.dom4j.tree.DefaultAttribute;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.common.collect.Lists;
import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.hikvision.finance.core.util.CollectionUtils;
import com.hikvision.finance.core.util.XMLUtils;
import com.hikvision.finance.fms.cms.hpp.csclient.SystemConfigProto;
import com.hikvision.finance.fms.cms.hpp.csclient.SystemConfigProto.ReqSysconfig;
import com.hikvision.finance.fms.cms.hpp.csclient.SystemConfigProto.RspSysconfig;
import com.hikvision.finance.fms.cms.hpp.csclient.SystemConfigProto.SystemConfigService;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.common.util.Constants.SysConfigType;
import com.hikvision.finance.fms.common.util.Constants.SysDictionary.ServerType;
import com.hikvision.finance.fms.model.Sysconfig;
import com.hikvision.finance.fms.modules.server.dao.IServerDao;
import com.hikvision.finance.fms.modules.sysconfig.dao.ISysconfigDao;
import com.ivms6.core.hpp.codec.HppConstants;

/**
 * <p></p>
 * @author zhoujiajun 2016年10月24日 下午6:58:01
 * @version V1.0 
 */
public class HSystemConfigServiceImpl extends SystemConfigService {

	private static Logger logger = LoggerFactory.getLogger(HSystemConfigServiceImpl.class);
	
	private ISysconfigDao sysconfigDao;
	private IServerDao serverDao;
	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.cms.hpp.csclient.SystemConfigProto.SystemConfigService#qyerySysconfigInfo(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.csclient.SystemConfigProto.ReqSysconfig, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void querySysconfigInfo(RpcController controller, ReqSysconfig request, RpcCallback<RspSysconfig> done) {
		
		logger.info("开始获取参数列表信息list，Cmd：【" + CMD.CMD_DT_SYSTEM_CONFIG_VALUE + "】 request：【" + request.toString() + "】 ");
		SystemConfigProto.RspSysconfig.Builder rspSysconfig = SystemConfigProto.RspSysconfig.newBuilder();
		rspSysconfig.setCmd(CMD.CMD_DT_SYSTEM_CONFIG_VALUE);
		try {
			List<Integer> keys = Lists.newArrayList();
			keys.add(SysConfigType.MAXIMUM_VIDEO_LENGTH);
			keys.add(SysConfigType.VIDEO_UPLOAD_BANDWIDTH);
			List<Sysconfig> dbSysconfigList = sysconfigDao.getSysconfigByKey(keys);
			for (com.hikvision.finance.fms.model.Sysconfig dbSysconfig : dbSysconfigList) {
				SystemConfigProto.SysconfigInfo.Builder sysconfig = SystemConfigProto.SysconfigInfo.newBuilder();
				if ( dbSysconfig.getNkey() == SysConfigType.MAXIMUM_VIDEO_LENGTH  //最大录像时长
					 || dbSysconfig.getNkey() == SysConfigType.VIDEO_UPLOAD_BANDWIDTH) { //上传录像带宽
					sysconfig.setNKey(dbSysconfig.getNkey());
					if (dbSysconfig.getStrValue() != null){
						sysconfig.setStrValue(dbSysconfig.getStrValue());
					}
					if (dbSysconfig.getStrComment() != null){
						sysconfig.setStrComment(dbSysconfig.getStrComment());
					}
					rspSysconfig.addSysconfigInfo(sysconfig);
				}
			}
			com.hikvision.finance.fms.model.Server dbServer = serverDao.findUniqueBy("ntype", ServerType.MT_SERVICE_UPLOAD);
			if (dbServer == null || dbServer.getStrConfig() == null) {
				rspSysconfig.setResult(HppConstants.RESULT_ERROR);
				rspSysconfig.setResultInfo("系统没有配置上传服务");
				done.run(rspSysconfig.build());
				return;
			}
			//解析上传服务的xml来获取上传的开始和结束时间
			String strConfig = dbServer.getStrConfig();
			org.dom4j.Document document = XMLUtils.convertStringToXml(strConfig);
			List<org.dom4j.Element> elements = XMLUtils.getElementsByXPath(document, "/ROOT/PARAM_COLLECT");
			if (CollectionUtils.isNotEmpty(elements)) {
				for (org.dom4j.Element element : elements){
					List<DefaultAttribute> attributes = element.attributes();
					for (DefaultAttribute attribute : attributes){
						SystemConfigProto.SysconfigInfo.Builder sysconfig = SystemConfigProto.SysconfigInfo.newBuilder();
						if ("startTime".equals(attribute.getName())) {
							sysconfig.setNKey(SysConfigType.UPLOAD_START_TIME);
						}else {
							sysconfig.setNKey(SysConfigType.UPLOAD_END_TIME);
						}
						sysconfig.setStrComment(attribute.getName());
						sysconfig.setStrValue(attribute.getValue());
						rspSysconfig.addSysconfigInfo(sysconfig);
					}
					break;
				}
			}
			rspSysconfig.setResult(HppConstants.RESULT_OK);
			rspSysconfig.setResultInfo("获取参数列表信息成功");
			done.run(rspSysconfig.build());
		} catch (Exception e) {
			logger.error("获取参数列表信息异常：" + e);
			rspSysconfig.setResult(HppConstants.RESULT_ERROR);
			rspSysconfig.setResultInfo("获取参数列表信息异常");
			done.run(rspSysconfig.build());
		}
	}
	
	public ISysconfigDao getSysconfigDao() {
		return sysconfigDao;
	}
	
	public void setSysconfigDao(ISysconfigDao sysconfigDao) {
		this.sysconfigDao = sysconfigDao;
	}

	public IServerDao getServerDao() {
		return serverDao;
	}

	public void setServerDao(IServerDao serverDao) {
		this.serverDao = serverDao;
	}

}
