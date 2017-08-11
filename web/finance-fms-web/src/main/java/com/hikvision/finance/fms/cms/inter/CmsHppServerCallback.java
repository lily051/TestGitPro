/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-3-11 下午3:08:38
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.inter;

import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.Message;
import com.hikvision.finance.fms.cms.inter.CmsHppServerCallback;
import com.hikvision.rpc.util.CmsProtoUtil;
import com.ivms6.core.hpp.HppCallback;
import com.ivms6.core.hpp.codec.HppConstants;
import com.ivms6.core.hpp.codec.HppMessage;
import com.ivms6.core.hpp.exception.HppException;

/**
 * <p></p>自定义请求之后回调
 * @author xujiangfei 2016-3-11 下午3:08:38
 * @version V1.0 
 */
public class CmsHppServerCallback extends HppCallback{
	private static final Logger loggerRsp = LoggerFactory.getLogger(CmsHppServerCallback.class);

	public void run(Message response) {
		String errorCode = "";
		if (getChannel() != null && getMessage() != null) {
			String key = CmsProtoUtil.callbackKey(getMessage().getService(), getMessage().getCommand(), getMessage().getSequence());
			if (getChannel().delCallback(key) == null) {
				errorCode = key + " is delete";
				setErrorCode(errorCode);
				return;
			}
			try {
				if (response != null) {
					String responseStr = response.toString();
					if(loggerRsp.isDebugEnabled()){
						loggerRsp.debug(CmsProtoUtil.getSessionAddr(getChannel().getSession()) + getMessage().toString() + "\r\n" + responseStr);
					}
					if (StringUtils.isNotBlank(responseStr) && (responseStr.indexOf("result_code:") >= 0)) {
						errorCode = responseStr.substring(responseStr.indexOf("result_code:") + 12).trim();
						if (errorCode.indexOf("}") >= 0) {
							errorCode = errorCode.substring(0, errorCode.indexOf("}")).trim();
						}
					}

				} else {
					errorCode = "response is null";
				}
				setErrorCode(errorCode);
				HppMessage msg = new HppMessage();
				msg.setType(HppConstants.RESPONSE);
				msg.setCompress(getMessage().isCompress());
				msg.setSequence(getMessage().getSequence());
				msg.setFlag(HppConstants.FLAG_RESPONSE_COMPLETED);
				msg.setService(getMessage().getService());
				Integer command = CmsProtoUtil.getRspByReq(getMessage().getCommand());
				if (command != null) {
					msg.setCommand(command);
				}
				if (response == null) {
					msg.setProtobuf(null);
				} else {
					msg.setProtobuf(response.toByteString());
				}
				if(loggerRsp.isDebugEnabled()){
					loggerRsp.debug(msg.toString());
				}
				getChannel().getSession().write(msg);
			} catch (Exception e) {
				errorCode = "RPC_FAILED";
				throw new HppException(HppException.RPC_FAILED, e);
			}
		} else {
			errorCode = "HppChannel is null and HppMessage is null";
		}
	}
}
