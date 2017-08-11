var FMS = FMS || {};
FMS.analysis = {
	linkId:'#complianceStatistic',
	itemStyle:{
		emphasis: {
			label : {
				show : true,
				position:'center',
				formatter: '{b}\n\n{c}' 
			}
		},
        normal : {
            label : {
                show : true,
                position: 'center',
                formatter: '{b}\n\n{c}',
               	textStyle:{
               		color:'#fff'
               	}
            },
            labelLine : {
                show : false
            }
        }},
	isChild:'',
	orderData:'',//保存订单数据 用来排序
	complainData:'', //保存投诉数据 用来排序
	conditions:{
		regionId:'',
		saleType:0,
		saleMonthForm:'',
		saleQuarterForm:'',
		saleYearForm:'',
		complainType:0,
		complainMonthForm:'',
		complainQuarterForm:'',
		complainYearForm:'',
		recordStart:'',
		recordEnd:''
	},
	pieCheckChart:null,//录像检查结果的echarts
	pieComplianceChart:null,//录像合规结果的echarts
	globalSetting:function(){
		var dateDay = new Date().Format("yyyy-MM-dd");
		
		var beginDate = new Date(dateDay.replace(/-/g, '/'));
		beginDate.setDate(beginDate.getDate() - 2); // 前3天
		var startDate = beginDate.Format("yyyy-MM-dd");
		
		$("input[name='startTime']").val(startDate);
		$("input[name='endTime']").val(dateDay);
		//$("input[name='startTime'],input[name='endTime']").val(dateDay)
		for(var i=dateDay.substring(0,4);i>2000;i--){
			var newOption = $('<option value='+i+'>'+i+'</option>');
			$("select[name=staYear]").append(newOption);
		}
		FinanceUtil.initFormUI("form");
	},
	initOrgTree:function(){
		function nodeCreate(event, treeId, treeNode,type){
			if (treeNode.getParentNode() == null) {
				var treeObj = $.fn.zTree.getZTreeObj(treeId);
				treeObj.selectNode(treeNode);
				var node = treeObj.getSelectedNodes()[0];
				treeObj.expandNode(node, true,false ,true);
				FMS.analysis.conditions.regionId = treeNode.id.split('_')[1];
				var parentId = treeNode.id;
				var children=treeNode.children;
				/*try{
					var length = children.length;
				}catch(e){
					$("#businessNull").empty().addClass('businessNull').html("<span class='nullDate'>无数据</span>");
					return;
				}
				for(var i=0;i<children.length;i++){
					if(children[i].parentId==parentId){
						var id=children[i].tId;
//						$("#"+id).find('a').eq(0).addClass('children_active');
						//.prev('span').addClass('children_span_active');
					}
				}*/
				FMS.analysis.complianceStatistic();//初始化合规统计
			}
		}
		function nodeClick(event, treeId, treeNode){
			FMS.analysis.conditions.regionId = treeNode.id.split('_')[1];
//			$(".children_active").removeClass('children_active');
			/*$(".curSpanActive").removeClass('curSpanActive');
			$(".children_span_active").removeClass('children_span_active');
			$(".ztree li a.curSelectedNode").prev("span").addClass('curSpanActive');*/
			var parentId = treeNode.id;
			var children=treeNode.children;
			try{
				var length = children.length;
			}catch(e){
				return;
			}
			for(var i=0;i<children.length;i++){
				if(children[i].parentId==parentId){
					var id=children[i].tId;
//					$("#"+id).find('a').eq(0).addClass('children_active');
					//.prev('span').addClass('children_span_active');
				}
			}
			//$("#search").click();
		}
		FMS.regionTree.init('orgTree','/web/tree/getRegionTree.action',nodeCreate,nodeClick);
	},
	getSelectedTreeNode:function(){
		var treeInfo = $.fn.zTree.getZTreeObj('orgTree');
		var selNode = treeInfo.getSelectedNodes()[0];
		return selNode;
	},
	bindEvent:function(){
		var that = this;
		$("#tab-withoutBorder2").tabs({
			activate: function(e, obj) {
				var selector = obj.newPanel.selector;
				var context = obj.newTab.context;
				var clickNum=$(context).data("click");
				FMS.analysis.linkId = selector;
				if(clickNum==0){
					if(selector == "#businessStatistic"){
						//$(".ButtonPos").hide();
						that.businessStatistic();
						$(context).data("click",1);
					}
				}
			}
		});
		$("span[name=staButton]").on('click', function(event) {
			var judge = FMS.globalFun.compareTime($(that.linkId + " input[name='startTime']").val()+" 00:00:00",$(that.linkId + " input[name='endTime']").val()+" 23:59:59");
			if(judge){
				if(that.linkId == '#complianceStatistic'){
					that.complianceStatistic();
				}else if(that.linkId == '#businessStatistic'){
					$(".ButtonPos").show();
					that.businessStatistic();
				}
			}else{
				jAlert("开始时间和结束时间间隔不能大于1年！");
			}

		});
		$(".ButtonPos").on('click', 'a', function(event) {
			var target = event.target||event.srcElement;
			if($(this).hasClass('active')){
				return;
			}else{
				$(this).addClass('active').siblings().removeClass('active');
			}
			switch(target.id){
				case 'orderMost':
								if(that.orderData!=''){
									that.orderData.sort(creatComparisonFunction('value'));
									FMS.analysis.businessCompare('orderCompare',that.orderData,'订单排行榜','订单总量');
								}
								break;
				case 'orderLeast':
								if(that.orderData!=''){
									that.orderData.sort(creatComparisonAscFunction('value'));
									FMS.analysis.businessCompare('orderCompare',that.orderData,'订单排行榜','订单总量');
								}
								break;
				case 'compMost':
								if(that.complainData!=''){//降序排序
									that.complainData.sort(creatComparisonFunction('value'));
									FMS.analysis.businessCompare('complainCompare',that.complainData,'投诉排行榜','投诉总量');
								}
								break;
				case 'compLeast':
								if(that.complainData!=''){//升序排序
									that.complainData.sort(creatComparisonAscFunction('value'));
									FMS.analysis.businessCompare('complainCompare',that.complainData,'投诉排行榜','投诉总量');
								}
								break;
				default:break;

			}
		});
	},
	//合规统计
	complianceStatistic:function(){
		var select_link = this.linkId;
		this.conditions.recordStart = $(select_link + " input[name='startTime']").val()+" 00:00:00";
		this.conditions.recordEnd = $(select_link + " input[name='endTime']").val()+" 23:59:59";
		var ajaxData={
			'analysisInfo.nregionId':this.conditions.regionId,
			'analysisInfo.recordStart':this.conditions.recordStart,
			'analysisInfo.recordEnd':this.conditions.recordEnd
		};
		$.ajax({
			url:'/modules/analysis/getRateNum.action',//TODO 合规统计
			type:'post',
			dataType:'json',
			data:ajaxData,
			success:function(data){
				if(data&&data.success){
					var chartsData = data.data.analysisInfo;
					var columnMap = chartsData.columnMap,
						columnList = chartsData.columnList,
						peCheckList = chartsData.peCheckList,//检查未检查
						peList = chartsData.peList,//合规不合规
						nregionNum = chartsData.nregionNum!=null?chartsData.nregionNum:0,//已选组织
						rate = chartsData.rate!=null?chartsData.rate:0,
						checdRecord = chartsData.checdRecord!=null?chartsData.checdRecord:0;
					var sortData = [];
					for(var key in columnMap){
						sortData.push({name:key,value:columnMap[key]});
					}
					sortData.sort(creatComparisonFunction("value"));
					$(select_link + " label[name=sel-region]").text(nregionNum).attr("title",nregionNum);
					$(select_link + " label#checkedRecord").text(checdRecord).attr("title",checdRecord);
					$(select_link + " #comRate").text(Number(rate).toFixed(2)).attr("title",Number(rate).toFixed(2));
					$(select_link + " #rateHigh").text(sortData[0].name).attr("title",sortData[0].name);
					$(select_link + " #rateLow").text(sortData[sortData.length-1].name).attr("title",sortData[sortData.length-1].name);
					if(isIE6){
						$("#complianceInfo label").each(function(index, el) {
							var width = $(el).width();
							if(width>100){
								$(el).width(100);
							}
						});
					}
					FMS.analysis.checkChart('checkStatistic',peCheckList);
					FMS.analysis.complianceChart('normalStatistic',peList);
					FMS.analysis.recordCompareChart('recordCompare',columnList);
				}else{
					jAlert(data.msg,'错误','error');
				}
			}

		});
	},
	//业务统计
	businessStatistic:function(){
		var select_link = this.linkId;
		this.conditions.recordStart = $(select_link + " input[name='startTime']").val()+" 00:00:00";
		this.conditions.recordEnd = $(select_link + " input[name='endTime']").val()+" 23:59:59";
		var ajaxData={
			'analysisInfo.nregionId':this.conditions.regionId,
			'analysisInfo.recordStart':this.conditions.recordStart,
			'analysisInfo.recordEnd':this.conditions.recordEnd
		};
		$.ajax({
			url:'/modules/analysis/getOrdersNum.action',//TODO 业务统计
			type:'post',
			dataType:'json',
			data:ajaxData,
			success:function(data){
				if(data&&data.success){
					var chartsData = data.data.analysisInfo;
					var columnMapOrder = chartsData.columnMapOrder,
						columnMapComplaint = chartsData.columnMapComplaint,
						nregionNum = chartsData.nregionNum!=null?chartsData.nregionNum:0,//已选组织
						complaintOrder = chartsData.complaintOrder!=null?chartsData.complaintOrder:0,
						orderMax = chartsData.orderMax!=null?chartsData.orderMax:0,
						complaintMax = chartsData.complaintMax!=null?chartsData.complaintMax:0;
					var orderData = [],complainData = [];
					for(var key in columnMapOrder){
						orderData.push({name:key,value:columnMapOrder[key]});
					}
					for(var key in columnMapComplaint){
						complainData.push({name:key,value:columnMapComplaint[key]});
					}
					orderData.sort(creatComparisonFunction("value"));
					complainData.sort(creatComparisonFunction("value"));
					FMS.analysis.orderData = orderData;
					FMS.analysis.complainData = complainData;
					$(select_link + " label[name=sel-region]").text(nregionNum).attr("title",nregionNum);
					$(select_link + " label#signSum").text(orderMax).attr("title",orderMax);
					$(select_link + " label#complainSum").text(complaintMax).attr("title",complaintMax);
					$(select_link + " #complainRate").text(Number(complaintOrder).toFixed(2)).attr("title",Number(complaintOrder).toFixed(2));
					if(isIE6){
						$("#businessInfo label").each(function(index, el) {
							var width = $(el).width();
							if(width>140){
								$(el).width(140);
							}
						});
					}
					FMS.analysis.businessCompare('orderCompare',orderData,'订单排行榜','订单总量');
					FMS.analysis.businessCompare('complainCompare',complainData,'投诉排行榜','投诉总量');
				}else{
					jAlert(data.msg,'错误','error');
				}
			},
			error:function(xhr){
				
			}

		});
	},
	/**
	 * 业务统计图表
	 * @param  {[type]} id       div的id
	 * @param  {[type]} dataMap  要显示的数据
	 * @param  {[type]} title    图标标题
	 * @param  {[type]} showText tooltip里面显示的中文
	 * @return {[type]}          [description]
	 */
	businessCompare:function(id,dataMap,title,showText){
		var xList=[],dataList=[];
		try{
			for(var i=0;i<dataMap.length;i++){
				xList.push(dataMap[i].name);
				dataList.push(dataMap[i].value);
			}
			if(dataMap.length>30){
				var width = dataMap.length*40;
				$('#'+id).css("width",width+'px');
			}
		}catch(e){

		}
		var colorList = ['#A4C7F6','#cd5c5c','#da70d6','#32cd32'];
		var itemStyle = {
			normal: {
				color: function(params) {
				  return colorList[params.seriesIndex];
				}
			}
		};
		
	    var option = {
	        tooltip : {
	            trigger: 'axis',
				axisPointer: {
					type: 'shadow'
				}
	        },
	        title:{
	        	show:true,
	        	text:title,
	        	textStyle:{
	        		fontSize:14
	        	},
	        	padding:[20, 0, 0,40]
	        },
	        calculable : false,
			grid : {
				show:true,
				borderWidth:0,
				y2:100
			},
	        xAxis : [
	            {
	                type : 'category',
					splitLine:{show:false},//是否显示分割线
					axisLabel:{
	    				show:true,
	    				textStyle:{
	    					fontFamily:'Microsoft YaHei',
	    					fontSize:14
	    				}
					},
					axisLabel:{
						show:true,
						interval:0,
						rotate:35,
						formatter:function(param){
							var str = param;
							var bytesCount = 0;
						  	var i = 0;
						  	for (; i < str.length; i++) {
						  		var c = str.charAt(i);
						  		if (/^[\u0000-\u00ff]$/.test(c)) {//匹配双字节
						  			bytesCount += 1;
						  		} else {
						  			bytesCount += 2;
						  		}
						  		if(bytesCount >= 20) {
						  			break;
						  		}
						 	}
						  	if(bytesCount < 20) {
							  	return param;
						  	}
						  	return param.substring(0,i) + "...";
						}
					},
	                data : xList
	            }
	        ],
	        yAxis : [
	            {
	                type : 'value',
					splitLine:{
						show:true,
						lineStyle: {       // 属性lineStyle（详见lineStyle）控制线条样式
			                color: ['#ccc'],
			                width: 1,
			                type: 'dashed'
			            }
					},
					axisLine:{
						show:true
					},
					axisLabel:{
	    				show:true,
	    				textStyle:{
	    					fontFamily:'Microsoft YaHei',
	    					fontSize:14
	    				}
					},
	                splitArea : {show : false}
	            }
	        ],
	        series : [
	            {
	                name:showText,
	                type:'bar',
					itemStyle: itemStyle,
					barCategoryGap:'50%',
	                data:dataMap
	            }
	        ]
	    };
		var myChart = echarts.init(document.getElementById(id));
	    myChart.setOption(option);
	    myChart.on(echarts.config.EVENT.CLICK, function(param){
			//alert(1);
		});
	},
	//录像检查结果
	checkChart:function(id,dataMap){
		$("#"+id).empty();
		if(FMS.analysis.pieCheckChart != null){
			FMS.analysis.pieCheckChart.clear();
			FMS.analysis.pieCheckChart = null;
		}
		if(dataMap[0].value=="0"&&dataMap[1].value=="0"){
			dataMap=[];
			$("#"+id).append('<div class="pieNoData">无数据</div>');
			return;
		}else{
			if(dataMap[0].value=="0"){
				dataMap[1].itemStyle=FMS.analysis.itemStyle;
			}else if(dataMap[1].value=="0"){
				dataMap[0].itemStyle=FMS.analysis.itemStyle;
			}
			var legendData= [{name:dataMap[0].name,icon:'bar'},{name:dataMap[1].name,icon:'bar'}];
		}
		var colorList = ['#AAB2FF','#98CDFA'];
		option = {
		    tooltip : {
		        trigger: 'item',
		        formatter: "{a} <br/>{b} : {c} ({d}%)"
		    },
		    legend: {
		    	show:true,
		        orient: 'vertical',
		        data: legendData,
		        x:'right',
		        y:'center',
		        textStyle:{
		        	color:'auto'
		        },
		        selectedMode:false
		    },
		    color:colorList,
		    series : [
		        {
		            name: '检查结果',
		            type: 'pie',
		            radius : '40%',
		            center: ['50%', '50%'],
        		    /*label: {
                        normal: {
                            show: true,
                            position: 'inside',
                            formatter: '{b}\n\n{c}'
                        },
		                emphasis: {
		                    show: true,
		                    textStyle: {
		                        fontSize: '30',
		                        fontWeight: 'bold'
		                    }
		                }
                    },*/
		            data:dataMap,
		            itemStyle: {
		                emphasis: {
		                    shadowBlur: 10,
		                    shadowOffsetX: 0,
		                    shadowColor: 'rgba(0, 0, 0, 0.5)',
		                    label : {
                               show : true,
                               position : 'inside',
                               formatter: function(params){
                               		if(params.value == 0){
                               			return "";
                               		}else{
                               			return params.name+"\n\n"+params.value;
                               		}
                               		
                               }
                              /* textStyle : {
                                   fontSize : '30',
                                   fontWeight : 'bold'
                               }*/
                           }
		                },
		                normal : {
                            label : {
                                show : true,
                                position: 'inside',
                                formatter: function(params){
                               		if(params.value == 0){
                               			return "";
                               		}else{
                               			return params.name+"\n\n"+params.value;
                               		}
                               		
                               	}
                            },
                            labelLine : {
                                show : false
                            }
                        }
		            }
		        }
		    ]
		};
		FMS.analysis.pieCheckChart = echarts.init(document.getElementById(id));
	    FMS.analysis.pieCheckChart.setOption(option);
	    FMS.analysis.pieCheckChart.on(echarts.config.EVENT.CLICK, function(param){
			//alert(1);
		});
	},
	//录像合规结果
	complianceChart:function(id,dataMap){
		$("#" + id).empty();
		if(FMS.analysis.pieComplianceChart != null){
			FMS.analysis.pieComplianceChart.clear();
			FMS.analysis.pieComplianceChart = null;
		}
		if(dataMap[0].value=="0"&&dataMap[1].value=="0"){
			dataMap=[];
			$("#" + id).append('<div class="pieNoData">无数据</div>');
			return;
		}else{
			if(dataMap[0].value=="0"){
				dataMap[1].itemStyle=FMS.analysis.itemStyle;
			}else if(dataMap[1].value=="0"){
				dataMap[0].itemStyle=FMS.analysis.itemStyle;
			}
			var legendData= [{name:dataMap[0].name,icon:'bar'},{name:dataMap[1].name,icon:'bar'}];
		}
		var colorList = ['#AAB2FF','#98CDFA'];
		option = {
		    tooltip : {
		        trigger: 'item',
		        formatter: "{a} <br/>{b} : {c} ({d}%)"
		    },
		    legend: {
		    	show:true,
		        orient: 'vertical',
		        data: legendData,
		        x:'right',
		        y:'center',
		        textStyle:{
		        	color:'auto'
		        },
		        selectedMode:false
		    },
		    color:colorList,
		    series : [
		        {
		            name: '检查结果',
		            type: 'pie',
		            radius : '40%',
		            center: ['50%', '50%'],
		            data:dataMap,
		            itemStyle: {
		                emphasis: {
		                    shadowBlur: 10,
		                    shadowOffsetX: 0,
		                    shadowColor: 'rgba(0, 0, 0, 0.5)',
		                    label : {
                               show : true,
                               position : 'inside',
                               formatter: function(params){
                               		if(params.value == 0){
                               			return "";
                               		}else{
                               			return params.name+"\n\n"+params.value;
                               		}
                               		
                               	}
                           }
		                },
		                normal : {
                            label : {
                                show : true,
                                position: 'inside',
                                formatter: function(params){
                               		if(params.value == 0){
                               			return "";
                               		}else{
                               			return params.name+"\n\n"+params.value;
                               		}
                               		
                               	}
                            },
                            labelLine : {
                                show : false
                            }
                        }
		            }
		        }
		    ]
		};
		FMS.analysis.pieComplianceChart = echarts.init(document.getElementById(id));
	    FMS.analysis.pieComplianceChart.setOption(option);
	    FMS.analysis.pieComplianceChart.on(echarts.config.EVENT.CLICK, function(param){
			//alert(1);
		});
	},
	//各组织合规率比较
	recordCompareChart:function(id, dataMap){
		var xList=[],dataList=[];
		try{
			for(var i=0;i<dataMap.length;i++){
				xList.push(dataMap[i].name);
				dataList.push((parseInt)((dataMap[i].value)*100));
			}
			if(dataMap.length>30){
				var width = dataMap.length*40;
				$('#'+id).css("width",width+'px');
			}
		}catch(e){

		}
		
		var colorList = ['#A4C7F6','#cd5c5c','#da70d6','#32cd32'];
		var itemStyle = {
			normal: {
				color: function(params) {
				  return colorList[params.seriesIndex];
				}
			}
		};
		
	    var option = {
	        tooltip : {
	            trigger: 'axis',
				axisPointer: {
					type: 'shadow'
				},
				formatter:function(params,ticket,callback){
					return params[0].name+"<br/>" + params[0].seriesName +":"+Number(params[0].value)+"%";
				}
	        },
	       /* toolbox:{
	        	show:true,
	        	feature : {
    	           print : {
    	           		show:true,
    	           		title:'打印',
    	           		icon:'/images/icons/add.png',
    	           		onclick:function(){
    	           			
    	           		}
    	           }
	        	}
	        },*/
	        title:{
	        	show:true,
	        	text:'各组织合规率比较',
	        	textStyle:{
	        		fontSize:14
	        	},
	        	padding:[20, 0, 0,40]
	        },
	        calculable : false,
			grid : {
				show:true,
				borderWidth:0,
				y2:100
			},
	        xAxis : [
	            {
	                type : 'category',
					splitLine:{show:false},//是否显示分割线
					axisLabel:{
	    				show:true,
	    				textStyle:{
	    					fontFamily:'Microsoft YaHei',
	    					fontSize:14
	    				}
					},
					axisLabel:{
						show:true,
						interval:0,
						rotate:35,
						formatter:function(param){
							var str = param;
							var bytesCount = 0;
						  	var i = 0;
						  	for (; i < str.length; i++) {
						  		var c = str.charAt(i);
						  		if (/^[\u0000-\u00ff]$/.test(c)) {//匹配双字节
						  			bytesCount += 1;
						  		} else {
						  			bytesCount += 2;
						  		}
						  		if(bytesCount >= 20) {
						  			break;
						  		}
						 	}
						  	if(bytesCount < 20) {
							  	return param;
						  	}
						  	return param.substring(0,i) + "...";
						}
					},
	                data : xList
	            }
	        ],
	        yAxis : [
	            {
	                type : 'value',
	                min:0,
					max:100,
					name : '合规率(%)',
                    nameLocation : 'end',
                    nameTextStyle : {
                    	color:'#333333',
                        fontFamily:'宋体',
                        fontSize:12
                    },
					splitLine:{
						show:true,
						lineStyle: {       // 属性lineStyle（详见lineStyle）控制线条样式
			                color: ['#ccc'],
			                width: 1,
			                type: 'dashed'
			            }
					},
					axisLine:{
						show:true
					},
					axisLabel:{
	    				show:true,
	    				textStyle:{
	    					fontFamily:'Microsoft Yahei',
	    					fontSize:12
	    				}
					},
	                splitArea : {show : false}
	            }
	        ],
	        series : [
	            {
	                name:'合规率',
	                type:'bar',
					itemStyle: itemStyle,
					barCategoryGap:'50%',
	                data:dataList
	            }
	        ]
	    };
		
		var myChart = echarts.init(document.getElementById(id));
	    myChart.setOption(option);
	    myChart.on(echarts.config.EVENT.CLICK, function(param){
		});
	}
};
$(function(){
	//FMS.analysis.initOrgTree();
	/*$('.sideBar li').on('click',function(){
		var url = $(this).data('url');
		$("#main-wrap").load(url);
		$('#sideBar li').removeClass('active');
		$(this).addClass('active');
	});
	$('#sideBar li').eq(0).click();*/
	$("#main-wrap").load('/modules/analysis/toBusinessSts.action');
});
//降序
 function creatComparisonFunction(propertyName) {
 	return function(object1, object2){
 		var value1 = object1[propertyName];
 		var value2 = object2[propertyName];

 		if(value1 < value2){
 			return 1;
 		} else if(value1 > value2){
 			return -1;
 		} else {
 			return 0;
 		}
 	}
 }
//升序
 function creatComparisonAscFunction(propertyName) {
 	return function(object1, object2){
 		var value1 = object1[propertyName];
 		var value2 = object2[propertyName];

 		if(value1 < value2){
 			return -1;
 		} else if(value1 > value2){
 			return 1;
 		} else {
 			return 0;
 		}
 	}
 }

 //data.sort(creatComparisonFunction("value"));
 // alert(data[0].value);