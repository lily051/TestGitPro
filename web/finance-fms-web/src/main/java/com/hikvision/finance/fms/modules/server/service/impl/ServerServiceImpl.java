/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月11日 下午5:21:59
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.server.service.impl;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import com.hikvision.cms.cache.manager.DataManager;
import com.hikvision.finance.core.util.BeanUtils;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.enums.LogModuledEnum;
import com.hikvision.finance.fms.common.init.FmsPropertyPlaceholder;
import com.hikvision.finance.fms.common.util.Constants.OperStatus;
import com.hikvision.finance.fms.common.util.Constants.ServerStatus;
import com.hikvision.finance.fms.common.util.Constants.SysDictionary.ServerType;
import com.hikvision.finance.fms.model.Server;
import com.hikvision.finance.fms.model.Sysdictionary;
import com.hikvision.finance.fms.modules.log.dao.IConfigLogDao;
import com.hikvision.finance.fms.modules.server.dao.IServerDao;
import com.hikvision.finance.fms.modules.server.dto.ServerInfo;
import com.hikvision.finance.fms.modules.server.qo.ServerQo;
import com.hikvision.finance.fms.modules.server.service.IServerService;
import com.hikvision.finance.fms.modules.server.util.ServerDataToXmlUtil;
import com.hikvision.finance.fms.modules.sysdictionary.service.ISysdictionaryService;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.AjaxData;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;


/**
 * <p></p>
 * @author jinxindong 2016年3月11日 下午5:21:59
 * @version V1.0 
 */
public class ServerServiceImpl extends BaseServiceImpl< Server,  ServerQo, IServerDao> implements IServerService{

	private ISysdictionaryService sysdictionaryService;
	private IServerDao serverDao;
	private IConfigLogDao configLogDao;
	private DataManager dataManager;
	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.IBaseService#deleteByIds(java.io.Serializable[])
     */
    @Override
    public void deleteByIds(Serializable[] paramArrayOfSerializable) {
	    // TODO Auto-generated method stub
	    
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.BaseServiceImpl#getDao()
     */
    @Override
    protected IServerDao getDao() {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.server.service.IServerService#getDicByNtype(java.lang.Integer)
     */
    @Override
    public List<Sysdictionary> getDicByNtype(Integer ntype) {
    	List<Sysdictionary> sysdictionaries = sysdictionaryService.getDicByNtype(ntype);
	    return sysdictionaries;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.server.service.IServerService#getServerPage()
     */
    @Override
	public List<ServerInfo> getServerPage(String keyWord) {
		List<ServerInfo> serverInfos = new ArrayList<ServerInfo>();
		List<Server> servers = null;
		if (StringUtils.isNotBlank(keyWord)) {
			servers = serverDao.getServerByKeyword(keyWord);
		} else {
			servers = serverDao.getAll();
		}
		if (servers != null) {
			for (Server server : servers) {
				ServerInfo serverInfo = new ServerInfo();
				try {
					BeanUtils.copyNotNullProperties(serverInfo, server);
				} catch (Exception e) {
					throw new ExpectedException("", "拷贝bean失败[getServerPage]");
				}
				
				//服务状态   由中心自己判断    心跳超过4次   120秒  判断为离线
				Date newDate = new Date();
				Date hertDate = null==serverInfo.getHeartbeatTime()?null:serverInfo.getHeartbeatTime();
				if(hertDate != null){
					if((newDate.getTime()-hertDate.getTime())/1000>120){
						serverInfo.setNonline(ServerStatus.NOTONLINE);
						serverInfo.setOnlineSatus(ServerStatus.NOT_ONLINE);
					}else{
						serverInfo.setNonline(ServerStatus.ONLINE);
						serverInfo.setOnlineSatus(ServerStatus.ON_LINE);
					}
				}else{
					serverInfo.setNonline(ServerStatus.NOTONLINE);
					serverInfo.setOnlineSatus(ServerStatus.NOT_ONLINE);
				}
				
				if (server.getNtype() == ServerType.MT_SERVICE_UPLOAD) {// 上传服务
					serverInfo = ServerDataToXmlUtil.resolveUploadXml(serverInfo);
				} else if (server.getNtype() == ServerType.MT_SERVICE_TIMING) {// 校时服务
					serverInfo = ServerDataToXmlUtil.resolveInspectionXml(serverInfo);
				} else if (server.getNtype() == ServerType.MT_SERVICE_GUARD) {// 录像守卫服务
					serverInfo = ServerDataToXmlUtil.resolveRecordXml(serverInfo);
				} else {// web服务
					serverInfo.setNonline(ServerStatus.ONLINE);
					serverInfo.setOnlineSatus(ServerStatus.ON_LINE);
				}
				
				serverInfos.add(serverInfo);
			}
		}
		return serverInfos;
	}
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.server.service.IServerService#addServer(com.hikvision.finance.fms.modules.server.dto.ServerInfo)
     */
    @Override
	public AjaxData addServer(ServerInfo serverInfo) {
		AjaxData ajaxData = new AjaxData(false);
		
		String strChoiceUsbIpc = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
		if (StringUtils.isEmpty(strChoiceUsbIpc)) {
			return ajaxData.setFailureMsg("未获取到通道方式");
		}
		String isNonCenterStorage = "";
		if ("ipc".equals(strChoiceUsbIpc)) {
			isNonCenterStorage = FmsPropertyPlaceholder.getProperty("nonCenterStorage");
	    	if (StringUtils.isEmpty(isNonCenterStorage)) {
	    		return ajaxData.setFailureMsg("未获取到中心存储的状态");
	    	}
		} else {
			isNonCenterStorage = "true";
		}
		
		Server server = new Server();
		server.setNonline(ServerStatus.NOTONLINE);//在线标志位 1表示在线 0表示不
		if (serverInfo.getNtype() == ServerType.MT_SERVICE_UPLOAD) {// 上传服务
	    	serverInfo = ServerDataToXmlUtil.buildUploadXml(serverInfo, strChoiceUsbIpc, isNonCenterStorage);
		} else if (serverInfo.getNtype() == ServerType.MT_SERVICE_TIMING) {// 系统巡检服务
			serverInfo = ServerDataToXmlUtil.buildInspectionXml(serverInfo, strChoiceUsbIpc, isNonCenterStorage);
		} else if (serverInfo.getNtype() == ServerType.MT_SERVICE_GUARD) {// 录像守卫服务
			serverInfo = ServerDataToXmlUtil.buildRecordXml(serverInfo, strChoiceUsbIpc, isNonCenterStorage);
		} else {// web服务
			server.setHeartbeatTime(new Date());
			server.setNonline(ServerStatus.ONLINE);//覆盖上面的默认不在线
		}
		//校验服务的唯一性
		List<Server> ser = serverDao.findBy("ntype", serverInfo.getNtype());
		if(ser!=null && ser.size()>0){
			throw new ExpectedException("", "已存在相同服务");
		}
		server.setStrUser(serverInfo.getStrUser());//服务名称
		server.setStrIp(null == serverInfo.getStrIp()?"":serverInfo.getStrIp());//服务ip
		server.setStrConfig(null == serverInfo.getStrConfig()?"":serverInfo.getStrConfig());//配置信息
		server.setStrComment(null == serverInfo.getStrComment()?"":serverInfo.getStrComment());//备注
		server.setNtype(serverInfo.getNtype());//服务类型
		server.setNdataPort(null == serverInfo.getNdataPort()?null:serverInfo.getNdataPort());//数据端口
		server.setNctrlPort(null == serverInfo.getNctrlPort()?null:serverInfo.getNctrlPort());//控制端口
		serverDao.save(server);
		configLogDao.setLogContent(OperStatus.ADD, LogModuledEnum.SERVER.getCode(),GlobalMessageUtil.getMessage("log.server.add",new String[] {serverInfo.getStrUser()}));
		return ajaxData;
	}
    
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.server.service.IServerService#updateServer(com.hikvision.finance.fms.modules.server.dto.ServerInfo)
     */
    @Override
	public AjaxData updateServer(ServerInfo serverInfo) {
		AjaxData ajaxData = new AjaxData(false);
		
		String strChoiceUsbIpc = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
		if (StringUtils.isEmpty(strChoiceUsbIpc)) {
			return ajaxData.setFailureMsg("未获取到通道方式");
		}
		String isNonCenterStorage = "";
		if ("ipc".equals(strChoiceUsbIpc)) {
			isNonCenterStorage = FmsPropertyPlaceholder.getProperty("nonCenterStorage");
	    	if (StringUtils.isEmpty(isNonCenterStorage)) {
	    		return ajaxData.setFailureMsg("未获取到中心存储的状态");
	    	}
		} else {
			isNonCenterStorage = "true";
		}
		
		if (serverInfo.getNtype() == ServerType.MT_SERVICE_UPLOAD) {// 上传服务
			serverInfo = ServerDataToXmlUtil.buildUploadXml(serverInfo, strChoiceUsbIpc, isNonCenterStorage);
		} else if (serverInfo.getNtype() == ServerType.MT_SERVICE_TIMING) {// 校时服务
			serverInfo = ServerDataToXmlUtil.buildInspectionXml(serverInfo, strChoiceUsbIpc, isNonCenterStorage);
		} else if (serverInfo.getNtype() == ServerType.MT_SERVICE_GUARD) {// 录像守卫服务
			serverInfo = ServerDataToXmlUtil.buildRecordXml(serverInfo, strChoiceUsbIpc, isNonCenterStorage);
		} else {// web服务
		
		}
		Server server = serverDao.get(serverInfo.getId());
		server.setStrUser(serverInfo.getStrUser());// 服务名称
		server.setStrIp(null == serverInfo.getStrIp()?"":serverInfo.getStrIp());// 服务ip
		server.setStrConfig(null == serverInfo.getStrConfig()?"":serverInfo.getStrConfig());// 配置信息
		server.setStrComment(null == serverInfo.getStrComment()?"":serverInfo.getStrComment());// 备注
		server.setNtype(serverInfo.getNtype());// 服务类型
		server.setNdataPort(null == serverInfo.getNdataPort()?null:serverInfo.getNdataPort());// 数据端口
		server.setNctrlPort(null == serverInfo.getNctrlPort()?null:serverInfo.getNctrlPort());// 控制端口
		serverDao.update(server);
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.SERVER.getCode(),GlobalMessageUtil.getMessage("log.server.update",new String[] {serverInfo.getStrUser()}));
		return ajaxData;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.server.service.IServerService#deleteServer(java.lang.Integer)
     */
    @Override
	public AjaxData deleteServer(Integer serverId) {
		AjaxData ajaxData = new AjaxData(false);
		Server server = serverDao.get(serverId);
		boolean del = serverDao.deleteById(serverId);
		if(del){
			configLogDao.setLogContent(OperStatus.DELETE, LogModuledEnum.SERVER.getCode(),GlobalMessageUtil.getMessage("log.server.delete",new String[] {server.getStrUser()}));
		}
		return ajaxData;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.server.service.IServerService#getServerById(java.lang.Integer)
     */
    @Override
	public ServerInfo getServerById(Integer serverId) {
		ServerInfo serverInfo = new ServerInfo();
		Server server = serverDao.get(serverId);
		try {
			BeanUtils.copyNotNullProperties(serverInfo, server);
		} catch (Exception e) {
			throw new ExpectedException("", "拷贝bean失败[getServerPage]");
		}
		if (server.getNtype() == ServerType.MT_SERVICE_UPLOAD) {// 上传服务
			serverInfo = ServerDataToXmlUtil.resolveUploadXml(serverInfo);
		} else if (server.getNtype() == ServerType.MT_SERVICE_TIMING) {// 校时服务
			serverInfo = ServerDataToXmlUtil.resolveInspectionXml(serverInfo);
		} else if (server.getNtype() == ServerType.MT_SERVICE_GUARD) {// 录像守卫服务
			serverInfo = ServerDataToXmlUtil.resolveRecordXml(serverInfo);
		} else {// web服务
		
		}
		return serverInfo;
	}

    /* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.server.service.IServerService#restartServer(java.lang.Integer)
     */
    @Override
    public AjaxData restartServer(Integer serverId) {
    	AjaxData ajaxData = new AjaxData(false);
		Server server = serverDao.get(serverId);		
		//通知服务器所在看门狗重启服务
		if(server==null){
			
		}
		//boolean b = watchDogService.serverRestart(serverId);
		return ajaxData;
    }
    //********************************************getter/setter*************************************
    public ISysdictionaryService getSysdictionaryService() {
    	return sysdictionaryService;
    }

	
    public void setSysdictionaryService(ISysdictionaryService sysdictionaryService) {
    	this.sysdictionaryService = sysdictionaryService;
    }

	
    public IServerDao getServerDao() {
    	return serverDao;
    }

	
    public void setServerDao(IServerDao serverDao) {
    	this.serverDao = serverDao;
    }
	
    public IConfigLogDao getConfigLogDao() {
    	return configLogDao;
    }

	
    public void setConfigLogDao(IConfigLogDao configLogDao) {
    	this.configLogDao = configLogDao;
    }

    public DataManager getDataManager() {
		return dataManager;
	}
    
    public void setDataManager(DataManager dataManager) {
		this.dataManager = dataManager;
	}





	



	
    
}
