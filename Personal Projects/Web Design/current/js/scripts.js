function checkTime(i) {
  if (i < 10) {
    i = "0" + i;
  }
  return i;
}

function getDate(){
	
}

function startTime() {
	var mydate= new Date();
	var theyear=mydate.getFullYear();
	var themonth=mydate.getMonth()+1;
	var thetoday=mydate.getDate();
	
	var today = new Date(),
    h = checkTime(today.getHours()),
    m = checkTime(today.getMinutes()),
    s = checkTime(today.getSeconds());
	
	if (h > 13){
		h = h - 12;
	}
	
	document.getElementById('date').innerHTML = themonth+"/"+thetoday+"/"+theyear;
    document.getElementById('time').innerHTML = h + ":" + m + ":" + s;
    t = setTimeout(function () {
		startTime()
        }, 500);
    }
