/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月22日 下午2:37:34
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.listener;

import java.io.Serializable;
import java.util.Date;

import org.hibernate.engine.SessionImplementor;
import org.hibernate.event.SaveOrUpdateEvent;
import org.hibernate.event.def.DefaultUpdateEventListener;
import org.hibernate.proxy.HibernateProxy;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.hikvision.finance.fms.common.model.BaseEntity;

/**
 * @author jinxindong 2016年2月22日 下午2:37:34
 * @version V1.0
 */
public class FmsUpdateEventListener extends DefaultUpdateEventListener {
	
	private static final long serialVersionUID = -8086928768559438884L;
	private static final Logger log = LoggerFactory.getLogger(FmsUpdateEventListener.class);
	
	public void onSaveOrUpdate(SaveOrUpdateEvent event) {
		SessionImplementor source = event.getSession();
		Object object = event.getObject();
		Serializable requestedId = event.getRequestedId();
		
		if (object instanceof BaseEntity) {
			BaseEntity entity = (BaseEntity)event.getObject();
			entity.setDtUpdateTime(new Date());
		}
		if (requestedId != null) {
			// assign the requested id to the proxy, *before*
			// reassociating the proxy
			if (object instanceof HibernateProxy) {
				((HibernateProxy)object).getHibernateLazyInitializer().setIdentifier(requestedId);
			}
		}
		
		if (reassociateIfUninitializedProxy(object, source)) {
			log.trace("reassociated uninitialized proxy");
			// an uninitialized proxy, noop, don't even need to
			// return an id, since it is never a save()
		} else {
			// initialize properties of the event:
			final Object entity = source.getPersistenceContext().unproxyAndReassociate(object);
			event.setEntity(entity);
			event.setEntry(source.getPersistenceContext().getEntry(entity));
			// return the id in the event object
			event.setResultId(performSaveOrUpdate(event));
		}
		
	}
	
}
