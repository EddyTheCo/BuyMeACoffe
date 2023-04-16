function init() {
	initQTwasm('.', 'buymeacoffe', '#qtrootDiv', 'img/qtlogo.svg');
	checkModuleLoad = setInterval(() => {
		if (qtLoader.module()) {
			valChanged();
			resizeSplitX();
			clearInterval(checkModuleLoad);
		}

		if (typeof counter === 'undefined') {
			counter = 0;
		}

		counter++;
		if (counter > 60) {
			clearInterval(checkModuleLoad);
		}
	}, 1000);

}


function resizeSplitX(event) {
	const canvas = document.querySelector('#screen');
	qtLoader.resizeCanvasElement(canvas);
}
function valChanged()
{
	nodaddr= document.getElementById('nodeaddr').value;
	addr= document.getElementById('addr').value;
	console.log("node:",nodaddr);
	console.log("addr:",addr);
		if (qtLoader.module()) {
			qtLoader.module().Monitor.get_monitor().set_properties(nodaddr,addr);
		}
}
