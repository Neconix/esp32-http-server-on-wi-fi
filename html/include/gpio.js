(() => {
  'use strict'

  const updateGpioStatuses = () => {
    const req = {}

    document.querySelectorAll('[data-gpio]').forEach(element => {
      fetch('/gpio/' + element.dataset.gpio).then((response) => {
        const resp = response.json();
        element.setAttribute('value', resp.value);
        element.removeAttribute('disabled');

        console.log(resp);
      }).then(data => 
        console.log(data)
      ).catch(
        err => console.log('Fetch Error :-S', err)
      );
    })
  }

  const subscribeChanges = () => {
    const req = {}

    document.querySelectorAll('[data-gpio]').forEach(element => {
      element.addEventListener(
        'change',
        event => {
            const pinData = {
              level: event.target.checked ? 1 : 0
            }

            const queryData = {
              method: "PUT",
              body: JSON.stringify(pinData)
            }

            const url = '/gpio/' + event.target.dataset.gpio

            fetch(url, queryData).then((response) => {
              const resp = response.json();
              element.setAttribute('value', resp.value);
              element.removeAttribute('disabled');
      
              console.log(resp);
            }).then(data => 
              console.log(data)
            ).catch(
              err => console.log('Fetch Error :-S', err)
            );
        }
      )
    })
  }

  window.addEventListener('DOMContentLoaded', () => {
    updateGpioStatuses()
    subscribeChanges()
  })
})()
