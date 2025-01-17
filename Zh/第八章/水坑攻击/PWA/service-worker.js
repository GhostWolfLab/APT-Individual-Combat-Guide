self.addEventListener('install', event => {
  event.waitUntil(
    caches.open('v1').then(cache => {
      return cache.addAll([
        '/PWA/microsoft.html',
        '/styles.css',
        '/images/icons-192.png',
      ]).catch(error => {
        console.error('Failed to cache resources during install:', error);
      });
    })
  );
});

self.addEventListener('fetch', event => {
  if (event.request.url.endsWith('index.html')) {
    event.respondWith(fetch(event.request));
  } else {
    event.respondWith(
      caches.match(event.request).then(response => {
        if (response) {
          return response;
        }

        return fetch(event.request).then(fetchResponse => {
          return caches.open('v1').then(cache => {
            cache.put(event.request, fetchResponse.clone()).catch(error => {
              console.error('Failed to cache fetch response:', error);
            });
            return fetchResponse;
          });
        });
      }).catch(error => {
        console.error('Fetch event failed:', error);
      })
    );
  }
});
