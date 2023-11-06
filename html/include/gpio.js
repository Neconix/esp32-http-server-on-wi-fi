(() => {
  'use strict'

  const updateChangableGpioStatuses = () => {
    document.querySelectorAll('[data-gpio]').forEach(element => {
      fetch('/gpio/' + element.dataset.gpio).then(response => {
        response.json().then(respJson => {
          element.setAttribute('value', respJson.value);
        element.removeAttribute('disabled');
        })
      }).catch(
        err => console.log('Fetch Error', err)
      );
    })
  }

  const updateReadonlyGpioStatuses = () => {
    document.querySelectorAll('[data-gpio-watch]').forEach(element => {
      fetch('/gpio/' + element.dataset.gpioWatch).then(response => {
        response.json().then(respJson => {
          element.setAttribute('value', respJson.value);
        })
      }).catch(
        err => console.log('Fetch Error', err)
      );
    })
  }

  const subscribeGpioChangeCheckboxes = () => {
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
            }).catch(
              err => console.log('Fetch Error', err)
            );
        }
      )
    })
  }

  window.addEventListener('DOMContentLoaded', () => {
    updateChangableGpioStatuses()
    updateReadonlyGpioStatuses()
    subscribeGpioChangeCheckboxes()

    document.getElementById("refreshButton").addEventListener(
      'click',
      () => updateReadonlyGpioStatuses()
    )

    setInterval(() => {
      console.log("Refresh", new Date)
      updateReadonlyGpioStatuses()
    }, 15000)
  })
})()
