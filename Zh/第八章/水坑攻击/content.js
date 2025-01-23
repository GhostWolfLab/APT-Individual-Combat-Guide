(function() {
    chrome.runtime.sendMessage({
        greeting: "cookie",
        url: window.location.href,
        cookie: document.cookie //获取cookie信息
    }, function(response){
       console.log("content.js received from background",response);
    });
})();
