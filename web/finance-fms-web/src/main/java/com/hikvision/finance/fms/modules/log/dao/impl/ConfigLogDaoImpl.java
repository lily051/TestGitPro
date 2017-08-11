/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午4:43:09
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.log.dao.impl;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.apache.struts2.ServletActionContext;
import org.hibernate.Criteria;
import org.hibernate.Query;

import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.extend.hibernate.qo.SqlQuery;
import com.hikvision.finance.fms.common.model.UserSession;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.OperStatus;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.model.Configlog;
import com.hikvision.finance.fms.model.Privilege;
import com.hikvision.finance.fms.modules.log.dao.IConfigLogDao;
import com.hikvision.finance.fms.modules.log.dto.ConfiglogInfo;
import com.hikvision.finance.fms.modules.log.qo.ConfiglogQo;
import com.hikvision.finance.fms.modules.role.dao.IPrivilegeDao;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.ServletUtil;


/**
 * <p></p>
 * @author jinxindong 2016年4月8日 下午4:43:09
 * @version V1.0 
 */
public class ConfigLogDaoImpl extends BaseDao<Configlog,ConfiglogQo> implements IConfigLogDao{
	private IPrivilegeDao privilegeDao;
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -3994187469667398050L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, ConfiglogQo qo) {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.dao.IConfigLogDao#getConfiglogDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.log.dto.ConfiglogInfo)
     */
    @Override
    public Page getConfiglogDataPage(Integer limit, Integer start, ConfiglogInfo configlogInfo) {
		//1.封装查询条件
		SqlQuery sqlQuery = conditionQuery(configlogInfo);
		
		//2.设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
	    page = this.queryPaginationBySql(page);
	    return page;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.dao.IConfigLogDao#exportConfiglogExcel(com.hikvision.finance.fms.modules.log.dto.ConfiglogInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
	public List<ConfiglogInfo> exportConfiglogExcel(ConfiglogInfo configlogInfo) {
		// 1.封装查询条件
		SqlQuery sqlQuery = conditionQuery(configlogInfo);
		
		// 2.计入循环数据
		List<ConfiglogInfo> configlogInfos = (List<ConfiglogInfo>)this.queryListBySql(sqlQuery);
		List<ConfiglogInfo> configLogInfos = new ArrayList<ConfiglogInfo>();
		Integer count = configlogInfos.size();
		if (count > Constants.EXCEL_MAX_NUM) {
			throw new ExpectedException("", "导出数据超过" + Constants.EXCEL_MAX_NUM + "条,无法导出");
		}
		if (configlogInfos.size() > 0) {
			// 数据库中取出可用的模块名
			List<Privilege> privileges = privilegeDao.findBy("nenabled", Constants.CONS_1);
			Map<String, Privilege> privilegeMap = new HashMap<String, Privilege>();
			for (Privilege privilege : privileges) {
				privilegeMap.put(privilege.getStrCode(), privilege);
			}
			
			for (ConfiglogInfo info : configlogInfos) {
				// 添加模块类型和操作类型
				if (OperStatus.ADD == info.getNactionType()) {
					info.setNactionTypeName("添加");
				} else if (OperStatus.DELETE == info.getNactionType()) {
					info.setNactionTypeName("删除");
				} else if (OperStatus.UPDATE == info.getNactionType()) {
					info.setNactionTypeName("修改");
				} else if (OperStatus.IMPORT == info.getNactionType()) {
					info.setNactionTypeName("导入");
				} else if (OperStatus.EXPORT == info.getNactionType()) {
					info.setNactionTypeName("导出");
				}else {
					info.setNactionTypeName("未知");
				}
				info.setNtypeName(privilegeMap.get(info.getNtype().toString()).getStrName());
				configLogInfos.add(info);
			}
		}
		return configLogInfos;
	}
	
	/**
	 * 设置操作日志的内容
	 * @author jinxindong 2016年4月11日 上午11:31:39
	 * @param operType 操作类型
	 * @param strCode 操作模块
	 * @param conetnt 操作内容
	 */
	public void setLogContent(Integer operType,String strCode,String conetnt){
		UserSession userSession = SessionUtil.getUserSession();
        Configlog configlog = new Configlog();
        configlog.setNactionType(operType);
        configlog.setNtype(Integer.valueOf(strCode));
        configlog.setStrDate(new Date());
        HttpServletRequest request = ServletActionContext.getRequest();
        configlog.setStrIp(ServletUtil.getRemortIP(request));
        configlog.setStrUser(userSession.getUserName());
        configlog.setStrText(conetnt);
        this.save(configlog);
        
	}
	public void setLogContent(Integer operType,String strCode,String conetnt,String userName){
		Configlog configlog = new Configlog();
		configlog.setNactionType(operType);
		configlog.setNtype(Integer.valueOf(strCode));
		configlog.setStrDate(new Date());
		HttpServletRequest request = ServletActionContext.getRequest();
		configlog.setStrIp(ServletUtil.getRemortIP(request));
		configlog.setStrUser(userName);
		configlog.setStrText(conetnt);
		this.save(configlog);
		
	}

	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.log.dao.IConfigLogDao#deleteLog(java.util.Date)
	 */
	@Override
	public void deleteLog(Date thresholddate) {
		String sql = "delete from Configlog where strDate < :thresholddate";
		Query query = createQuery(sql);
		query.setParameter("thresholddate", thresholddate);
		query.executeUpdate();
	}
	
	
	/**
	 * 封装查询条件
	 * @author jinxindong 2016年5月19日 下午4:53:20
	 * @param configlogInfo
	 * @return
	 */
	private SqlQuery conditionQuery(ConfiglogInfo configlogInfo){
		Date strDateStart = null;
		if(StringUtils.isNotBlank(configlogInfo.getStrDateStart())){
			strDateStart = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", configlogInfo.getStrDateStart());// 业务时间 起始时间
		}
		Date strDateEnd = null;
		if(StringUtils.isNotBlank(configlogInfo.getStrDateEnd())){
			strDateEnd = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", configlogInfo.getStrDateEnd());// 业务时间 结束时间	
		}
		
		String strUser = null == configlogInfo.getStrUser()?"":configlogInfo.getStrUser();// 操作用户
		Integer ntype = null == configlogInfo.getNtype()?null:configlogInfo.getNtype();// 操作模块
		Integer nactionType = null == configlogInfo.getNactionType()?null:configlogInfo.getNactionType();// 操作类型

    	SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = new StringBuffer("SELECT "
				+ "c.id AS id,"
				+ "c.nType AS ntype,"
				+ "c.strText AS strText,"
				+ "c.strUser AS strUser,"
				+ "c.strDate AS strDate,"
				+ "c.strIp AS strIp,"
				+ "c.nActionType AS nactionType "
				+ "FROM configlog c "
				+ "where 1=1 ");
				
		if (StringUtils.isNotBlank(strUser)) {
			sql.append(" and  c.strUser like ?  escape '/'");
			sqlQuery.addParams("%"+StringUtilExpand.escapeSQLLike(strUser)+"%");
		}
		
		if (null != nactionType) {
			sql.append(" and c.nActionType = ? ");
			sqlQuery.addParams(nactionType);
		}
		
		if (null != ntype) {
			sql.append(" and c.nType = ? ");
			sqlQuery.addParams(ntype);
		}
		
		// 时间
		if (strDateStart != null) {
			sql.append(" and c.strDate >= ?");
			sqlQuery.addParams(strDateStart);
		}
		if (strDateEnd != null) {
			sql.append("and c.strDate <= ? ");
			sqlQuery.addParams(strDateEnd);
		}
		// 默认倒序
		sql.append(" order by c.id desc");
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(ConfiglogInfo.class);
		
		return sqlQuery;
	}
	public IPrivilegeDao getPrivilegeDao() {
		return privilegeDao;
	}
	
	public void setPrivilegeDao(IPrivilegeDao privilegeDao) {
		this.privilegeDao = privilegeDao;
	}
}
