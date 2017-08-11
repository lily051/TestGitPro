<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<link rel="stylesheet" type="text/css" href="/modules/analysis/css/analysis.css" />
<script src="/modules/analysis/js/businessSta.js"></script>
<body>
	<div class="main-wrap wrapFixed" id="businessNull">
		<div class="fullfit">
			<div id="tab-withoutBorder2" class="tab-container tab-withoutBorder">
	            <ul class="tabs">
	               <li><a href="#complianceStatistic" data-click='0'>合规统计</a></li>
	               <li><a href="#businessStatistic" data-click='0'>业务统计</a></li>
	               <!-- <li><a href="#saleStatistic" data-click='0'>销售统计</a></li>
	               <li><a href="#complainStatistic" data-click='0'>投诉统计</a></li> -->
	            </ul>
       			<div class="tab-content" id="tab-content">
       				 <div id="complianceStatistic"><!-- 合规统计 -->
       			    	<div class="filterFormBox ieStyle">
	    				    <form class="form-horizontal" id="compliancefilterForm">
	    				        <fieldset class="noLegend fieldset_50">
	    					    	<div class="control-group">
	    					    		<label class="control-label">统计时间区间：</label>
	    					    		<div class="controls conditions">
	    					    			<input type="text" class="auto-input" hikui="calendar" isShowClear="false" readonly="readonly"
	    					    			datefmt="yyyy-MM-dd" name="startTime" maxdate="#F{$dp.$N('endTime');}">
	    					    			<label for="">—</label>
	    					    			<input type="text" class="auto-input" hikui="calendar" isShowClear="false" readonly="readonly"
	    					    			datefmt="yyyy-MM-dd" name="endTime" mindate="#F{$dp.$N('startTime');}">
	    					    		</div>
	    					    	</div>
	    					    </fieldset>
	    				        <div class="filterDIV filterBtn">
	    				            <span class="btn btn-submit blockBtn" name="staButton">统计</span>
	    				        </div>
	    				    </form>
	    				</div>
	    				<div class="staInfo" id="complianceInfo">
	    					<div class="info sel-region">
	    						<div class="top" data-image="/images/statistic/sel-region.png"><label name="sel-region"></label><span>个</span></div>
	    						<div class="bottom" data-color="#EF9383">已选组织</div>
	    					</div>
	    					<div class="info sale-all">
	    						<div class="top" data-image="/images/statistic/checkedVideo.png"><label id="checkedRecord"></label><span>个</span></div>
	    						<div class="bottom" data-color="#67C593">已选录像</div>
	    					</div>
	    					<div class="info sale-pro">
	    						<div class="top" data-image="/images/statistic/comRate.png"><label id="comRate"></label><span>%</span></div>
	    						<div class="bottom" data-color="#40B9E8">合规率</div>
	    					</div>
	    					<div class="info sale-champion">
	    						<div class="top" data-image="/images/statistic/rate-high.png"><label id="rateHigh" style="font-size:20px;max-width:100px;_width:100px"></label></div>
	    						<div class="bottom" data-color="#EA798B">合规率最高</div>
	    					</div>
	    					<div class="info sale-best">
	    						<div class="top" data-image="/images/statistic/rate-low.png"><label id="rateLow" style="font-size:20px;max-width:100px;_width:100px"></label></div>
	    						<div class="bottom" data-color="#F3D64D">合规率最低</div>
	    					</div>
	    				</div>
	    				<div class="chartInfo">
                                          <div class="pieTitle clearfix">
                                                 <div class="halfTitle" style="float:left">录像检查结果</div>
                                                 <div class="halfTitle" style="float:right">录像合规结果</div>
                                          </div>
	    					<div id="checkStatistic" style="height:400px;width:400px;float:left"></div>
	    					<div id="normalStatistic" style="height:400px;width:400px;float:right;"></div>
						<div class="barChart">
                                                 <div id="recordCompare" class="barChartsDiv"></div>
						</div>
							 
	    				</div>
       			    </div>
       			    <div id="businessStatistic">
       			    	<div class="filterFormBox ieStyle">
	    				    <form class="form-horizontal" id="compliancefilterForm">
	    				        <fieldset class="noLegend fieldset_50">
	    					    	<div class="control-group">
	    					    		<label class="control-label">统计时间区间：</label>
	    					    		<div class="controls conditions">
	    					    			<input type="text" class="auto-input" hikui="calendar" isShowClear="false" readonly="readonly"
	    					    			datefmt="yyyy-MM-dd" name="startTime" maxdate="#F{$dp.$N('endTime');}">
	    					    			<label for="">—</label>
	    					    			<input type="text" class="auto-input" hikui="calendar" isShowClear="false" readonly="readonly"
	    					    			datefmt="yyyy-MM-dd" name="endTime" mindate="#F{$dp.$N('startTime');}">
	    					    		</div>
	    					    	</div>
	    					    </fieldset>
	    				        <div class="filterDIV filterBtn">
	    				            <span class="btn btn-submit blockBtn" name="staButton">统计</span>
	    				        </div>
	    				    </form>
	    				</div>
	    				<div class="staInfo" id="businessInfo">
	    					<div class="info kind4">
	    						<div class="top" data-image="/images/statistic/sel-region.png"><label name="sel-region" class="busiSta"></label><span>个</span></div>
	    						<div class="bottom" data-color="#EF9383">已选组织</div>
	    					</div>
	    					<div class="info kind4">
	    						<div class="top" data-image="/images/statistic/signSum.png"><label id="signSum" class="busiSta"></label><span>个</span></div>
	    						<div class="bottom" data-color="#67C593">签约单总数</div>
	    					</div>
	    					<div class="info kind4">
	    						<div class="top" data-image="/images/statistic/complainSum.png"><label id="complainSum" class="busiSta"></label><span>个</span></div>
	    						<div class="bottom" data-color="#40B9E8">投诉单总数</div>
	    					</div>
	    					<div class="info kind4">
	    						<div class="top" data-image="/images/statistic/complainRate.png"><label id="complainRate" class="busiSta"></label><span>%</span></div>
	    						<div class="bottom" data-color="#EA798B">投诉率</div>
	    					</div>
	    				</div>
	    				<div class="chartInfo">
	    					<div class="barChart">
							 	<div id="orderCompare" class="barChartsDiv"></div>
							</div>
							<div class="barChart">
							 	<div id="complainCompare" class="barChartsDiv"></div>
							</div>
							<div id="orderButton" class="ButtonPos"><a class="chartButton active" id="orderMost">订单最多</a><a class="chartButton" id="orderLeast">订单最少</a></div>
							<div id="complainButton" class="ButtonPos"><a class="chartButton active" id="compMost">投诉最多</a><a class="chartButton" id="compLeast">投诉最少</a></div>
	    				</div>
       			    </div>
       			</div>
       		</div>
		</div>
	</div>
	<script>
		
	</script>
</body>
</html>


