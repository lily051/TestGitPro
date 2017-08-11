/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月11日 下午5:22:23
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.server.dao.impl;

import java.util.List;

import org.hibernate.Criteria;

import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.model.Server;
import com.hikvision.finance.fms.modules.server.dao.IServerDao;
import com.hikvision.finance.fms.modules.server.qo.ServerQo;



/**
 * <p></p>
 * @author jinxindong 2016年3月11日 下午5:22:23
 * @version V1.0 
 */
public class ServerDaoImpl extends BaseDao<Server, ServerQo> implements IServerDao{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -2225645043791106775L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, ServerQo qo) {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.server.dao.IServerDao#getServerByKeyword(java.lang.String)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<Server> getServerByKeyword(String keyWord) {
		StringBuffer sql = new StringBuffer("select t FROM Server t");
    		sql.append("  where t.strUser LIKE ? ");
    		String parma = "%"+keyWord+"%";
    		return this.createSQLQuery(sql.toString()).addEntity(Server.class).setParameter(0, parma).list();
		
    }

	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.server.dao.IServerDao#findServerByip(java.lang.String, java.lang.Integer)
	 */
	@Override
	public Server findServerByip(String ip, Integer moudleType,String Modelusb) {
		Server returnServer = null;
		if("ipc".equals(Modelusb)){// ipc模式
			StringBuffer sql = new StringBuffer("select * FROM Server t where t.strIP = ? and t.nType = ?");
			returnServer = (Server) this.createSQLQuery(sql.toString()).addEntity(Server.class).setParameter(0, ip).setParameter(1, moudleType).uniqueResult();
		} else if("usb".equals(Modelusb)){// usb模式
			StringBuffer sql = new StringBuffer("select * FROM Server t where t.nType = ?");
			returnServer = (Server) this.createSQLQuery(sql.toString()).addEntity(Server.class).setParameter(0, moudleType).uniqueResult();
		}
		return returnServer;
	}

	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.server.dao.IServerDao#setHeartbeatsTime(java.lang.String, int)
	 */
	@Override
	public void setHeartbeatsTime(String ip, int ntype) {
		StringBuffer sql  = new StringBuffer("update server set heartbeatTime = now() WHERE strIP = ? and nType = ?");	
		this.createSQLQuery(sql.toString()).setParameter(0, ip).setParameter(1, ntype).executeUpdate();
	}
	
}
