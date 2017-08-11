function onIndex(){
	window.history.go("-1");
	/*if (self != top) {
		var serviceURL = top.location.href;
		var serviceIP = serviceURL.substring(serviceURL.indexOf('://')+3);
		serviceIP = serviceIP.substring(0,serviceIP.indexOf('/'));
		top.location.href = "http://"+serviceIP;
	} else {
		var serviceURL = window.location.href;
		var serviceIP = serviceURL.substring(serviceURL.indexOf('://')+3);
		serviceIP = serviceIP.substring(0,serviceIP.indexOf('/'));
		window.location.href = "http://"+serviceIP;
	}*/
}