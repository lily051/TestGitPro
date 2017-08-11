var FMS = FMS ||{};
FMS.index = {
	dueList:'',
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
    pieIndexChart:null,
	init:function(){
		this.setHeight();
		//$(document).pngFix();
		this.initStyle();
		this.getAlarmInfo();
		this.getDueInfo();
		this.complianceStatistic();
		this.bindEvent();
	},
	bindEvent:function(){
		$(window).resize(function(event) {
			FMS.index.setHeight();
			var dueHeight = $("#dueInfo").height()-21;
			var num = Math.floor(dueHeight/67);
			var list = FMS.index.dueList;
			$("#dueInfoList").empty();
			for(var i=0;i<list.length&&i<num;i++){
				var newLi = $('<li class="show-info-li"><p class="show-info-p p-pointer">'+list[i].nproId+'</p>'+
					'<p class="show-info-p tip-time">到期时间：'+list[i].strVideoEndDate+'</p></li>');
				if(i==list.length-1||i==num-1){
					newLi.addClass('lastRow');
				}
				$("#dueInfoList").append(newLi);
			}
			//FMS.index.complianceStatistic();
		});
	},
	setHeight:function(){
		$("#content-inner").height($("#content").height());
		var outerHeight = $("#chartOut").outerHeight();/*
		if(outerHeight-60<306){
			$(".indexChart").height(outerHeight-60);
		}else{
			$(".indexChart").height(306);
		}*/
		
	},
	initStyle:function(){
		$("#navbarIndex").addClass('active');
		$(".linkIcon a").hover(function() {
			var img = $(this).find('img');
			var defaultSrc = img.attr("src");
			var newSrc=img.data("src");
			img.data('src', defaultSrc);
			img.attr("src",newSrc);
		}, function() {
			var img = $(this).find('img');
			var defaultSrc = img.attr("src");
			var newSrc=img.data("src");
			img.data('src', defaultSrc);
			img.attr("src",newSrc);
		});
	},
	getAlarmInfo:function(){
		$.ajax({
			url:'/web/getIndexAlarm.action',
			type:'post',
			dataType:'json',
			success:function(data){
				if(data&&data.success){
					var list = data.data.alarmlogList;
					for(var i=0;i<list.length&&i<5;i++){
						var newLi = $('<li class="show-info-li"><p class="show-info-p p-pointer" title="'+list[i].comments+'">'+list[i].alarmTypeName+'（'+list[i].strHost+'）</p><p class="show-info-p tip-time">'+list[i].alarmTime+'</p></li>');
						if(i==4){
							newLi.addClass('lastRow');
						}
						$("#alarmInfo").append(newLi);
					}
				}
			}
		});
	},
	getDueInfo:function(){
		$.ajax({
			url:'/web/getIndexDueRecord.action',
			type:'post',
			dataType:'json',
			success:function(data){
				if(data&&data.success){
					var dueHeight = $("#dueInfo").height()-21;
					var num = Math.floor(dueHeight/63);
					var list = data.data.dueRecordInfoList;
					FMS.index.dueList = list;
					for(var i=0;i<list.length&&i<num;i++){
						var newLi = $('<li class="show-info-li"><p class="show-info-p p-pointer">'+list[i].nproId+'</p>'+
							'<p class="show-info-p tip-time">到期时间：'+list[i].strVideoEndDate+'</p></li>');
						if(i==list.length-1||i==num-1){
							newLi.addClass('lastRow');
						}
						$("#dueInfoList").append(newLi);
					}
				}
			}
		});
	},
	complianceStatistic:function(){
		$.ajax({
			url:'/web/getIndexRateNum.action',
			type:'post',
			dataType:'json',
			success:function(data){
				if(data&&data.success){
					var chartsData = data.data.analysisInfo;
						peCheckList = chartsData.peCheckList,//检查未检查
						peList = chartsData.peList,//合规不合规
						columnMap = chartsData.columnMap;//各组织合规率
					var columnList = chartsData.columnList;

					var sortData = [];
					for(var key in columnMap){
						sortData.push({name:key,value:columnMap[key]});
					}
					FMS.index.pieChart('checkStatistic',peCheckList,'本周录像检查结果');
					FMS.index.barChart('complainceComp',columnList,'本周各组织合规率')
					//FMS.index.pieChart('normalStatistic',peList,'本周合规检查结果');
				}else{
					jAlert(data.msg,'错误','error');
				}
			}

		});
	},
	pieChart:function(id, dataMap, titleInfo){
		var legendData;
		$("#" + id).empty();
		if(FMS.index.pieIndexChart != null){
			FMS.index.pieIndexChart.clear();
			FMS.index.pieIndexChart = null;
		}
		if(dataMap[0].value=="0"&&dataMap[1].value=="0"){
			dataMap=[];
			$("#checkStatisticLegend").hide();
			$("#"+id).append('<div id="checkStatisticNone" class="indexChart"><span class="span-center">无数据</span></div>');
			return;
		}else{
			if(dataMap[0].value=="0"){
				dataMap[1].itemStyle=FMS.index.itemStyle;
			}else if(dataMap[1].value=="0"){
				dataMap[0].itemStyle=FMS.index.itemStyle;
			}
			legendData= [{name:dataMap[0].name,icon:'bar'},{name:dataMap[1].name,icon:'bar'}];
		}
		var colorList = ['#AAB2FF','#98CDFA'];
		var titleH = $("#"+id).height()-95;
		option = {
		    title : {
		    	show:false,
		        text: titleInfo,
		        textStyle:{
	        		fontSize:12,
	        		color:'#b9b9b9'
	        	},
	        	x:'center'
		    },
		    tooltip : {
		        trigger: 'item',
		        position: function(point, params, dom){
		        	var width = $("#checkStatistic").width();
		        	if(point[0] + 140 > width) {
		        		return [point[0] - 140,point[1]];
		        	}
		        	// if(point[0]>140){
		        	// 	return [140,point[1]];
		        	// }else {
		        	// 	return [point[0],point[1]];
		        	// }
		        },
		        formatter: "{a} <br/>{b} : {c} ({d}%)"
		    },
		    legend: {
		    	show:false,
		        orient: 'horizontal',
		        data: legendData,
		        y:titleH,
		        itemGap:20,
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
		            radius : '50%',
		            center: ['50%', '33%'],
		            data:dataMap,
		            itemStyle: {
		                emphasis: {
		                    shadowBlur: 10,
		                    shadowOffsetX: 0,
		                    shadowColor: 'rgba(0, 0, 0, 0.5)',
		                    label : {
                               show : true,
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
                               		
                               	},
                               	textStyle:{
                               		color:'#fff'
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
		FMS.index.pieIndexChart = echarts.init(document.getElementById(id));
	    FMS.index.pieIndexChart.setOption(option);
	    FMS.index.pieIndexChart.on(echarts.config.EVENT.CLICK, function(param){
			//alert(1);
		});
	},
	barChart:function(id, dataMap, titleInfo){
		var xList=[],dataList=[];
		try{
			for(var i=0;i<dataMap.length&&i<6;i++){
				xList.push(dataMap[i].name);
				dataList.push((parseInt)((dataMap[i].value)*100));
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
					var paramsName = params[0].name;
					if(paramsName.length>25){
						return paramsName.substring(0,25)+"<br/>"+ paramsName.substring(25,paramsName.length)+"<br/>"+params[0].seriesName +":"+Number(params[0].value)+"%";
					}else{
						return paramsName+"<br/>" + params[0].seriesName +":"+Number(params[0].value)+"%";
					}
					
				}
	        },
	        title:{
	        	show:false,
	        	text:titleInfo/*'各组织合规率比较'*/,
	        	textStyle:{
	        		fontSize:14,
	        		color:'#b9b9b9'
	        	},
	        	x:'center',
	        	padding:[20,0,0,0]
	        },
	        calculable : false,
			grid : {
				show:true,
				borderWidth:0,
				x:40,
				y:30,
				y2:90,
				x2:20
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
					splitLine:{//分割线
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
			//alert(1);
		});
	}
};

$(function(){
	//$(document).pngFix();
	FMS.index.init();
});
/*window.onbeforeunload = function(){
	/*if(document.all){ 
	   var n = window.event.screenX - window.screenLeft;    
	   var b = n > document.documentElement.scrollWidth-20; 
	   if(b || window.event.clientY < 0 || window.event.altKey){    
	          alert("IE关闭"); 
	   } 
	} 
	var isIE=document.all?true:false;  
	    if(isIE){//IE浏览器  
	        var n = window.event.screenX - window.screenLeft;  
	        var b = n > document.documentElement.scrollWidth-20;  
	        if(b || window.event.clientY<0 || window.event.altKey){  
	            alert("是关闭而非刷新");  
	        }else{  
	            alert("是刷新而非关闭");  
	        }  
	    } 
}
window.onunload = function(){
	alert("unload");
}
window.onload = function(){
	alert("load");
}*/