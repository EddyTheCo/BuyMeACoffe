function init() {
	initQTwasm('.', 'buymeacoffe', '#qtrootDiv', 'img/qtlogo.svg');
	checkModuleLoad = setInterval(() => {
		if (qtLoader.module()) {
			qtLoader.module().Monitor.get_monitor().set_properties("https://api.testnet.shimmer.network","rms1qrzgmpr0lzvqxzu73qakkvg7v2qd2lngkaf64w256c76vepag4sqs27e25s");
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

function LoadFile(readme) {
	axios.get(readme).then(response => {
		document.querySelector('#md').mdContent = response.data;
	}).catch(error => {
		console.log(error);
	});
}

function resizeSplitX(event) {
	const canvas = document.querySelector('#screen');
	qtLoader.resizeCanvasElement(canvas);
}
