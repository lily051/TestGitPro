/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-3-11 下午3:02:57
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.inter;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;

import org.apache.commons.lang.StringUtils;
import org.apache.mina.core.future.IoFutureListener;
import org.apache.mina.core.session.IdleStatus;
import org.apache.mina.core.session.IoSession;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.Message;
import com.hikvision.finance.fms.cms.inter.CmsHppIoHandler;
import com.hikvision.finance.fms.cms.inter.CmsHppServerCallback;
import com.hikvision.finance.fms.cms.util.CsClientContextUtil;
import com.hikvision.rpc.handler.FinIoHandlerAdapter;
import com.hikvision.rpc.util.CmsProtoUtil;
import com.ivms6.core.hpp.HppCallback;
import com.ivms6.core.hpp.HppChannel;
import com.ivms6.core.hpp.HppIoIntercept;
import com.ivms6.core.hpp.codec.HppConstants;
import com.ivms6.core.hpp.codec.HppMessage;
import com.ivms6.core.hpp.exception.HppException;
import com.ivms6.core.hpp.exception.HppInitException;
import com.ivms6.core.hpp.service.ProtoBean;
import com.ivms6.core.hpp.service.RpcControllerImpl;
import com.ivms6.core.hpp.util.ContextUtil;

/**
 * <p></p>
 * @author xujiangfei 2016-3-11 下午3:02:57
 * @version V1.0 
 * protocol buffer Io handler
 * 修改记录:
 * 20150414: 返回值cmd不再是传入值cmd + 1, 而是根据CmsProtoUtil.reqRspMap决定
 */

public class CmsHppIoHandler extends FinIoHandlerAdapter{

	private static final Logger log = LoggerFactory.getLogger(CmsHppIoHandler.class);
	private HppIoIntercept hppIoIntercept;
	private String type;

	@Override
    public void fillProperty(HppIoIntercept hppIoIntercept, String type) throws NullPointerException, HppInitException {
		if (hppIoIntercept == null) {
			throw new NullPointerException("hpp拦截器不能为空，如果没有自己的实现，请默认实例化HppIoInterceptImpl");
		}
		if (StringUtils.isBlank(type)) {
			throw new HppInitException("HppIoHandler的type值必须不为空");
		} else if (!(StringUtils.equals(type, HppConstants.TYPE_SERVER) || StringUtils.equals(type, HppConstants.TYPE_CLIENT))) {
			throw new HppInitException("HppIoHandler的type值必须为HppConstants中预设的TYPE_SERVER或TYPE_CLIENT常量");
		}
		this.hppIoIntercept = hppIoIntercept;
		this.type = type;
    }

	/**
	 * 协议发送和接收的处理
	 * @author shanguoming 2011-7-6 上午09:27:58
	 * @param session 会话ID
	 * @param message 消息
	 * @throws Exception
	 */
	public void messageReceived(IoSession session, Object message) throws Exception {
		hppIoIntercept.beforeMessageReceived(session, message);
		long start = System.currentTimeMillis();
		HppMessage hppMessage = (HppMessage) message;
		if (hppMessage != null) {
			String resultCode = "";
			String msgKey = CmsProtoUtil.callbackKey(hppMessage.getService(), hppMessage.getCommand(), hppMessage.getSequence());
			Message msg = null;
			ProtoBean bean = CmsProtoUtil.builder(hppMessage.getCommand(), hppMessage.getProtobuf());
			if (bean != null && bean.getErrorCode() == 0) {
				if (hppMessage.getType() == HppConstants.REQUEST) {
					msg = bean.buildReq();
					hppIoIntercept.mssageReceived(session, hppMessage.getCommand(), msg);
					if (msg == null) {
						resultCode = "message build is null";
						nullResponse(session, hppMessage, null);
					} else {
						RpcControllerImpl controller = new RpcControllerImpl();
						controller.setSessionId(session.getId());
						HppChannel channel = ContextUtil.getHppChannel(type + session.getId());
						if (channel == null) {
							resultCode = "getHppChannel is null";
							nullResponse(session, hppMessage, null);
							return;
						} else {
							CmsHppServerCallback callback = new CmsHppServerCallback();
							callback.setMessage(hppMessage);
							callback.setChannel(channel);
							if (channel.addCallback(msgKey, callback)) {
								try {
									bean.getService().callMethod(bean.getMethod(), controller, msg, callback);
									resultCode = callback.getErrorCode();
								} catch (Exception e) {
									log.error("sessionId:" + session.getId() + ";callback key:" + msgKey + ";运行时异常:", new HppException(HppException.RPC_ERROR, e));
									resultCode = "rpc service run is error:" + e.getMessage();
									callback.run(null);
								}
							} else {
								resultCode = "addCallback is false";
								nullResponse(session, hppMessage, null);
							}
						}
					}
					long end = System.currentTimeMillis();
					String log = CmsProtoUtil.getSessionAddr(session) + ";key:" + msgKey + ";serviceName:" + bean.getFullServiceName() + ";result_code:" + resultCode + ";process time:<" + (end - start)
					        + ">ms";
					hppIoIntercept.afterMessageReceived(session, log);
				} else if (hppMessage.getType() == HppConstants.RESPONSE) {
					HppChannel channel = ContextUtil.getHppChannel(type + session.getId());
					if (channel != null) {
						HppCallback callback = channel.delCallback(msgKey);
						if (callback != null) {
							callback.run(bean.buildRsp());
							resultCode = callback.getErrorCode();
							long end = System.currentTimeMillis();
							String log = "type:"+type+"sessionId:" + session.getId() + ",key:" + msgKey + ",serviceName:" + bean.getFullServiceName() + ",result_code:" + resultCode + ",process time:<" + (end - start)
							        + ">ms";
							hppIoIntercept.afterMessageReceived(session, log);
						}
					}
				}
			} else {
				if (HppConstants.TYPE_SERVER.equals(type)) {
					nullResponse(session, hppMessage, null);
				}
			}
		}
	}

	/**
	 * 消息发送成功时的回调函数
	 * @author shanguoming 2011-7-6 上午09:28:42
	 * @param session 会话
	 * @param message 消息
	 * @throws Exception
	 */
	public void messageSent(IoSession session, Object message) throws Exception {
		hppIoIntercept.messageSent(session, message);
	}

	public void sessionCreated(IoSession session) throws Exception {
		// sessionCreated是主线程,尽量不要处理
		if (HppConstants.TYPE_SERVER.equals(type)) {
			HppChannel channel = ContextUtil.getHppChannel(type + session.getId());
			if (channel == null) {
				channel = new HppChannel();
				channel.setSession(session);
				InetSocketAddress sa = (InetSocketAddress)session.getServiceAddress();
				InetAddress ia = sa.getAddress();
				channel.setHost(ia.getHostAddress());
				channel.setPort(sa.getPort());
				channel.setType(type);
				ContextUtil.addHppChannel(type + session.getId(), channel);
			}
		}
	}

	public void sessionOpened(IoSession session) throws Exception {
		hppIoIntercept.sessionOpened(session);
	}

	public void sessionClosed(IoSession session) throws Exception {
		HppChannel channel = ContextUtil.delHppChannel(type + session.getId());
		CsClientContextUtil.delClientSession(type + session.getId());
		if (channel != null) {
			channel.clear();
		}
		hppIoIntercept.sessionClosed(session, type);
	}

	public void sessionIdle(IoSession session, IdleStatus status) throws Exception {
		if (session.isBothIdle()) {
			log.debug("SessionIdle:sessionId:" + session.getId() + " is idle " + session.getRemoteAddress() + ",so sessionClosed");
			HppChannel channel = ContextUtil.delHppChannel(type + session.getId());
			CsClientContextUtil.delClientSession(type + session.getId());
			if (channel != null) {
				channel.sessionClose(true);
			} else {
				session.close(true).addListener(IoFutureListener.CLOSE);
			}
		}
		hppIoIntercept.sessionIdle(session, status);
	}

	public void exceptionCaught(IoSession session, Throwable cause) throws Exception {
		hppIoIntercept.exceptionCaught(session, cause);
	}

	public void nullResponse(IoSession session, HppMessage message, Message msg) throws IOException {
		try {
			HppMessage response = new HppMessage();
			response.setType(HppConstants.RESPONSE);
			response.setCompress(message.isCompress());
			response.setSequence(message.getSequence());
			response.setFlag(HppConstants.FLAG_RESPONSE_COMPLETED);
			response.setService(message.getService());
			Integer command = CmsProtoUtil.getRspByReq(message.getCommand());
			if(command != null){
				response.setCommand(command);
			}
			if (msg == null) {
				response.setProtobuf(null);
			} else {
				response.setProtobuf(msg.toByteString());
			}
			session.write(response);
		} catch (Exception e) {
			log.error("sessionId:" + session.getId() + ";handle outputResponse err", e);
		}
	}

}
