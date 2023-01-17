<script type="module" src="https://md-block.verou.me/md-block.js"></script>
<script>
qtLoader = initQTwasm('https://raw.githubusercontent.com/EddyTheCo/BuyMeACoffe/main/wasm/', 'buymeacoffe', '#qtrootDiv', 'img/qtlogo.svg');

checkModuleLoad=setInterval(function() {
		if (qtLoader.module())
		{
			qtLoader.module().Monitor.get_monitor().set_properties("https://api.testnet.shimmer.network","rms1qrzgmpr0lzvqxzu73qakkvg7v2qd2lngkaf64w256c76vepag4sqs27e25s");
			clearInterval(checkModuleLoad);
		}

		if( typeof counter == 'undefined' ) {
			counter = 0;
		}
		counter++;
		if(counter>60)clearInterval(checkModuleLoad);
	}, 1000);
</script>
