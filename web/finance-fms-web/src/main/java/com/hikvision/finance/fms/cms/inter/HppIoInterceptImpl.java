/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-3-11 下午3:10:10
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.inter;

import java.io.IOException;

import org.apache.mina.core.session.IdleStatus;
import org.apache.mina.core.session.IoSession;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.Message;
import com.google.protobuf.Service;
import com.hikvision.rpc.handler.RpcIoInterceptImpl;
import com.hikvision.rpc.inter.RpcServiceIntercept;
import com.hikvision.rpc.util.CmsProtoUtil;
import com.ivms6.core.hpp.HppIoIntercept;
import com.ivms6.core.hpp.codec.HppMessage;
import com.ivms6.core.hpp.exception.HppException;

/**
 * <p></p>
 * @author xujiangfei 2016-3-11 下午3:10:10
 * @version V1.0 
 */
public class HppIoInterceptImpl implements HppIoIntercept{
	private static final Logger hppLog = LoggerFactory.getLogger(RpcIoInterceptImpl.class);

	public void beforeMessageReceived(IoSession session, Object message) {
		if (message != null) {
			HppMessage hppMessage = (HppMessage)message;
			StringBuffer sb = new StringBuffer("beforeMessageReceived:");
			sb.append(CmsProtoUtil.getSessionAddr(session));
			sb.append(session.getId());
			sb.append(";message=");
			sb.append(hppMessage.toString());
			sb.append(";");
			sb.append(CmsProtoUtil.getServerName(hppMessage.getService()));
			sb.append(".");
			sb.append(CmsProtoUtil.getMethodName(hppMessage.getCommand()));
			
			Integer command = hppMessage.getCommand();
			Service service = CmsProtoUtil.getService(command);
			
			if (service instanceof RpcServiceIntercept) {
				RpcServiceIntercept serviceInter = (RpcServiceIntercept) service;
				if (service != null) {
					try {
						serviceInter.beforeExecute(hppMessage);
	                } catch (SecurityException e) {
	                	hppLog.error(e.getMessage(), e);
	                } catch (IllegalArgumentException e) {
	                	hppLog.error(e.getMessage(), e);
	                }
				}
			}

			hppLog.debug(sb.toString());
		} else {
			hppLog.warn("接收的消息为空！", new HppException(HppException.BAD_MESSAGE, "messageReceived:" + CmsProtoUtil.getSessionAddr(session) + ",message is null!"));
		}
	}

	public void mssageReceived(IoSession session, int cmdId, Message message) {
		if(hppLog.isDebugEnabled()){
			if (message != null) {
				StringBuffer sb = new StringBuffer("mssageReceived:sessionId=");
				sb.append(session.getId());
				sb.append(";methodName=");
				sb.append(CmsProtoUtil.getMethodName(cmdId));
				sb.append(";request=");
				sb.append(message.toString());
				hppLog.debug(sb.toString());
			}
		}
	}

	public void afterMessageReceived(IoSession session, String msg) {
		if(hppLog.isDebugEnabled()){
			StringBuffer sb = new StringBuffer("afterMessageReceived:");
			sb.append(msg);
			hppLog.debug(sb.toString());
		}
	}

	public void messageSent(IoSession session, Object message) {
		if(hppLog.isDebugEnabled()){
			hppLog.debug(message.toString());
		}
	}

	public void sessionOpened(IoSession session) {
		if(hppLog.isDebugEnabled()){
			StringBuffer sb = new StringBuffer("SessionOpened:sessionId:");
			sb.append(session.getId());
			sb.append(";local=");
			sb.append(session.getLocalAddress() == null?"null":session.getLocalAddress().toString());
			sb.append(";remote=");
			sb.append(session.getRemoteAddress() == null?"null":session.getRemoteAddress().toString());
			sb.append(";service=");
			sb.append(session.getServiceAddress() == null?"null":session.getServiceAddress().toString());
			hppLog.debug(sb.toString());
		}
	}

	public void sessionClosed(IoSession session, String type) {
		if(hppLog.isDebugEnabled()){
			if (session != null) {
				StringBuffer sb = new StringBuffer("SessionClosed:sessionId:");
				sb.append(session.getId());
				sb.append(";local=");
				sb.append(session.getLocalAddress() == null?"null":session.getLocalAddress().toString());
				sb.append(";remote=");
				sb.append(session.getRemoteAddress() == null?"null":session.getRemoteAddress().toString());
				sb.append(";service=");
				sb.append(session.getServiceAddress() == null?"null":session.getServiceAddress().toString());
				sb.append(";type=");
				sb.append(type);
				hppLog.debug(sb.toString());
			} else {
				hppLog.debug("SessionClosed: session is null;type="+type);
			}
		}
	}

	public void sessionIdle(IoSession session, IdleStatus status) {
	
	}

	public void exceptionCaught(IoSession session, Throwable cause) {
		if (session == null) {
			hppLog.debug("exceptionCaught:session is null");
		} else {
			String str = "exceptionCaught:sessionId=" + session.getId() + ";local=" + session.getLocalAddress() + ";remote=" + session.getRemoteAddress() + ";service=" + session.getServiceAddress();
			hppLog.debug(str);
			if (cause instanceof IOException) {
				hppLog.warn("rpc error, connection closed by remote host:" + cause.getMessage());
			} else {
				hppLog.warn("hpp error:" + cause.getMessage(), cause);
			}
		}
	}
}
