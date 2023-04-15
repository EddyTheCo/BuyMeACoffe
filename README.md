# [BuyMeACoffe](https://eddytheco.github.io/BuyMeACoffe/wasm/index.html)

This repo produce a wasm file able to monitor certain IOTA-address(Startdust).
The application will show the total amount of funds (from basic outputs) on the address and also the metadata of the new transfer funds to that address.
This can be used as a BuyMeACoffe app that one can embed on your website.
Notice that the application do not collect the new outputs created on your address.
Due to that, one needs to take care of collecting the dust on your address.

### How to embed it into your webpage

* Include [qtloader.js](https://eddytheco.github.io/BuyMeACoffe/wasm/qtloader.js)
* Include [initQTwasm](https://eddytheco.github.io/BuyMeACoffe/wasm/js/initQTwasm.js) in your JavaScripts.

* Select an element of your webpage where you want the BuyMeACoffe to show and set the 'id'.
like:

```html
 <div id="qtrootDiv">
 </div>
```

* Load  the wasm and emscriptem module with:

```js
qtLoader = initQTwasm('.', 'buymeacoffe', '#qtrootDiv', 'img/qtlogo.svg');

```

where the first argument is the address where the buymeacoffe.wasm and buymeacoffe.js are.
The second argument is the name of the application, #qtrootDiv represent the element id where you want to show it.
And the last argument is the logo to show when the application is loading. 

* Check that the module was created and once created set the node address and Startdust address(from javascript) for recieving payments like :

```js
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
``` 


## How to compile the .wasm

The only dependencies of this code are [Qt](https://www.qt.io/) and [Libsodium](https://doc.libsodium.org/).

It is necessary to have installed [Qt for WebAssembly](https://doc.qt.io/qt-6/wasm.html) and [libsodium.js](https://github.com/jedisct1/libsodium.js). 

Clone the repo

```
git clone git@github.com:EddyTheCo/BuyMeACoffe.git 
```

Create build directory and build from there. One needs to use the qt-cmake from wasm_32 folder.

```
mkdir build 
cd build
qt-cmake -DCMAKE_BUILD_TYPE=Release ../BuyMeACoffe/
cmake --build . 
```

If everything went well, the corresponding WASM file and JavaScript Api should be updated on the BuyMeACoffe/wasm source directory. 

## Running the web page locally
 
```
cd ../BuyMeACoffe/wasm
emrun  buymeacoffe.html
```

