function is_same(l_start,l_end,p,q){
	 var dx = l_end.x - l_start.x;
     var dy = l_end.y - l_start.y;
 
     var dx1= p.x - l_start.x;
     var dy1= p.y - l_start.y;
     
     var dx2= q.x - l_end.x;
     var dy2= q.y - l_end.y;
 
     return ((dx*dy1-dy*dx1)*(dx*dy2-dy*dx2) > 0? 1 : 0);
}
function is_intersect(s1_start, s1_end, s2_start, s2_end){
	return (is_same(s1_start, s1_end, s2_start, s2_end)==0 && 
		is_same(s2_start, s2_end, s1_start, s1_end)==0)? 1: 0;

}

function isinpolygon(polygon,point){
	var extent = vertices_get_extent(polygon,polygon.length);
    if (point.x < extent.min_x || point.x > extent.max_x || point.y < extent.min_y || point.y > extent.max_y)
         return 0;
    var newpoint = {x:extent.max_x+1,y:point.y};
	var res=0;
	for(var i=1;i<polygon.length;i++){
		if(is_intersect(polygon[i-1],polygon[i],point,newpoint)){
			res++;
		}else if(polygon[i].y==point.y&&polygon[i].x>point.x){//每条线算其最后个点是否相交
			res++;
		}
	}
	return   res%2;
}
//获取矩形框
function vertices_get_extent(vl, np)
{
     var i;
     var min_x=0,min_y=0,max_x=0,max_y=0;
     if (np > 0){
         min_x = max_x = vl[0].x; 
         min_y = max_y = vl[0].y;
     }
 
     for(i=1; i<np; i++)
     {
         if(vl[i].x < min_x) min_x = vl[i].x;
         if(vl[i].y < min_y) min_y = vl[i].y;       
         if(vl[i].x > max_x) max_x = vl[i].x;
         if(vl[i].y > max_y) max_y = vl[i].y;
     }
     var json = {'min_x':min_x,"min_y":min_y,"max_x":max_x,"max_y":max_y};
     return json;
}
//面数据转成串
function getjsonString( obj ){
	var objarr=obj[0];
	var typeNO = objarr.length;
	var jarr = "[";
	for (var i = 0 ;i < typeNO ; i++){
	 	jarr+="{x:'"+objarr[i][0]+"',y:'"+objarr[i][1]+"'}";
	 	 if(i<typeNO-1){
	 		 jarr+=",";
	 	 };
	}
	jarr+="]";
	return jarr;
}

//面数据转换成json数组
function getPolygonJson(obj){
	var objarr=obj[0];
	var typeNO = objarr.length;
	var jarr = new Array;
	for (var i = 0 ;i < typeNO ; i++){
		jarr.push({x:objarr[i][0],y:objarr[i][1]});
	}
	return jarr;
}