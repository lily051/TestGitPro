/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月15日 下午8:32:12
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.record.dto;


/**
 * <p></p>
 * @author jinchenwu 2016年4月15日 下午8:32:12
 * @version V1.0 
 */
public class RecordDto {
	
	private RecordInfo recordInfo;
	private PlaybackInfo playbackInfo;
	
	public RecordInfo getRecordInfo() {
		return recordInfo;
	}
	
	public void setRecordInfo(RecordInfo recordInfo) {
		this.recordInfo = recordInfo;
	}
	
	public PlaybackInfo getPlaybackInfo() {
		return playbackInfo;
	}
	
	public void setPlaybackInfo(PlaybackInfo playbackInfo) {
		this.playbackInfo = playbackInfo;
	}
}
